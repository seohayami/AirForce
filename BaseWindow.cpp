// Win32Project_WDC(managingAppState).cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "BaseWindow.h"
#include "resource1.h"
#include <windows.h>
#include <Windowsx.h>
#include <d2d1.h>
#include <list>
#include <memory>
#include <time.h>

#include <iostream>
#include <fstream>
#include <cstdlib>

#include <ShObjIdl.h>

#include <Winuser.h>

#pragma comment(lib, "d2d1")

#define MAX_LOADSTRING 100

using namespace std;





float GetRandomFloat0to1() {
	float x = static_cast<float>(rand()) / 32767.0f;
	return x;
}

/*
class DPIScale
{
	static float scaleX;
	static float scaleY;

	public: static void Initialize(ID2D1Factory *pFactory)
	{
		FLOAT dpiX, dpiY;
		pFactory->GetDesktopDpi(&dpiX, &dpiY);
		scaleX = dpiX/96.0f;
		scaleY = dpiY/96.0f;
	}

	template <typename T>
	static D2D1_POINT_2F PixelsToDips(T x, T y)
	{
		return D2D1::Point2F(static_cast<float>(x) / scaleX, 
				     static_cast<float>(y) / scaleY);
	}
};

 */
/////////////////////////////////////////////////////////////////////////////
//
//	instanciate static member variables of clas DPIScale
//
/////////////////////////////////////////////////////////////////////////////

float DPIScale::scaleX = 1.0f;
float DPIScale::scaleY = 1.0f;

LPCTSTR liststr[] = {L"Ju87D",L"Ju87D-5", L"Ju87G-1", 
	             L"Ju88C",L"Ju88A",L"HE111H",L"Me262A", L"Me262A-2a",   
		     L"ME109E", L"FW190A", L"FW190A-8/U11", L"FW190A-8/R1", 
		     L"ME109F", L"ME109F-4/B", L"ME109F-4/R1", 
		     L"ME110C", L"ME110G", L"ME110G-2/R3",
		     L"ME109G", L"ME109G-6", L"ME109G-6/U4",
		     L"Ju87B"};

LPCTSTR	liststr_Pilots[] 
		= {L"Random", L"Novice", L"Average", L"Ace"};
//		  {L"Random", L"Novice", L"Rank-R", L"Rank-T",  
//		   L"Rank-ET", L"Rank-RV", L"Rank-TR", 
//		   L"Rank-ETR",L"Rank-ETT", 
//		   L"Rank-ETTR", L"Rank-ETRV", L"Rank-ETTRV", L"Ace"}; 
// You need to add "L" to show that these char are wide(2byte)
// 


//template <class T> void SafeRelease(T **ppT)
//{
//	if (*ppT) {
//		(*ppT)->Release();
//		*ppT = NULL;
//	}
//}


MyEllipse MainWindow::sentEllipse;  //instanciate static member variable

// Recalculate drawing layout when the size of the window changes.

void MainWindow::CalculateLayout()
{
    if (pRenderTarget != NULL)
    {
        D2D1_SIZE_F size = pRenderTarget->GetSize();
        const float x = size.width / 2;
        const float y = size.height / 2;
        const float radius = min(x, y);
        ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
    }
}

bool MainWindow::HitTest(float dipX, float dipY) 
{
	std::list<std::shared_ptr<MyEllipse>>::iterator itr;

	for (itr = ellipses.begin(); itr != ellipses.end(); ++itr) {
		if ((*itr)->HitTest(dipX, dipY)) {
			selection = itr;
			return TRUE;
		}
	}
	selection = ellipses.end();
	return FALSE;
}



void MainWindow::InsertEllipse (float dipx, float dipy)
{
//	std::shared_ptr<MyEllipse>	 ptrEllipse = new MyEllipse;
//	MyEllipse*	 ptrEllipse = new MyEllipse;
	std::shared_ptr<MyEllipse> 	 ptrEllipse(new MyEllipse);

	ptrEllipse->ellipse.point.x = dipx;
	ptrEllipse->ellipse.point.y = dipy;
	ptrEllipse->ellipse.radiusX = 1.0f;
	ptrEllipse->ellipse.radiusY = 1.0f;

	ptrEllipse->color = D2D1::ColorF(
		GetRandomFloat0to1(),
		GetRandomFloat0to1(),
		GetRandomFloat0to1(),
		GetRandomFloat0to1());

	ellipses.push_front(ptrEllipse);

	selection = ellipses.begin();
}

