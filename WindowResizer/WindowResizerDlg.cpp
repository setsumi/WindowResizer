
// WindowResizerDlg.cpp: 实现文件
//

#include "stdafx.h"

#include "WindowResizer.h"
#include "WindowResizerDlg.h"
#include "afxdialogex.h"
#include "Common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//=============================================================
typedef LONG NTSTATUS, * PNTSTATUS;
#define STATUS_SUCCESS (0x00000000)

typedef NTSTATUS(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);

RTL_OSVERSIONINFOW GetRealOSVersion() {
	HMODULE hMod = ::GetModuleHandleW(L"ntdll.dll");
	if (hMod) {
		RtlGetVersionPtr fxPtr = (RtlGetVersionPtr)::GetProcAddress(hMod, "RtlGetVersion");
		if (fxPtr != nullptr) {
			RTL_OSVERSIONINFOW rovi = { 0 };
			rovi.dwOSVersionInfoSize = sizeof(rovi);
			if (STATUS_SUCCESS == fxPtr(&rovi)) {
				return rovi;
			}
		}
	}
	RTL_OSVERSIONINFOW rovi = { 0 };
	return rovi;
}

void SetForegroundWindowInternal(HWND hWnd)
{
	AllocConsole();
	auto hWndConsole = GetConsoleWindow();
	SetWindowPos(hWndConsole, 0, 0, 0, 0, 0, SWP_NOACTIVATE);
	FreeConsole();
	SetForegroundWindow(hWnd);
}

CString GetExecutablePath()
{
	TCHAR path[1024];
	if (GetModuleFileName(NULL, path, 1024))
	{
		return CString(path);
	}
	else
	{
		// Handle error here
		return CString(L"");
	}
}
//=============================================================

// CMyStatic subclass of CStatic

BEGIN_MESSAGE_MAP(CMyStatic, CStatic)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void CMyStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	CStatic::OnLButtonDown(nFlags, point);
	// Handle left mouse button click here
	SetWindowTextW(L"");
}

void CMyStatic::OnMouseMove(UINT nFlags, CPoint point)
{
	CStatic::OnMouseMove(nFlags, point);
	// Handle mouse move event here
	//SetWindowTextW(L"");
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif
protected:
	CToolTipCtrl m_Mytip;		//鼠标指向时的工具提示

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMClickSyslinkMail(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickSyslinkUpdate(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_MAIL, &CAboutDlg::OnNMClickSyslinkMail)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_UPDATE, &CAboutDlg::OnNMClickSyslinkUpdate)
END_MESSAGE_MAP()


BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Mytip.Create(this);
	m_Mytip.AddTool(GetDlgItem(IDC_SYSLINK_MAIL), _T("向作者发送电子邮件\r\nmailto:zhongyang219@hotmail.com"));	//添加工具提示
	m_Mytip.AddTool(GetDlgItem(IDC_SYSLINK_UPDATE), _T("转到GitHub页面查看更新\r\nhttps://github.com/zhongyang219/WindowResizer/releases"));	//添加工具提示
	m_Mytip.SetDelayTime(300);	//设置延迟
	m_Mytip.SetMaxTipWidth(800);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CAboutDlg::OnNMClickSyslinkMail(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add control notification handler code here
	ShellExecute(NULL, _T("open"), _T("mailto:zhongyang219@hotmail.com"), NULL, NULL, SW_SHOW);
	*pResult = 0;
}


void CAboutDlg::OnNMClickSyslinkUpdate(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add control notification handler code here
	ShellExecute(NULL, _T("open"), _T("https://github.com/zhongyang219/WindowResizer/releases"), NULL, NULL, SW_SHOW);
	*pResult = 0;
}


// CWindowResizerDlg 对话框

CWindowResizerDlg::CWindowResizerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_WINDOWRESIZER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWindowResizerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HANDLE_EDIT, m_handle_edit);
	DDX_Control(pDX, IDC_TITLE_EDIT, m_title_edit);
	DDX_Control(pDX, IDC_CLASS_NAME_EDIT, m_class_name_edit);
	DDX_Control(pDX, IDC_WIDTH_EDIT, m_width_edit);
	DDX_Control(pDX, IDC_HEIGHT_EDIT, m_height_edit);
	DDX_Control(pDX, IDC_PERCENTAGE_EDIT, m_percentage_edit);
	DDX_Control(pDX, IDC_ALWAYS_ON_TOP_CHECK, m_always_on_top_chk);
	DDX_Control(pDX, IDC_SHOW_IN_TASKBAR_CHECK, m_show_in_taskbar_chk);
	DDX_Control(pDX, IDC_INFO_STATIC, m_status_static);
	DDX_Control(pDX, IDC_MODIFY_CLINET_SIZE_CHECK, m_modify_clinet_size_chk);
}

