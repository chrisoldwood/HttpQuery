/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PREFSDLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CPrefsDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "PrefsDlg.hpp"

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

/******************************************************************************
** Method:		Default constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CPrefsDlg::CPrefsDlg()
	: CDialog(IDD_PREFS)
{
	DEFINE_CTRL_TABLE
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
	END_CTRLMSG_TABLE
}

/******************************************************************************
** Method:		OnInitDialog()
**
** Description:	Initialise the dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CPrefsDlg::OnInitDialog()
{
}

/******************************************************************************
** Method:		OnOk()
**
** Description:	The OK button was pressed.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CPrefsDlg::OnOk()
{
	return true;
}
