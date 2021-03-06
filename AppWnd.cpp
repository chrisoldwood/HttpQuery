/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPWND.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppWnd class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "AppWnd.hpp"
#include "HttpQueryApp.hpp"

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

CAppWnd::CAppWnd()
	: CDlgFrame(IDR_APPICON, m_AppDlg, false)
{
}

/******************************************************************************
** Method:		Destructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CAppWnd::~CAppWnd()
{
}

/******************************************************************************
** Method:		OnCreate()
**
** Description:	Creates the toolbar and status bars.
**
** Parameters:	rcClient	The client rectangle.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppWnd::OnCreate(const CRect& rcClient)
{
	//
	// Create and attach the components.
	//
	m_Accel.LoadRsc(IDR_APPACCEL);
	Accel(&m_Accel);

	m_Menu.LoadRsc(IDR_APPMENU);
	Menu(&m_Menu);

	m_StatusBar.Create(*this, IDC_STATUS_BAR, rcClient);
	StatusBar(&m_StatusBar);

	m_AppDlg.RunModeless(*this);

	// Call base class.
	CDlgFrame::OnCreate(rcClient);
}

/******************************************************************************
** Method:		OnQueryClose()
**
** Description:	Check if the app can close.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CAppWnd::OnQueryClose()
{
	// Fetch windows final placement.
	App.m_rcLastPos = Placement();

	// Close connection.
	App.m_AppCmds.OnServerDisconnect();

	return true;
}

/******************************************************************************
** Method:		ProcessMsg()
**
** Description:	Forward to the main dialog.
**
** Parameters:	rMsg	The message.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CAppWnd::ProcessMsg(MSG& rMsg)
{
	// Check accelerators first.
	if (CDlgFrame::ProcessMsg(rMsg))
		return true;

	// Now check the view dialogs.
	return m_AppDlg.ProcessMsg(rMsg);
}

/******************************************************************************
** Method:		UpdateTitle()
**
** Description:	Update the title bar with the connection status.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppWnd::UpdateTitle()
{
	CString strTitle = App.m_strTitle;

	// Append connection, if open.
	if (App.m_pSocket != nullptr)
	{
		strTitle += TXT(" - ");
		strTitle += App.m_strLastHost;
	}

	Title(strTitle);
}
