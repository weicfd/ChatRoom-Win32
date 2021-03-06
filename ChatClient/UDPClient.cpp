#include "StdAfx.h"
#include "UDPClient.h"
#include "ChatClientDlg.h"
#define MAX_LENGTH (8*1024)
#define PORT 20000

char IP[20] = {0};  // m用于存放用户 连接到的 服务器的IP
MSG_INFO mi;  // m一个消息类型变量，用于收发消息
CChatClientDlg *m_pmainwnd;//主窗口
SOCKET m_sUDP;  // m用于网络连接的socket
char m_name[20];  // m当前用户的用户名

CUDPClient::CUDPClient(CChatClientDlg *pmainwnd)
{
	m_pmainwnd = pmainwnd;  // m初始化主窗口
	m_sUDP = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);  //m创建一个UDP socket
}

CUDPClient::~CUDPClient(void)
{
	closesocket(m_sUDP);   // m关闭socket 连接
}

void CUDPClient::Connect(LPCTSTR name,LPCTSTR ip)
{
	memset(&mi,0,sizeof(MSG_INFO));   
	strcpy(IP,ip);
	strcpy(mi.m_From,name);
	strcpy(m_name,name);
	mi.m_Type = -1;//上线  ，  m用户在线状态
	SendMsg(mi);
	CreateThread(NULL,0,CUDPClient::WorkThread,this,0,NULL);
}
void CUDPClient::DisConnect()
{
	memset(&mi,0,sizeof(MSG_INFO));
	strcpy(mi.m_From,m_name);
	mi.m_Type = -2;//下线
	SendMsg(mi);
}
//中间的函数
DWORD WINAPI CUDPClient::WorkThread(void* param)
{
	((CUDPClient*)param)->ReceiveProc();
	return 1;
}

//m接收服务器端数据
void CUDPClient::ReceiveProc()
{
	int count = 0;
	SOCKADDR_IN clientaddr;
	int len = sizeof(SOCKADDR);
	char buf[MAX_LENGTH];   // m接收来自 UDP 的信息
	while(TRUE)
	{
		count = recvfrom(m_sUDP,buf,MAX_LENGTH,0,(SOCKADDR*)&clientaddr,&len); // m接收的字节数？
		if(count != sizeof(MSG_INFO)) continue;
		else
			memcpy(&mi,buf,sizeof(MSG_INFO));
		switch(mi.m_Type)
		{
		case -1://上线
			OnLine();
			break;
		case -2://下线
			OffLine();
			break;
		case -3://重名
			MessageBox(NULL,"用户名已存在，请换一个！","提示",MB_OK|MB_ICONINFORMATION);
			break;
		case -4://服务器关闭
			ServerShutDown();
			break;
		case 0://隐身
			Hide();
			break;
		default:
			if (mi.m_Type >= 1)//正常的交谈
			{
				Talk();
			}
			else
				break;
		}
	}
}

//发送消息
void CUDPClient::SendMsg(MSG_INFO &msg_info)
{
	SOCKADDR_IN addr_in; 
	addr_in.sin_addr.S_un.S_addr = inet_addr(IP);
	addr_in.sin_port = htons(PORT);
	addr_in.sin_family = AF_INET;
	int len = sizeof(SOCKADDR);
	len = sendto(m_sUDP,(char *)&msg_info,sizeof(MSG_INFO),0,(SOCKADDR*)&addr_in,len);  // addr_in,  target socket
}

//上线
void CUDPClient::OnLine()
{
 /*   m_pmainwnd->TextOut(mi.m_From, RGB(0,0,255));
	m_pmainwnd->TextOut(_TEXT(" 上线!\r\n"),RGB(255,0,0));*/

	CString tmp = "  上线!\r\n";
	CString message = mi.m_From + tmp;
	m_pmainwnd->TextOut(message, RGB(0, 255, 0));   // green

	m_pmainwnd->m_bConnected = TRUE;
	m_pmainwnd->AddItemOfList(mi.m_From,mi.m_IP);
}


//下线
void CUDPClient::OffLine()
{
	if (strcmp(mi.m_From , m_name)==0)  // m自己下线
	{
		m_pmainwnd->m_bConnected = FALSE;
		((CListCtrl *)m_pmainwnd->GetDlgItem(IDC_USERLIST))->DeleteAllItems();
	}
	else  
	{
		m_pmainwnd->RemoveItemOfList(mi.m_From);
		// m别人下线，在下拉框中删除下线用户
		m_pmainwnd->RemoveItemOfBox(mi.m_From);
	}
/*
	m_pmainwnd->TextOut(mi.m_From, RGB(0,0,255));
	m_pmainwnd->TextOut(_TEXT(" 下线!\r\n"),RGB(255,0,0));
*/
	CString tmp = "  下线!\r\n";
	CString message = mi.m_From + tmp;
	m_pmainwnd->TextOut(message, RGB(255, 0, 0));  // red
}

//服务器关闭
void CUDPClient::ServerShutDown()
{
	m_pmainwnd->TextOut(_T("系统： 服务器已关闭!\r\n"),RGB(255,0,0));   // red
	m_pmainwnd->m_bConnected = FALSE;
	((CListCtrl *)m_pmainwnd->GetDlgItem(IDC_USERLIST))->DeleteAllItems();
}

//交谈
void CUDPClient::Talk()
{
	CString from = mi.m_From;
	CString to = mi.m_To;
	CString text = mi.m_Text;
	//int type = mi.m_Type;
	COLORREF crf = mi.m_Color;

	//if (from != m_name&&to != m_name && mi.m_Secret)
	// 如果要添加群发功能，可以删掉这句，然后在服务器端维护一个 target group，由服务器只给目标用户发消息
	// 目标客户端就直接显示出来
	if( from != m_name && to != m_name && to!="" )   
		return;  //m这是别人的信息你不要管

	
	CString message = from + ": " + text + "\r\n";
	m_pmainwnd->TextOut((LPCTSTR)message, crf);

/*
	m_pmainwnd->TextOut((LPCTSTR)from, RGB(0, 0, 255));
	m_pmainwnd->TextOut(": ", RGB(0, 0, 0));
	m_pmainwnd->TextOut((LPCTSTR)text, crf);
	m_pmainwnd->TextOut((LPCTSTR)"\r\n", crf);
*/
	
}

void CUDPClient::Hide()
{
	CString from = mi.m_From;
	CString to = mi.m_To;
	CString text = mi.m_Text;
	COLORREF crf = mi.m_Color;

	if (from != m_name && to != m_name && to != "")
		return;  //m这是别人的信息你不要管

	CString a_from = "匿名者";
	CString message = a_from + ": " + text + "\r\n";
	m_pmainwnd->TextOut((LPCTSTR)message, crf);

	//m_pmainwnd->TextOut((LPCTSTR)"匿名者", RGB(0, 0, 255));
	//m_pmainwnd->TextOut(": ", RGB(0, 0, 0));
	//m_pmainwnd->TextOut((LPCTSTR)text, crf);
	//m_pmainwnd->TextOut((LPCTSTR)"\r\n", crf);
}
