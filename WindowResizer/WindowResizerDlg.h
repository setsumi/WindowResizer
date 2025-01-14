
// WindowResizerDlg.h: 头文件
//

#pragma once
#include "afxwin.h"

class CMyStatic : public CStatic
{
public:
	DECLARE_MESSAGE_MAP()
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


// CWindowResizerDlg 对话框
class CWindowResizerDlg : public CDialog
{
	// 构造
public:
	CWindowResizerDlg(CWnd* pParent = NULL);	// 标准构造函数

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WINDOWRESIZER_DIALOG };
#endif

protected:
	CEdit m_handle_edit;
	CEdit m_title_edit;
	CEdit m_class_name_edit;
	CEdit m_width_edit;
	CEdit m_height_edit;
	CEdit m_percentage_edit;
	CButton m_always_on_top_chk;
	CButton m_show_in_taskbar_chk;
	CMyStatic m_status_static;
	CButton m_modify_clinet_size_chk;

	HWND m_window_handle = NULL;	//Handle of the found window
	CString m_window_title;			//Title of the found window
	CString m_class_name;			//Class name of the found window
	CWnd* m_pWnd = NULL;			//Pointer to the found window
	CSize m_window_size;			//Original size of the found window

	int m_scale_percentage{ 100 };		//窗口要缩放的百分比
	bool m_user_define_scale{ false };

	int m_find_countdown{ 0 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	bool IsWindowValid();		//Determine whether the current window to be operated is valid
	void SetControlEnable();
	void EnableAllControls(bool enable);
	void UpdateControlState();
	void SetWindowSize(int width, int height);
	bool IsWindows11Above();
	void PrintFindCountdown();
	void CheckElevation();
	DWORD ProcessElevation(HANDLE hproc);
	void UpdateTitle();


	// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedFindWindowButton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedSetSizeButton();
	afx_msg void OnBnClickedRadio50();
	afx_msg void OnBnClickedRadio100();
	afx_msg void OnBnClickedRadio125();
	afx_msg void OnBnClickedRadio150();
	afx_msg void OnBnClickedRadio200();
	afx_msg void OnBnClickedRadioUserDefine();
	afx_msg void OnBnClickedNoFramButton();
	afx_msg void OnBnClickedForceMiximizeButton();
	afx_msg void OnBnClickedRestoreNoFramButton();
	afx_msg void OnBnClickedMiximizeRestoreButton();
	afx_msg void OnBnClickedForceSizableButton();
	afx_msg void OnBnClickedSetScaleButton();
	afx_msg void OnBnClickedAboutButton();
	afx_msg void OnBnClickedAlwaysOnTopCheck();
	afx_msg void OnBnClickedShowInTaskbarCheck();
	afx_msg void OnBnClickedCenterButton();
	afx_msg void OnBnClickedButtonRoundCorner();
	afx_msg void OnBnClickedButton720p();
	afx_msg void OnBnClickedButton1080p();
	afx_msg void OnBnClickedButton1440p();
	afx_msg void OnBnClickedButton2160p();
	afx_msg void OnBnClickedButton640();
	afx_msg void OnBnClickedButton800();
	afx_msg void OnBnClickedButton1024();
	afx_msg void OnBnClickedButton1152();
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnBnClickedButtonElevate();
};