void MainWindow::RemoveEllipse()
{
	(*selection).reset();
	ellipses.erase(selection);
	ClearSelection();
}

HRESULT MainWindow::CreateGraphicsResources()
{
    HRESULT hr = S_OK;

	D2D1_PIXEL_FORMAT	pixel_fmt
		= D2D1::PixelFormat(
			DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_PREMULTIPLIED
//			D2D1_ALPHA_MODE_UNKNOWN
//			D2D1_ALPHA_MODE_STRAIGHT
		);
	D2D1_RENDER_TARGET_PROPERTIES renderTargetProp
		= D2D1::RenderTargetProperties();
	renderTargetProp.pixelFormat = pixel_fmt;


    if (pRenderTarget == NULL)
    {
        RECT rc;
        GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        hr = pFactory->CreateHwndRenderTarget(
//            D2D1::RenderTargetProperties(),
            renderTargetProp,
            D2D1::HwndRenderTargetProperties(m_hwnd, size),
            &pRenderTarget);

        if (SUCCEEDED(hr))
        {
            const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
            hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);

            if (SUCCEEDED(hr))
            {
/*                CalculateLayout();
 */
            }
        }
    }
    return hr;
}



void MainWindow::DiscardGraphicsResources()
{
    SafeRelease(&pRenderTarget);
    SafeRelease(&pBrush);
}

void MainWindow::OnPaint()
{
    HRESULT hr = CreateGraphicsResources();
    if (SUCCEEDED(hr))
    {
        PAINTSTRUCT ps;
        BeginPaint(m_hwnd, &ps);
     
        pRenderTarget->BeginDraw();

        pRenderTarget->Clear( D2D1::ColorF(D2D1::ColorF::SkyBlue) );

	std::list<std::shared_ptr<MyEllipse>>::iterator itr;

	for (itr = ellipses.begin(); itr != ellipses.end(); ++itr) {
//		pRenderTarget->FillEllipse((*itr)->ellipse, pBrush);
		(*itr)->Draw(pRenderTarget, pBrush);
	}
        pRenderTarget->FillEllipse(ellipse, pBrush);

        hr = pRenderTarget->EndDraw();
        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
        {
            DiscardGraphicsResources();
        }
        EndPaint(m_hwnd, &ps);
    }
}


