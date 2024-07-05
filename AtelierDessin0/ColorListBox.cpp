// ColorListBox.cpp : fichier d'implémentation
//

#include "pch.h"
#include "AtelierDessin0.h"
#include "ColorListBox.h"


// CColorListBox

IMPLEMENT_DYNAMIC(CColorListBox, CListBox)

CColorListBox::CColorListBox()
{

}

CColorListBox::~CColorListBox()
{
}


BEGIN_MESSAGE_MAP(CColorListBox, CListBox)
END_MESSAGE_MAP()



// gestionnaires de messages de CColorListBox


struct ColorEntry
{
	CString title;
	CBrush brush;
};

int CColorListBox::AddColorEntry(LPCTSTR title, COLORREF color)
{
	int index = AddString(title); //fonction de CListBox pour ajouter 1 ligne dans la liste

	SetItemDataPtr(index, new ColorEntry{ title, color });
	return index;
}

void CColorListBox::DrawItem(LPDRAWITEMSTRUCT pdis)
{
	if (pdis->itemID < 0)
	{
		return;
	}
	auto entry = (ColorEntry*)GetItemDataPtr(pdis->itemID);
	auto text = COLOR_WINDOWTEXT;
	auto pdc = CDC::FromHandle(pdis->hDC);
	auto oldpen = pdc->SelectStockObject(NULL_PEN);
	auto oldbrush = pdc->SelectStockObject(NULL_BRUSH);

	// Background
	CRect rc{ pdis->rcItem };

	rc.InflateRect(1, 1);
	FillRect(pdis->hDC, rc, GetSysColorBrush(COLOR_WINDOW));
	if (pdis->itemState & ODS_FOCUS)
	{
		pdc->DrawFocusRect(rc);
		rc.top++;
		pdc->SelectObject(GetSysColorBrush(COLOR_HIGHLIGHT));
		pdc->Rectangle(rc);
		text = COLOR_HIGHLIGHTTEXT;
	}

	// Color
	rc = pdis->rcItem;
	pdc->SelectStockObject(BLACK_PEN);
	pdc->SelectObject(entry->brush);
	rc.right = rc.left + rc.Height();
	rc.DeflateRect(2, 2);
	pdc->Rectangle(rc);

	// Text
	rc = pdis->rcItem;
	rc.left += rc.Height();
	pdc->SetBkMode(TRANSPARENT);
	pdc->SetTextColor(GetSysColor(text));
	pdc->DrawText(entry->title, rc, DT_LEFT | DT_SINGLELINE | DT_END_ELLIPSIS);

	// Clear
	pdc->SelectObject(oldpen);
	pdc->SelectObject(oldbrush);
}

LPCTSTR CColorListBox::GetStr(int id, ULONG_PTR data)
{
	return id < 0
		? reinterpret_cast<LPCWSTR>(data)
		: static_cast<ColorEntry*>(GetItemDataPtr(id))->title;
}

int CColorListBox::CompareItem(LPCOMPAREITEMSTRUCT pcis)
{
	CString s = GetStr(pcis->itemID1, pcis->itemData1);
	return s.Compare(GetStr(pcis->itemID2, pcis->itemData2));
}


void CColorListBox::DeleteItem(LPDELETEITEMSTRUCT pdis)
{
	delete GetItemDataPtr(pdis->itemID);
	CListBox::DeleteItem(pdis);
}

