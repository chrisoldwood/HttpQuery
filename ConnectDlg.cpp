/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CONNECTDLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CConnectDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "ConnectDlg.hpp"
#include <limits.h>

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
	char szValue[50];

	// Display defaults.
	m_ebHost.Text(m_strHost);
	m_ebPort.Text(itoa(m_nPort, szValue, 10));
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
		AlertMsg("Please enter a host name or IP address.");
		m_ebHost.Focus();
		return false;
	}

	m_nPort = atoi(m_ebPort.Text());

	// Validate port number.
	if ( (m_ebPort.TextLength() == 0) || (m_nPort < 0) || (m_nPort > USHRT_MAX) )
	{
		AlertMsg("Please enter a valid port number (0 - 65535).");
		m_ebPort.Focus();
		return false;
	}

	return true;
}
