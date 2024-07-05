
// ChildView.cpp : implémentation de la classe CChildView
//

#include "pch.h"
#include "framework.h"
#include "AtelierDessin0.h"
#include "ChildView.h"
#include "ShapeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	inline int Rand(int from, int to) { return from + rand() % (to - from); }
	inline CSize RandSize(int from, int to) { return { Rand(from, to), Rand(from, to) }; }
}

// CChildView

CChildView::CChildView() : m_selection(PS_DOT, 1, RGB(0, 0, 0))
{
	for (int i = 0; i < 100; ++i)
	{
		shape item;

		item.type = static_cast<Type>(rand() % 3);
		item.white = 0 != (rand() % 2);
		item.box = CRect(CPoint(RandSize(10, 800)), RandSize(100, 300));
		item.textbg = (COLORREF)(rand() | 0x80'80'80) & 0xff'ff'ff;
		item.text = _T("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum");
		m_items.push_back(item);
	}
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_EDITION_SUPPRIMER, &CChildView::OnEditionSupprimer)
	ON_UPDATE_COMMAND_UI(ID_EDITION_SUPPRIMER, &CChildView::OnUpdateEditionSupprimer)
	ON_UPDATE_COMMAND_UI(ID_EDITION_RECTANGLE, &CChildView::OnUpdateEditionMode)
	ON_UPDATE_COMMAND_UI(ID_EDITION_CERCLE	, &CChildView::OnUpdateEditionMode)
	ON_UPDATE_COMMAND_UI(ID_EDITION_TEXTE	, &CChildView::OnUpdateEditionMode)
	ON_COMMAND_RANGE(ID_EDITION_RECTANGLE, ID_EDITION_TEXTE, &CChildView::OnEditionMode)
	ON_WM_LBUTTONDBLCLK()
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
#else
	CPaintDC front(this);
	CDC dc; // DC pour back buffer
	CRect rc;
	CBitmap backbuf;
	
	GetClientRect(rc); // Récup. taille zone à dessiner
	dc.CreateCompatibleDC(&front); // Création d'un DC mémoire compatible avec front
	backbuf.CreateCompatibleBitmap(&front, rc.Width(), rc.Height()); // Création d'une zone mémoire compatible
	
	auto oldbmp = dc.SelectObject(&backbuf); // Sélection du backbuf (pour recevoir tous les dessins)
	
	dc.FillSolidRect(rc, GetSysColor(COLOR_WINDOW)); // Dessin du fond
#endif
	// RAII
	srand(0); 

	{
		CGdiRaii oldpen (dc, dc.SelectStockObject(BLACK_PEN)); // Contour
		CGdiRaii oldbrush (dc, dc.SelectStockObject(NULL_BRUSH)); // Pas de remplissage
		dc.SelectStockObject(BLACK_PEN); // Contour
		dc.SelectStockObject(NULL_BRUSH); // Pas de remplissage

		dc.SetROP2(R2_XORPEN);
		for (auto item : m_items)
		{
			if (item.white)
			{
				dc.SelectObject(GetSysColorBrush(COLOR_WINDOW));
			}
			else
			{
				dc.SelectStockObject(LTGRAY_BRUSH);
			}
			switch (item.type)
			{
			case Type::Rectangle: dc.Rectangle(item.box); break;
			case Type::Ellipse: dc.Ellipse  (item.box); break;
			case Type::Text: 
				dc.SetBkColor(item.textbg);
				dc.SetTextColor(RGB(0, 0, 0));
				//DRAWTEXTPARAMS params; // Informations de marge
				dc.DrawTextEx(
					item.text, item.box, 
					DT_CENTER|DT_WORDBREAK|DT_WORD_ELLIPSIS/*|DT_CALCRECT*/, 
					NULL //&params
				);
				// DT_CALCRECT : rc contient le rectangle nécessaire pour l'affichage du texte
			}
		}
		if (m_isel)
		{
			CRect rc = m_items[m_isel.value()].box;

			if (m_startmove)
			{
				rc.OffsetRect(m_curmove.value() - m_startmove.value());
			}
			dc.SelectObject(m_selection);
			dc.SelectStockObject(NULL_BRUSH);
			dc.Rectangle(rc);
		}
	}
#ifdef DBLBUF
	front.BitBlt(0, 0, rc.Width(), rc.Height(), &dc, 0, 0, SRCCOPY);
	dc.SelectObject(oldbmp);
#endif
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

void CChildView::EndMove()
{
	ReleaseCapture();
	m_startmove.reset();
	m_curmove.reset();
	Invalidate();
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_isel.reset();
	for (int i = m_items.size(); i--; )
	{
		if (m_items[i].box.PtInRect(point))
		{
			m_isel = i;
			m_startmove = m_curmove = point;
			SetCapture(); // Prend la main sur les message souris PARTOUT
			// Invalidate(FALSE); pas d'effacement
			// InvalidateRect(rectARedessiner);
			break;
		}
	}
	Invalidate();
	CWnd::OnLButtonDown(nFlags, point);
}


void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_startmove)
	{
		m_curmove = point;
		Invalidate();
	}
	CWnd::OnMouseMove(nFlags, point);
}


void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_startmove)
	{
		m_items[m_isel.value()].box.OffsetRect(
			m_curmove.value() - m_startmove.value()
		);
		EndMove();
	}
	CWnd::OnLButtonUp(nFlags, point);
}


void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_ESCAPE)
	{
		EndMove();
	}
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CChildView::OnEditionSupprimer()
{
	ASSERT(m_isel);
	m_items.erase(m_items.begin() + m_isel.value());
	m_isel.reset();
	Invalidate();
}


void CChildView::OnUpdateEditionSupprimer(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_isel.has_value());
}


/*
void CChildView::OnUpdateEditionRectangle(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(m_mode && m_mode.value()==Type::Rectangle);
}


void CChildView::OnEditionRectangle()
{
	m_mode = Type::Rectangle;
}

*/

void CChildView::OnUpdateEditionMode(CCmdUI* pCmdUI)
{
	int indice_mode = pCmdUI->m_nID - ID_EDITION_RECTANGLE;
	//ASSERT(0 <= indice_mode && indice_mode <= ...);
	pCmdUI->SetRadio(m_mode && m_mode.value()==static_cast<Type>(indice_mode));
}


void CChildView::OnEditionMode(UINT nID)
{
	auto mode = static_cast<Type>(nID - ID_EDITION_RECTANGLE);

	if (m_mode == mode)
	{
		m_mode.reset();
	}
	else
	{
		m_mode = mode;
	}
}


void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_isel)
	{
		CShapeDlg dlg(this);
		shape& item = m_items[m_isel.value()];

		dlg.m_nType = static_cast<int>(item.type);
		dlg.m_sContenu = item.text;
		if (dlg.DoModal() == IDOK)
		{
			item.type = static_cast<Type>(dlg.m_nType);
			item.text = dlg.m_sContenu;
			Invalidate();
		}
	}
	CWnd::OnLButtonDblClk(nFlags, point);
}
