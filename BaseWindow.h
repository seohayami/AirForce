#pragma once

#include "resource.h"
#include "resource1.h"
#include <d2d1.h>
#include <list>
#include <memory>
#include <time.h>

extern LPCTSTR liststr[];
// LPCTSTR liststr[] = {L"Ju87D",L"Ju87D-5", L"Ju87G-1", 
// 	             L"Ju88C",L"Ju88A",L"HE111H",L"Me262A", L"Me262A-2a",   
// 		     L"ME109E", L"FW190A", L"FW190A-8/U11", L"FW190A-8/R1", 
// 		     L"ME109F", L"ME109F-4/B", L"ME109F-4/R1", 
// 		     L"ME110C", L"ME110G", L"ME110G-2/R3",
//		     L"ME109G", L"ME109G-6", L"ME109G-6/U4",
//		     L"Ju87B"};

extern LPCTSTR	liststr_Pilots[];
// LPCTSTR	liststr_Pilots[] 
// 		= {L"Random", L"Novice", L"Average", L"Ace"};
//		  {L"Random", L"Novice", L"Rank-R", L"Rank-T",  
//		   L"Rank-ET", L"Rank-RV", L"Rank-TR", 
//		   L"Rank-ETR",L"Rank-ETT", 
//		   L"Rank-ETTR", L"Rank-ETRV", L"Rank-ETTRV", L"Ace"}; 
// You need to add "L" to show that these char are wide(2byte).
// You need to write real body of global variable in .cpp file and 
//   extern in .h file
//   otherwise, you will get link error saying "symbol already defined".


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


//float DPIScale::scaleX = 1.0f;
//float DPIScale::scaleY = 1.0f;

template <class T> void SafeRelease(T **ppT)
{
	if (*ppT) {
		(*ppT)->Release();
		*ppT = NULL;
	}
}


inline void initRand(){
	srand((unsigned int) time(NULL));
}

struct MyEllipse
{
	D2D1_ELLIPSE	ellipse;
	D2D1_COLOR_F	color;


	void Draw(ID2D1RenderTarget *pRT, ID2D1SolidColorBrush *pBrush){
		pBrush->SetColor(color);
		pRT->FillEllipse(ellipse, pBrush);
		pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
		pRT->DrawEllipse(ellipse, pBrush, 1.0f);
	}

	BOOL HitTest(float x, float y) {
		const float a = ellipse.radiusX;
		const float b = ellipse.radiusY;
		const float x1 = x - ellipse.point.x;
		const float y1 = y - ellipse.point.y;
		const float d =((x1 * x1) / (a *a)) + ((y1 * y1) / (b * b));
	return d <= 1.0f;
	}
};



template <class DERIVED_TYPE>
class BaseWindow
{
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		DERIVED_TYPE *pThis = NULL;

		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

