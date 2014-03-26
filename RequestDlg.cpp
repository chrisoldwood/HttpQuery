/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		REQUESTDLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CRequestDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "RequestDlg.hpp"
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

CRequestDlg::CRequestDlg()
	: CDialog(IDD_REQUEST)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_REQUEST_VERB,			&m_cbVerb   )
		CTRL(IDC_REQUEST_URL,			&m_ebURL    )
		CTRL(IDC_REQUEST_FORMAT,		&m_cbFormat )
		CTRL(IDC_REQUEST_HEADERS_LABEL,	&m_txHeaders)
		CTRL(IDC_REQUEST_HEADERS,		&m_ebHeaders)
		CTRL(IDC_REQUEST_CONTENT_LABEL,	&m_txContent)
		CTRL(IDC_REQUEST_CONTENT,		&m_ebContent)
	END_CTRL_TABLE

	DEFINE_GRAVITY_TABLE
		CTRLGRAV(IDC_REQUEST_VERB,	        LEFT_EDGE,  TOP_EDGE, LEFT_EDGE,  TOP_EDGE   )
		CTRLGRAV(IDC_REQUEST_URL,	        LEFT_EDGE,  TOP_EDGE, RIGHT_EDGE, TOP_EDGE   )
		CTRLGRAV(IDC_REQUEST_FORMAT,        RIGHT_EDGE, TOP_EDGE, RIGHT_EDGE, TOP_EDGE   )
		CTRLGRAV(IDC_REQUEST_HEADERS_LABEL, LEFT_EDGE,  TOP_EDGE, RIGHT_EDGE, TOP_EDGE   )
		CTRLGRAV(IDC_REQUEST_HEADERS,		LEFT_EDGE,  TOP_EDGE, RIGHT_EDGE, TOP_EDGE   )
		CTRLGRAV(IDC_REQUEST_CONTENT_LABEL, LEFT_EDGE,  TOP_EDGE, RIGHT_EDGE, TOP_EDGE   )
		CTRLGRAV(IDC_REQUEST_CONTENT,		LEFT_EDGE,  TOP_EDGE, RIGHT_EDGE, BOTTOM_EDGE)
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

void CRequestDlg::OnInitDialog()
{
	const size_t first = 0;

	// Initialise edit boxes.
	m_ebHeaders.Font(CFont(ANSI_FIXED_FONT));
	m_ebContent.Font(CFont(ANSI_FIXED_FONT));

	// Add request types.
	m_cbVerb.Add(TXT("GET"));
	m_cbVerb.Add(TXT("HEAD"));
	m_cbVerb.Add(TXT("POST"));
	m_cbVerb.Add(TXT("PUT"));
	m_cbVerb.Add(TXT("DELETE"));
	m_cbVerb.CurSel(first);

	// Set default URL.
	m_ebURL.Text(TXT("/"));

	// Add request formats.
	m_cbFormat.Add(TXT("HTTP/1.0"));
	m_cbFormat.Add(TXT("HTTP/1.1"));
	m_cbFormat.CurSel(first);

	// Add default headers.
	for (size_t i = 0; i < App.m_astrDefHeaders.Size(); ++i)
		m_ebHeaders.Append(App.m_astrDefHeaders[i] + TXT("\r\n"));
}
