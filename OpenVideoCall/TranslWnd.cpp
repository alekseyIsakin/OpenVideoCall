#include "stdafx.h"
#include "OpenVideoCall.h"
#include "TranslWnd.h"

IMPLEMENT_DYNAMIC(CAGBckWnd, CWnd)

#define IDC_BTN_AUDIO	138
#define IDC_BTN_VIDEO	139

CAGBckWnd::CAGBckWnd()
	: m_bShowTip(TRUE)
	, m_nWidth(0)
	, m_nHeight(0)
	, m_nFps(0)
	, m_nBitrate(0)
{
	m_brBack.CreateSolidBrush(RGB(0x3F, 0x3F, 0x3F));
}

CAGBckWnd::~CAGBckWnd()
{
	m_brBack.DeleteObject();
}


BEGIN_MESSAGE_MAP(CAGBckWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_AUDIO, &CAGBckWnd::OnMuteClick)
	ON_BN_CLICKED(IDC_BTN_VIDEO, &CAGBckWnd::OnVideoMuteClick)
	//ON_MESSAGE(WM_RESIZED, &CAGBckWnd::OnResize)
END_MESSAGE_MAP()


void CAGBckWnd::SetUID(int nUID)
{
	m_nUID = nUID;
}

void CAGBckWnd::OnPaint()
{
	CPaintDC dc(this);
	CRect	rcClient;
	CPoint		ptDraw;

	dc.SetBkMode(TRANSPARENT);

	GetClientRect(&rcClient);
	dc.FillSolidRect(5, 5, 179, 111, RGB(0x00, 0x00, 0x00));
}

BOOL CAGBckWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CRect rcClient;

	GetClientRect(&rcClient);
	pDC->FillRect(&rcClient, &m_brBack);
	
	return TRUE;
}

void CAGBckWnd::OnMuteClick() //Mic mute
{
	if (bMuted == FALSE)
	{
		::SendMessage(GetParent()->GetSafeHwnd(), WM_AUDIOMUTECLIENT, (WPARAM)this, (LPARAM)m_nUID);
		m_btnEnableAudio.SwitchButtonStatus(CAGButton::AGBTN_PUSH);
		bMuted = TRUE;
	}
	else
	{
		::SendMessage(GetParent()->GetSafeHwnd(), WM_AUDIOUNMUTECLIENT, (WPARAM)this, (LPARAM)m_nUID);
		m_btnEnableAudio.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
		bMuted = FALSE;
	}
}

void CAGBckWnd::OnVideoMuteClick() //Webcam mute
{
	if (bHidden == FALSE)
	{
		::SendMessage(GetParent()->GetSafeHwnd(), WM_VIDEOMUTECLIENT, (WPARAM)this, (LPARAM)m_nUID);
		m_btnShowVid.SwitchButtonStatus(CAGButton::AGBTN_PUSH);
		bHidden = TRUE;
	}
	else
	{
		::SendMessage(GetParent()->GetSafeHwnd(), WM_VIDEOUNMUTECLIENT, (WPARAM)this, (LPARAM)m_nUID);
		m_btnShowVid.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
		bHidden = FALSE;
	}
}

//LRESULT CAGBckWnd::OnResize(WPARAM wParam, LPARAM lParam)
//{
//	CRect		rcClient;
//	GetClientRect(&rcClient);
//
//	m_btnEnableAudio.MoveWindow(rcClient.Width() - 48, rcClient.Height() - 84, 48, 48, TRUE);
//	m_btnShowVid.MoveWindow(rcClient.Width() - 105, rcClient.Height() - 84, 48, 48, TRUE);
//	return 0;
//}


void CAGBckWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//::SendMessage(GetParent()->GetSafeHwnd(), WM_SHOWMODECHANGED, (WPARAM)this, (LPARAM)m_nUID);
	CWnd::OnRButtonDown(nFlags, point);
}

void CAGBckWnd::InitCtrls()
{
	if (!ctr_Created && CAgoraObject::GetAgoraObject()->GetHostUID() != m_nUID && m_nUID != 0)
	{
		CRect		rcClient;
		GetClientRect(&rcClient);


		m_btnEnableAudio.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTN_AUDIO);
		m_btnShowVid.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTN_VIDEO);

		m_btnEnableAudio.MoveWindow(rcClient.Width() - 60, rcClient.Height() - 55, 48, 48, TRUE);
		m_btnEnableAudio.SetBackImage(IDB_BITMAP3);
		m_btnEnableAudio.SetBackColor(RGB(0x3F, 0x3F, 0x3F), RGB(0x3F, 0x3F, 0x3F), RGB(0x3F, 0x3F, 0x3F), RGB(0x3F, 0x3F, 0x3F));
		m_btnEnableAudio.EnableFrameEffect(FALSE);

		m_btnShowVid.MoveWindow(12, rcClient.Height() - 55, 48, 48, TRUE);
		m_btnShowVid.SetBackImage(IDB_BITMAP4);
		m_btnShowVid.SetBackColor(RGB(0x3F, 0x3F, 0x3F), RGB(0x3F, 0x3F, 0x3F) , RGB(0x3F, 0x3F, 0x3F), RGB(0x3F, 0x3F, 0x3F));
		m_btnShowVid.EnableFrameEffect(FALSE);

		m_btnShowVid.ShowWindow(1);
		m_btnEnableAudio.ShowWindow(1);
		ctr_Created = TRUE;
	}
}

BOOL CAGBckWnd::SetBackImage(UINT nID, UINT nWidth, UINT nHeight, COLORREF crMask)
{
	CBitmap bmBackImage;

	if (!bmBackImage.LoadBitmap(nID))
		return FALSE;

	m_imgBackGround.DeleteImageList();

	m_imgBackGround.Create(nWidth, nHeight, ILC_COLOR24 | ILC_MASK, 1, 1);
	m_imgBackGround.Add(&bmBackImage, crMask);
	bmBackImage.DeleteObject();

	Invalidate(TRUE);

	return TRUE;
}

BOOL CAGBckWnd::GetIsHidden()
{
	return bMuted;
}