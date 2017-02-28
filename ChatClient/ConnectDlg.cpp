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
	m_image = 0;
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
	//m_image = m_comboType.GetCurSel();
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
	OnOK();
}

BOOL CConnectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//初始化图像列表
	/*RECT rc;
	COMBOBOXEXITEM ci;
	SetRect(&rc,134,74,219,180);
	m_imgList.Create(IDB_IMAGE, 16, 1, RGB(0, 255, 0));
	HIMAGELIST him = m_imgList.m_hImageList;
	m_comboType.Create(CBS_DROPDOWNLIST|WS_CHILD|CBS_AUTOHSCROLL|WS_VISIBLE,rc,this,IDC_TYPE);
	m_comboType.SetImageList(&m_imgList);
	ImageList_SetBkColor(him,CLR_NONE);*/

	//给m_strArray添加字符串
	/*m_strArray.Add("男孩1");
	m_strArray.Add("女孩");
	m_strArray.Add("男孩2");
	m_strArray.Add("尖鼻男");
	m_strArray.Add("羊");
	m_strArray.Add("鸡");
	m_strArray.Add("企鹅");*/

	//for (int i =0;i<7;i++)//添加图像
	//{
	//	ci.mask = CBEIF_IMAGE|CBEIF_TEXT|CBEIF_SELECTEDIMAGE;
	//	ci.iImage = i;
	//	ci.iSelectedImage =i;
	//	ci.pszText = m_strArray.GetAt(i);
	//	ci.iItem = i;
	//	m_comboType.InsertItem(&ci);
	//}

	//m_comboType.SetCurSel(0); // 重设备选头像

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