bool CWindowResizerDlg::IsWindowValid()
{
	if (m_pWnd == nullptr)
		return false;
	HWND handle = m_pWnd->GetSafeHwnd();
	if (handle == NULL)
		return false;
	if (!IsWindow(handle))
		return false;
	return true;
}

void CWindowResizerDlg::SetControlEnable()
{
	if (IsWindowValid())
		m_percentage_edit.EnableWindow(m_user_define_scale);
	else
		m_percentage_edit.EnableWindow(FALSE);
}

void CWindowResizerDlg::EnableAllControls(bool enable)
{
	GetDlgItem(IDC_WIDTH_EDIT)->EnableWindow(enable);
	GetDlgItem(IDC_HEIGHT_EDIT)->EnableWindow(enable);
	GetDlgItem(IDC_SET_SIZE_BUTTON)->EnableWindow(enable);
	GetDlgItem(IDC_RADIO_50)->EnableWindow(enable);
	GetDlgItem(IDC_RADIO_100)->EnableWindow(enable);
	GetDlgItem(IDC_RADIO_125)->EnableWindow(enable);
	GetDlgItem(IDC_RADIO_150)->EnableWindow(enable);
	GetDlgItem(IDC_RADIO_200)->EnableWindow(enable);
	GetDlgItem(IDC_RADIO_USER_DEFINE)->EnableWindow(enable);
	GetDlgItem(IDC_SET_SCALE_BUTTON)->EnableWindow(enable);
	GetDlgItem(IDC_NO_FRAM_BUTTON)->EnableWindow(enable);
	GetDlgItem(IDC_RESTORE_NO_FRAM_BUTTON)->EnableWindow(enable);
	GetDlgItem(IDC_FORCE_MIXIMIZE_BUTTON)->EnableWindow(enable);
	GetDlgItem(IDC_MIXIMIZE_RESTORE_BUTTON)->EnableWindow(enable);
	GetDlgItem(IDC_FORCE_SIZABLE_BUTTON)->EnableWindow(enable);
	GetDlgItem(IDC_CENTER_BUTTON)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTON_ROUND_CORNER)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTON_720P)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTON_1080P)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTON_1440P)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTON_2160P)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTON_640)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTON_800)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTON_1024)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTON_1152)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTON_REFRESH)->EnableWindow(enable);
	m_always_on_top_chk.EnableWindow(enable);
	m_show_in_taskbar_chk.EnableWindow(enable);
	SetControlEnable();
}

void CWindowResizerDlg::UpdateControlState()
{
	EnableAllControls(IsWindowValid());

	m_always_on_top_chk.SetCheck(CCommon::IsWindowAlwaysOnTop(m_window_handle));
	m_show_in_taskbar_chk.SetCheck(CCommon::IsWindowShownInTaskBar(m_window_handle));
}

void CWindowResizerDlg::SetWindowSize(int width, int height)
{
	//修改客户区大小
	if (IsDlgButtonChecked(IDC_MODIFY_CLINET_SIZE_CHECK))
	{
		CRect clientrect;
		::GetClientRect(m_window_handle, &clientrect);
		CRect windowrect;
		::GetWindowRect(m_window_handle, &windowrect);
		int client_width = width + windowrect.Width() - clientrect.Width();
		int client_height = height + windowrect.Height() - clientrect.Height();
		::SetWindowPos(m_window_handle, NULL, 0, 0, client_width, client_height, SWP_NOZORDER | SWP_NOMOVE);
	}
	//修改窗口大小
	else
	{
		::SetWindowPos(m_window_handle, NULL, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE);
	}
}

