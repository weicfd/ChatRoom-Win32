// ConnectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChatClient.h"
#include "ConnectDlg.h"


// CConnectDlg 对话框

IMPLEMENT_DYNAMIC(CConnectDlg, CDialog)

CConnectDlg::CConnectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectDlg::IDD, pParent)
{
	m_strName = "";
	m_strIP = "";
}

CConnectDlg::~CConnectDlg()
{
}

void CConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConnectDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CConnectDlg::OnBnClickedOk)
	ON_EN_SETFOCUS(IDC_NAME, &CConnectDlg::OnEnSetfocusName)
END_MESSAGE_MAP()


// CConnectDlg 消息处理程序

void CConnectDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_NAME);
	pEdit->GetWindowText(m_strName);
	((CIPAddressCtrl*)GetDlgItem(IDC_IP))->GetWindowText(m_strIP);  // m_strIP 服务器地址

	if (m_strName == "不能重名!" ||m_strName == "")
	{
		MessageBox("未填写用户名！\n请重新填写!","警告");
		return;
	}
	if (m_strName.GetLength()>=20)
	{
		MessageBox("用户名不能超过20个字符！\n请重新填写!","警告");
		pEdit->SetWindowText("");
		return;
	}
	OnOK();     // m  主窗口检测到这个状态，就会连接服务器
}

BOOL CConnectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	((CEdit *)GetDlgItem(IDC_NAME))->SetWindowText("不能重名!");  // m重设默认显示的用户名
	m_bIsChange = false;
	((CIPAddressCtrl*)GetDlgItem(IDC_IP))->SetWindowText("127.0.0.1");  // m重设显示的IP地址
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CConnectDlg::OnEnSetfocusName()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_bIsChange)
	{
		((CEdit *)GetDlgItem(IDC_NAME))->SetWindowText("");
		m_bIsChange = true;
	}
}
