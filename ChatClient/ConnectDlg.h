#pragma once


// CConnectDlg 对话框

class CConnectDlg : public CDialog
{
	DECLARE_DYNAMIC(CConnectDlg)

public:
	CConnectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConnectDlg();

// 对话框数据
	enum { IDD = IDD_CONNECTSET };
public://用于返回
	CString m_strName;//用户名
	CString m_strIP;//服务器端的IP
	//int m_image;//选择的图标

private://内部使用
	//CImageList m_imgList;//图像列表
	CComboBoxEx m_comboType;//要创建的图标选择框
	CArray<LPSTR,LPSTR> m_strArray;
	BOOL m_bIsChange;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnEnSetfocusName();
};
