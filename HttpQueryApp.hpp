/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		HTTPQUERYAPP.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CHttpQueryApp class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef HTTPQUERYAPP_HPP
#define HTTPQUERYAPP_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/App.hpp>
#include "AppWnd.hpp"
#include "AppCmds.hpp"
#include <WCL/IniFile.hpp>
#include <WCL/StrArray.hpp>

// Forward declarations.
class CTCPCltSocket;

/******************************************************************************
** 
** The application class.
**
*******************************************************************************
*/

class CHttpQueryApp : public CApp
{
public:
	//
	// Constructors/Destructor.
	//
	CHttpQueryApp();
	~CHttpQueryApp();

	//
	// Members
	//
	CAppWnd			m_AppWnd;			// Main window.
	CAppCmds		m_AppCmds;			// Command handler.

	CIniFile		m_oIniFile;			// .INI FIle
	CRect			m_rcLastPos;		// Main window position.
	CString			m_strLastHost;		// Last host contacted.
	int				m_nLastPort;		// Last port contacted.
	CStrArray		m_astrDefHeaders;	// Default headers.

	CTCPCltSocket*	m_pSocket;			// Connection.

	//
	// Helper methods.
	//
	static bool GetHeaderValue(const tchar* pszHeaders, const tchar* pszField, CString& strValue);

	//
	// Constants.
	//
	static const tchar* VERSION;

protected:
	//
	// Startup and Shutdown template methods.
	//
	virtual	bool OnOpen();
	virtual	bool OnClose();

	//
	// Internal methods.
	//
	void LoadConfig();
	void SaveConfig();

	//
	// Constants.
	//
	static const tchar* INI_FILE_VER;
};

/******************************************************************************
**
** Global variables.
**
*******************************************************************************
*/

// The application object.
extern CHttpQueryApp App;

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/


#endif //HTTPQUERYAPP_HPP