BEGIN_MESSAGE_MAP(CWindowResizerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_FIND_WINDOW_BUTTON, &CWindowResizerDlg::OnBnClickedFindWindowButton)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SET_SIZE_BUTTON, &CWindowResizerDlg::OnBnClickedSetSizeButton)
	ON_BN_CLICKED(IDC_RADIO_50, &CWindowResizerDlg::OnBnClickedRadio50)
	ON_BN_CLICKED(IDC_RADIO_100, &CWindowResizerDlg::OnBnClickedRadio100)
	ON_BN_CLICKED(IDC_RADIO_125, &CWindowResizerDlg::OnBnClickedRadio125)
	ON_BN_CLICKED(IDC_RADIO_150, &CWindowResizerDlg::OnBnClickedRadio150)
	ON_BN_CLICKED(IDC_RADIO_200, &CWindowResizerDlg::OnBnClickedRadio200)
	ON_BN_CLICKED(IDC_RADIO_USER_DEFINE, &CWindowResizerDlg::OnBnClickedRadioUserDefine)
	ON_BN_CLICKED(IDC_NO_FRAM_BUTTON, &CWindowResizerDlg::OnBnClickedNoFramButton)
	ON_BN_CLICKED(IDC_FORCE_MIXIMIZE_BUTTON, &CWindowResizerDlg::OnBnClickedForceMiximizeButton)
	ON_BN_CLICKED(IDC_RESTORE_NO_FRAM_BUTTON, &CWindowResizerDlg::OnBnClickedRestoreNoFramButton)
	ON_BN_CLICKED(IDC_MIXIMIZE_RESTORE_BUTTON, &CWindowResizerDlg::OnBnClickedMiximizeRestoreButton)
	ON_BN_CLICKED(IDC_FORCE_SIZABLE_BUTTON, &CWindowResizerDlg::OnBnClickedForceSizableButton)
	ON_BN_CLICKED(IDC_SET_SCALE_BUTTON, &CWindowResizerDlg::OnBnClickedSetScaleButton)
	ON_BN_CLICKED(IDC_ABOUT_BUTTON, &CWindowResizerDlg::OnBnClickedAboutButton)
	ON_BN_CLICKED(IDC_ALWAYS_ON_TOP_CHECK, &CWindowResizerDlg::OnBnClickedAlwaysOnTopCheck)
	ON_BN_CLICKED(IDC_SHOW_IN_TASKBAR_CHECK, &CWindowResizerDlg::OnBnClickedShowInTaskbarCheck)
	ON_BN_CLICKED(IDC_CENTER_BUTTON, &CWindowResizerDlg::OnBnClickedCenterButton)
	ON_BN_CLICKED(IDC_BUTTON_ROUND_CORNER, &CWindowResizerDlg::OnBnClickedButtonRoundCorner)
	ON_BN_CLICKED(IDC_BUTTON_720P, &CWindowResizerDlg::OnBnClickedButton720p)
	ON_BN_CLICKED(IDC_BUTTON_1080P, &CWindowResizerDlg::OnBnClickedButton1080p)
	ON_BN_CLICKED(IDC_BUTTON_1440P, &CWindowResizerDlg::OnBnClickedButton1440p)
	ON_BN_CLICKED(IDC_BUTTON_2160P, &CWindowResizerDlg::OnBnClickedButton2160p)
	ON_BN_CLICKED(IDC_BUTTON_640, &CWindowResizerDlg::OnBnClickedButton640)
	ON_BN_CLICKED(IDC_BUTTON_800, &CWindowResizerDlg::OnBnClickedButton800)
	ON_BN_CLICKED(IDC_BUTTON_1024, &CWindowResizerDlg::OnBnClickedButton1024)
	ON_BN_CLICKED(IDC_BUTTON_1152, &CWindowResizerDlg::OnBnClickedButton1152)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CWindowResizerDlg::OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_ELEVATE, &CWindowResizerDlg::OnBnClickedButtonElevate)
