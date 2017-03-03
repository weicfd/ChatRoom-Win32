// ConnectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChatClient.h"
#include "ConnectDlg.h"


// CConnectDlg �Ի���

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


// CConnectDlg ��Ϣ�������

void CConnectDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_NAME);
	pEdit->GetWindowText(m_strName);
	((CIPAddressCtrl*)GetDlgItem(IDC_IP))->GetWindowText(m_strIP);  // m_strIP ��������ַ

	if (m_strName == "��������!" ||m_strName == "")
	{
		MessageBox("δ��д�û�����\n��������д!","����");
		return;
	}
	if (m_strName.GetLength()>=20)
	{
		MessageBox("�û������ܳ���20���ַ���\n��������д!","����");
		pEdit->SetWindowText("");
		return;
	}
	OnOK();     // m  �����ڼ�⵽���״̬���ͻ����ӷ�����
}

BOOL CConnectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	((CEdit *)GetDlgItem(IDC_NAME))->SetWindowText("��������!");  // m����Ĭ����ʾ���û���
	m_bIsChange = false;
	((CIPAddressCtrl*)GetDlgItem(IDC_IP))->SetWindowText("127.0.0.1");  // m������ʾ��IP��ַ
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CConnectDlg::OnEnSetfocusName()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_bIsChange)
	{
		((CEdit *)GetDlgItem(IDC_NAME))->SetWindowText("");
		m_bIsChange = true;
	}
}
