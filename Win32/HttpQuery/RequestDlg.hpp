/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		REQUESTDLG.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CRequestDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef REQUESTDLG_HPP
#define REQUESTDLG_HPP

/******************************************************************************
** 
** The view used to enter the request details.
**
*******************************************************************************
*/

class CRequestDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CRequestDlg();
	
	//
	// Controls.
	//
	CComboBox	m_cbVerb;
	CEditBox	m_ebURL;
	CComboBox	m_cbFormat;
	CLabel		m_txHeaders;
	CEditBox	m_ebHeaders;
	CLabel		m_txContent;
	CEditBox	m_ebContent;

protected:
	//
	// Message handlers.
	//
	virtual void OnInitDialog();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // REQUESTDLG_HPP
