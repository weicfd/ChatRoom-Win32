// ChatClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChatClient.h"
#include "ChatClientDlg.h"
#include "ConnectDlg.h"
#include "UDPClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SAFEDELETE(x) if(x!=NULL){delete x;x=NULL;}//删除句柄的 宏
#define PORT 2310//端口号
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




// CChatClientDlg 对话框


CChatClientDlg::CChatClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatClientDlg::IDD, pParent)
{
	m_pUDPClient = new CUDPClient(this);
	//m_bFilter = FALSE;
	m_bConnected = FALSE;
	crf = RGB(0,0,0);   // m字体颜色
	m_hIcon = AfxGetApp()->LoadIcon(IDI_LARGEQQ);  // m软件图标
}

void CChatClientDlg::DoDataExchange(CDataExchange* pDX)    // m数据交换
{
	CDialog::DoDataExchange(pDX);
}  

BEGIN_MESSAGE_MAP(CChatClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_DBLCLK, IDC_USERLIST, &CChatClientDlg::OnNMDblclkUserlist)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SEND, &CChatClientDlg::OnBnClickedSend)
	ON_COMMAND(ID_CONNECT, &CChatClientDlg::OnConnect)
//	ON_BN_CLICKED(IDC_FILTER, &CChatClientDlg::OnBnClickedFilter)
	ON_COMMAND(ID_DISCONNECT, &CChatClientDlg::OnDisconnect)
	ON_BN_CLICKED(IDC_COLORSELECT, &CChatClientDlg::OnBnClickedColorselect)
	ON_WM_CTLCOLOR()
	ON_WM_INITMENUPOPUP()
	ON_WM_DESTROY()
	ON_COMMAND(ID_EXIT, &CChatClientDlg::OnExit)   // m菜单 -> 退出
//	ON_COMMAND(ID_SHOWIP, &CChatClientDlg::OnShowip)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_USERLIST, &CChatClientDlg::OnLvnItemchangedUserlist) // m（消息，控件，处理函数）
//	ON_BN_CLICKED(IDC_QUIET, &CChatClientDlg::OnBnClickedQuiet)
ON_CBN_SELCHANGE(IDC_TYPE, &CChatClientDlg::OnCbnSelchangeType)
END_MESSAGE_MAP()


// CChatClientDlg 消息处理程序

BOOL CChatClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//初始化SOCKET
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );   // initiates use of the Winsock DLL 初始化Win Socket 动态链接库
	if ( err != 0 ) {
		MessageBox(_T("SOCKET初始化没有成功"),_T("警告"),MB_ICONWARNING|MB_OK);
		PostQuitMessage(0);
	}


	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	// 执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//初始化LIST
	InitList();
	//初始化TYPE COMBO
	InitType();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CChatClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CChatClientDlg::OnPaint()
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
HCURSOR CChatClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//list控件，显示在线用户的函数

void CChatClientDlg::InitList()
{
	//初始化LIST控件，加上标题
	CListCtrl * pList = (CListCtrl *)GetDlgItem(IDC_USERLIST);
	pList->ModifyStyleEx(0, WS_EX_CLIENTEDGE);  // 修改List扩展风格
	AddExStyleOfList(LVS_EX_FULLROWSELECT | LVS_OWNERDRAWFIXED);

	int i;
	LV_COLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	CString	strTemp[2] = {"用户名", "IP"};
	int size[2] = {60,90};//添加标题栏,长度
	for(i = 0; i < 2; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = (char*)(LPCTSTR)strTemp[i];   // 此时列表为空，只显示 "用户名，IP"
		lvc.cx = size[i];
		lvc.fmt = LVCFMT_CENTER;
		pList->InsertColumn(i, &lvc);//添加一列
	}
	//初始化图像列表
	//m_imgList.Create(IDB_IMAGE, 16, 1, RGB(0, 255, 0));
	//HIMAGELIST him = m_imgList.m_hImageList;
	//::SendMessage(GetDlgItem(IDC_USERLIST)->m_hWnd,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)him);
	//ImageList_SetBkColor(him,CLR_NONE);
}

//初始化TYPE，依次添加STRING
void CChatClientDlg::InitType()
{
	CComboBox* pTo = (CComboBox*)GetDlgItem(IDC_CURRENT);
	pTo->AddString("所有人");
	pTo->SetCurSel(0);

	// 沟通类型
	CComboBox* pType = (CComboBox*)GetDlgItem(IDC_TYPE);
	pType->ResetContent();
	CString str;
	for(int i = 0; i <= 32; i++)
	{
		str.LoadString(IDS_S0 + i);
		pType->AddString(str);
	}
	pType->SetCurSel(0);
}

