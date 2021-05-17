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
	//DDX_Control(pDX, IDC_BTNJOIN_APPLICATION, m_btnJoin);
}

BEGIN_MESSAGE_MAP(CEnterApplication, CDialogEx)
	ON_BN_CLICKED(IDC_BTNJOIN_APPLICATION, &CEnterApplication::OnBnClickedBtnjoinChannel)
END_MESSAGE_MAP()

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