END_MESSAGE_MAP()


// CWindowResizerDlg 消息处理程序

BOOL CWindowResizerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	UpdateTitle();
	EnableAllControls(false);
	((CButton*)GetDlgItem(IDC_RADIO_100))->SetCheck(TRUE);
	SetTimer(TIMER2_ID, 1000, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWindowResizerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWindowResizerDlg::OnPaint()
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
HCURSOR CWindowResizerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CWindowResizerDlg::OnBnClickedFindWindowButton()
{
	// TODO: Add control notification handler code here
	SetTimer(TIMER_ID, 1000, NULL);
	m_find_countdown = 5; // 5 second wait until user activates target window
	PrintFindCountdown();
	GetDlgItem(IDC_FIND_WINDOW_BUTTON)->EnableWindow(FALSE);
}

void CWindowResizerDlg::PrintFindCountdown()
{
	CString str;
	str.Format(L"The search window button has been pressed, please activate the window to be searched in %d seconds.", m_find_countdown);
	m_status_static.SetWindowText(str);
}

void CWindowResizerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add message handler code and/or call defaults here
	if (nIDEvent == TIMER_ID)
	{
		m_find_countdown--;
		PrintFindCountdown();
		if (m_find_countdown == 0)
		{
			CWnd* pWnd = GetForegroundWindow();
			m_window_handle = pWnd->GetSafeHwnd();
			if (m_window_handle != NULL && m_window_handle != m_hWnd)		//点击了开始查找按钮后，如果处于前台的窗口不是程序本身的窗口，就说明找到了
			{
				m_pWnd = pWnd;
				CString handle_str;
				handle_str.Format(_T("%.8x"), m_window_handle);
				handle_str.MakeUpper();
				m_handle_edit.SetWindowText(handle_str);

				pWnd->GetWindowText(m_window_title);
				m_title_edit.SetWindowText(m_window_title);

				TCHAR buff[256];
				GetClassName(m_window_handle, buff, 256);		//获取前台窗口的类名
				m_class_name_edit.SetWindowText(buff);

				//获取窗口的大小
				CRect rect;
				::GetWindowRect(m_window_handle, rect);
				m_window_size = rect.Size();
				CString str;
				str.Format(_T("%d"), m_window_size.cx);
				m_width_edit.SetWindowText(str);
				str.Format(_T("%d"), m_window_size.cy);
				m_height_edit.SetWindowText(str);

				UpdateControlState();

				KillTimer(TIMER_ID);

				GetDlgItem(IDC_FIND_WINDOW_BUTTON)->EnableWindow(TRUE);

				m_status_static.SetWindowText(L"");
				CheckElevation();

				SetForegroundWindowInternal(m_hWnd);
			}
			else
			{
				// reset countdown and continue waiting while not focused another window
				m_find_countdown = 6; // 5+1 second because of m_find_countdown-- first
			}
		}
	}
	else if (nIDEvent == TIMER2_ID)
	{
		//Checks the validity of the window to be manipulated every 1 second
		//and invokes the enabled or disabled state of the controls
		UpdateControlState();
	}

	CDialog::OnTimer(nIDEvent);
}

// Return values:
//  0 - not elevated
//  1 - elevated
// >1 - error code (GetLastError)
DWORD CWindowResizerDlg::ProcessElevation(HANDLE hproc)
{
	HANDLE hToken = NULL;
	TOKEN_ELEVATION elevation = { 0 };
	DWORD cbSize = sizeof(TOKEN_ELEVATION);

	if (!OpenProcessToken(hproc, TOKEN_QUERY, &hToken))
	{
		DWORD err = GetLastError();
		CString str;
		str.Format(L"Failed to open process token : %d", err);
		m_status_static.SetWindowText(str);
		return err;
	}
	if (!GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &cbSize))
	{
		DWORD err = GetLastError();
		CString str;
		str.Format(L"Failed to get token information : %d", err);
		m_status_static.SetWindowText(str);
		return err;
	}
	CloseHandle(hToken);

	return elevation.TokenIsElevated ? 1 : 0;
}

