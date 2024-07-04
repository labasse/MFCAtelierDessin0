
// ChildView.cpp : implémentation de la classe CChildView
//

#include "pch.h"
#include "framework.h"
#include "AtelierDessin0.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	inline int Rand(int from, int to) { return from + rand() % (to - from); }
	inline CSize RandSize(int from, int to) { return { Rand(from, to), Rand(from, to) }; }
}

// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// gestionnaires de messages de CChildView

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

class CGdiRaii
{
	CDC& m_dc;
	CGdiObject* m_old;
public:
	CGdiRaii(CDC& dc, CGdiObject* old) : m_dc(dc), m_old(old) { }
	~CGdiRaii() noexcept { m_dc.SelectObject(m_old); }
};

#define DBLBUF

void CChildView::OnPaint() 
{
#ifndef DBLBUF
	CPaintDC dc(this);

	auto oldpen   = dc.SelectStockObject(BLACK_PEN); // Contour
	auto oldbrush = dc.SelectStockObject(NULL_BRUSH); // Pas de remplissage
#else
	CPaintDC front(this);
	CDC dc; // DC pour back buffer
	CRect rc;
	CBitmap backbuf;
	
	GetClientRect(rc); // Récup. taille zone à dessiner
	dc.CreateCompatibleDC(&front); // Création d'un DC mémoire compatible avec front
	backbuf.CreateCompatibleBitmap(&front, rc.Width(), rc.Height()); // Création d'une zone mémoire compatible
	
	auto oldbmp = dc.SelectObject(&backbuf); // Sélection du backbuf (pour recevoir tous les dessins)
	auto oldpen = dc.SelectStockObject(NULL_PEN);
	auto oldbrush = dc.SelectObject(GetSysColorBrush(COLOR_WINDOW));

	dc.Rectangle(rc); // Dessin du fond
#endif
	// RAII
	srand(0); 

	{
		dc.SelectStockObject(BLACK_PEN); // Contour
		dc.SelectStockObject(NULL_BRUSH); // Pas de remplissage

		dc.SetROP2(R2_XORPEN);
		for (int i = 0; i < 100; ++i)
		{
			CRect rc = CRect(CPoint(RandSize(10, 800)), RandSize(100, 300));

			if (rand() % 2)
			{
				dc.SelectObject(GetSysColorBrush(COLOR_WINDOW));
			}
			else
			{
				dc.SelectStockObject(LTGRAY_BRUSH);
			}
			switch (rand() % 3)
			{
			case 0: dc.Rectangle(rc); break;
			case 1: dc.Ellipse(rc); break;
			case 2: 
				dc.SetBkColor((COLORREF)(rand() | 0x80'80'80) & 0xff'ff'ff);
				dc.SetTextColor(RGB(0, 0, 0));
				//DRAWTEXTPARAMS params; // Informations de marge
				dc.DrawTextEx(
					_T("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum"),
					rc, DT_CENTER|DT_WORDBREAK|DT_WORD_ELLIPSIS/*|DT_CALCRECT*/, NULL //&params
				);
				// DT_CALCRECT : rc contient le rectangle nécessaire pour l'affichage du texte
			}
		}
	}
#ifdef DBLBUF
	front.BitBlt(0, 0, rc.Width(), rc.Height(), &dc, 0, 0, SRCCOPY);
	dc.SelectObject(oldbmp);
#endif
	dc.SelectObject(oldpen);
	dc.SelectObject(oldbrush);	
}



BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
#ifdef DBLBUF
	return FALSE;
#else
	// Erase typique
	// CRect rc;
	// GetClientRect(rc);
	// Sélectionner la brosse
	// pDC->Rectangle(rc);
	// Dessélectionner
	// return TRUE;
	return CWnd::OnEraseBkgnd(pDC);
#endif
}
