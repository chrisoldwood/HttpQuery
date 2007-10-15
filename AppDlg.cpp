/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "AppDlg.hpp"

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

CAppDlg::CAppDlg()
	: CMainDlg(IDD_MAIN)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_TAB,	&m_tcTabCtrl)
	END_CTRL_TABLE

	DEFINE_GRAVITY_TABLE
		CTRLGRAV(IDC_TAB, LEFT_EDGE, TOP_EDGE, RIGHT_EDGE, BOTTOM_EDGE)
	END_GRAVITY_TABLE
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

void CAppDlg::OnInitDialog()
{
	// Create the tab page dialogs.
	m_dlgRequest.RunModeless(*this);
	m_dlgResponse.RunModeless(*this);

	// Add the tab pages.
	m_tcTabCtrl.AddTab("Request",  m_dlgRequest);
	m_tcTabCtrl.AddTab("Response", m_dlgResponse);
	m_tcTabCtrl.CurSel(REQUEST_TAB);
}

/******************************************************************************
** Method:		ProcessMsg()
**
** Description:	Handle dialog messages for the active tab.
**
** Parameters:	rMsg	The message.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CAppDlg::ProcessMsg(MSG& rMsg)
{
	// Try Request dialog...
	if (::IsDialogMessage(m_dlgRequest.Handle(), &rMsg))
		return true;

	// Try Response dialog...
	if (::IsDialogMessage(m_dlgResponse.Handle(), &rMsg))
		return true;

	return false;
}
