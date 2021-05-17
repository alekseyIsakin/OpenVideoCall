#include "stdafx.h"
#include "OpenVideoCall.h"
#include "EnterApplication.h"


#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CEnterApplication, CDialogEx)

CEnterApplication::CEnterApplication(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEnterApplication::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CEnterApplication::~CEnterApplication()
{
}

void CEnterApplication::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNJOIN_APPLICATION, m_btnJoin);
	DDX_Control(pDX, IDC_BTNMIN, m_btnMin);
	DDX_Control(pDX, IDC_BTNCLOSE, m_btnClose);
}

BEGIN_MESSAGE_MAP(CEnterApplication, CDialogEx)
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BTNMIN, &CEnterApplication::OnBnClickedBtnmin)
	ON_BN_CLICKED(IDC_BTNCLOSE, &CEnterApplication::OnBnClickedBtnclose)
	ON_BN_CLICKED(IDC_BTNJOIN_APPLICATION, &CEnterApplication::OnBnClickedBtnjoinChannel)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CEnterApplication::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

void CEnterApplication::OnBnClickedBtnmin()
{
	ShowWindow(SW_MINIMIZE);
}


void CEnterApplication::OnBnClickedBtnclose()
{
	CDialogEx::OnCancel();
}

LRESULT CEnterApplication::OnNcHitTest(CPoint point)
{
	LRESULT lResult = CDialogEx::OnNcHitTest(point);
	if (lResult == HTCLIENT && ::GetAsyncKeyState(MK_LBUTTON) < 0)
		lResult = HTCAPTION;

	return lResult;
}

void CEnterApplication::DrawClient(CDC* lpDC)
{
	CRect	rcText;
	CRect	rcClient;
	LPCTSTR lpString = NULL;
	//CFont* defFont = lpDC->SelectObject(&m_ftTitle);

	//m_imgNetQuality.Draw(lpDC, m_nLastmileQuality, CPoint(16, 40), ILD_NORMAL);

	GetClientRect(&rcClient);
	lpDC->FillSolidRect(0, 0, 720, 24, RGB(0, 161, 230));
	lpDC->SetBkColor(RGB(0x00, 0x9E, 0xEB));
	lpDC->SetTextColor(RGB(0xFF, 0xFF, 0xFF));


	lpString = _T("RSI Exchange Interpreter Desktop");
	lpDC->TextOut(rcClient.Width() / 2 - 100, 3, lpString, _tcslen(lpString));

	//lpDC->SelectObject(&m_ftDes);
	//lpDC->SetTextColor(RGB(0x91, 0x96, 0xA0));
	//lpDC->SetBkColor(RGB(0xFF, 0xFF, 0xFF));
	//lpString = LANG_STR("IDS_CONTACT");
	//lpDC->TextOut(rcClient.Width() / 2 - 50, rcClient.Height() - 55, lpString, _tcslen(lpString));

	//lpDC->SelectObject(&m_ftPhone);
	//lpDC->SetTextColor(RGB(0x44, 0x45, 0x46));
	//lpString = LANG_STR("IDS_PHONENUMBER");
	//lpDC->TextOut(rcClient.Width() / 2 + 45, rcClient.Height() - 55, lpString, _tcslen(lpString));

	//lpDC->SelectObject(&m_ftDes);
	//lpDC->SetTextColor(RGB(0x91, 0x96, 0xA0));
	//lpDC->SetBkColor(RGB(0xFF, 0xFF, 0xFF));
	//lpString = AVC_VER;// _T("v1.3");
	CString strVersion = CAgoraObject::GetSDKVersionEx();

	rcText.SetRect(0, rcClient.Height() - 30, rcClient.Width(), rcClient.Height() - 5);
	//lpDC->DrawText(strVersion, _tcslen(strVersion), &rcText, DT_CENTER | DT_SINGLELINE);
	//lpDC->SelectObject(defFont);
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
	CDialogEx::OnOK();
}

void CEnterApplication::OnPaint()
{
	CPaintDC dc(this);

	DrawClient(&dc);
}