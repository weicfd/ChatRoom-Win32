#include "StdAfx.h"
#include "UDPClient.h"
#include "ChatClientDlg.h"
#define MAX_LENGTH (8*1024)
#define PORT 6000

char IP[20] = {0};  // m���ڴ���û� ���ӵ��� ��������IP
MSG_INFO mi;  // mһ����Ϣ���ͱ����������շ���Ϣ
CChatClientDlg *m_pmainwnd;//������
SOCKET m_sUDP;  // m�����������ӵ�socket
char m_name[20];  // m��ǰ�û����û���

CUDPClient::CUDPClient(CChatClientDlg *pmainwnd)
{
	m_pmainwnd = pmainwnd;  // m��ʼ��������
	m_sUDP = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);  //m����һ��UDP socket
}

CUDPClient::~CUDPClient(void)
{
	closesocket(m_sUDP);   // m�ر�socket ����
}

void CUDPClient::Connect(LPCTSTR name,LPCTSTR ip)
{
	memset(&mi,0,sizeof(MSG_INFO));   
	strcpy(IP,ip);
	strcpy(mi.m_From,name);
	strcpy(m_name,name);
	//mi.m_Image = image;
	mi.m_Type = -1;//����  ��  m�û�����״̬
	SendMsg(mi);
	CreateThread(NULL,0,CUDPClient::WorkThread,this,0,NULL);
}
void CUDPClient::DisConnect()
{
	memset(&mi,0,sizeof(MSG_INFO));
	strcpy(mi.m_From,m_name);
	mi.m_Type = -2;//����
	SendMsg(mi);
}
//�м�ĺ���
DWORD WINAPI CUDPClient::WorkThread(void* param)
{
	((CUDPClient*)param)->ReceiveProc();
	return 1;
}

//m���շ�����������
void CUDPClient::ReceiveProc()
{
	int count = 0;
	SOCKADDR_IN clientaddr;
	int len = sizeof(SOCKADDR);
	char buf[MAX_LENGTH];   // m�������� UDP ����Ϣ
	while(TRUE)
	{
		count = recvfrom(m_sUDP,buf,MAX_LENGTH,0,(SOCKADDR*)&clientaddr,&len); // m���յ��ֽ�����
		if(count != sizeof(MSG_INFO)) continue;
		else
			memcpy(&mi,buf,sizeof(MSG_INFO));
		switch(mi.m_Type)
		{
		case -1://����
			OnLine();
			break;
		case -2://����
			OffLine();
			break;
		case -3://����
			MessageBox(NULL,"�û����Ѵ��ڣ��뻻һ����","��ʾ",MB_OK|MB_ICONINFORMATION);
			break;
		case -4://�������ر�
			ServerShutDown();
			break;
		default:
			if (mi.m_Type>=0)//�����Ľ�̸
			{
				Talk();
			}
			else
				break;
		}
	}
}

//������Ϣ
void CUDPClient::SendMsg(MSG_INFO &msg_info)
{
	SOCKADDR_IN addr_in; 
	addr_in.sin_addr.S_un.S_addr = inet_addr(IP);
	addr_in.sin_port = htons(PORT);
	addr_in.sin_family = AF_INET;
	int len = sizeof(SOCKADDR);
	len = sendto(m_sUDP,(char *)&msg_info,sizeof(MSG_INFO),0,(SOCKADDR*)&addr_in,len);  // addr_in,  target socket
}

//����
void CUDPClient::OnLine()
{
    m_pmainwnd->TextOut(mi.m_From, RGB(0,0,255));
	m_pmainwnd->TextOut(_TEXT(" ����!\r\n"),RGB(255,0,0));
	m_pmainwnd->m_bConnected = TRUE;
	m_pmainwnd->AddItemOfList(mi.m_From,mi.m_IP);
}


//����
void CUDPClient::OffLine()
{
	if (strcmp(mi.m_From , m_name)==0)
	{
		m_pmainwnd->m_bConnected = FALSE;
		((CListCtrl *)m_pmainwnd->GetDlgItem(IDC_USERLIST))->DeleteAllItems();
	}
	else
	{
		m_pmainwnd->RemoveItemOfList(mi.m_From);
	}
	m_pmainwnd->TextOut(mi.m_From, RGB(0,0,255));
	m_pmainwnd->TextOut(_TEXT(" ����!\r\n"),RGB(255,0,0));
}

//�������ر�
void CUDPClient::ServerShutDown()
{
	m_pmainwnd->TextOut(_T("ϵͳ���������ѹر�!"),RGB(0,0,0));
	m_pmainwnd->m_bConnected = FALSE;
	((CListCtrl *)m_pmainwnd->GetDlgItem(IDC_USERLIST))->DeleteAllItems();
}

//��̸
void CUDPClient::Talk()
{
	CString from = mi.m_From;
	CString to = mi.m_To;
	CString text = mi.m_Text;
	//int type = mi.m_Type;
	COLORREF crf = mi.m_Color;

	//if (from != m_name&&to != m_name && mi.m_Secret)
	// ���Ҫ���Ⱥ�����ܣ�����ɾ����䣬Ȼ���ڷ�������ά��һ�� target group���ɷ�����ֻ��Ŀ���û�����Ϣ
	// Ŀ��ͻ��˾�ֱ����ʾ����
	if(to == "") to = "������";
	if( (from != m_name&&to != m_name) || to!="������" )
		return;  //m���Ǳ��˵���Ϣ�㲻Ҫ��

	//if(from != m_name&&to != m_name && m_pmainwnd->m_bFilter)
	//	return;//���˲������Լ�����Ϣ

	m_pmainwnd->TextOut((LPCTSTR)from, RGB(0, 0, 255));
	m_pmainwnd->TextOut(": ", RGB(0, 0, 0));
	m_pmainwnd->TextOut((LPCTSTR)text, crf);
	m_pmainwnd->TextOut((LPCTSTR)"\r\n", crf);

	//CString temp,first,second;
	//if(type > 32 || type < 0)    
	//	return;  
	//temp.LoadString(IDS_TALK0 + type);	
	//int i=temp.Find(",");
	//if(i != -1){
	//	first = temp.Left(i);
	//	if(i != temp.GetLength() - 1){
	//		second = temp.Mid(i + 1);
	//		second += "��";
	//	}
	//	else{
	//		second="��";
	//	}
	//	if(to == "") to = "������";
	//	m_pmainwnd->TextOut((LPCTSTR)from, RGB(0,0,255));
	//	//m_pmainwnd->TextOut((LPCTSTR)first, RGB(0,0,0));    // m������
	//	m_pmainwnd->TextOut((LPCTSTR)"����", RGB(0, 0, 0));
	//	m_pmainwnd->TextOut((LPCTSTR)to, RGB(0,0,255));
	//	//m_pmainwnd->TextOut((LPCTSTR)second, RGB(0,0,0));    // m������
	//	m_pmainwnd->TextOut((LPCTSTR)text,crf);
	//	m_pmainwnd->TextOut((LPCTSTR)"\r\n",crf);
	//}
	//else{
	//	first=temp;
	//	second="�� ";
	//	m_pmainwnd->TextOut(from,RGB(0,0,255));
	//	//m_pmainwnd->TextOut(first,RGB(0,0,0));
	//	//m_pmainwnd->TextOut(second,RGB(0,0,0));
	//	m_pmainwnd->TextOut(": ", RGB(0, 0, 0));
	//	m_pmainwnd->TextOut(text,crf);
	//	m_pmainwnd->TextOut("\r\n",crf);
	//}
}