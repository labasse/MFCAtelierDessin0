#pragma once


// CColorListBox

class CColorListBox : public CListBox
{
	DECLARE_DYNAMIC(CColorListBox)

public:
	CColorListBox();
	virtual ~CColorListBox();
	int AddColorEntry(LPCTSTR title, COLORREF color);

protected:
	LPCTSTR GetStr(int id, ULONG_PTR data);

	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT /*lpCompareItemStruct*/);
	virtual void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
//	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
};


