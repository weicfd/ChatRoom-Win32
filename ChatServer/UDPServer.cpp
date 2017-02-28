#include "StdAfx.h"
#include "UDPServer.h"
#include "ChatServerDlg.h"
#define MAX_LENGTH (8*1024)

CPtrList m_UsesrList;//所有的用户
SOCKET m_sUDP;//UDPSOCKET
MSG_INFO * m_pMsg;
CChatServerDlg * m_pmainwnd;

CUDPServer::CUDPServer(CChatServerDlg *pWnd)
{
	m_sUDP = INVALID_SOCKET;
	m_pmainwnd = pWnd;
}

CUDPServer::~CUDPServer(void)
{
}

BOOL CUDPServer::StartListen()
{
	m_sUDP = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);

	SOCKADDR_IN local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons(6000);

	int i = bind (m_sUDP, (SOCKADDR *)&local, sizeof(local) );
	CreateThread(NULL,0,CUDPServer::WorkProc,this,0,NULL);

	if (i<0) return FALSE;
	else return TRUE;
}
//中间的函数
DWORD WINAPI CUDPServer::WorkProc(void* param)
{
	((CUDPServer*)param)->ListenProc();
	return 1;
}

//监听
void CUDPServer::ListenProc()
{
	int count = 0;
	SOCKADDR_IN clientaddr;
	int len = sizeof(SOCKADDR);
	char buf[MAX_LENGTH];
	while(TRUE)
	{
		count = recvfrom(m_sUDP,buf,MAX_LENGTH,0,(SOCKADDR*)&clientaddr,&len);
		if(count != sizeof(MSG_INFO)) continue;
		else
			m_pMsg = (MSG_INFO*)buf;
		switch(m_pMsg->m_Type)
		{
		case -1://上线
			OnLine(&clientaddr);
			break;
		case -2://下线
			OffLine(&clientaddr);
			break;
		default:
			if (m_pMsg->m_Type>=0)//正常的交谈
			{
				Talk();
			}
			else
				break;
		}
	}
}

//上线
void CUDPServer::OnLine(SOCKADDR_IN * psa)
{//每一个用户都有一个USER_INFO
	USER_INFO  *p = new USER_INFO();
	memcpy(p->m_User , m_pMsg->m_From,20);
	//p->m_Image = m_pMsg->m_Image;
	strcpy(p->m_IP,inet_ntoa(psa->sin_addr));
	p->m_SA = *psa;
	//判断用户是否存在
	for(POSITION pos = m_UsesrList.GetHeadPosition(); pos != NULL;)
	{
		USER_INFO * pui = (USER_INFO *)m_UsesrList.GetNext(pos);
		if (pui != NULL)
		{
			if (strcmp(pui->m_User,m_pMsg->m_From)==0)//存在同名
			{
				m_pMsg->m_Type = -3;//重名
				SendMsg( p);
				delete p;
				return ;
			}
		}
	}
	//
	UpdateAllClients();
	m_UsesrList.AddTail(p);
	SendListToNew(*p);
	m_pmainwnd->AddItemOfList(p->m_User,p->m_IP);
	m_pmainwnd->Message(p->m_User);
	m_pmainwnd->MessageReturn("风尘仆仆地推门而入");
}


//下线
void CUDPServer::OffLine(SOCKADDR_IN * psa)
{
	POSITION pos,temp;
	USER_INFO * pui;
	m_pmainwnd->RemoveItemOfList(m_pMsg->m_From);
	UpdateAllClients();
	for(pos=m_UsesrList.GetHeadPosition();pos!=NULL;) 
	{ 
		temp=pos; 
		pui=(USER_INFO *)m_UsesrList.GetNext(pos); 
		if (strcmp(pui->m_User,m_pMsg->m_From)==0)
		{
			m_UsesrList.RemoveAt(temp); 
			delete   pui;   
		}
	}
	m_pmainwnd->Message(m_pMsg->m_From);
	m_pmainwnd->MessageReturn("静静地离开了，孤单的背影显得格外潇洒");
}

//通知所有的客户端
void CUDPServer::UpdateAllClients()
{
	for(POSITION pos = m_UsesrList.GetHeadPosition(); pos != NULL;)
	{
		USER_INFO * pui = (USER_INFO *)m_UsesrList.GetNext(pos);
		if (pui != NULL)
		{
			strcpy(m_pMsg->m_IP,pui->m_IP);//传送给客户端IP
			SendMsg( pui);
		}
	}
}

//发送所有的用户名单
void CUDPServer::SendListToNew(USER_INFO &ui)  // ui 为新用户
{
	for(POSITION pos = m_UsesrList.GetHeadPosition(); pos != NULL;)
	{
		USER_INFO * pui = (USER_INFO *)m_UsesrList.GetNext(pos);
		if (pui != NULL)
		{
			m_pMsg->m_Type = -1;
			//m_pMsg->m_Image = pui->m_Image;
			strcpy(m_pMsg->m_From, pui->m_User);
			strcpy(m_pMsg->m_IP,pui->m_IP);//传送给客户端IP
			SendMsg( &ui);
			Sleep(50);
		}
	}
}

void CUDPServer::SendMsg(USER_INFO * pui)
{
	sendto(m_sUDP,(char *)m_pMsg,sizeof(MSG_INFO),0,(SOCKADDR*)&pui->m_SA,sizeof(SOCKADDR));
}

void CUDPServer::SevverShutDown()
{
	//给客户端发消息
	m_pMsg = new MSG_INFO();
	m_pMsg->m_Type = -4;
	UpdateAllClients();
	delete m_pMsg;
	//删除所有的用户名单
	POSITION pos,temp;
	USER_INFO * pui;
	for(pos=m_UsesrList.GetHeadPosition();pos!=NULL;) 
	{ 
		temp=pos; 
		pui=(USER_INFO *)m_UsesrList.GetNext(pos); 
		m_UsesrList.RemoveAt(temp); 
		delete   pui;   
	}
}

void CUDPServer::Talk()
{
	//通知客户端
	UpdateAllClients();
	//服务器显示
	CString temp,first,second;
	CString from = m_pMsg->m_From;
	CString to = m_pMsg->m_To;
	CString text = m_pMsg->m_Text;    //  m用户在输入框中的消息内容
	int type = m_pMsg->m_Type;    // m沟通类型

	if(to.IsEmpty()) to = "所有人";
	if(type > 32 || type < 0)
		return;
	temp.LoadString(IDS_TALK0 + type);	// m载入对应 type 的“表情字符”
	int i=temp.Find(",");
	if(i!=-1){
		first=temp.Left(i);
		if(i!=temp.GetLength()-1){
			second=temp.Mid(i+1);
			second+="：";
		}
		else{
			second="：";
		}
		m_pmainwnd->Message(from);
		//m_pmainwnd->Message(first);   // m表情字
		m_pmainwnd->Message("告诉");  
		m_pmainwnd->Message(to);
		//m_pmainwnd->Message(second);    // m表情字
		m_pmainwnd->MessageReturn(text);
	}
	else{
		first=temp;
		second="： ";
		m_pmainwnd->Message(from);
		//m_pmainwnd->Message(first);
		//m_pmainwnd->Message(second);
		m_pmainwnd->Message(": ");
		m_pmainwnd->MessageReturn(text);
	}
}