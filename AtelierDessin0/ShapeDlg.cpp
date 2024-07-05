// ShapeDlg.cpp : fichier d'implémentation
//

#include "pch.h"
#include "AtelierDessin0.h"
#include "afxdialogex.h"
#include "ShapeDlg.h"

#undef max
#include <algorithm>


// boîte de dialogue de CShapeDlg

IMPLEMENT_DYNAMIC(CShapeDlg, CDialogEx)

CShapeDlg::CShapeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SHAPE, pParent)
	, m_nType(0)
	, m_sContenu(_T(""))
{

}

CShapeDlg::~CShapeDlg()
{
}

void CShapeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COLOR, m_lbColor);
	DDX_Radio(pDX, IDC_RECTANGLE, m_nType);
	DDX_Control(pDX, IDC_CONTENU, m_txtContenu);
	DDX_Text(pDX, IDC_CONTENU, m_sContenu);
	DDV_MaxChars(pDX, m_sContenu, 200);
}


BEGIN_MESSAGE_MAP(CShapeDlg, CDialogEx)
	ON_BN_CLICKED(IDC_TEXTE, &CShapeDlg::OnBnClickedTexte)
	ON_BN_CLICKED(IDC_RECTANGLE, &CShapeDlg::OnBnClickedRectangle)
	ON_BN_CLICKED(IDC_CERCLE, &CShapeDlg::OnBnClickedRectangle)
END_MESSAGE_MAP()


// gestionnaires de messages de CShapeDlg


BOOL CShapeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_txtContenu.EnableWindow(m_nType == 2);
	m_lbColor.AddString(_T("Rouge"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}


void CShapeDlg::OnBnClickedTexte()
{
	m_txtContenu.EnableWindow(TRUE);
}


void CShapeDlg::OnBnClickedRectangle()
{
	m_txtContenu.EnableWindow(FALSE);
}