//修改List扩展风格
void CChatClientDlg::AddExStyleOfList(DWORD dwNewStyle)
{
	CListCtrl * pList = (CListCtrl *)GetDlgItem(IDC_USERLIST);
	//获得LIST的风格
	DWORD dwStyle = ::SendMessage (pList->m_hWnd, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	dwStyle |= dwNewStyle;
	//向LIST发修改风格消息
	::SendMessage (pList->m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle);
}

//从LIST中移除一行
void CChatClientDlg::RemoveItemOfList(char* name)
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

//为LIST添加一行, name为用户名，IP地址
void CChatClientDlg::AddItemOfList(char* name, char* IP)
{
	LVITEM			lvi;
	CListCtrl * pList = (CListCtrl *)GetDlgItem(IDC_USERLIST);
	lvi.mask = LVIF_TEXT | LVIF_PARAM;
	lvi.iItem = pList->GetItemCount();
	//lvi.iImage = i;
	lvi.iSubItem = 0;
	lvi.pszText = name;
	lvi.cchTextMax = 64;
	lvi.lParam = 0;
	pList->InsertItem(&lvi);

	lvi.mask = LVIF_TEXT;
	lvi.pszText = IP;
	lvi.cchTextMax = 32;
	lvi.iSubItem = 1;
	pList->SetItem(&lvi);

}

//当用户双击LIST的时候给CURRENT COMBO中添加一行
void CChatClientDlg::OnNMDblclkUserlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMItem = (NM_LISTVIEW*)pNMHDR;
	CListCtrl * pList = (CListCtrl *)GetDlgItem(IDC_USERLIST);
	CString str;
	int iSel = pNMItem->iItem;
	if (iSel >= 0 ){
		str = pList->GetItemText(iSel,0);
		if(str == m_strName) return;//点到了自己
		CComboBox* pCurr = (CComboBox*)GetDlgItem(IDC_CURRENT);
		if (pCurr->FindStringExact(-1,(LPCTSTR)str)<0)//当CURRENT COMBO中有相同的时候不添加
		{
			pCurr->AddString(str);  // m下拉框中添加点击的用户名
			pCurr->SelectString(-1,(LPCTSTR)str);  // m选中点击的用户名
		}else
			pCurr->SelectString(-1,(LPCTSTR)str);
	}
	*pResult = 0;
}




//显示消息
void CChatClientDlg::DisplayMsg(LPCTSTR lpszText)
{
	TextOut(lpszText, RGB(128,0,0));
}

//真正的显示函数
void CChatClientDlg::TextOut(LPCTSTR lpszMessage, COLORREF clr)
{
	CRichEditCtrl * pView = (CRichEditCtrl *)GetDlgItem(IDC_CHATVIEW);

	memset(&cfm,   0,   sizeof(cfm));   
	cfm.cbSize   =   sizeof(CHARFORMAT);   
	cfm.dwMask   =   CFM_BOLD   |   CFM_COLOR   |   CFM_FACE   |   CFM_ITALIC   |   CFM_SIZE   |   CFM_STRIKEOUT   |   CFM_UNDERLINE;   
	cfm.crTextColor   =   clr;//颜色   
	_tcscpy(cfm.szFaceName,   _T("宋体"));//字体   
	cfm.bPitchAndFamily   =   FF_SWISS;   
	cfm.yHeight   =   200;     

	int len = pView->GetWindowTextLength();
	pView->SetSel(len,len);

	pView->SetSelectionCharFormat(cfm);
	pView->ReplaceSel(lpszMessage);
}

//发送
void CChatClientDlg::OnBnClickedSend()
{
	// TODO: 在此添加控件通知处理程序代码
	CComboBox* pTo = (CComboBox*)GetDlgItem(IDC_CURRENT);  //m收件人
	CComboBox* pType = (CComboBox*)GetDlgItem(IDC_TYPE);    // m沟通类型
	CWnd* pText = (CWnd*)GetDlgItem(IDC_MSG);  // m消息内容
	//CButton* pSecret = (CButton*)GetDlgItem(IDC_QUIET);  // 悄悄的

	MSG_INFO mi;   // 为要发送的消息体
	CString tmp;
	strcpy(mi.m_From,(LPCTSTR)m_strName); 

	pTo->GetWindowText(tmp);    // tmp -- 收件人
	if (tmp == "所有人")		tmp = "";
	strcpy(mi.m_To,(LPCTSTR)tmp);
	tmp.Empty();
	
	pType->GetWindowText(tmp);
	mi.m_Type = pType->FindString(0, tmp);
	tmp.Empty();
	
	pText->GetWindowText(tmp);
	strcpy(mi.m_Text,(LPCTSTR)tmp);
	
	if(tmp.IsEmpty()) return;  // 消息内容为空，则不发送
	//mi.m_Secret = pSecret->GetCheck();
	
	mi.m_Color = crf;
	pText->SetWindowText(_TEXT(""));
	m_pUDPClient->SendMsg(mi);
}