void MainWindow::Resize()
{
    if (pRenderTarget != NULL)
    {
        RECT rc;
        GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        pRenderTarget->Resize(size);
/*        CalculateLayout();
 */
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
}

void MainWindow::OnLButtonDown(int pixelX, int pixelY, DWORD flags)
{
/*	SetCapture(m_hwnd);
 *	ellipse.point = ptMouse = DPIScale::PixelsToDips(pixelX, pixelY);
 *	ellipse.radiusX = ellipse.radiusY = 1.0f;
 *	InvalidateRect(m_hwnd, NULL, FALSE);
 */
	
        ptMouse = DPIScale::PixelsToDips(pixelX, pixelY);
	const float dipX = ptMouse.x;
	const float dipY = ptMouse.y;

	if (mode == DrawMode) {
		POINT pt = {pixelX, pixelY};

		if (DragDetect(m_hwnd, pt)) {
			SetCapture(m_hwnd);
			// Start a new ellipse
			InsertEllipse(dipX, dipY);
		}else {
			if (HitTest(dipX, dipY)) {
				SetMode(SelectMode);
			}
		}
	} else {
		ClearSelection();
		if (HitTest(dipX, dipY)) {
			SetCapture(m_hwnd);
			ptMouse = Selection()->ellipse.point;
			ptMouse.x -= dipX;
			ptMouse.y -= dipY;

			SetMode(DragMode);
		} else {
			SetMode(DrawMode);
		}
	}
	InvalidateRect(m_hwnd, NULL, FALSE);

}

void MainWindow::OnPopRemove()
{
	RemoveEllipse();
	SetMode(DrawMode);
	InvalidateRect(m_hwnd, NULL, FALSE);
}

void MainWindow::OnPopSend()
{
	sentEllipse = **selection;
	SendMessage(PeerWindow(), WM_USER + 5, 0, 0);

	RemoveEllipse();
	SetMode(DrawMode);
	InvalidateRect(m_hwnd, NULL, FALSE);
}

void MainWindow::OnRButtonDown(int pixelX, int pixelY, DWORD flags)
{
        ptMouse = DPIScale::PixelsToDips(pixelX, pixelY);
	const float dipX = ptMouse.x;
	const float dipY = ptMouse.y;

	POINT pt;
	pt.x = pixelX;
	pt.y = pixelY;
	
	HMENU hmenu, hSubmenu;

	if (HitTest(dipX, dipY)) {
		hmenu = LoadMenu((HINSTANCE)GetWindowLong(m_hwnd, GWLP_HINSTANCE), 
				  MAKEINTRESOURCE(MYPOPUP));
		hSubmenu = GetSubMenu(hmenu, 0);
		ClientToScreen(m_hwnd, &pt);
		TrackPopupMenu(hSubmenu, TPM_LEFTALIGN, pt.x, pt.y, 0, m_hwnd, NULL);
		DestroyMenu(hmenu);
	}
}


void MainWindow::OnMouseMove(int pixelX, int pixelY, DWORD flags)
{
/*	if (flags & MK_LBUTTON) {
 *		const D2D1_POINT_2F dips = DPIScale::PixelsToDips(pixelX, pixelY);
 *		const float width = (dips.x - ptMouse.x) / 2;
 *		const float height = (dips.y - ptMouse.y) / 2;
 *		const float x1 = ptMouse.x + width;
 *		const float y1 = ptMouse.y + height;
 *
 *		ellipse = D2D1::Ellipse(D2D1::Point2F(x1, y1), width, height);
 *		InvalidateRect(m_hwnd, NULL, FALSE);
 *	}
 */

        D2D1_POINT_2F	pnt = DPIScale::PixelsToDips(pixelX, pixelY);
	const float dipX = pnt.x;
	const float dipY = pnt.y;
//	const float dipX = DPIScale::PixelsToDipsX(pixelX);
//	const float dipY = DPIScale::PixelsToDipsY(pixelY);

	if ((flags &MK_LBUTTON) && Selection()) {
		if (mode == DrawMode) {
			// Resize the ellipse
			const float width = (dipX - ptMouse.x) / 2;
			const float height = (dipY - ptMouse.y) / 2;
			const float x1 = ptMouse.x + width;
			const float y1 = ptMouse.y + height;

			Selection()->ellipse = D2D1::Ellipse(D2D1::Point2F(x1, y1), width, height);
		} else if (mode == DragMode) {
			// Move the ellipse
			Selection()->ellipse.point.x = dipX + ptMouse.x;
			Selection()->ellipse.point.y = dipY + ptMouse.y;

		}
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
}


void MainWindow::OnLButtonUp()
{
	if ((mode == DrawMode) && Selection()) {
		ClearSelection();
		InvalidateRect(m_hwnd, NULL, FALSE);
	} else if (mode == DragMode) {
		SetMode(SelectMode);
	}
	MessageBox(NULL, 
		   L"L Button Up. \n", 
		   NULL, 
		   MB_OKCANCEL | MB_ICONSTOP); 
	ReleaseCapture();
}


bool MainWindow::OnFileSave()
{
	PWSTR pszFilePath;

	if (fileMode == SpecifiedMode) {
		pszFilePath = pszFilePathSpecified;
	} else {
		HRESULT hr = CoInitializeEx(NULL,
				            COINIT_APARTMENTTHREADED
					  | COINIT_DISABLE_OLE1DDE);
		if (SUCCEEDED(hr)) {
			IFileSaveDialog *pFileSave;
			hr = CoCreateInstance(CLSID_FileSaveDialog,
					      NULL,
					      CLSCTX_ALL,
					      IID_IFileSaveDialog,
					      reinterpret_cast<void**> (&pFileSave));
			if (SUCCEEDED(hr)) {
				hr = pFileSave->Show(NULL);
				if (SUCCEEDED(hr)) {
					IShellItem *pItem;
					hr = pFileSave->GetResult(&pItem);
					if (SUCCEEDED(hr)) {
						hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
						if (SUCCEEDED(hr)) {
							SetFileMode(SpecifiedMode);
							pszFilePathSpecified = pszFilePath;	
						}
					}
					pItem->Release();
				}
				pFileSave->Release();
			}
			CoUninitialize();
		}
	}


	fstream	file;
	std::list<std::shared_ptr<MyEllipse>>::iterator itr;

	file.open(pszFilePath, ios::out | ios::binary);
	if (! file.is_open()) {
		return FALSE;
	}


	for (itr = ellipses.begin(); itr != ellipses.end(); ++itr) {
		file.write((char*)&((*itr)->ellipse), sizeof(MyEllipse));
	}
	file.close();
	return TRUE;
}


bool MainWindow::OnFileSaveAs()
{
	PWSTR pszFilePath;

	HRESULT hr = CoInitializeEx(NULL,
			            COINIT_APARTMENTTHREADED
				  | COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr)) {
		IFileSaveDialog *pFileSave;
		hr = CoCreateInstance(CLSID_FileSaveDialog,
				      NULL,
				      CLSCTX_ALL,
				      IID_IFileSaveDialog,
				      reinterpret_cast<void**> (&pFileSave));
		if (SUCCEEDED(hr)) {
			hr = pFileSave->Show(NULL);
			if (SUCCEEDED(hr)) {
				IShellItem *pItem;
				hr = pFileSave->GetResult(&pItem);
				if (SUCCEEDED(hr)) {
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
					if (SUCCEEDED(hr)) {
						SetFileMode(SpecifiedMode);
						pszFilePathSpecified = pszFilePath;	
					}
				}
				pItem->Release();
			}
			pFileSave->Release();
		}
		CoUninitialize();
	}

	fstream	file;
	std::list<std::shared_ptr<MyEllipse>>::iterator itr;

	file.open(pszFilePath, ios::out | ios::binary);
	if (! file.is_open()) {
		return FALSE;
	}


	for (itr = ellipses.begin(); itr != ellipses.end(); ++itr) {
		file.write((char*)&((*itr)->ellipse), sizeof(MyEllipse));
	}
	file.close();
	return TRUE;
}

bool MainWindow::OnFileOpen()
{
	PWSTR pszFilePath;

	HRESULT hr = CoInitializeEx(NULL,
			            COINIT_APARTMENTTHREADED
				  | COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr)) {
		IFileOpenDialog *pFileOpen;
		hr = CoCreateInstance(CLSID_FileOpenDialog,
				      NULL,
				      CLSCTX_ALL,
				      IID_IFileOpenDialog,
				      reinterpret_cast<void**> (&pFileOpen));
		if (SUCCEEDED(hr)) {
			hr = pFileOpen->Show(NULL);
			if (SUCCEEDED(hr)) {
				IShellItem *pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr)) {
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
					if (SUCCEEDED(hr)) {
						SetFileMode(SpecifiedMode);
						pszFilePathSpecified = pszFilePath;	
					}
				}
				pItem->Release();
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}

	fstream file;
	std::list<std::shared_ptr<MyEllipse>>::iterator itr;

	file.open(pszFilePath, ios::in | ios::binary);
	if (! file.is_open()) {
		return FALSE;
	}


	do {
		MyEllipse tmp;
		file.read((char*)&tmp, sizeof(MyEllipse));

		if (file.fail()) {  // I dont know why, but eof() succeeds even after reading last element.
			break;
		}

		std::shared_ptr<MyEllipse> 	 ptrEllipse(new MyEllipse);

//		ptrEllipse->ellipse.point.x = tmp.ellipse.point.x;
//		ptrEllipse->ellipse.point.y = tmp.ellipse.point.y;
//		ptrEllipse->ellipse.radiusX = tmp.ellipse.radiusX;
//		ptrEllipse->ellipse.radiusY = tmp.ellipse.radiusY;
//
//		ptrEllipse->color = tmp.color;
		*ptrEllipse = tmp;

		ellipses.push_front(ptrEllipse);

	} while (! file.eof());

	selection = ellipses.begin();

	file.close();
	return TRUE;

}


