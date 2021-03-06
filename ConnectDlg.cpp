/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CONNECTDLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CConnectDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "ConnectDlg.hpp"
#include <limits.h>
#include <WCL/StrCvt.hpp>

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

CConnectDlg::CConnectDlg()
	: CDialog(IDD_CONNECT)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_HOST, &m_ebHost)
		CTRL(IDC_PORT, &m_ebPort)
	END_CTRL_TABLE
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

void CConnectDlg::OnInitDialog()
{
	// Display defaults.
	m_ebHost.Text(m_strHost);
	m_ebPort.Text(CStrCvt::FormatUInt(m_nPort));
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

bool CConnectDlg::OnOk()
{
	m_strHost = m_ebHost.Text();

	// Validate host name.
	if (m_strHost.Length() == 0)
	{
		AlertMsg(TXT("Please enter a host name or IP address."));
		m_ebHost.Focus();
		return false;
	}

	m_nPort = CStrCvt::ParseUInt(m_ebPort.Text());

	// Validate port number.
	if ( (m_ebPort.TextLength() == 0) || (m_nPort < 0) || (m_nPort > USHRT_MAX) )
	{
		AlertMsg(TXT("Please enter a valid port number (0 - 65535)."));
		m_ebPort.Focus();
		return false;
	}

	return true;
}
