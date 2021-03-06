// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "SnapshotDlg.h"

class CMainDlg : public SHostWnd
{
public:
	CMainDlg();
	~CMainDlg();

	void OnClose();
	void OnStart();
	
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);

	LRESULT OnUserDefinedMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
		//soui消息
	EVENT_MAP_BEGIN()
		EVENT_ID_COMMAND(R.id.btn_exit, OnClose)
		EVENT_ID_COMMAND(R.id.btn_start, OnStart)
	EVENT_MAP_END()
		
	//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX(CMainDlg)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		MESSAGE_RANGE_HANDLER_EX(WM_USER, WM_USER+10000, OnUserDefinedMsg)
		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()
private:
	BOOL			m_bLayoutInited;

	bool			m_Captured;
};
