/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		HTTPQUERYAPP.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CHttpQueryApp class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "HttpQueryApp.hpp"
#include <WCL/StrTok.hpp>
#include <NCL/WinSock.hpp>

/******************************************************************************
**
** Global variables.
**
*******************************************************************************
*/

// "The" application object.
CHttpQueryApp App;

/******************************************************************************
**
** Class constants.
**
*******************************************************************************
*/

#ifdef _DEBUG
const tchar* CHttpQueryApp::VERSION = TXT("v1.1 [Debug]");
#else
const tchar* CHttpQueryApp::VERSION = TXT("v1.1");
#endif
const tchar* CHttpQueryApp::INI_FILE_VER = TXT("1.0");

/******************************************************************************
** Method:		Constructor
**
** Description:	Default constructor.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CHttpQueryApp::CHttpQueryApp()
	: CApp(m_AppWnd, m_AppCmds)
	, m_pSocket(NULL)
	, m_nLastPort(80)
{

}

/******************************************************************************
** Method:		Destructor
**
** Description:	Cleanup.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CHttpQueryApp::~CHttpQueryApp()
{

}

/******************************************************************************
** Method:		OnOpen()
**
** Description:	Initialises the application.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CHttpQueryApp::OnOpen()
{
	// Set the app title.
	m_strTitle = TXT("Http Query");

	// Load settings.
	LoadConfig();

	// Initialise WinSock.
	int nResult = CWinSock::Startup(1, 1);

	if (nResult != 0)
	{
		FatalMsg(TXT("Failed to initialise WinSock layer: %d."), nResult);
		return false;
	}

	// Load the toolbar bitmap.
	m_rCmdControl.CmdBitmap().LoadRsc(IDR_APPTOOLBAR);

	// Create the main window.
	if (!m_AppWnd.Create())
		return false;

	// Show it.
	if (ShowNormal() && !m_rcLastPos.Empty())
		m_AppWnd.Move(m_rcLastPos);

	// Show it.
	m_AppWnd.Show(m_iCmdShow);

	// Update UI.
	m_AppCmds.UpdateUI();

	return true;
}

/******************************************************************************
** Method:		OnClose()
**
** Description:	Cleans up the application resources.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CHttpQueryApp::OnClose()
{
	// Terminate WinSock.
	CWinSock::Cleanup();

	// Save settings.
	SaveConfig();

	return true;
}

/******************************************************************************
** Method:		LoadConfig()
**
** Description:	Load the app configuration.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CHttpQueryApp::LoadConfig()
{
	// Read the file version.
	CString strVer = m_oIniFile.ReadString(TXT("Version"), TXT("Version"), INI_FILE_VER);

	// Read the MRU settings.
	m_strLastHost = m_oIniFile.ReadString(TXT("Main"), TXT("LastHost"), TXT(""));
	m_nLastPort   = m_oIniFile.ReadInt   (TXT("Main"), TXT("LastPort"), m_nLastPort);

	// Read the window pos and size.
	m_rcLastPos.left   = m_oIniFile.ReadInt(TXT("UI"), TXT("Left"),   0);
	m_rcLastPos.top    = m_oIniFile.ReadInt(TXT("UI"), TXT("Top"),    0);
	m_rcLastPos.right  = m_oIniFile.ReadInt(TXT("UI"), TXT("Right"),  0);
	m_rcLastPos.bottom = m_oIniFile.ReadInt(TXT("UI"), TXT("Bottom"), 0);

	// Read the default headers.
	int nDefHdrs = m_oIniFile.ReadInt(TXT("Headers"), TXT("Count"), 0);

	for (int i = 0; i < nDefHdrs; ++i)
	{
		CString strEntry, strHeader;

		strEntry.Format(TXT("Header[%d]"), i);
		strHeader = m_oIniFile.ReadString(TXT("Headers"), strEntry, TXT(""));

		if (!strHeader.Empty())
			m_astrDefHeaders.Add(strHeader);
	}
}

/******************************************************************************
** Method:		SaveConfig()
**
** Description:	Save the app configuration.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CHttpQueryApp::SaveConfig()
{
	// Write the file version.
	m_oIniFile.WriteString(TXT("Version"), TXT("Version"), INI_FILE_VER);

	// Write the MRU settings.
	m_oIniFile.WriteString(TXT("Main"), TXT("LastHost"), m_strLastHost);
	m_oIniFile.WriteInt   (TXT("Main"), TXT("LastPort"), m_nLastPort);

	// Write the window pos and size.
	m_oIniFile.WriteInt(TXT("UI"), TXT("Left"),   m_rcLastPos.left  );
	m_oIniFile.WriteInt(TXT("UI"), TXT("Top"),    m_rcLastPos.top   );
	m_oIniFile.WriteInt(TXT("UI"), TXT("Right"),  m_rcLastPos.right );
	m_oIniFile.WriteInt(TXT("UI"), TXT("Bottom"), m_rcLastPos.bottom);

	// Write the default headers.
	m_oIniFile.WriteInt(TXT("Headers"), TXT("Count"), m_astrDefHeaders.Size());

	for (size_t i = 0; i < m_astrDefHeaders.Size(); ++i)
	{
		CString strEntry;

		strEntry.Format(TXT("Header[%d]"), i);

		m_oIniFile.WriteString(TXT("Headers"), strEntry, m_astrDefHeaders[i]);
	}
}

/******************************************************************************
** Method:		GetHeaderValue()
**
** Description:	Helper method to get the value for a response header.
**
** Parameters:	pszHeaders		The response headers.
**				pszField		The header field to find.
**				strValue		The return buffer for the value.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CHttpQueryApp::GetHeaderValue(const tchar* pszHeaders, const tchar* pszField, CString& strValue)
{
	bool bFound    = false;
	int  nFieldLen = tstrlen(pszField);

	// Split headers string into separate lines.
	CStrTok oStrTok(pszHeaders, TXT("\r\n"), CStrTok::MERGE_SEPS);

	// Find field...
	while ( (oStrTok.MoreTokens()) && (!bFound) )
	{
		// Get next line.
		CString strLine = oStrTok.NextToken().Trim(true, false);

		// Is the field we're after?
		if (tstrnicmp(strLine, pszField, nFieldLen) == 0)
		{
			const tchar* pszValue = tstrchr(strLine, ':');

			// Extract field value.
			if (pszValue != NULL)
				strValue = pszValue+1;

			bFound = true;
		}
	}

	return bFound;
}
