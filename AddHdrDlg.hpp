/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		ADDHDRDLG.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CAddHdrDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef ADDHDRDLG_HPP
#define ADDHDRDLG_HPP

/******************************************************************************
** 
** .
**
*******************************************************************************
*/

class CAddHdrDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CAddHdrDlg();
	
	//
	// Members.
	//
	CString	m_strHeader;

protected:
	
	//
	// Controls.
	//
	CComboBox	m_cbHeader;

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

#endif // ADDHDRDLG_HPP
