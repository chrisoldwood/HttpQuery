/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PREFSDLG.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CPrefsDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef PREFSDLG_HPP
#define PREFSDLG_HPP

/******************************************************************************
** 
** The preferences configuration dialog.
**
*******************************************************************************
*/

class CPrefsDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CPrefsDlg();
	
protected:
	//
	// Members.
	//
	
	//
	// Controls.
	//
	CListBox	m_lbHeaders;

	//
	// Message handlers.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();

	void OnAdd();
	void OnRemove();

	//
	// Internal methods.
	//
	void UpdateUI();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // PREFSDLG_HPP
