// ChatServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChatServer.h"
#include "UDPServer.h"
#include "ChatServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SAFEDELETE(x) if(x!=NULL){delete x;x=NULL;}//删除句柄的 宏

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CChatServerDlg 对话框
CChatServerDlg::CChatServerDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CChatServerDlg::IDD, pParent),UDPSocket(this)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);  // m服务器左上角图表
}

void CChatServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CChatServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_START, &CChatServerDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_EXIT, &CChatServerDlg::OnBnClickedExit)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CChatServerDlg 消息处理程序

BOOL CChatServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//初始化SOCKET
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		MessageBox(_T("SOCKET初始化没有成功"),_T("警告"),MB_ICONWARNING|MB_OK);
		PostQuitMessage(0);
	}


	CMenu* pSysMenu = GetSystemMenu(FALSE);  // m获取菜单
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);  //m 关于 ChatServer(&A)...
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	InitList();//初始化列表框
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CChatServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CChatServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CChatServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CChatServerDlg::OnClose()
{
	if (MessageBox("确定要关闭服务器?","警告",MB_OKCANCEL|MB_ICONWARNING) == IDOK)
	{
		CDialog::OnClose();
	}
}

//这里开始打开该端口的网络服务，等待客户的连接
void CChatServerDlg::OnBnClickedStart()
{
	if(UDPSocket.StartListen())
	{
		this->GetDlgItem(IDC_START)->EnableWindow(FALSE);
		MessageReturn("成功开启服务器！");
	}
	else
		MessageReturn("服务器开启失败！");
}

void CChatServerDlg::MessageReturn(LPCTSTR lpszMessage)
{
	CRichEditCtrl * pView = (CRichEditCtrl *)GetDlgItem(IDC_VIEW);
	CString strTemp = lpszMessage;
	strTemp += _T("\r\n");
	int len = pView->GetWindowTextLength();
	pView->SetSel(len,len);
	pView->ReplaceSel(strTemp);
}

void CChatServerDlg::Message(LPCTSTR lpszMessage)
{
	CRichEditCtrl * pView = (CRichEditCtrl *)GetDlgItem(IDC_VIEW);
	CString strTemp = lpszMessage;
	int len = pView->GetWindowTextLength();
	pView->SetSel(len,len);
	pView->ReplaceSel(strTemp);
}

//初始化在线用户列表
void CChatServerDlg::InitList()
{
	//初始化LIST控件，加上标题
	CListCtrl * pList = (CListCtrl *)GetDlgItem(IDC_USERLIST);
	pList->ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	AddExStyleOfList(LVS_EX_FULLROWSELECT | LVS_OWNERDRAWFIXED);

	int i;
	LV_COLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	CString	strTemp[2] = {"名称", "IP"};
	int size[2] = {60,90};//添加标题栏,长度
	for(i = 0; i < 2; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = (char*)(LPCTSTR)strTemp[i];
		lvc.cx = size[i];
		lvc.fmt = LVCFMT_CENTER;
		pList->InsertColumn(i, &lvc);//添加一列
	}
	//初始化图像列表
	//m_imgList.Create(IDB_IMAGE, 16, 1, RGB(0, 255, 0));
	//HIMAGELIST him = m_imgList.m_hImageList;
	//::SendMessage(GetDlgItem(IDC_USERLIST)->m_hWnd,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL, (LPARAM)him);
	//ImageList_SetBkColor(him,CLR_NONE);
}

//修改风格
void CChatServerDlg::AddExStyleOfList(DWORD dwNewStyle)
{
	CListCtrl * pList = (CListCtrl *)GetDlgItem(IDC_USERLIST);
	//获得LIST的风格
	DWORD dwStyle = ::SendMessage (pList->m_hWnd, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	dwStyle |= dwNewStyle;
	//向LIST发修改风格消息
	::SendMessage (pList->m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle);
}


//从LIST中移除一行
void CChatServerDlg::RemoveItemOfList(char* name)
{
	CString str;
	CListCtrl * pList = (CListCtrl *)GetDlgItem(IDC_USERLIST);
	for(int i = 0; i < pList->GetItemCount(); i++){
		str =  pList->GetItemText(i,0);
		if(strcmp(str, name) == 0){
			pList->DeleteItem(i);
			break;
		}
	}
}

//为LIST添加一列,其中i为图标的类型，name为用户名，text为IP地址
void CChatServerDlg::AddItemOfList(short i, char* name, char* text)
{
	LVITEM			lvi;
	CListCtrl * pList = (CListCtrl *)GetDlgItem(IDC_USERLIST);
	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	lvi.iItem = pList->GetItemCount();
	lvi.iImage = i;
	lvi.iSubItem = 0;
	lvi.pszText = name;
	lvi.cchTextMax = 64;
	lvi.lParam = 0;
	pList->InsertItem(&lvi);

	lvi.mask = LVIF_TEXT;
	lvi.pszText = text;
	lvi.cchTextMax = 32;
	lvi.iSubItem = 1;
	pList->SetItem(&lvi);

}
void CChatServerDlg::OnBnClickedExit()
{
	PostMessage(WM_CLOSE,0,0);
}

void CChatServerDlg::OnDestroy()
{
	UDPSocket.SevverShutDown();
	CDialog::OnDestroy();
}
