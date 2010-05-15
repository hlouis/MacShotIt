// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"

#include "Utility.h"

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	m_mt = MT_NONE;
	ChangeCursor(m_mt);

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd); // This tells CUpdateUI that our dialog contains child windows that will need updating.

	// Make the dialog same size as the desktop
	int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	//SetWindowPos(HWND_TOPMOST, 0, 0, nScreenWidth, nScreenHeight, 0);
	SetWindowPos(HWND_TOP, 0, 0, nScreenWidth, nScreenHeight, 0);

	// Capture the screen
	m_pImg = NULL;
	TakeScreenShot();

	if (m_pImg)
	{
		Graphics grap(GetDC());
		grap.DrawImage(m_pImg, 0, 0, m_pImg->GetWidth(), m_pImg->GetHeight());
	}

	// Initial mouse point
	m_startPT.x = -1;
	m_startPT.y = -1;
	m_rc = CRect(0, 0, 0, 0);
//	m_bClicked = false;

	// Initial desktop folder and file name
	int nFolder = CSIDL_DESKTOPDIRECTORY;
 	WTL::CString path;
	LPITEMIDLIST idList;
	LPMALLOC pMalloc=0;
	if(SUCCEEDED(SHGetSpecialFolderLocation(0, nFolder, &idList)))
	{
		SHGetPathFromIDList( idList, path.GetBuffer(_MAX_PATH*4));
		path.ReleaseBuffer();
		if (SUCCEEDED(SHGetMalloc(&pMalloc)))
		{
			pMalloc->Free(idList);
			pMalloc->Release();
		}
	}

	SYSTEMTIME st;
	GetLocalTime(&st);
	WTL::CString fileName;
	fileName.Format("\\%u%002u%002u%002u%002u%002u%s", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, ".png");

	LPWSTR lpwPath = cloud::Lpstr2Lpwstr(path);
	LPWSTR lpwFilename = cloud::Lpstr2Lpwstr(fileName);
 	m_path = lpwPath;
	m_path += lpwFilename;

	delete lpwPath;
	delete lpwFilename;

	ShowWindow(SW_SHOW);

	return TRUE;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	if (m_pImg)
		delete m_pImg;

	DestroyWindow();
	::PostQuitMessage(nVal);
}


/************************************************************************/
// Helper function
/************************************************************************/
bool CMainDlg::TakeScreenShot()
{
	if (m_pImg)
	{
		delete m_pImg;
		m_pImg = NULL;
	}

	int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	HWND hDesktopWnd = GetDesktopWindow();
	HDC hDesktopDC = ::GetDC(hDesktopWnd);
	HDC hCaptureDC = CreateCompatibleDC(hDesktopDC);
	HBITMAP hCaptureBitmap =CreateCompatibleBitmap(hDesktopDC, 
		nScreenWidth, nScreenHeight);
	SelectObject(hCaptureDC,hCaptureBitmap); 
	BitBlt(hCaptureDC,0,0,nScreenWidth,nScreenHeight,hDesktopDC,0,0,SRCCOPY); 

	HPALETTE hpal = (HPALETTE)GetCurrentObject(hDesktopDC, OBJ_PAL);
	m_pImg = Bitmap::FromHBITMAP(hCaptureBitmap, hpal);

	//SaveCapturedBitmap(hCaptureBitmap); //Place holder - Put your code
	//here to save the captured image to disk
	::ReleaseDC(hDesktopWnd,hDesktopDC);
	DeleteDC(hCaptureDC);
	DeleteObject(hCaptureBitmap);

	// test code
// 	CLSID pngClsid;
// 	GetEncoderClsid(L"image/png", &pngClsid);
// 	m_pImg->Save(L"test.png", &pngClsid, NULL);
 	return true;
}

int CMainDlg::GetEncoderClsid(const   WCHAR*   format,   CLSID*   pClsid) 
{  
	UINT num = 0;                     // number of image encoders  
	UINT size = 0;                   // size of the image encoder array in bytes  

	ImageCodecInfo* pImageCodecInfo = NULL;  

	GetImageEncodersSize(&num, &size);  
	if(size == 0)  
		return -1;     //   Failure  

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));  
	if(pImageCodecInfo == NULL)  
		return   -1;     //   Failure  

	GetImageEncoders(num, size, pImageCodecInfo);  

	for(UINT j = 0; j < num; ++j)  
	{  
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )  
		{  
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;     // Success  
		}
	}

	free(pImageCodecInfo);
	return -1;     // Failure  
}
LRESULT CMainDlg::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// Use CPaintDC to draw only invalidate rect
	CPaintDC dc(m_hWnd);

	if (m_pImg)
	{
 		Graphics grap(dc.m_hDC);
		Bitmap* pBitmap = new Bitmap(m_pImg->GetWidth(), m_pImg->GetHeight(), &grap);
		Graphics grapImage(pBitmap);
 		grapImage.DrawImage(m_pImg, 0, 0, m_pImg->GetWidth(), m_pImg->GetHeight());
		DrawHighlightBox(grapImage);
		grap.DrawImage(pBitmap, 0, 0, m_pImg->GetWidth(), m_pImg->GetHeight());
	}

	dc.RestoreDC(-1);

	return 0;
}

