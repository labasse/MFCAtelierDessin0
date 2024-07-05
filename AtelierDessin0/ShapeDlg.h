#pragma once
#include "afxdialogex.h"

#include "ColorListBox.h"

// boîte de dialogue de CShapeDlg

class CShapeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShapeDlg)

public:
	CShapeDlg(CWnd* pParent = nullptr);   // constructeur standard
	virtual ~CShapeDlg();

// Données de boîte de dialogue
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHAPE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

	DECLARE_MESSAGE_MAP()
private:
	CColorListBox m_lbColor;
	CRect m_rcInit;
public:
	int m_nType;
private:
	CEdit m_txtContenu;
public:
	CString m_sContenu;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedTexte();
	afx_msg void OnBnClickedRectangle();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
};