//确定连接，启动连接
void CChatClientDlg::OnConnect()
{
	CConnectDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_strName = dlg.m_strName;
		this->SetWindowText(m_strName);
		m_pUDPClient->Connect(dlg.m_strName,dlg.m_strIP);
	}
}

//void CChatClientDlg::OnBnClickedFilter()
//{
//	CButton* pFilter = (CButton*)GetDlgItem(IDC_FILTER);
//	m_bFilter = pFilter->GetCheck();
//}


void CChatClientDlg::OnDisconnect()
{
	m_pUDPClient->DisConnect();
}

void CChatClientDlg::OnBnClickedColorselect()
{
	// TODO: 在此添加控件通知处理程序代码
	CEdit * pEdit  = (CEdit *)GetDlgItem(IDC_MSG);
	CColorDialog dlg;
	if(dlg.DoModal()==IDOK)
	{
		if(dlg.GetColor() == RGB(255,255,255))
			AfxMessageBox("把字体设为白色就看不见了啊!");
		else
		{
			crf = dlg.GetColor();
			pEdit->Invalidate(TRUE);
			pEdit->UpdateWindow();
		}
	}	
}


//设置编辑框的字体颜色
HBRUSH CChatClientDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (nCtlColor == CTLCOLOR_EDIT)
	{
		CEdit * pEdit  = (CEdit *)GetDlgItem(IDC_MSG);
		if (pEdit->m_hWnd == pWnd->m_hWnd)
		{
			pDC->SetTextColor(crf);
		}
	}
	return hbr;
}


//启用或禁用（功能菜单）连接或断开连接
void CChatClientDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	if (nIndex == 0)
	{
		if (m_bConnected == false)
		{
			pPopupMenu->EnableMenuItem(ID_CONNECT,MF_BYCOMMAND|MF_ENABLED);
			pPopupMenu->EnableMenuItem(ID_DISCONNECT,MF_BYCOMMAND|MF_GRAYED);
		}
		else
		{
			pPopupMenu->EnableMenuItem(ID_CONNECT,MF_BYCOMMAND|MF_GRAYED);
			pPopupMenu->EnableMenuItem(ID_DISCONNECT,MF_BYCOMMAND|MF_ENABLED);
		}
	}
	CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
}

void CChatClientDlg::OnDestroy()
{
	if (m_bConnected)
	{
		m_pUDPClient->DisConnect();
	}
	CDialog::OnDestroy();
}

void CChatClientDlg::OnClose()
{
	OnDisconnect();  // m断开连接(如果这句在if里面会有空指针报错)
	if(MessageBox("确定要退出！","通知",MB_OKCANCEL|MB_ICONINFORMATION) == IDOK)
	{
		CDialog::OnClose();  // m会自动调用 OnDestroy 吧
	}
}

void CChatClientDlg::OnExit()
{
	PostMessage(WM_CLOSE,0,0);
}

//void CChatClientDlg::OnShowip()
//{
//	PHOSTENT hostinfo;
//	char hostname[255];
//	CString str,str1,str2;
//	LPSTR ip;
//	if(gethostname(hostname, sizeof(hostname)) != 0)    //获得本地主机名
//	{
//		MessageBox("获取主机名出错！","警告",MB_OK | MB_ICONWARNING);
//		return;
//	}
//	if((hostinfo = gethostbyname(hostname)) == NULL)    //获得本地IP地址
//	{
//		MessageBox("获取本机IP出错！","警告",MB_OK | MB_ICONWARNING);
//		return;
//	}
//	str.Format("主机名：%s\n",hostname);
//	while(*(hostinfo->h_addr_list) != NULL)        //输出IP地址
//	{
//		ip = inet_ntoa(*(struct in_addr *) *hostinfo->h_addr_list);
//		str1.Format("ip 为 : %s\n", ip);
//		str2+=str1;
//		hostinfo->h_addr_list++;
//	}
//	str+=str2;
//	MessageBox(str,"IP地址",0);
//}


void CChatClientDlg::OnLvnItemchangedUserlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


//void CChatClientDlg::OnBnClickedQuiet()
//{
//	// TODO: Add your control notification handler code here
//}


void CChatClientDlg::OnCbnSelchangeType()
{
	// TODO: Add your control notification handler code here
}
