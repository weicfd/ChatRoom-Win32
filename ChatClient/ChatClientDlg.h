// ChatClientDlg.h : 头文件
//

#pragma once


// CChatClientDlg 对话框
#include "Message.h"
#include "resource.h"
class CUDPClient;

class CChatClientDlg : public CDialog
{
// 构造
public:
	CChatClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CHATCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

//LIST控件
public:
	//CImageList m_imgList;//图像列表，显示在线用户的图标
	void RemoveItemOfList(char* name);//移除
	void AddItemOfList(char* name, char* IP);//添加
	void AddExStyleOfList(DWORD dwNewStyle);//给USERLIST添加风格
	void InitList();//初始化USERLIST
	void InitType();//初始化TYPE COMBO控件
public:
//数据的操作函数和变量
	CHARFORMAT cfm;//用于给RICHEDIT设置字体
	void DisplayMsg(LPCTSTR lpszText);
	void TextOut(LPCTSTR lpszMessage, COLORREF clr);
// 实现
protected:
	HICON m_hIcon;
	//
public:
	COLORREF crf;//字体颜色
    BOOL m_bConnected;//是否连接
	//BOOL m_bFilter;
	CUDPClient *m_pUDPClient;
	CString m_strName;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMDblclkUserlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
	afx_msg void OnBnClickedSend();
	afx_msg void OnConnect();
//	afx_msg void OnBnClickedFilter();//直接改变本地变量，而QUIET则是发送的时候改变
	afx_msg void OnDisconnect();
	afx_msg void OnBnClickedColorselect();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnDestroy();
	afx_msg void OnExit();
//	afx_msg void OnShowip();
	afx_msg void OnLvnItemchangedUserlist(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnBnClickedQuiet();
	afx_msg void OnCbnSelchangeType();
};
