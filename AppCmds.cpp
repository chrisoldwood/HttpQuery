/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPCMDS.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppCmds class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "AppCmds.hpp"
#include "HttpQueryApp.hpp"
#include "ConnectDlg.hpp"
#include "AboutDlg.hpp"
#include <limits.h>
#include "PrefsDlg.hpp"
#include <WCL/BusyCursor.hpp>
#include <NCL/TCPCltSocket.hpp>
#include <NCL/SocketException.hpp>
#include <WCL/File.hpp>
#include <WCL/FileException.hpp>

/******************************************************************************
**
** Local variables.
**
*******************************************************************************
*/

static char szTXTExts[] = {	"Text Files (*.txt)\0*.txt\0"
							"All Files (*.*)\0*.*\0"
							"\0\0"							};

static char szTXTDefExt[] = { "txt" };

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CAppCmds::CAppCmds()
{
	// Define the command table.
	DEFINE_CMD_TABLE
		// Server menu.
		CMD_ENTRY(ID_SERVER_CONNECT,	&CAppCmds::OnServerConnect,			&CAppCmds::OnUIServerConnect,		-1)
		CMD_ENTRY(ID_SERVER_DISCONNECT,	&CAppCmds::OnServerDisconnect,		&CAppCmds::OnUIServerDisconnect,	-1)
		CMD_ENTRY(ID_SERVER_EXIT,		&CAppCmds::OnServerExit,			NULL,								-1)
		// Request menu.
		CMD_ENTRY(ID_REQUEST_SEND,		&CAppCmds::OnRequestSend,			&CAppCmds::OnUIRequestSend,			-1)
		// Response menu.
		CMD_ENTRY(ID_RESPONSE_XLATE,	&CAppCmds::OnResponseConvert,		&CAppCmds::OnUIResponseConvert,		-1)
		CMD_ENTRY(ID_RESPONSE_LOCN,		&CAppCmds::OnResponseCopyLocn,		&CAppCmds::OnUIResponseCopyLocn,	-1)
		CMD_ENTRY(ID_RESPONSE_COOKIE,	&CAppCmds::OnResponseCopyCookie,	&CAppCmds::OnUIResponseCopyCookie,	-1)
		CMD_ENTRY(ID_RESPONSE_SAVE_AS,	&CAppCmds::OnResponseSaveAs,		&CAppCmds::OnUIResponseSaveAs,		-1)
		// Options menu.
		CMD_ENTRY(ID_OPTIONS_PREFS,		&CAppCmds::OnOptionsPrefs,			NULL,								-1)
		// Window menu.
		CMD_ENTRY(ID_WINDOW_REQUEST,	&CAppCmds::OnWindowRequest,			NULL,								-1)
		CMD_ENTRY(ID_WINDOW_RESPONSE,	&CAppCmds::OnWindowResponse,		NULL,								-1)
		CMD_ENTRY(ID_WINDOW_NEXT,		&CAppCmds::OnWindowNext,			NULL,								-1)
		CMD_ENTRY(ID_WINDOW_PREV,		&CAppCmds::OnWindowPrev,			NULL,								-1)
		// Help menu.
		CMD_ENTRY(ID_HELP_ABOUT,		&CAppCmds::OnHelpAbout,				NULL,								10)
	END_CMD_TABLE
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

CAppCmds::~CAppCmds()
{
}

/******************************************************************************
** Method:		OnServerConnect()
**
** Description:	Open a conection to a server.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnServerConnect()
{
	CConnectDlg Dlg;

	// Initialise with previous settings.
	Dlg.m_strHost = App.m_strLastHost;
	Dlg.m_nPort   = App.m_nLastPort;

	// Get host and port.
	if (Dlg.RunModal(App.m_AppWnd) == IDOK)
	{
		CBusyCursor oCursor;

		try
		{
			// Close existing connection.
			OnServerDisconnect();

			ASSERT(App.m_pSocket == NULL);

			// Allocate a socket and connect.
			App.m_pSocket = new CTCPCltSocket;
			App.m_pSocket->Connect(Dlg.m_strHost, Dlg.m_nPort);

			// Remember new settings.
			App.m_strLastHost = Dlg.m_strHost;
			App.m_nLastPort   = Dlg.m_nPort;

			// Set focus to URL field.
			App.m_AppWnd.m_AppDlg.m_tcTabCtrl.CurSel(CAppDlg::REQUEST_TAB);
			App.m_AppWnd.m_AppDlg.m_dlgRequest.m_ebURL.Focus();
		}
		catch (CSocketException& e)
		{
			OnServerDisconnect();
			App.AlertMsg("Failed to connect to server:\n\n%s", e.ErrorText());
		}
	}

	UpdateUI();
	App.m_AppWnd.UpdateTitle();
}

/******************************************************************************
** Method:		OnServerDisconnect()
**
** Description:	Close the connection to the server.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnServerDisconnect()
{
	// Connection open?
	if (App.m_pSocket != NULL)
	{
		delete App.m_pSocket;

		App.m_pSocket = NULL;
	}

	UpdateUI();
	App.m_AppWnd.UpdateTitle();
}

/******************************************************************************
** Method:		OnServerExit()
**
** Description:	Terminate the app.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnServerExit()
{
	// Close main window.
	App.m_AppWnd.Close();
}

/******************************************************************************
** Method:		OnRequestSend()
**
** Description:	Send the request.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnRequestSend()
{
	ASSERT((App.m_pSocket != NULL) && (App.m_pSocket->IsOpen()));

	CBusyCursor oCursor;

	try
	{
		// Get a shorthand to the Request dialog.
		CRequestDlg& Dlg = App.m_AppWnd.m_AppDlg.m_dlgRequest;

		// Get the request parameters.
		CString strVerb    = Dlg.m_cbVerb.Text();
		CString strURL     = Dlg.m_ebURL.Text();
		CString strFormat  = Dlg.m_cbFormat.Text();
		CString strHeaders = Dlg.m_ebHeaders.Text();
		CString strContent = Dlg.m_ebContent.Text();

		// URL not set?
		if (strURL.Length() == 0)
		{
			App.AlertMsg("Please enter the URL.");
			App.m_AppWnd.m_AppDlg.m_tcTabCtrl.CurSel(CAppDlg::REQUEST_TAB);
			Dlg.m_ebURL.Focus();
			return;
		}

		// Headers set?
		if (strHeaders.Length() > 0)
		{
			int nPos;

			// Ensure header lines are not doubly terminated.
			while ((nPos = strHeaders.Find("\r\n\r\n")) != -1)
				strHeaders.Delete(nPos, 2);

			int nLength = strHeaders.Length();

			// Ensure final header line is terminated.
			if ( (strHeaders[nLength-1] != '\r') && (strHeaders[nLength-1] != '\n') )
				strHeaders += "\r\n";
		}

		// Format the full request.
		CString strRequest;

		strRequest.Format("%s %s %s\r\n%s\r\n%s", strVerb, strURL, strFormat, strHeaders, strContent);

		// Send it...
		App.m_pSocket->Send(strRequest);
	}
	catch (CSocketException& e)
	{
		OnServerDisconnect();
		App.AlertMsg("Failed to send request:\n\n%s", e.ErrorText());
		return;
	}

	try
	{
		// Get a shorthand to the Response dialog.
		CResponseDlg& Dlg = App.m_AppWnd.m_AppDlg.m_dlgResponse;

		CBuffer oBuffer;
		CString strHeaders;
		CString strContent;
		CString strValue;

		int     nContentLen = -1;
		DWORD   dwStartTime = ::GetTickCount();

		// Read response.
		for(;;)
		{
			int nAvail = App.m_pSocket->Available();

			// Data to read?
			if (nAvail > 0)
			{
				oBuffer.Size(nAvail);

				// Read all data available.
				int nRead = App.m_pSocket->Recv(oBuffer);

				ASSERT(nRead > 0);

				char* pszBuffer = (char*) oBuffer.Buffer();

				// Convert binary data to text.
				for (int i = 0; i < nRead; ++i)
				{
					uchar cChar = pszBuffer[i];

					if (!isprint(cChar) && !isspace(cChar))
						pszBuffer[i] = '.';
				}

				// Append to content.
				strContent += oBuffer.ToString(nRead);

				// Got headers yet?
				if (strHeaders.Empty())
				{
					int nPos = -1;

					// Look for the CRLFCRLF or CRCR termination characters.
					if ( ((nPos = strContent.Find("\r\n\r\n")) != -1)
					  || ((nPos = strContent.Find("\r\r"))     != -1) )
					{
						// Calculate header size.
						int nTermChars = (strContent[nPos+1] == '\r') ? 2 : 4;
						int nHdrSize   = nPos + nTermChars;

						// Extract headers from content buffer.
						strHeaders = strContent.Left(nHdrSize);
						strContent.Delete(0, nHdrSize);

						// Parse Content-Lemgth, if returned.
						if (App.GetHeaderValue(strHeaders, "content-length:", strValue))
							nContentLen = atoi(strValue);

						// Display the response headers and any initial content.
						App.m_AppWnd.m_AppDlg.m_tcTabCtrl.CurSel(CAppDlg::RESPONSE_TAB);		
						Dlg.m_ebHeaders.Text(strHeaders);
						Dlg.m_ebContent.Text(strContent);

						Dlg.m_ebHeaders.Update();
						Dlg.m_ebContent.Update();

						// Content length unspecified?
						if (nContentLen == -1)
						{
							const char* pszMsg = "Response field 'Content-Length' was not specified.\n\n"
												 "Do you want to wait for more content?";

							// Query user to wait for content.
							if (App.QueryMsg(pszMsg) != IDYES)
								break;

							// Assume infinite content length.
							nContentLen = INT_MAX;
						}
					}
				}

				// Read all content?
				if (strContent.Length() >= nContentLen)
					break;

				// Update content read so far.
				Dlg.m_ebContent.Text(strContent);
				Dlg.m_ebContent.Update();
			}

			// Query user every 30 secs to wait for more content.
			if ((::GetTickCount() - dwStartTime) > 10000)
			{
				const char* pszMsg = "Do you want to wait for more content?";

				// Query user to wait for content.
				if (App.QueryMsg(pszMsg) != IDYES)
					break;
				
				dwStartTime = ::GetTickCount();
			}

			::Sleep(1);
		}

		// Display the final response content.
		Dlg.m_ebContent.Text(strContent);
	}
	catch (CSocketException& e)
	{
		OnServerDisconnect();
		App.AlertMsg("Failed to read response:\n\n%s", e.ErrorText());
		return;
	}

	UpdateUI();
}

/******************************************************************************
** Method:		OnResponseConvert()
**
** Description:	Convert LF to CR/LF pairs.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnResponseConvert()
{
	// Get a shorthand to the Response dialog.
	CResponseDlg& Dlg = App.m_AppWnd.m_AppDlg.m_dlgResponse;

	// Get response content.
	CString strOldContent = Dlg.m_ebContent.Text();

	// Calculate buffer size.
	int nLength  = strOldContent.Length();
	int nNumLFs  = strOldContent.Count('\n');
	int nBufSize = (nLength - nNumLFs) + (nNumLFs * 2) + 1;
	// Allocate space for the new content.
	char* pszNewContent = (char*) alloca(nBufSize);

	int n = 0;

	// Replace chars...
	for (int i = 0; i < nLength; ++i)
	{
		char cThisChar = strOldContent[i];
		char cNextChar = ((i+1) < nLength) ? strOldContent[i+1] : '\0';

		// Copy char.
		pszNewContent[n++] = cThisChar;

		// Is next a LF AND this not a CR?
		if ( (cNextChar == '\n') && (cThisChar != '\r') )
			pszNewContent[n++] = '\r';
	}

	// Terminate string.
	pszNewContent[n] = '\0';

	// Update response content.
	Dlg.m_ebContent.Text(pszNewContent);
}

/******************************************************************************
** Method:		OnResponseCopyLocn()
**
** Description:	Copy the response header value for "Location:" back into the
**				request.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnResponseCopyLocn()
{
	// Get shorthands to the Request & Response dialogs.
	CRequestDlg&  ReqDlg = App.m_AppWnd.m_AppDlg.m_dlgRequest;
	CResponseDlg& RspDlg = App.m_AppWnd.m_AppDlg.m_dlgResponse;

	CString strValue;

	// Find header value, if returned.
	if (App.GetHeaderValue(RspDlg.m_ebHeaders.Text(), "Location:", strValue))
	{
		// Trim whitespace.
		strValue.Trim(true, true);

		// Replace URL.
		ReqDlg.m_ebURL.Text(ReqDlg.m_ebURL.Text() + strValue);
	}
}

/******************************************************************************
** Method:		OnResponseCopyCookie()
**
** Description:	Copy the response header value for "Set-Cookie:" back into the
**				request as the "Cookie:".
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnResponseCopyCookie()
{
	// Get shorthands to the Request & Response dialogs.
//	CRequestDlg&  ReqDlg = App.m_AppWnd.m_AppDlg.m_dlgRequest;
	CResponseDlg& RspDlg = App.m_AppWnd.m_AppDlg.m_dlgResponse;

	CString strValue;

	// Find header value, if returned.
	if (App.GetHeaderValue(RspDlg.m_ebHeaders.Text(), "Set-Cookie:", strValue))
	{
	}
}

/******************************************************************************
** Method:		OnResponseSaveAs()
**
** Description:	Save the response to a file.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnResponseSaveAs()
{
	CPath strPath;

	// Select the filename.
	if (!strPath.Select(App.m_AppWnd, CPath::SaveFile, szTXTExts, szTXTDefExt, CPath::ApplicationDir()))
		return;

	try
	{
		// Get a shorthand to the Response dialog.
		CResponseDlg& Dlg = App.m_AppWnd.m_AppDlg.m_dlgResponse;

		CFile oFile;

		oFile.Create(strPath);

		// Write the headers.
		oFile.Write(Dlg.m_ebHeaders.Text(), Dlg.m_ebHeaders.TextLength());
		
		// Write the content.
		oFile.Write(Dlg.m_ebContent.Text(), Dlg.m_ebContent.TextLength());

		oFile.Close();
	}
	catch(CFileException& e)
	{
		// Notify user.
		App.AlertMsg(e.ErrorText());
	}
}

/******************************************************************************
** Method:		OnOptionsPrefs()
**
** Description:	Configure the application.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnOptionsPrefs()
{
	CPrefsDlg Dlg;

	if (Dlg.RunModal(App.m_AppWnd) == IDOK)
	{
	}
}

/******************************************************************************
** Method:		OnWindow*()
**
** Description:	Switch the current view.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnWindowRequest()
{
	App.m_AppWnd.m_AppDlg.m_tcTabCtrl.CurSel(CAppDlg::REQUEST_TAB);		
}

void CAppCmds::OnWindowResponse()
{
	App.m_AppWnd.m_AppDlg.m_tcTabCtrl.CurSel(CAppDlg::RESPONSE_TAB);		
}

void CAppCmds::OnWindowNext()
{
	int nCurTab = App.m_AppWnd.m_AppDlg.m_tcTabCtrl.CurSel();

	App.m_AppWnd.m_AppDlg.m_tcTabCtrl.CurSel((nCurTab+1) % 2);		
}

void CAppCmds::OnWindowPrev()
{
	int nCurTab = App.m_AppWnd.m_AppDlg.m_tcTabCtrl.CurSel();

	App.m_AppWnd.m_AppDlg.m_tcTabCtrl.CurSel((nCurTab+1) % 2);		
}

/******************************************************************************
** Method:		OnHelpAbout()
**
** Description:	Show the about dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnHelpAbout()
{
	CAboutDlg Dlg;

	Dlg.RunModal(App.m_rMainWnd);
}

/******************************************************************************
** Method:		OnUI...()
**
** Description:	UI update handlers.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnUIServerConnect()
{
	App.m_AppWnd.Menu()->EnableCmd(ID_SERVER_CONNECT, true);
}

void CAppCmds::OnUIServerDisconnect()
{
	bool bConnOpen = (App.m_pSocket != NULL);

	App.m_AppWnd.Menu()->EnableCmd(ID_SERVER_DISCONNECT, bConnOpen);
}

void CAppCmds::OnUIRequestSend()
{
	bool bConnOpen = (App.m_pSocket != NULL);

	App.m_AppWnd.Menu()->EnableCmd(ID_REQUEST_SEND, bConnOpen);
}

void CAppCmds::OnUIResponseConvert()
{
	bool bContent = (App.m_AppWnd.m_AppDlg.m_dlgResponse.m_ebContent.TextLength() > 0);

	App.m_AppWnd.Menu()->EnableCmd(ID_RESPONSE_XLATE, bContent);
}

void CAppCmds::OnUIResponseCopyLocn()
{
	bool bHeaders = (App.m_AppWnd.m_AppDlg.m_dlgResponse.m_ebHeaders.TextLength() > 0);

	App.m_AppWnd.Menu()->EnableCmd(ID_RESPONSE_LOCN, bHeaders);
}

void CAppCmds::OnUIResponseCopyCookie()
{
	bool bHeaders = (App.m_AppWnd.m_AppDlg.m_dlgResponse.m_ebHeaders.TextLength() > 0);

	App.m_AppWnd.Menu()->EnableCmd(ID_RESPONSE_COOKIE, bHeaders);
}

void CAppCmds::OnUIResponseSaveAs()
{
	bool bHeaders = (App.m_AppWnd.m_AppDlg.m_dlgResponse.m_ebHeaders.TextLength() > 0);
	bool bContent = (App.m_AppWnd.m_AppDlg.m_dlgResponse.m_ebContent.TextLength() > 0);

	App.m_AppWnd.Menu()->EnableCmd(ID_RESPONSE_SAVE_AS, (bHeaders || bContent));
}