			pThis->m_hwnd = hwnd;
		}
		else
		{
			pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		}
		if (pThis)
		{
			return pThis->HandleMessage(uMsg, wParam, lParam);
		}
		else
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}

	static LRESULT CALLBACK MyDlgProc(HWND hDlgWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg) {
			case WM_COMMAND:
				switch (LOWORD(wParam)) {
					case IDOK:
						EndDialog(hDlgWnd, IDOK);
						break;
					case IDCANCEL:
						EndDialog(hDlgWnd, IDOK);
						break;
					case IDC_BUTTON1:
						LRESULT result1, result2;
						int	i;
						result1 = SendMessage(GetDlgItem(hDlgWnd, IDC_LIST1),
							             LB_GETCURSEL, 
							       	     0,
							             0);	     
						result2 = SendMessage(GetDlgItem(hDlgWnd, IDC_LIST2),
							             LB_GETCURSEL, 
							       	     0,
							             0);	     
						if ((result1 == LB_ERR) || (result2 == LB_ERR) ){
							MessageBox(NULL, 
							           L"Select Aircraft and Pilot..\n",
								   NULL,
								   MB_OKCANCEL | MB_ICONSTOP
								   );
						} else {
							i = (int)result1;
							SendMessage(GetDlgItem(hDlgWnd, IDC_LIST3), 
							       LB_ADDSTRING, 
							       0,
							       (LPARAM)liststr[i]);
						}


						break;

					default:
						return FALSE;
				}
				break;
			case WM_INITDIALOG:
				int i;
				for (i = 0; i < 21; i++) {
					SendMessage(GetDlgItem(hDlgWnd, IDC_LIST1), 
							       LB_INSERTSTRING, 
							       (WPARAM)i,
							       (LPARAM)liststr[i]);
				}
				for (i = 0; i < 4; i++) {
					SendMessage(GetDlgItem(hDlgWnd, IDC_LIST2), 
							       LB_INSERTSTRING, 
							       (WPARAM)i,
							       (LPARAM)liststr_Pilots[i]);
				}
				break;
			default:
				return FALSE;
					              
		}
		return TRUE;
	}
	
	BaseWindow() : m_hwnd(NULL), m_h_peerwnd(NULL) { }

	BOOL Create(
		PCWSTR lpWindowName,
		DWORD dwStyle,
		DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT,
		int nHeight = CW_USEDEFAULT,
		HWND hWndParent = 0,
		HMENU hMenu = 0
	)
	{
		WNDCLASS wc = { 0 };

		wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
		wc.lpszClassName = ClassName();

		RegisterClass(&wc);

		m_hwnd = CreateWindowEx(
			dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
			nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
		);

		return (m_hwnd ? TRUE : FALSE);
	}

	void SetPeerWnd(HWND hwnd)
	{
		m_h_peerwnd = hwnd;
	}

	HWND Window() const { return m_hwnd; }
	HWND PeerWindow() const { return m_h_peerwnd; }

protected:

	virtual PCWSTR  ClassName() const = 0;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	HWND m_hwnd;
	HWND m_h_peerwnd;
	
};


class MainWindow : public BaseWindow<MainWindow>
{
protected:
    ID2D1Factory            *pFactory;
    ID2D1HwndRenderTarget   *pRenderTarget;
    ID2D1SolidColorBrush    *pBrush;
    D2D1_ELLIPSE            ellipse;

    static MyEllipse            sentEllipse;

    D2D1_POINT_2F	ptMouse;

    enum MainWinMode {
	    DrawMode	= 0,
	    SelectMode	= 1,
	    DragMode	=2
    };

    MainWinMode	mode = DrawMode;

    enum MainWinFileMode {
	    UnspecifiedMode	= 0,
	    SpecifiedMode	= 1
    };

    MainWinFileMode fileMode = UnspecifiedMode;
    PWSTR pszFilePathSpecified = NULL;

    std::list<std::shared_ptr<MyEllipse>>::iterator selection;
    std::list<std::shared_ptr<MyEllipse>> 	ellipses;

    HCURSOR hCursor;

    void    CalculateLayout();
    bool    HitTest(float dipX, float dipY);
    void    InsertEllipse (float x, float y);
    void    RemoveEllipse();
    virtual HRESULT CreateGraphicsResources();
    virtual void    DiscardGraphicsResources();
    virtual void    OnPaint();
    void    Resize();

    void	OnLButtonDown(int pixelX, int pixelY, DWORD flags);
    void 	OnPopRemove();
    void 	OnPopSend();
    void	OnRButtonDown(int pixelX, int pixelY, DWORD flags);
    void	OnLButtonUp();
    void 	OnMouseMove(int pixelX, int pixelY, DWORD flags);
    bool 	OnFileSave();
    bool 	OnFileSaveAs();
    bool 	OnFileOpen();
    virtual void        OnEditShowDialog();

    std::shared_ptr<MyEllipse> Selection();
    void    ClearSelection();
    void    SetMode(MainWinMode m);
    void    SetFileMode(MainWinFileMode m);

public:

    MainWindow() : pFactory(NULL), pRenderTarget(NULL), pBrush(NULL), 
	ellipse(D2D1::Ellipse(D2D1::Point2F(), 0, 0)),
	ptMouse(D2D1::Point2F())
    {
    }

    virtual PCWSTR  ClassName() const { return L"Circle Window Class"; }
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

