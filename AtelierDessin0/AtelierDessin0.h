
// AtelierDessin0.h : fichier d'en-tête principal de l'application AtelierDessin0
//
#pragma once

#ifndef __AFXWIN_H__
	#error "incluez 'pch.h' avant d'inclure ce fichier pour PCH"
#endif

#include "resource.h"       // symboles principaux


// CAtelierDessin0App :
// Consultez AtelierDessin0.cpp pour l'implémentation de cette classe
//

class CAtelierDessin0App : public CWinAppEx
{
public:
	CAtelierDessin0App() noexcept;


// Substitutions
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implémentation

public:
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CAtelierDessin0App theApp;
