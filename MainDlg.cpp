// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"
	
#ifdef DWMBLUR	//win7毛玻璃开关
#include <dwmapi.h>
#pragma comment(lib,"dwmapi.lib")
#endif

#define ORVALUE 0x8000
HHOOK g_Keyboard_Hook = 0;
HWND  g_hWnd;

/*
@	全局键盘钩子回调函数
*/
LRESULT CALLBACK Keyboard_Proc (int code, WPARAM wParam, LPARAM lParam)
{
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;

	int nControl = GetKeyState(VK_CONTROL);
	int nShift = GetKeyState(VK_SHIFT);
	if (nShift & ORVALUE && nControl & ORVALUE)
	{
		if (wParam == WM_KEYDOWN && p->vkCode == 'A')
		{
			SendMessage(g_hWnd, WM_CAPTURE_SCREEN, 0,0);
			return 1;
		}
	}

	return CallNextHookEx (g_Keyboard_Hook, code, wParam, lParam);
};
	
CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:XML_MAINWND"))
{
	m_bLayoutInited = FALSE;
	m_Captured = false;
}

CMainDlg::~CMainDlg()
{
	//卸载钩子
	UnhookWindowsHookEx (g_Keyboard_Hook);
}

int CMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	#ifdef DWMBLUR	//win7毛玻璃开关
	MARGINS mar = {5,5,30,5};
	DwmExtendFrameIntoClientArea ( m_hWnd, &mar );
	#endif

	SetMsgHandled(FALSE);
	return 0;
}

BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	m_bLayoutInited = TRUE;
	ShowWindow(SW_HIDE);

	g_hWnd = this->m_hWnd;
	//装载钩子
	g_Keyboard_Hook = 
		SetWindowsHookEx(WH_KEYBOARD_LL,
		&Keyboard_Proc,
		0,
		0);
	if (g_Keyboard_Hook == NULL)
	{
		MessageBox(NULL, L"SetWindowsHookEx failed!", NULL, IDOK);
	}

	OnStart();
	return 0;
}
//TODO:消息映射
void CMainDlg::OnClose()
{
	CSimpleWnd::DestroyWindow();
}

void CMainDlg::OnStart()
{
	if (!m_Captured)
	{
		m_Captured = true;
		CSnapshotDlg snapshotDlg;
		snapshotDlg.DoModal();
		m_Captured = false;
	}
}

LRESULT CMainDlg::OnUserDefinedMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (WM_CAPTURE_SCREEN == uMsg)
		OnStart();

	return 0;
}