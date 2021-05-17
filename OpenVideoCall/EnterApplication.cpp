#include "stdafx.h"
#include "OpenVideoCall.h"
#include "EnterApplication.h"


#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CEnterApplication, CDialogEx)

CEnterApplication::CEnterApplication(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEnterApplication::IDD, pParent)
{

}

CEnterApplication::~CEnterApplication()
{
}

void CEnterApplication::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNJOIN_APPLICATION, m_btnJoin);
}

BEGIN_MESSAGE_MAP(CEnterApplication, CDialogEx)
	ON_BN_CLICKED(IDC_BTNJOIN_APPLICATION, &CEnterApplication::OnBnClickedBtnjoinChannel)
END_MESSAGE_MAP()

BOOL CEnterApplication::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	//m_ftTitle.CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	//m_ftDescp.CreateFont(15, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

	SetBackgroundColor(RGB(0xFF, 0xFF, 0xFF));
	InitCtrls();

	AnimateWindow(500, AW_BLEND | AW_SLIDE);
	//m_nTimerID = SetTimer(1, 1000, NULL);

	Invalidate(TRUE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEnterApplication::OnPaint()
{
	CPaintDC dc(this);

	DrawClient(&dc);
}

void CEnterApplication::DrawClient(CDC* lpDC)
{
	CRect	rcText;
	CRect	rcClient;
	LPCTSTR lpString = NULL;

	GetClientRect(&rcClient);

	CFont* defFont = lpDC->SelectObject(&m_ftHead);
	lpDC->SetBkColor(RGB(0xFF, 0xFF, 0xFF));
	lpDC->SetTextColor(RGB(0x44, 0x45, 0x46));
	lpString = LANG_STR("IDS_CHN_TITLE");
	lpDC->TextOut(12, 10, lpString, _tcslen(lpString));

	lpDC->SelectObject(&m_penFrame);
	rcText.SetRect(rcClient.Width() / 2 - 188, 120, rcClient.Width() / 2 + 172, 152);
	lpDC->RoundRect(&rcText, CPoint(32, 32));

	rcText.OffsetRect(0, 48);
	lpDC->RoundRect(&rcText, CPoint(32, 32));

	lpDC->SelectObject(&m_ftDesc);
	lpDC->SetTextColor(RGB(0x91, 0x96, 0xA0));
	lpString = LANG_STR("IDS_CHN_DSC1");
	lpDC->TextOut(12, 45, lpString, _tcslen(lpString));
	lpString = LANG_STR("IDS_CHN_DSC2");
	lpDC->TextOut(12, 65, lpString, _tcslen(lpString));

	lpDC->SetTextColor(RGB(0xD8, 0xD8, 0xD8));
	lpString = LANG_STR("IDS_CHN_ENCTYPE");
	lpDC->TextOut(240, 176, lpString, _tcslen(lpString));

	lpDC->SelectObject(defFont);
}

void CEnterApplication::InitCtrls()
{
	CRect ClientRect;
	CBitmap	bmpNetQuality;

	MoveWindow(0, 0, 720, 600, 1);
	GetClientRect(&ClientRect);

	bmpNetQuality.LoadBitmap(IDB_NETWORK_QUALITY);

	m_btnMin.MoveWindow(ClientRect.Width() - 46, 1, 22, 22, TRUE);
	m_btnClose.MoveWindow(ClientRect.Width() - 23, 1, 22, 22, TRUE);

	m_btnMin.SetBackImage(IDB_BTNMIN, RGB(0xFF, 0, 0xFF));
	m_btnClose.SetBackImage(IDB_BTNCLOSE, RGB(0xFF, 0, 0xFF));

	CMFCButton::EnableWindowsTheming(FALSE);
}

void CEnterApplication::OnBnClickedBtnjoinChannel()
{
	//GetParent()->SendMessage(WM_JOINCHANNEL, 0, 0);
}