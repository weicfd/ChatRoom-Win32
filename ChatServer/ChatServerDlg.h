// ChatServerDlg.h : ͷ�ļ�
//

#pragma once
#include <afxtempl.h>
#include "resource.h"
#include "UDPServer.h"
class CMessage;
class CUDPServer;
// CChatServerDlg �Ի���
class CChatServerDlg : public CDialog
{
// ����
public:
	CChatServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CHATSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
// ʵ��
protected:
	HICON m_hIcon;

public:
	CTypedPtrList<CPtrList, CMessage*> m_lsMsg;//��Ϣ�б�
	CPtrList m_connectionList;//���ӵĿͻ������б�
	CUDPServer UDPSocket;
	// ��������
public:
	void MessageReturn(LPCTSTR lpszMessage);//��ʾ��Ϣ(�л���)
	void Message(LPCTSTR lpszMessage);//��ʾ��Ϣ(û�л���)

	//USERLIST�ķ���
	void InitList();
	void AddExStyleOfList(DWORD dwNewStyle);
	void RemoveItemOfList(char* name);
	void AddItemOfList(char* name, char* IP);
public:
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnClose();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedExit();
	afx_msg void OnDestroy();
};
