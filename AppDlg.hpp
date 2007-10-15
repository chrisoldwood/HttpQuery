/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CAppDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef APPDLG_HPP
#define APPDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/MainDlg.hpp>
#include <WCL/TabWndHost.hpp>
#include "RequestDlg.hpp"
#include "ResponseDlg.hpp"

/******************************************************************************
** 
** This is the main application dialog.
**
*******************************************************************************
*/

class CAppDlg : public CMainDlg
{
public:
	//
	// Constructors/Destructor.
	//
	CAppDlg();
	
	//
	// Methods.
	//
	virtual bool ProcessMsg(MSG& rMsg);

	//
	// Controls.
	//
	CTabWndHost		m_tcTabCtrl;
	CRequestDlg		m_dlgRequest;
	CResponseDlg	m_dlgResponse;

	// Tabs
	enum Tab
	{
		REQUEST_TAB  = 0,
		RESPONSE_TAB = 1,
	};

protected:
	//
	// Message processors.
	//
	virtual void OnInitDialog();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //APPDLG_HPP
