/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPCMDS.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CAppCmds class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef APPCMDS_HPP
#define APPCMDS_HPP

/******************************************************************************
** 
** Application command controller.
**
*******************************************************************************
*/

class CAppCmds : public CCmdControl
{
public:
	//
	// Constructors/Destructor.
	//
	CAppCmds();
	~CAppCmds();

	//
	// Commands.
	//

	// Server menu.
	void OnServerConnect();
	void OnServerDisconnect();
	void OnServerExit();

	// Request menu.
	void OnRequestSend();

	// Response menu.
	void OnResponseSaveAs();

	// Window menu.
	void OnWindowRequest();
	void OnWindowResponse();
	void OnWindowNext();
	void OnWindowPrev();

	// Help menu.
	void OnHelpAbout();

	//
	// UI handlers.
	//
	void OnUIServerConnect();
	void OnUIServerDisconnect();

	void OnUIRequestSend();

protected:
	//
	// Internal methods.
	//
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //APPCMDS_HPP
