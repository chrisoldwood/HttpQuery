/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		RESPONSEDLG.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CResponseDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef RESPONSEDLG_HPP
#define RESPONSEDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>
#include <WCL/Brush.hpp>

/******************************************************************************
** 
** The view used to display the response details.
**
*******************************************************************************
*/

class CResponseDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CResponseDlg();
	
	//
	// Controls.
	//
	CLabel		m_txHeaders;
	CEditBox	m_ebHeaders;
	CLabel		m_txContent;
	CEditBox	m_ebContent;

protected:
	//
	// Members.
	//
	CBrush		m_oBgBrush;

	//
	// Message handlers.
	//
	virtual void OnInitDialog();
	virtual void OnCtlColour(uint nCtlClrMsg, HDC hDC, HWND hCtlWnd);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // RESPONSEDLG_HPP
