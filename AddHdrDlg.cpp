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
		CTRL(IDC_HEADER, &m_cbHeader)
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
	// Set header listbox style.
	m_cbHeader.Font(CFont(ANSI_FIXED_FONT));

	// Load header combo with defaults.
	m_cbHeader.Add("Connection: Close");
	m_cbHeader.Add("Connection: Keep-Alive");
	m_cbHeader.Add("User-Agent: Mozilla/4.0");
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
	m_strHeader = m_cbHeader.Text();

	// Validate input.
	if (m_strHeader.Length() == 0)
	{
		AlertMsg("The header cannot be empty.");
		m_cbHeader.Focus();
		return false;
	}

	if (m_strHeader.Find(':') == -1)
	{
		AlertMsg("The header format should be \"Name: Value\".");
		m_cbHeader.Focus();
		return false;
	}

	return true;
}
