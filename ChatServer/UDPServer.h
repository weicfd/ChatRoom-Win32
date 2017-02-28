#pragma once

class CChatServerDlg;
class CUDPServer
{
public:
	CUDPServer(CChatServerDlg *pWnd);
	~CUDPServer(void);
public:
	BOOL StartListen();
	void ListenProc();
	void SendMsg(USER_INFO * pui);
	void OnLine(SOCKADDR_IN* psa);
	void OffLine(SOCKADDR_IN*psa);
	void UpdateAllClients();
	void SendListToNew(USER_INFO &ui);
	void SevverShutDown();
	void Talk();
	static DWORD WINAPI WorkProc(void* param);
};
