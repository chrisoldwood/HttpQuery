/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		RESPONSEDLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CResponseDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "ResponseDlg.hpp"

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

CResponseDlg::CResponseDlg()
	: CDialog(IDD_RESPONSE)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_RESPONSE_HEADERS_LABEL,	&m_txHeaders)
		CTRL(IDC_RESPONSE_HEADERS,			&m_ebHeaders)
		CTRL(IDC_RESPONSE_CONTENT_LABEL,	&m_txContent)
		CTRL(IDC_RESPONSE_CONTENT,			&m_ebContent)
	END_CTRL_TABLE

	DEFINE_GRAVITY_TABLE
		CTRLGRAV(IDC_RESPONSE_HEADERS_LABEL, LEFT_EDGE, TOP_EDGE, RIGHT_EDGE, TOP_EDGE   )
		CTRLGRAV(IDC_RESPONSE_HEADERS,		 LEFT_EDGE, TOP_EDGE, RIGHT_EDGE, TOP_EDGE   )
		CTRLGRAV(IDC_RESPONSE_CONTENT_LABEL, LEFT_EDGE, TOP_EDGE, RIGHT_EDGE, TOP_EDGE   )
		CTRLGRAV(IDC_RESPONSE_CONTENT,		 LEFT_EDGE, TOP_EDGE, RIGHT_EDGE, BOTTOM_EDGE)
	END_GRAVITY_TABLE

	// Disable size grip.
	m_bNoSizeGrip = true;
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

void CResponseDlg::OnInitDialog()
{
	// Initialise edit boxes.
	m_ebHeaders.Font(CFont(ANSI_FIXED_FONT));
	m_ebContent.Font(CFont(ANSI_FIXED_FONT));

	// Create the edit box background brush.
	m_oBgBrush.Create(::GetSysColor(COLOR_WINDOW));
}

/******************************************************************************
** Method:		OnCtlColour()
**
** Description:	Override the default colours used for the headers and content
**				edit boxes which are read-only.
**
** Parameters:	nCtlClrMsg	The WM_CTLCOLOR* message,
**				hDC			The drawing HDC.
**				hCtlWnd		The controls hWhd.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CResponseDlg::OnCtlColour(uint nCtlClrMsg, HDC hDC, HWND hCtlWnd)
{
	// One of the edit boxes?
	if ( (hCtlWnd == m_ebHeaders.Handle()) || (hCtlWnd == m_ebContent.Handle()) )
	{
		// Return the normal window colour brush.
		MsgHandled((BOOL)m_oBgBrush.Handle());
		MsgResult (0);
	}
	else
	{
		CMsgWnd::OnCtlColour(nCtlClrMsg, hDC, hCtlWnd);
	}
}