void MainWindow::OnEditShowDialog()
{
	int	id;

	id = DialogBox(GetModuleHandle(NULL), 
		       MAKEINTRESOURCE(IDD_DIALOG1), 
//		       this->Window(), 
			m_hwnd,
		       (DLGPROC)MyDlgProc); 
        InvalidateRect(m_hwnd, NULL, TRUE);

}

std::shared_ptr<MyEllipse> MainWindow::Selection()
{
	if (selection == ellipses.end()) {
		return nullptr;
	} else {
		return (*selection);
	}
}


void MainWindow::ClearSelection()
{
	selection = ellipses.end();
}

void MainWindow::SetMode(MainWinMode m)
{
	mode = m;

	//Update the cursor
	LPWSTR cursor;
	switch (mode) {
		case DrawMode :
			cursor = IDC_CROSS;
			break;
		case SelectMode :
			cursor = IDC_HAND;
			break;
		case DragMode :
			cursor = IDC_SIZEALL;
			break;

	}
	hCursor = LoadCursor(NULL, cursor);
	SetCursor(hCursor);
}


void MainWindow::SetFileMode(MainWinFileMode m)
{
	fileMode = m;
}


LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
    	{
	    case WM_CREATE:
	        if (FAILED(D2D1CreateFactory(
	                D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
	        {
	            return -1;  // Fail CreateWindowEx.
	        }
	        return 0;

	    case WM_DESTROY:
	        DiscardGraphicsResources();
	        SafeRelease(&pFactory);
	        PostQuitMessage(0);
	        return 0;

	    case WM_PAINT:
	        OnPaint();
	        return 0;


	    case WM_SIZE:
	        Resize();
	        return 0;

	    case WM_LBUTTONDOWN:
		OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
		return 0;

	    case WM_LBUTTONUP:
		OnLButtonUp();
		return 0;

	    case WM_RBUTTONDOWN:
		OnRButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
		return 0;

	    case WM_MOUSEMOVE:
		OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
		return 0;	

	    case WM_USER + 5:
		{
		if (InSendMessage()) {
			ReplyMessage(TRUE);
		}
		std::shared_ptr<MyEllipse> 	 ptrEllipse(new MyEllipse);
		*ptrEllipse = sentEllipse;

		ellipses.push_front(ptrEllipse);

	        InvalidateRect(m_hwnd, NULL, FALSE);
		return 0;	
		}
		
	    case WM_COMMAND:
		switch (LOWORD(wParam)) {
			case IDM_FILESAVE:
				if (OnFileSave()) {
				} else {
					return EXIT_FAILURE;
				}
				break;
			case IDM_FILESAVEAS:
				if (OnFileSaveAs()) {
				} else {
					return EXIT_FAILURE;
				}
				break;
			case IDM_FILEOPEN:
				if (OnFileOpen()) {
					OnPaint();
				} else {
					return EXIT_FAILURE;
				}
				break;
			case IDM_FILEEXIT:
	        		DiscardGraphicsResources();
			        SafeRelease(&pFactory);
	       			PostQuitMessage(0);
				break;
			case IDM_POPUPREMOVE:
				OnPopRemove();
				break;
			case IDM_POPUPSEND:
				OnPopSend();
				break;
			case IDM_NEWPLAYER:
				OnEditShowDialog();
				break;
		}
		return 0;

	    case WM_SETCURSOR:
		if (LOWORD(lParam) == HTCLIENT) {
			SetCursor(hCursor);
			return TRUE;
		}
		break;
	    }
	    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}