void CWindowResizerDlg::CheckElevation()
{
	DWORD processId = 0;
	GetWindowThreadProcessId(m_pWnd->m_hWnd, &processId);
	HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processId);
	if (!processHandle)
	{
		DWORD err = GetLastError();
		CString str;
		str.Format(L"Failed to access window process : %d", err);
		if (err == 5 /*access denied*/)
			str += L"  WindowResizer needs to be restarted elevated to be able to operate on this window.";
		m_status_static.SetWindowText(str);
		return;
	}

	// this is probably never happen because access error above
	if (ProcessElevation(GetCurrentProcess()) == 0 && ProcessElevation(processHandle) == 1)
	{
		m_status_static.SetWindowText(L"Target window is elevated. WindowResizer needs to be restarted elevated to be able to operate on this window.");
	}

	CloseHandle(processHandle);
}


void CWindowResizerDlg::OnBnClickedSetSizeButton()
{
	// TODO: Add control notification handler code here
	if (IsWindowValid())
	{
		int width;
		int height;
		CString temp;
		m_width_edit.GetWindowText(temp);
		width = _ttoi(temp);
		m_height_edit.GetWindowText(temp);
		height = _ttoi(temp);
		if (width == 0 || height == 0)
			return;

		SetWindowSize(width, height);
		//GetClientR
	}
}


void CWindowResizerDlg::OnBnClickedRadio50()
{
	// TODO: Add control notification handler code here
	m_scale_percentage = 50;
	m_user_define_scale = false;
	SetControlEnable();
}


void CWindowResizerDlg::OnBnClickedRadio100()
{
	// TODO: Add control notification handler code here
	m_scale_percentage = 100;
	m_user_define_scale = false;
	SetControlEnable();
}

void CWindowResizerDlg::OnBnClickedRadio125()
{
	// TODO: Add control notification handler code here
	m_scale_percentage = 125;
	m_user_define_scale = false;
	SetControlEnable();
}


void CWindowResizerDlg::OnBnClickedRadio150()
{
	// TODO: Add control notification handler code here
	m_scale_percentage = 150;
	m_user_define_scale = false;
	SetControlEnable();
}


void CWindowResizerDlg::OnBnClickedRadio200()
{
	// TODO: Add control notification handler code here
	m_scale_percentage = 200;
	m_user_define_scale = false;
	SetControlEnable();
}


void CWindowResizerDlg::OnBnClickedRadioUserDefine()
{
	// TODO: Add control notification handler code here
	m_user_define_scale = true;
	SetControlEnable();
}


//Setting a window without borders
void CWindowResizerDlg::OnBnClickedNoFramButton()
{
	// TODO: Add control notification handler code here
	if (!IsWindowValid())
		return;
	m_pWnd->ModifyStyle(WS_CAPTION, 0);		//Remove the title bar
	//m_pWnd->ModifyStyleEx(WS_EX_DLGMODALFRAME, 0);	 //Remove the border
}


//Restore window without border
void CWindowResizerDlg::OnBnClickedRestoreNoFramButton()
{
	// TODO: Add control notification handler code here
	if (!IsWindowValid())
		return;
	m_pWnd->ModifyStyle(0, WS_CAPTION);		//Restore title bar
	//m_pWnd->ModifyStyleEx(0, WS_EX_DLGMODALFRAME);	 //Restoree the border
}


void CWindowResizerDlg::OnBnClickedForceMiximizeButton()
{
	// TODO: Add control notification handler code here
	if (!IsWindowValid())
		return;
	m_pWnd->ShowWindow(SW_SHOWMAXIMIZED);	//最大化

}


void CWindowResizerDlg::OnBnClickedMiximizeRestoreButton()
{
	// TODO: Add control notification handler code here
	if (!IsWindowValid())
		return;
	m_pWnd->ShowWindow(SW_RESTORE);	//还原
}


