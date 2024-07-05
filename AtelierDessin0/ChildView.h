
// ChildView.h : interface de la classe CChildView
//


#pragma once


#include <vector>
#include <optional>

// fenêtre de CChildView

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributs
public:

// Opérations
public:

// Substitutions
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implémentation
public:
	virtual ~CChildView();
private:
	enum class Type { Rectangle, Ellipse, Text };
	struct shape
	{
		Type type{ 0 };
		CRect box;
		CString text;
		BOOL white{ FALSE };
		COLORREF textbg{ RGB(0, 0, 0) };
	};
	std::optional<Type> m_mode;
	std::vector<shape> m_items;
	CPen m_selection;
	std::optional<int> m_isel;
	std::optional<CPoint> m_startmove, m_curmove;
	void EndMove();
	// Fonctions générées de la table des messages
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEditionSupprimer();
	afx_msg void OnUpdateEditionSupprimer(CCmdUI* pCmdUI);
	// v1 afx_msg void OnUpdateEditionRectangle(CCmdUI* pCmdUI);
	// v1 afx_msg void OnEditionRectangle();
	afx_msg void OnUpdateEditionMode(CCmdUI* pCmdUI);
	afx_msg void OnEditionMode(UINT nId);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

