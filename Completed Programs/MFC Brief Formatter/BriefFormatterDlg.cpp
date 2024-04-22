// BriefFormatterDlg.cpp : implementation file
//

#include <stdlib.h>
#include "stdafx.h"
#include "BriefFormatter.h"
#include "BriefFormatterDlg.h"

CString	stroutfile = "Briefs.htm";
CString strinifile = "C:\\Windows\\BriefFormatter.ini";


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBriefFormatterDlg dialog

CBriefFormatterDlg::CBriefFormatterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBriefFormatterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBriefFormatterDlg)
	m_strfilename = _T("");
	m_strbyline = _T("");
	m_strheadline = _T("");
	m_strstory = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBriefFormatterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBriefFormatterDlg)
	DDX_Text(pDX, IDC_STATIC_filename, m_strfilename);
	DDX_Text(pDX, IDC_EDIT_byline, m_strbyline);
	DDX_Text(pDX, IDC_EDIT_headline, m_strheadline);
	DDX_Text(pDX, IDC_EDIT_story, m_strstory);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBriefFormatterDlg, CDialog)
	//{{AFX_MSG_MAP(CBriefFormatterDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_file, OnBUTTONfile)
	ON_BN_CLICKED(IDC_BUTTON_exit, OnBUTTONexit)
	ON_BN_CLICKED(IDC_BUTTON_add, OnBUTTONadd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBriefFormatterDlg message handlers

BOOL CBriefFormatterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	CStdioFile OutStream;
	if(OutStream.Open(strinifile, CFile::modeRead))		// open ini file to read output directory
	{
		OutStream.ReadString(stroutfile);
		OutStream.Close();	
	}
	else
		MessageBox("Failed to open .ini file for reading.", "Open failed", MB_OK | MB_ICONWARNING);

	m_strfilename = stroutfile;
	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBriefFormatterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBriefFormatterDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBriefFormatterDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CBriefFormatterDlg::OnBUTTONfile() 
{
	CFileDialog dlg( TRUE,_T("htm"),_T("briefs.htm"),
					 OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|
					 OFN_EXTENSIONDIFFERENT,
					 _T("HTML (*.htm)|*.htm|"));

	if( dlg.DoModal()==IDOK )
	{
		UpdateData(TRUE);				// to avoid internally altered strings
										// from appearing
		stroutfile = dlg.GetPathName(); 
		m_strfilename = stroutfile;

		UpdateData(FALSE);
	}


	// WRITE CURRENT FILE TO INI
	CStdioFile OutStream;
	if(OutStream.Open(strinifile, CFile::modeCreate | CFile::modeWrite))
	{
		OutStream.WriteString(stroutfile);
		OutStream.Close();
	}
	else
		MessageBox("Failed to open .ini file for writing.", "Open failed", MB_OK | MB_ICONWARNING);
}

void CBriefFormatterDlg::OnBUTTONexit() 
{
	OnOK();
}

void CBriefFormatterDlg::OnBUTTONadd() 
{
	UpdateData(TRUE);
	
	CString	strhtml;
	CStdioFile OutStream;

	if(m_strheadline == "" || m_strbyline == "" || m_strstory == "")
	{
		MessageBox("All fields are not filled.", "Fields not filled!", MB_OK | MB_ICONWARNING);
		return;
	}

	if(OutStream.Open(stroutfile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite))
	{
		int i, ncurrline = 0, nsecnum = 0;
		CString strbrief = "000";
		CString strnewfile[5000];

		// format the new story beforehand
		m_strheadline = "<h1>" + m_strheadline + "</h1>\n";
		m_strbyline   = "<h3>" + m_strbyline + "</h3>\n\n";
		m_strstory	  = "<blockquote>" + m_strstory + "</blockquote>\n\n";

		for(i=0;i<m_strstory.GetLength();i++)
		{
			if(m_strstory[i] == '\r')
			{
				m_strstory.Delete(i, 2);		// delete the carriage feed
				m_strstory.Insert(i, "</blockquote>\n\n<blockquote>");
				i += 27; 
			}			
		}


		strnewfile[ncurrline++] = "<link rel>\n";		// write html header
		strnewfile[ncurrline++] = "<body>\n\n";

		OutStream.SeekToBegin();
		while(OutStream.ReadString(strhtml))
		{
			if(strhtml.Left(4) == "<!--" && isdigit(strhtml.GetAt(5))) // a write section found!
			{
				for(i=0;i<3;i++)					// obtain the brief number
					strbrief.SetAt(i, strhtml[i+5]);
				i = atoi(strbrief);
			
				strbrief = itoa(++i, strbrief.GetBuffer(3), 10);
				
				if(i < 100) strbrief.Insert(0, "0");	// insert zero placeholders for # brief
				if(i <  10) strbrief.Insert(0, "0");

				nsecnum++;								// which section?	
				if(nsecnum == 1)						// the link portion
				{
					strnewfile[ncurrline++] = "\n";

					strnewfile[ncurrline++] = "<a href=\"#B" + strbrief + "\">" + m_strheadline.Mid(4, m_strheadline.GetLength() - 10) + "</a><br>\n";
				}
				
				if(nsecnum == 2)						// the full text portion
				{
					strnewfile[ncurrline++] = "<hr>\n\n";
					strnewfile[ncurrline++] = "<a name=\"B" + strbrief + "\"></a>\n";
					
					strnewfile[ncurrline++] = m_strheadline;
					strnewfile[ncurrline++] = m_strbyline;
					strnewfile[ncurrline++] = m_strstory;
				}
				
				strhtml = "<!-- " + strbrief + " DO NOT EDIT THIS LINE! -->\n";	// make another ref
			}
			
			if(strhtml.IsEmpty())
				strnewfile[ncurrline-1] += "\n";
			if(strhtml.Left(7) != "</body>" && strhtml.Left(5) != "<link" && strhtml.Left(6) != "<body>")
				strnewfile[ncurrline++] = strhtml;
		}

		if(nsecnum == 0)			// the file was newly created
		{
			strnewfile[ncurrline++] = "<a href=\"#B" + strbrief + "\">" + m_strheadline.Mid(4, m_strheadline.GetLength() - 10) + "</a>\n\n";
			strnewfile[ncurrline++] = "<!-- " + strbrief + " DO NOT EDIT THIS LINE! -->\n\n";
			
			strnewfile[ncurrline++] = "<a name=\"B" + strbrief + "\"></a>\n\n";
			strnewfile[ncurrline++] = m_strheadline;
			strnewfile[ncurrline++] = m_strbyline;
			strnewfile[ncurrline++] = m_strstory + "\n\n";
			strnewfile[ncurrline++] = "<!-- " + strbrief + " DO NOT EDIT THIS LINE! -->\n";
		}

		strnewfile[ncurrline++] = "</body>";	// end writing

		OutStream.SeekToBegin();
		for(i=0;i<ncurrline;i++)
			OutStream.WriteString(strnewfile[i]);

		OutStream.Close();

		m_strbyline   = "";						// clear fields in app
		m_strheadline = "";
		m_strstory    = "";
		UpdateData(FALSE);
	}
	else
		MessageBox("Failed to open output file for writing.", "Open failed", MB_OK | MB_ICONWARNING);
}
