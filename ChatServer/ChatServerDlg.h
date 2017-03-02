// ChatServerDlg.h : 头文件
//

#pragma once
#include <afxtempl.h>
#include "resource.h"
#include "UDPServer.h"
class CMessage;
class CUDPServer;
// CChatServerDlg 对话框
class CChatServerDlg : public CDialog
{
// 构造
public:
	CChatServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CHATSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
// 实现
protected:
	HICON m_hIcon;

public:
	CTypedPtrList<CPtrList, CMessage*> m_lsMsg;//消息列表
	CPtrList m_connectionList;//连接的客户机的列表
	CUDPServer UDPSocket;
	// 操作函数
public:
	void MessageReturn(LPCTSTR lpszMessage);//显示消息(有换行)
	void Message(LPCTSTR lpszMessage);//显示消息(没有换行)

	//USERLIST的方法
	void InitList();
	void AddExStyleOfList(DWORD dwNewStyle);
	void RemoveItemOfList(char* name);
	void AddItemOfList(char* name, char* IP);
public:
	// 生成的消息映射函数
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
