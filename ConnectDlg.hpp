/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CONNECTDLG.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CConnectDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef CONNECTDLG_HPP
#define CONNECTDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>

/******************************************************************************
** 
** The server connection dialog.
**
*******************************************************************************
*/

class CConnectDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CConnectDlg();
	
	//
	// Members.
	//
	CString	m_strHost;
	uint	m_nPort;
	
protected:
	//
	// Controls.
	//
	CEditBox	m_ebHost;
	CEditBox	m_ebPort;

	//
	// Message handlers.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // CONNECTDLG_HPP