void CWindowResizerDlg::OnBnClickedForceSizableButton()
{
	// TODO: Add control notification handler code here
	if (!IsWindowValid())
		return;
	m_pWnd->ModifyStyle(0, WS_THICKFRAME);
	m_pWnd->ModifyStyle(DS_MODALFRAME, 0);
	m_pWnd->ModifyStyleEx(WS_EX_DLGMODALFRAME, 0);
}


void CWindowResizerDlg::OnBnClickedSetScaleButton()
{
	// TODO: Add control notification handler code here
	if (!IsWindowValid())
		return;
	if (m_user_define_scale)
	{
		CString temp;
		m_percentage_edit.GetWindowText(temp);
		m_scale_percentage = _ttoi(temp);
		if (m_scale_percentage <= 0)
			return;
	}
	int width;
	int height;
	width = m_window_size.cx * m_scale_percentage / 100;
	height = m_window_size.cy * m_scale_percentage / 100;
	SetWindowSize(width, height);
}


void CWindowResizerDlg::OnBnClickedAboutButton()
{
	// TODO: Add control notification handler code here
	CAboutDlg dlg;
	dlg.DoModal();
}



void CWindowResizerDlg::OnBnClickedAlwaysOnTopCheck()
{
	// TODO: Add control notification handler code here
	bool check = m_always_on_top_chk.GetCheck() != 0;
	CCommon::SetWindowAlwaysOnTop(m_window_handle, check);
}


void CWindowResizerDlg::OnBnClickedShowInTaskbarCheck()
{
	// TODO: Add control notification handler code here
	bool check = m_show_in_taskbar_chk.GetCheck() != 0;
	CCommon::ShowWindowInTaskBar(m_window_handle, check);
}


