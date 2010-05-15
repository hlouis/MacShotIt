// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{
public:
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
		/*MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)*/
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_LBUTTONDBLCLK(OnMouseDBLClick)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	void CloseDialog(int nVal);

//////////////////////////////////////////////////////////////////////////
// Helper function
private:

	enum MOUSE_TYPE
	{
		MT_NONE,
		MT_MAKERECT,
		MT_MOVE,
		MT_TOPLEFT,
		MT_TOPRIGHT,
		MT_BOTLEFT,
		MT_BOTRIGHT
	};
	MOUSE_TYPE	m_mt;

	bool TakeScreenShot();
	int GetEncoderClsid(const   WCHAR*   format,   CLSID*   pClsid);
	void MakeRect(const CPoint& pt1, const CPoint& pt2);
	void SaveImage();
	void DrawHighlightBox(Graphics& grap);
	void ChangeCursor(MOUSE_TYPE mt);

//////////////////////////////////////////////////////////////////////////
// Member data
private:
	Bitmap* m_pImg;
	CPoint m_startPT;
// 	CPoint m_endPT;
	CRect  m_rc;
	bool   m_bClicked;
	ATL::CStringW m_path;
	HCURSOR m_cursor;
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSetCursor(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonDown(UINT nFlag, CPoint pt);
	LRESULT OnLButtonUp(UINT nFlag, CPoint pt);
	LRESULT OnMouseDBLClick(UINT nFlag, CPoint pt);
	void OnMouseMove(UINT nFlag, CPoint pt);
};
