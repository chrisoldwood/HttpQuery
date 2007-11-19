/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PREFSDLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CPrefsDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "PrefsDlg.hpp"
#include "AddHdrDlg.hpp"
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

CPrefsDlg::CPrefsDlg()
	: CDialog(IDD_PREFS)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_HEADERS, &m_lbHeaders)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_ADD,    BN_CLICKED, &CPrefsDlg::OnAdd   )
		CMD_CTRLMSG(IDC_REMOVE, BN_CLICKED, &CPrefsDlg::OnRemove)
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

void CPrefsDlg::OnInitDialog()
{
	// Set headers listbox style.
	m_lbHeaders.Font(CFont(ANSI_FIXED_FONT));

	// Load the current headers.
	for (int i = 0; i < App.m_astrDefHeaders.Size(); ++i)
		m_lbHeaders.Add(App.m_astrDefHeaders[i]);

	// Select the 1st by default.
	if (m_lbHeaders.Count() > 0)
		m_lbHeaders.CurSel(0);

	UpdateUI();
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

bool CPrefsDlg::OnOk()
{
	// Clear old header list.
	App.m_astrDefHeaders.DeleteAll();

	// Save new header list.
	for (int i = 0; i < m_lbHeaders.Count(); ++i)
		App.m_astrDefHeaders.Add(m_lbHeaders.Text(i));

	return true;
}

/******************************************************************************
** Method:		OnAdd()
**
** Description:	Add a new header.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CPrefsDlg::OnAdd()
{
	CAddHdrDlg Dlg;

	// Query user.
	if (Dlg.RunModal(*this) == IDOK)
	{
		// Add to listbox.
		m_lbHeaders.Add(Dlg.m_strHeader);

		// Update selection, if required.
		if (m_lbHeaders.Count() == 1)
			m_lbHeaders.CurSel(0);
	}

	UpdateUI();
}

/******************************************************************************
** Method:		OnRemove()
**
** Description:	Remove the selected header.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CPrefsDlg::OnRemove()
{
	// Get current selection.
	int nSel = m_lbHeaders.CurSel();

	ASSERT(nSel != LB_ERR);

	// Remove from listbox.
	m_lbHeaders.Delete(nSel);

	// Update selection.
	if (nSel >= m_lbHeaders.Count())
		--nSel;

	m_lbHeaders.CurSel(nSel);

	UpdateUI();
}

/******************************************************************************
** Method:		UpdateUI()
**
** Description:	Update the UI state.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CPrefsDlg::UpdateUI()
{
	bool bSelHdr = (m_lbHeaders.CurSel() != LB_ERR);

	Control(IDC_REMOVE).Enable(bSelHdr);
}
