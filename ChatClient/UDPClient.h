#pragma once

class CChatClientDlg;

class CUDPClient
{
public:
	CUDPClient(CChatClientDlg *pmainwnd);
	~CUDPClient(void);
public:
	void ReceiveProc();
	void SendMsg(MSG_INFO &mi);
	void OnLine();
	void OffLine();
	static DWORD WINAPI WorkThread(void* param);
	void Connect(LPCTSTR name,LPCTSTR ip);
	void DisConnect();
	void ServerShutDown();
	void Talk();
	void Hide(); 
};
