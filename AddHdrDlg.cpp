/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		ADDHDRDLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CAddHdrDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "AddHdrDlg.hpp"

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

CAddHdrDlg::CAddHdrDlg()
	: CDialog(IDD_DEF_HEADER)
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

void CAddHdrDlg::OnInitDialog()
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

bool CAddHdrDlg::OnOk()
{
	return true;
}
