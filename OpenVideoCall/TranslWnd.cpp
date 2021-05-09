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
	if (!ctr_Created && CAgoraObject::GetAgoraObject()->GetHostUID() != m_nUID && m_nUID != 0) { InitCtrls(); ctr_Created = TRUE; }
}

void CAGBckWnd::OnPaint()
{
	CPaintDC dc(this);
	CRect	rcClient;
	CString strTip;

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0xFF, 0xFF, 0xFF));

	if (m_bShowTip) {
		// 640x480,15fps,400k
		GetClientRect(&rcClient);
		rcClient.top += 4;
		//strTip.Format(_T("%dx%d, %dfps, %dK"), m_nWidth, m_nHeight, m_nFps, m_nBitrate);
		dc.DrawText(strTip, &rcClient, DT_VCENTER | DT_CENTER);
	}
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
		bMuted = TRUE;
	}
	else
	{
		::SendMessage(GetParent()->GetSafeHwnd(), WM_AUDIOUNMUTECLIENT, (WPARAM)this, (LPARAM)m_nUID);
		bMuted = FALSE;
	}
}

void CAGBckWnd::OnVideoMuteClick() //Webcam mute
{
	if (bHidden == FALSE)
	{
		::SendMessage(GetParent()->GetSafeHwnd(), WM_VIDEOMUTECLIENT, (WPARAM)this, (LPARAM)m_nUID);
		//Sleep(500); //Waits for video stream to end
		SetBackImage(IDB_BACKGROUND_VIDEO, 96, 96, RGB(0x44, 0x44, 0x44));
		bHidden = TRUE;
	}
	else
	{
		::SendMessage(GetParent()->GetSafeHwnd(), WM_VIDEOUNMUTECLIENT, (WPARAM)this, (LPARAM)m_nUID);
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

	if (!ctr_Created && CAgoraObject::GetAgoraObject()->GetHostUID() != m_nUID && m_nUID != 0) { InitCtrls(); ctr_Created = TRUE; } // 
	else if (ctr_Created) {
		CtrlMode = !CtrlMode;
		m_btnShowVid.ShowWindow(CtrlMode);
		m_btnEnableAudio.ShowWindow(CtrlMode);
	}
	CWnd::OnRButtonDown(nFlags, point);
}

void CAGBckWnd::InitCtrls()
{
	CRect		rcClient;
	GetClientRect(&rcClient);


	m_btnEnableAudio.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTN_AUDIO);
	m_btnShowVid.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTN_VIDEO);

	m_btnEnableAudio.MoveWindow(rcClient.Width() - 60, rcClient.Height() - 60, 48, 48, FALSE);
	m_btnEnableAudio.SetBackImage(IDB_BTNMAUDIO_VIDEO, RGB(0x00, 0x00, 0x00));

	m_btnShowVid.MoveWindow(12, rcClient.Height() - 60, 48, 48, FALSE);
	m_btnShowVid.SetBackImage(IDB_BTNVIDEO_VIDEO, RGB(0x00, 0x00, 0x00));

	m_btnShowVid.ShowWindow(1);
	m_btnEnableAudio.ShowWindow(1);
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