void CWindowResizerDlg::OnBnClickedCenterButton()
{
	// TODO: Add your control notification handler code here
	if (!IsWindowValid())
		return;
	HMONITOR monitor = MonitorFromWindow(m_window_handle, MONITOR_DEFAULTTONEAREST);
	MONITORINFO info = {};
	info.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(monitor, &info);
	int monitor_width = info.rcMonitor.right - info.rcMonitor.left;
	int monitor_height = info.rcMonitor.bottom - info.rcMonitor.top;

	CRect windowrect;
	::GetWindowRect(m_window_handle, &windowrect);
	int window_left = (monitor_width - windowrect.Width()) / 2 + info.rcMonitor.left;
	int window_top = (monitor_height - windowrect.Height()) / 2 + info.rcMonitor.top;
	::SetWindowPos(m_window_handle, NULL, window_left, window_top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}


void CWindowResizerDlg::OnBnClickedButtonRoundCorner()
{
	// TODO: Add your control notification handler code here
	if (!IsWindows11Above())
	{
		m_status_static.SetWindowText(L"This function is only supported on Windows 11 and above.");
		return;
	}

	INT attr = DWMWCP_DONOTROUND;
	HRESULT hr = ::DwmSetWindowAttribute(m_pWnd->m_hWnd, DWMWA_WINDOW_CORNER_PREFERENCE, &attr, sizeof(attr));
	if (FAILED(hr))
	{
		m_status_static.SetWindowText(L"Failed to disable window rounded corners.");
	}
}


bool CWindowResizerDlg::IsWindows11Above()
{
	RTL_OSVERSIONINFOW osVersionInfo = GetRealOSVersion();
	int majorVersion = static_cast<int>(osVersionInfo.dwMajorVersion);
	int currentBuild = static_cast<int>(osVersionInfo.dwBuildNumber);

	return majorVersion >= 10 && currentBuild >= 22000; // Windows 11版本10.0.22000
}


void CWindowResizerDlg::OnBnClickedButton720p()
{
	// TODO: Add your control notification handler code here
	CString str;
	str.Format(_T("%d"), 1280);
	m_width_edit.SetWindowText(str);
	str.Format(_T("%d"), 720);
	m_height_edit.SetWindowText(str);
	m_modify_clinet_size_chk.SetCheck(BST_CHECKED);
}
void CWindowResizerDlg::OnBnClickedButton1080p()
{
	// TODO: Add your control notification handler code here
	CString str;
	str.Format(_T("%d"), 1920);
	m_width_edit.SetWindowText(str);
	str.Format(_T("%d"), 1080);
	m_height_edit.SetWindowText(str);
	m_modify_clinet_size_chk.SetCheck(BST_CHECKED);
}
void CWindowResizerDlg::OnBnClickedButton1440p()
{
	// TODO: Add your control notification handler code here
	CString str;
	str.Format(_T("%d"), 2560);
	m_width_edit.SetWindowText(str);
	str.Format(_T("%d"), 1440);
	m_height_edit.SetWindowText(str);
	m_modify_clinet_size_chk.SetCheck(BST_CHECKED);
}
void CWindowResizerDlg::OnBnClickedButton2160p()
{
	// TODO: Add your control notification handler code here
	CString str;
	str.Format(_T("%d"), 3840);
	m_width_edit.SetWindowText(str);
	str.Format(_T("%d"), 2160);
	m_height_edit.SetWindowText(str);
	m_modify_clinet_size_chk.SetCheck(BST_CHECKED);
}
void CWindowResizerDlg::OnBnClickedButton640()
{
	// TODO: Add your control notification handler code here
	CString str;
	str.Format(_T("%d"), 640);
	m_width_edit.SetWindowText(str);
	str.Format(_T("%d"), 480);
	m_height_edit.SetWindowText(str);
	m_modify_clinet_size_chk.SetCheck(BST_CHECKED);
}
void CWindowResizerDlg::OnBnClickedButton800()
{
	// TODO: Add your control notification handler code here
	CString str;
	str.Format(_T("%d"), 800);
	m_width_edit.SetWindowText(str);
	str.Format(_T("%d"), 600);
	m_height_edit.SetWindowText(str);
	m_modify_clinet_size_chk.SetCheck(BST_CHECKED);
}
void CWindowResizerDlg::OnBnClickedButton1024()
{
	// TODO: Add your control notification handler code here
	CString str;
	str.Format(_T("%d"), 1024);
	m_width_edit.SetWindowText(str);
	str.Format(_T("%d"), 768);
	m_height_edit.SetWindowText(str);
	m_modify_clinet_size_chk.SetCheck(BST_CHECKED);
}
void CWindowResizerDlg::OnBnClickedButton1152()
{
	// TODO: Add your control notification handler code here
	CString str;
	str.Format(_T("%d"), 1152);
	m_width_edit.SetWindowText(str);
	str.Format(_T("%d"), 864);
	m_height_edit.SetWindowText(str);
	m_modify_clinet_size_chk.SetCheck(BST_CHECKED);
}


void CWindowResizerDlg::OnBnClickedButtonRefresh()
{
	// TODO: Add your control notification handler code here
	if (!IsWindowValid())
		return;
	m_pWnd->ShowWindow(SW_MINIMIZE);
	m_pWnd->ShowWindow(SW_RESTORE);
	SetForegroundWindowInternal(m_hWnd);
}


void CWindowResizerDlg::OnBnClickedButtonElevate()
{
	// TODO: Add your control notification handler code here
	HINSTANCE result = ShellExecute(NULL, L"runas", GetExecutablePath(), NULL, NULL, SW_SHOWNORMAL);
	if (result <= (HINSTANCE)32)
	{
		DWORD err = GetLastError();
		CString str;
		str.Format(L"Failed to restart elevated : %d", err);
		MessageBox(str, L"WindowResizer", MB_OK | MB_ICONERROR);
	}
	else
	{
		AfxGetMainWnd()->PostMessage(WM_CLOSE);
	}
}

void CWindowResizerDlg::UpdateTitle()
{
	DWORD elevated = ProcessElevation(GetCurrentProcess());
	CString title = L"WindowResizer";
	if (elevated == 1)
		title += L" (Administrator)";
	SetWindowText(title);
}