LRESULT CMainDlg::OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return 0;
}
LRESULT CMainDlg::OnSetCursor(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	SetCursor(m_cursor);
	return 0;
}

void CMainDlg::ChangeCursor(CMainDlg::MOUSE_TYPE mt)
{
	HINSTANCE hInst = _AtlBaseModule.GetResourceInstance();
	if (mt == MT_NONE)
	{
		m_cursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSMAIN));
	}
	if (mt == MT_MOVE)
	{
		m_cursor = LoadCursor(NULL, IDC_SIZEALL);
	}
}

LRESULT CMainDlg::OnLButtonDown(UINT nFlag, CPoint pt)
{
	ATLTRACE("Left button down.\n");

	if (m_rc.PtInRect(pt))
	{
		m_mt = MT_MOVE;
	}
	else if (m_rc.Size() == CSize(0, 0))
	{
		m_mt = MT_MAKERECT;
	}

	m_startPT = pt;
//	m_bClicked = true;

	return 0;
}

LRESULT CMainDlg::OnMouseDBLClick(UINT nFlag, CPoint pt)
{
	ATLTRACE("Doublick button down.\n");
	if (m_rc.PtInRect(pt))
	{
		SaveImage();
		CloseDialog(1);
	}
	return 0;
}


void CMainDlg::OnMouseMove(UINT nFlag, CPoint pt)
{
	if (m_rc.PtInRect(pt))
		ChangeCursor(MT_MOVE);
	else
		ChangeCursor(MT_NONE);

	if (m_mt == MT_NONE)
		return;

	if (m_mt == MT_MAKERECT)
	{
		MakeRect(m_startPT, pt);
	}

	if (m_mt == MT_MOVE)
	{
		int dx = pt.x - m_startPT.x;
		int dy = pt.y - m_startPT.y;
		m_rc.OffsetRect(CSize(dx, dy));
		m_startPT = pt;
	}


//	InvalidateRect(&m_rc);
	Invalidate();
}

LRESULT CMainDlg::OnLButtonUp(UINT nFlag, CPoint pt)
{
// 	if (m_startPT == CPoint(-1, -1))
// 		CloseDialog(0);

// 	m_endPT = pt;
//	m_bClicked = false;

//	SaveImage();

//	CloseDialog(1);

	m_mt = MT_NONE;

	return 0;
}

void CMainDlg::MakeRect(const CPoint& pt1, const CPoint& pt2)
{
	m_rc.left = pt1.x <= pt2.x ? pt1.x : pt2.x;
	m_rc.top = pt1.y <= pt2.y ? pt1.y : pt2.y;
	m_rc.right = pt1.x >= pt2.x ? pt1.x : pt2.x;
	m_rc.bottom = pt1.y >= pt2.y ? pt1.y : pt2.y;
}

void CMainDlg::SaveImage()
{
	if (m_rc.IsRectEmpty())
		return;

	if (m_pImg)
	{
		Image* pImg = m_pImg->Clone(m_rc.left, m_rc.top, m_rc.Width(), m_rc.Height(), PixelFormatDontCare);
		CLSID pngClsid;
		GetEncoderClsid(L"image/png", &pngClsid);
		pImg->Save(m_path, &pngClsid, NULL);
	}
}

void CMainDlg::DrawHighlightBox(Graphics& grap)
{
	if (m_rc.IsRectEmpty() || m_rc.IsRectNull())
		return;
	
	Pen pen(Color(255, 128, 255, 128));
	SolidBrush brush(Color(100, 128, 255, 128));
	grap.FillRectangle(&brush, m_rc.left, m_rc.top, m_rc.Width(), m_rc.Height());
	grap.DrawRectangle(&pen, m_rc.left, m_rc.top, m_rc.Width(), m_rc.Height());

	// Draw 
}