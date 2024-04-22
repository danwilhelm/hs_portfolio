// BriefFormatterDlg.h : header file
//

#if !defined(AFX_BRIEFFORMATTERDLG_H__0E3193C6_E63D_11D3_8736_B19ECBB0103C__INCLUDED_)
#define AFX_BRIEFFORMATTERDLG_H__0E3193C6_E63D_11D3_8736_B19ECBB0103C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CBriefFormatterDlg dialog

class CBriefFormatterDlg : public CDialog
{
// Construction
public:
	CBriefFormatterDlg(CWnd* pParent = NULL);	// standard constructor


	// Dialog Data
	//{{AFX_DATA(CBriefFormatterDlg)
	enum { IDD = IDD_BRIEFFORMATTER_DIALOG };
	CString	m_strfilename;
	CString	m_strbyline;
	CString	m_strheadline;
	CString	m_strstory;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBriefFormatterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CBriefFormatterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBUTTONfile();
	afx_msg void OnBUTTONexit();
	afx_msg void OnBUTTONadd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BRIEFFORMATTERDLG_H__0E3193C6_E63D_11D3_8736_B19ECBB0103C__INCLUDED_)
