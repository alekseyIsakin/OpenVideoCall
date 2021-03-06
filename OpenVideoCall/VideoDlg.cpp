// VideoDlg.cpp 
//

#include "stdafx.h"
#include "OpenVideoCall.h"
#include "VideoDlg.h"
#include "afxdialogex.h"
#include "AGEventDef.h"
#include "video_preprocessing_plugin.h"
// CVideoDlg

IMPLEMENT_DYNAMIC(CVideoDlg, CDialogEx)

CVideoDlg::CVideoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoDlg::IDD, pParent)
{
	m_brHead.CreateSolidBrush(RGB(0, 160, 239));
	SetBackgroundColor(RGB(0x26, 0x26, 0x26));
	m_bLastStat = FALSE;
	m_nScreenMode = SCREEN_VIDEO1;
	m_bFullScreen = FALSE;
	m_lpBigShowed = NULL;
    m_bFilter = FALSE;
    m_bShowInfo = FALSE;
}

CVideoDlg::~CVideoDlg()
{
	m_brHead.DeleteObject();
}

void CVideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTNMIN_VIDEO, m_btnMin);
	DDX_Control(pDX, IDC_BTNRSTO_VIDEO, m_btnRst);
	DDX_Control(pDX, IDC_BTNCLOSE_VIDEO, m_btnClose);
}


BEGIN_MESSAGE_MAP(CVideoDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	//ON_WM_LBUTTONDOWN(IDB_BTNMCOUGH_VIDEO, &CVideoDlg::OnLButtonDown)
	ON_WM_LBUTTONDOWN()
	//ON_WM_LBUTTONUP(IDB_BTNMCOUGH_VIDEO, &CVideoDlg::OnLButtonUp)
	ON_WM_LBUTTONUP()

	ON_MESSAGE(WM_SHOWMODECHANGED, &CVideoDlg::OnShowModeChanged)
	ON_MESSAGE(WM_SHOWBIG, &CVideoDlg::OnShowBig)

	ON_MESSAGE(WM_WINDOWSHARE, &CVideoDlg::OnWindowShareStart)
	ON_MESSAGE(WM_DESKTOPSHARE, &CVideoDlg::OnDesktopShareStart)

	ON_MESSAGE(WM_MSGID(EID_JOINCHANNEL_SUCCESS), &CVideoDlg::OnEIDJoinChannelSuccess)
	ON_MESSAGE(WM_MSGID(EID_REJOINCHANNEL_SUCCESS), &CVideoDlg::OnEIDReJoinChannelSuccess)
	ON_MESSAGE(WM_MSGID(EID_FIRST_LOCAL_VIDEO_FRAME), &CVideoDlg::OnEIDFirstLocalFrame)

	ON_MESSAGE(WM_MSGID(EID_FIRST_REMOTE_VIDEO_DECODED), &CVideoDlg::OnEIDFirstRemoteFrameDecoded)
	ON_MESSAGE(WM_MSGID(EID_USER_OFFLINE), &CVideoDlg::OnEIDUserOffline)
	ON_MESSAGE(WM_MSGID(EID_USER_JOINED), &CVideoDlg::OnEIDUserJoined)


	ON_MESSAGE(WM_MSGID(EID_REMOTE_VIDEO_STAT), &CVideoDlg::OnRemoteVideoStat)

	ON_MESSAGE(WM_MSGID(EID_START_RCDSRV), &CVideoDlg::OnStartRecordingService)
	ON_MESSAGE(WM_MSGID(EID_STOP_RCDSRV), &CVideoDlg::OnStopRecordingService)

	ON_MESSAGE(WM_MSGID(EID_STREAM_MESSAGE), &CVideoDlg::OnStreamMessage)


	ON_BN_CLICKED(IDC_BTNMIN_VIDEO, &CVideoDlg::OnBnClickedBtnmin)
	ON_BN_CLICKED(IDC_BTNCLOSE_VIDEO, &CVideoDlg::OnBnClickedBtnclose)
	ON_BN_CLICKED(IDC_BTNRSTO_VIDEO, &CVideoDlg::OnBnClickedBtnrest)
	ON_BN_CLICKED(IDC_BTNENDCALL_VIDEO, &CVideoDlg::OnBnClickedBtnclose)

	ON_BN_CLICKED(IDC_BTNMESSAGE_VIDEO, &CVideoDlg::OnBnClickedBtnmessage)

	ON_BN_CLICKED(IDC_BTNSCRCAP_VIDEO, &CVideoDlg::OnBnClickedBtnScreenCapture)
	ON_BN_CLICKED(IDC_BTNMORE_VIDEO, &CVideoDlg::OnBnClickedBtnMore)
	ON_BN_CLICKED(IDC_BTNMODE_VIDEO, &CVideoDlg::OnBnClickedBtnmode)
	ON_BN_CLICKED(IDC_BTNAUDIO_VIDEO, &CVideoDlg::OnBnClickedBtnaudio)


	ON_BN_CLICKED(ID_IDR_VIDEOINFO, &CVideoDlg::OnBnClickedBtntip)
	ON_BN_CLICKED(ID_IDR_DEVICE, &CVideoDlg::OnBnClickedBtnsetup)
	ON_BN_CLICKED(ID_IDR_FILTER, &CVideoDlg::OnBnClickedBtnfilter)
	ON_BN_CLICKED(IDB_BTNMCOUGH_VIDEO, &CVideoDlg::OnBnClickedBtncough)

    ON_BN_CLICKED(ID_IDR_VIDEOINFO, &CVideoDlg::OnBnClickedBtntip)
    ON_BN_CLICKED(ID_IDR_DEVICE, &CVideoDlg::OnBnClickedBtnsetup)
    ON_BN_CLICKED(ID_IDR_FILTER, &CVideoDlg::OnBnClickedBtnfilter)

	//	ON_BN_CLICKED(IDC_BTNWHITEBOARD_VIDEO, &CVideoDlg::OnBnCliekedBtnWhiteBoard)
	//	ON_BN_CLICKED(IDC_BTNCLOSEWB_VIDEO, &CVideoDlg::OnBnCliekedBtnCloseWhiteBoard)

	ON_BN_CLICKED(IDC_BTNSCR_VIDEO, &CVideoDlg::OnBnClickedBtnfullscr)

	ON_BN_CLICKED(ID_SCRSHARE_DESKTOPSHARE, &CVideoDlg::OnBnClickedScreenshare)
	ON_BN_CLICKED(ID_SCRSHARE_WINDOWSHARE, &CVideoDlg::OnBnClickedWindowshare)

	//	ON_BN_CLICKED(ID_WHITEBOARD_HOSTMODE, &CVideoDlg::OnBnClickedHostMode)
	//	ON_BN_CLICKED(ID_WHITEBOARD_GUESTMODE, &CVideoDlg::OnBnClickedGuestMode)

	ON_CBN_SELCHANGE(IDC_CBXRELAY_VIDEO, &CVideoDlg::OnCbnSelchangeCmb)
	ON_CBN_SELCHANGE(IDC_CBXRDEST_VIDEO, &CVideoDlg::OnCbnSelchangeCmb)
	ON_BN_CLICKED(IDC_BTNPUBLISH_VIDEO, &CVideoDlg::OnBtnClickPublish)

	ON_MESSAGE(WM_AUDIOMUTECLIENT, &CVideoDlg::MuteClient)
	ON_MESSAGE(WM_AUDIOUNMUTECLIENT, &CVideoDlg::UnMuteClient)
	ON_MESSAGE(WM_VIDEOMUTECLIENT, &CVideoDlg::HideClient)
	ON_MESSAGE(WM_VIDEOUNMUTECLIENT, &CVideoDlg::UnHideClient)

	ON_WM_SHOWWINDOW()
    ON_WM_MOVE()
END_MESSAGE_MAP()

void CVideoDlg::OnCbnSelchangeCmb()
{
	int curSel = CollectSelInd();

	ListWindowRemoveAll();
	GetParent()->SendMessage(WM_LEAVECHANNEL, 0, 0);
	GetParent()->SendMessage(WM_JOINCHANNEL, (WPARAM)CHANNEL_CHANGE::CHANNEL_CHANGE_RELAY, curSel);
	RebindVideoWnd();
}

void CVideoDlg::OnBtnClickPublish()
{
	BOOL isPublish = CAgoraObject::GetAgoraObject()->IsPublish();
	ListWindowRemoveAll();

	if (!isPublish) PublishStream();
	else			UnPublishStream();

}

void CVideoDlg::UnPublishStream(BOOL joinBack)
{
	int				curSel = CollectSelInd();
	CHANNEL_CHANGE	ch_state = CHANNEL_CHANGE::CHANNEL_UNPUBLISH;

	EnableCBox(true);
	m_btnPublish.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);

	GetParent()->SendMessage(WM_LEAVECHANNEL, 0, 0);
	if (joinBack)
		GetParent()->SendMessage(WM_JOINCHANNEL, (WPARAM)ch_state, curSel);
}

void CVideoDlg::PublishStream()
{
	if (m_cmbDest.GetCurSel() != m_cmbRelay.GetCurSel())
	{
		CAgoraObject* lpAgora = CAgoraObject::GetAgoraObject();
		int				curSel = CollectSelInd();

		CHANNEL_CHANGE ch_state = CHANNEL_CHANGE::CHANNEL_PUBLISH;

		EnableCBox(false);

		if (lpAgora->IsLocalAudioMuted()) {
			lpAgora->MuteLocalAudio(FALSE);
			m_btnAudio.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
		}

		m_btnPublish.SwitchButtonStatus(CAGButton::AGBTN_PUSH);

		GetParent()->SendMessage(WM_LEAVECHANNEL, 0, 0);
		GetParent()->SendMessage(WM_JOINCHANNEL, (WPARAM)ch_state, curSel);
	}
}
// CVideoDlg

void CVideoDlg::UpdateDestCBox(Tokens token, int curSel)
{
	int nIndex = 0;
	m_cmbDest.ResetContent();

	//m_cmbDest.InsertString(nIndex, _T("Off"));
	//nIndex++;

	for (auto langIt = token.GetTargetLngBgnItr(); langIt != token.GetTargetLngEndItr()-1; langIt++) {

		CString str((*langIt).langShort.c_str());
		m_cmbDest.InsertString(nIndex, str);
		nIndex++;
	}
	m_cmbDest.SetCurSel(curSel);
}

void CVideoDlg::UpdateRelayCBox(Tokens token, int curSel)
{
	int nIndex = 0;
	m_cmbRelay.ResetContent();

	for (auto langIt = token.GetTargetLngBgnItr(); langIt != token.GetTargetLngEndItr(); langIt++) {

		CString str((*langIt).langShort.c_str());
		m_cmbRelay.InsertString(nIndex, str);
		nIndex++;
	}
	m_cmbRelay.SetCurSel(curSel);
}
void CVideoDlg::EnableCBox(BOOL state)
{
	m_cmbDest.EnableWindow(state);
	m_cmbRelay.EnableWindow(state);
	m_cmbRelay.ShowWindow(state);
	m_cmbDest.ShowWindow(state);
	if (state == 1)
		m_btnPublish.SetBackImage(IDB_PUBLISH, RGB(0x26, 0x26, 0x26));
	else
		m_btnPublish.SetBackImage(IDB_PUBLISHSTOP, RGB(0x26, 0x26, 0x26));
}
int CVideoDlg::CollectSelInd()
{
	return  m_cmbDest.GetCurSel() + 
			m_cmbRelay.GetCurSel() * 1000;
}
void CVideoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:
	if (m_btnMin.GetSafeHwnd() != NULL)
		m_btnMin.MoveWindow(cx - 72, 0, 24, 24, TRUE);
	if (m_btnRst.GetSafeHwnd() != NULL)
		m_btnRst.MoveWindow(cx - 48, 0, 24, 24, TRUE);
	if (m_btnClose.GetSafeHwnd() != NULL)
		m_btnClose.MoveWindow(cx - 24, 0, 24, 24, TRUE);

	GetClientRect(&m_rcVideoArea);
	m_rcVideoArea.top += 24;
	m_rcVideoArea.bottom -= 72;

	//
	m_rcChildVideoArea.top = m_rcVideoArea.top + 10;
	m_rcChildVideoArea.bottom = m_rcChildVideoArea.top + 144;
	m_rcChildVideoArea.right = m_rcVideoArea.right - 14;
	m_rcChildVideoArea.left = m_rcChildVideoArea.right - 192;

    if (::IsWindow(m_dlgChat.GetSafeHwnd()) && m_dlgChat.IsWindowVisible()) {
        CRect   rcChatArea;

        rcChatArea.CopyRect(&m_rcVideoArea);
        rcChatArea.top = rcChatArea.bottom - 150;
        ClientToScreen(&rcChatArea);
        m_dlgChat.MoveWindow(&rcChatArea);
    }

	if (m_wndLocal.GetSafeHwnd() == NULL || m_wndVideo[0].GetSafeHwnd() == NULL)
		return;

	switch (m_nScreenMode)
	{
	case SCREEN_VIDEO1:
		AdjustSizeVideo1(cx, cy);
		AdjustButtonsNormal(cx, cy);
		break;
	case SCREEN_VIDEO4:
		AdjustSizeVideo4(cx, cy);
		AdjustButtonsNormal(cx, cy);
		break;
	case SCREEN_VIDEOMULTI:
		AdjustSizeVideoMulti(cx, cy);
		AdjustButtonsNormal(cx, cy);
		break;
	default:
		break;
	}
	Invalidate(TRUE);
}

void CVideoDlg::ShowButtonsNormal()
{
	CRect	rcClient;
	int nShowMode = SW_HIDE;

	if (!m_bFullScreen)
		nShowMode = SW_SHOW;

    m_btnMessage.ShowWindow(nShowMode);
	m_btnMode.ShowWindow(nShowMode);
    m_btnMore.ShowWindow(nShowMode);
	
	m_btnCough.SetBackImage(IDB_COUGHMUTE, RGB(0x26, 0x26, 0x26));
	m_btnCough.ShowWindow(nShowMode);
	m_btnAudio.SetBackImage(IDB_BTNMAUDIO_VIDEO, RGB(0x26, 0x26, 0x26));
	m_btnAudio.ShowWindow(nShowMode);
	m_btnEndCall.SetBackImage(IDB_END_MEETING, RGB(0x26, 0x26, 0x26));
	m_btnEndCall.ShowWindow(nShowMode);
	m_btnScrCap.ShowWindow(nShowMode);
	//m_btnShow.ShowWindow(nShowMode);
	m_cmbDest.SetParent(this);
	m_cmbRelay.SetParent(this);

	GetClientRect(&rcClient);
	AdjustButtonsNormal(rcClient.Width(), rcClient.Height());
}


void CVideoDlg::AdjustButtonsNormal(int cx, int cy)
{
//	if (m_btnSetup.GetSafeHwnd() != NULL)
//		m_btnSetup.MoveWindow(30, cy - 48, 24, 24, TRUE);
	CRect	rcClient;
	GetClientRect(&rcClient);
	if (m_btnCough.GetSafeHwnd() != NULL)
		m_btnCough.MoveWindow(rcClient.left + 130, cy - 60, 128, 53, TRUE);
    if(m_btnMessage.GetSafeHwnd() != NULL)
        m_btnMessage.MoveWindow(rcClient.left + 10, cy - 60, 48, 48, TRUE);
	if (m_btnMode.GetSafeHwnd() != NULL)
		m_btnMode.MoveWindow(rcClient.left + 70, cy - 60, 48, 48, TRUE);
	if (m_btnAudio.GetSafeHwnd() != NULL)
		m_btnAudio.MoveWindow(cx / 2 - 24 * ((double)cx / 948), cy - 60, 48, 48, TRUE);
	if (m_btnEndCall.GetSafeHwnd() != NULL)
		m_btnEndCall.MoveWindow(rcClient.right - 135, cy - 62, 128, 53, TRUE);

	if (m_btnScrCap.GetSafeHwnd() != NULL)
		m_btnScrCap.MoveWindow(cx / 2 + 72 * ((double)cx / 948), cy - 60, 48, 48, TRUE);
    if (m_btnMore.GetSafeHwnd() != NULL)
        m_btnMore.MoveWindow(rcClient.right - 250, cy - 60, 48, 48, TRUE);
	
	//if (m_btnShow.GetSafeHwnd() != NULL)
	//	m_btnShow.MoveWindow(cx - 72, cy - 48, 48, 48, TRUE);

	if (m_cmbRelay.GetSafeHwnd() != NULL)
		m_cmbRelay.MoveWindow(rcClient.left + 50, cy - 550 * cy / 714, 60, 42, TRUE);;
	if (m_cmbDest.GetSafeHwnd() != NULL)
		m_cmbDest.MoveWindow(rcClient.right - 110, cy - 550 * cy / 714, 60, 42, TRUE);

	if (m_btnPublish.GetSafeHwnd() != NULL)
		m_btnPublish.MoveWindow(rcClient.right - 190, cy - 60, 48, 48, TRUE);

}

void CVideoDlg::AdjustSizeVideo1(int cx, int cy)
{
	if (ListWindowGetTotalCount() == 0)
		m_wndLocal.MoveWindow(&m_rcVideoArea, FALSE);
	else {
		m_wndVideo[0].MoveWindow(&m_rcVideoArea, FALSE);
		m_wndLocal.MoveWindow(&m_rcChildVideoArea, FALSE);
	}
}

void CVideoDlg::AdjustSizeVideo4(int cx, int cy)
{
	m_wndLocal.MoveWindow((m_rcVideoArea.Width() / 2) - 300, m_rcVideoArea.top + 400, 192, 144, TRUE);
	for (int nIndex = 0; nIndex < 4; nIndex++) {
		int nXPos = (m_rcVideoArea.Width() / 2) - 300 + (204 * nIndex);
		int nYPos = m_rcVideoArea.top + 400;

		if (!m_wndVideo[nIndex].IsBigShow()) {
			if (m_wndVideo[nIndex].GetUID() != 0) {
				m_wndVideo[nIndex].MoveWindow(nXPos, nYPos, 192, 144, TRUE);
			}
		}
	}
}

void CVideoDlg::AdjustSizeVideoMulti(int cx, int cy)
{
	int nLocalIndex = 0;

	m_lpBigShowed->MoveWindow(&m_rcVideoArea, TRUE);

	for (int nIndex = 0; nIndex < 4; nIndex++) {
		int nXPos = (m_rcVideoArea.Width() / 2) - 402 + (204 * nLocalIndex);
		int nYPos = m_rcVideoArea.bottom - 200;

		if (!m_wndVideo[nIndex].IsBigShow()) {
			if (m_wndVideo[nIndex].GetUID() != 0) {
				m_wndTransl[nIndex].MoveWindow(nXPos, nYPos, 191, 200, TRUE);
				m_wndVideo[nIndex].MoveWindow(nXPos + 5, nYPos + 5, 180, 135, TRUE);
				nLocalIndex++;
			}
		}
		else {
			m_wndTrLocal.MoveWindow(nXPos, nYPos, 191, 200, TRUE);
			m_wndLocal.MoveWindow(nXPos + 5, nYPos + 5, 180, 135, TRUE);
			nLocalIndex++;
		}
	}
}

POSITION CVideoDlg::ListWindowGetHeadPos(CHANNEL_TYPE channel)
{
	switch (channel)
	{
	case (CHANNEL_TYPE::CHANNEL_HOST):
		return m_listWndInfoHost.GetHeadPosition();
	case (CHANNEL_TYPE::CHANNEL_TRANSL):
	case (CHANNEL_TYPE::CHANNEL_DEST):
		return  m_listWndInfoDest.GetHeadPosition();
	case (CHANNEL_TYPE::CHANNEL_SRC):
		break;
	}
	return POSITION();
}

CVideoDlg::AGVIDEO_WNDINFO CVideoDlg::ListWindowGetNextPos(CHANNEL_TYPE channel, POSITION& pos)
{
	switch (channel)
	{
	case (CHANNEL_TYPE::CHANNEL_HOST):
		return m_listWndInfoHost.GetNext(pos);
	case (CHANNEL_TYPE::CHANNEL_TRANSL):
	case (CHANNEL_TYPE::CHANNEL_DEST):
		return m_listWndInfoDest.GetNext(pos);
	case (CHANNEL_TYPE::CHANNEL_SRC):
		break;
	}
	return CVideoDlg::AGVIDEO_WNDINFO();
}

CVideoDlg::AGVIDEO_WNDINFO CVideoDlg::ListWindowGetAt(CHANNEL_TYPE channel, POSITION pos)
{
	switch (channel)
	{
	case (CHANNEL_TYPE::CHANNEL_HOST):
		return m_listWndInfoHost.GetAt(pos);
	case (CHANNEL_TYPE::CHANNEL_TRANSL):
	case (CHANNEL_TYPE::CHANNEL_DEST):
		return m_listWndInfoDest.GetAt(pos);
	case (CHANNEL_TYPE::CHANNEL_SRC):
		break;
	}
	return CVideoDlg::AGVIDEO_WNDINFO();
}

void CVideoDlg::ListWindowRemoveAt(CHANNEL_TYPE channel, POSITION pos)
{
	switch (channel)
	{
	case (CHANNEL_TYPE::CHANNEL_HOST):
		m_listWndInfoHost.RemoveAt(pos);
		break;
	case (CHANNEL_TYPE::CHANNEL_TRANSL):
	case (CHANNEL_TYPE::CHANNEL_DEST):
		m_listWndInfoDest.RemoveAt(pos);
		break;
	case (CHANNEL_TYPE::CHANNEL_SRC):
		break;
	}
}

void CVideoDlg::ListWindowAddTail(CHANNEL_TYPE channel, AGVIDEO_WNDINFO wnd)
{
	switch (channel)
	{
	case (CHANNEL_TYPE::CHANNEL_HOST):
		m_listWndInfoHost.AddTail(wnd);
		break;
	case (CHANNEL_TYPE::CHANNEL_TRANSL):
	case (CHANNEL_TYPE::CHANNEL_DEST):
		m_listWndInfoDest.AddTail(wnd);
		break;
	case (CHANNEL_TYPE::CHANNEL_SRC):
		break;
	}
}

void CVideoDlg::ListWindowRemoveAll(CHANNEL_TYPE channel)
{
	switch (channel)
	{
	case (CHANNEL_TYPE::CHANNEL_HOST):
		m_listWndInfoHost.RemoveAll();
		break;
	case (CHANNEL_TYPE::CHANNEL_TRANSL):
	case (CHANNEL_TYPE::CHANNEL_DEST):
		m_listWndInfoDest.RemoveAll();
		break;
	case (CHANNEL_TYPE::CHANNEL_SRC):
		break;
	}
}

void CVideoDlg::ListWindowRemoveAll()
{
	m_listWndInfoHost.RemoveAll();
	m_listWndInfoDest.RemoveAll();
}

UINT CVideoDlg::ListWindowGetCount(CHANNEL_TYPE channel)
{
	switch (channel)
	{
	case (CHANNEL_TYPE::CHANNEL_HOST):
		return m_listWndInfoHost.GetCount();
		break;
	case (CHANNEL_TYPE::CHANNEL_TRANSL):
	case (CHANNEL_TYPE::CHANNEL_DEST):
		return m_listWndInfoDest.GetCount();
	case (CHANNEL_TYPE::CHANNEL_SRC):
		break;
	}
	return 0;
}

UINT CVideoDlg::ListWindowGetTotalCount()
{
	return	m_listWndInfoHost.GetCount() +
		m_listWndInfoDest.GetCount();
}



LRESULT CVideoDlg::MuteClient(WPARAM wParam, LPARAM lParam) //Mutes specific user
{
	CAgoraObject* lpAgora = CAgoraObject::GetAgoraObject();

	lpAgora->MuteClient(lParam, 1);

	return 0;
}

LRESULT CVideoDlg::UnMuteClient(WPARAM wParam, LPARAM lParam) //UnMutes specific user
{
	CAgoraObject* lpAgora = CAgoraObject::GetAgoraObject();

	lpAgora->MuteClient(lParam, 0);

	return 0;
}


LRESULT CVideoDlg::HideClient(WPARAM wParam, LPARAM lParam) //Hides user's webcam
{
	UpdateVideoWnd(lParam, 0);

	return 0;
}

LRESULT CVideoDlg::UnHideClient(WPARAM wParam, LPARAM lParam) //Shows user's webcam
{
	UpdateVideoWnd(lParam, 1);

	return 0;
}

void CVideoDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:

	CDialogEx::OnMouseMove(nFlags, point);
}

BOOL CVideoDlg::NeedShowSizeBox(CPoint point)
{
	CRect rcClient;

	GetClientRect(&rcClient);

	rcClient.left += 5;
	rcClient.right -= 5;
	rcClient.top += 5;
	rcClient.bottom -= 5;
	
	if (rcClient.PtInRect(point) || IsZoomed())
		return FALSE;

	return TRUE;
}

void CVideoDlg::EnableSize(BOOL bEnable)
{
	LONG lStyle = ::GetWindowLong(GetSafeHwnd(), GWL_STYLE);

	if (bEnable)
		::SetWindowLong(GetSafeHwnd(), GWL_STYLE, lStyle | WS_SIZEBOX);
	else
		::SetWindowLong(GetSafeHwnd(), GWL_STYLE, lStyle&(~WS_SIZEBOX));

	CRect rcClient;

	if (bEnable)
		GetWindowRect(&rcClient);
	else
		GetClientRect(&rcClient);

	int cx = rcClient.Width();
	int cy = rcClient.Height();

	if (m_bLastStat != bEnable) {
		m_bLastStat = bEnable;
		if (m_btnMin.GetSafeHwnd() != NULL)
			m_btnMin.MoveWindow(cx - 72, 0, 24, 24, TRUE);
		if (m_btnRst.GetSafeHwnd() != NULL)
			m_btnRst.MoveWindow(cx - 48, 0, 24, 24, TRUE);
		if (m_btnClose.GetSafeHwnd() != NULL)
			m_btnClose.MoveWindow(cx - 24, 0, 24, 24, TRUE);

//		if (m_btnTip.GetSafeHwnd() != NULL)
//			m_btnTip.MoveWindow(cx / 2 - 372, cy - 84, 72, 72, TRUE);

		if (m_btnScrCap.GetSafeHwnd() != NULL)
			m_btnScrCap.MoveWindow(48, cy - 84, 48, 48, TRUE);

//		if (m_btnSetup.GetSafeHwnd() != NULL)
//			m_btnSetup.MoveWindow(cx / 2 - 216, cy - 84, 72, 72, TRUE);
		if (m_btnCough.GetSafeHwnd() != NULL)
			m_btnCough.MoveWindow(cx / 2 + 192, cy - 84, 128, 53, TRUE);

		if (m_btnMode.GetSafeHwnd() != NULL)
			m_btnMode.MoveWindow(cx / 2 - 96, cy - 84, 48, 48, TRUE);

		if (m_btnAudio.GetSafeHwnd() != NULL)
			m_btnAudio.MoveWindow(cx / 2 + 24, cy - 84, 48, 48, TRUE);

		if (m_btnShow.GetSafeHwnd() != NULL)
            m_btnShow.MoveWindow(cx - 72, cy - 84, 48, 48, TRUE);

		if (m_btnEndCall.GetSafeHwnd() != NULL)
			m_btnEndCall.MoveWindow(cx - 120, cy - 84, 128, 53, TRUE);

		Invalidate(FALSE);
	}
}

void CVideoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	//CDialogEx::OnPaint()
	DrawHead(&dc);
}


LRESULT CVideoDlg::OnNcHitTest(CPoint point)
{
	LRESULT lResult = CDialogEx::OnNcHitTest(point);
	if (lResult == HTCLIENT && ::GetAsyncKeyState(MK_LBUTTON) < 0)
		lResult = HTCAPTION;

	return lResult;
}

void CVideoDlg::OnBnClickedBtnmin()
{
	ShowWindow(SW_MINIMIZE);
}

void CVideoDlg::OnBnClickedBtnclose()
{
	UnPublishStream(false);
	ListWindowRemoveAll();

	ListWindowRemoveAll();
	m_dlgChat.ShowWindow(SW_HIDE);
	m_dlgDevice.ShowWindow(SW_HIDE);

	// unmute local video
	CAgoraObject::GetAgoraObject()->MuteLocalVideo(FALSE);
	m_btnMode.SetBackImage(IDB_BTNAUDIO_VIDEO);

	// unmute local audio
	CAgoraObject::GetAgoraObject()->MuteLocalAudio(FALSE);
	m_btnAudio.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);

	CAgoraObject::GetAgoraObject()->MuteLocalAudio(FALSE);		//????? ???? ???
	m_btnCough.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);

    CAgoraObject::GetAgoraObject()->EnableScreenCapture(NULL, 0, NULL, FALSE);
    m_btnScrCap.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);

	m_dlgChat.ShowWindow(SW_HIDE);
	m_dlgChat.ClearHistory();
	m_btnMessage.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);

	CDialogEx::OnOK();
}

void CVideoDlg::OnBnClickedBtnrest()
{
	if (IsZoomed()) {
		m_btnRst.SetBackImage(IDB_BTNMAX, RGB(0xFF, 0x00, 0xFF));
		ShowWindow(SW_RESTORE);
	}
	else{
		m_btnRst.SetBackImage(IDB_BTNRESTORE, RGB(0xFF, 0x00, 0xFF));
		ShowWindow(SW_MAXIMIZE);
	}

	CRect	rcClient;

	GetClientRect(&rcClient);
	m_rcVideoArea.CopyRect(&rcClient);
	m_rcVideoArea.top += 24;
	m_rcVideoArea.bottom -= 72;
	  
	m_rcChildVideoArea.top = m_rcVideoArea.top + 16;
	m_rcChildVideoArea.bottom = m_rcChildVideoArea.top + 144;
	m_rcChildVideoArea.left = m_rcVideoArea.right - 212;
	m_rcChildVideoArea.right = m_rcVideoArea.right - 20;

	int cx = rcClient.Width();
	int cy = rcClient.Height();

	//switch (m_nScreenMode)
	//{
	//case SCREEN_VIDEO1:
	//	AdjustSizeVideo1(cx, cy);
	//	AdjustButtonsNormal(cx, cy);
	//	break;
	//case SCREEN_VIDEO4:
	//	AdjustSizeVideo4(cx, cy);
	//	AdjustButtonsNormal(cx, cy);
	//	break;
	//case SCREEN_VIDEOMULTI:
	//	AdjustSizeVideoMulti(cx, cy);
	//	AdjustButtonsNormal(cx, cy);
	//	break;
	//default:
	//	break;
	//}
	AdjustSizeVideoMulti(cx, cy);
	AdjustButtonsNormal(cx, cy);
	ShowMulti();
	Invalidate(TRUE);
}


void CVideoDlg::OnBnClickedBtnfullscr()
{
	int nShowMode = 0;

	if (m_bFullScreen) {
		m_bFullScreen = FALSE;
		nShowMode = SW_SHOW;		
		ShowWindow(SW_RESTORE);

		GetClientRect(&m_rcVideoArea);
		m_rcVideoArea.top += 24;
		m_rcVideoArea.bottom -= 72;	
	}
	else{
		m_bFullScreen = TRUE;
		nShowMode = SW_HIDE;
		ShowWindow(SW_MAXIMIZE);

		GetClientRect(&m_rcVideoArea);
	}

	m_btnMin.ShowWindow(nShowMode);
	m_btnRst.ShowWindow(nShowMode);
	m_btnClose.ShowWindow(nShowMode);

    m_btnMessage.ShowWindow(nShowMode);
	m_btnMode.ShowWindow(nShowMode);
	m_btnAudio.ShowWindow(nShowMode);
	m_btnEndCall.ShowWindow(nShowMode);
	m_btnScrCap.ShowWindow(nShowMode);
    m_btnMore.ShowWindow(nShowMode);
	m_btnPublish.ShowWindow(nShowMode);
	
	m_btnCough.ShowWindow(nShowMode);

	m_btnShow.ShowWindow(nShowMode);

	//switch (m_nScreenMode)
	//{
	//case SCREEN_VIDEO1:
	//	ShowVideo1();
	//	break;
	//case SCREEN_VIDEO4:
	//	ShowVideo4();
	//	break;
	//case SCREEN_VIDEOMULTI:
	//	ShowMulti();
	//	break;
	//default:
	//	break;
	//}
	ShowMulti();
	Invalidate(TRUE);
}

void CVideoDlg::OnBnClickedScreenshare()
{
	m_dlgDesktopCapture.SaveScreen(NULL);
	m_dlgDesktopCapture.ShowWindow(SW_MAXIMIZE);
}

void CVideoDlg::OnBnClickedWindowshare()
{
	m_dlgScreenCapture.RefreashWndInfo();
	m_dlgScreenCapture.SaveScreen(NULL);

	m_dlgScreenCapture.ShowWindow(SW_MAXIMIZE);
}

void CVideoDlg::ShowControlButton(BOOL bShow)
{
	int nShowMode = 0;
	if (bShow)
		nShowMode = SW_SHOW;
	else
		nShowMode = SW_HIDE;

	m_btnScrCap.ShowWindow(nShowMode);
    m_btnMessage.ShowWindow(nShowMode);
	m_btnMode.ShowWindow(nShowMode);
    m_btnMore.ShowWindow(nShowMode);
	m_btnAudio.ShowWindow(nShowMode);
	//m_btnShow.ShowWindow(nShowMode);
	m_btnEndCall.ShowWindow(nShowMode);
	m_btnPublish.ShowWindow(nShowMode);

	m_btnCough.ShowWindow(nShowMode);
}

void CVideoDlg::OnBnClickedBtntip()
{
	for (int nIndex = 0; nIndex < 4; nIndex++)
        m_wndVideo[nIndex].ShowVideoInfo(!m_bShowInfo);

    m_bShowInfo = !m_bShowInfo;
}

void CVideoDlg::OnBnClickedBtnScreenCapture()
{
	CAgoraObject *lpAgora = CAgoraObject::GetAgoraObject();
	
	if (lpAgora->IsScreenCaptureEnabled()) {
		lpAgora->EnableScreenCapture(NULL, 0, NULL, FALSE);
		m_btnScrCap.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
        CAgoraObject::GetAgoraObject()->EnableLocalRender(TRUE);
	}
	else
		CreateScreenShareMenu();

	Invalidate();
}

void CVideoDlg::OnBnClickedBtnMore()
{
    CreateMoreMenu();
}

void CVideoDlg::CreateScreenShareMenu()
{
	CRect				rcButton;
	CMenu				Menu;
	MENUITEMINFO		mii;

	Menu.LoadMenu(IDR_MENU_SCRSHARE);

	CMenu *pMenuPopup = Menu.GetSubMenu(0);
	
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STRING;

	mii.dwTypeData = const_cast<LPTSTR>LANG_STR("IDS_VIDEO_SCRSHARE");
	mii.cch = _tcslen(LANG_STR("IDS_VIDEO_SCRSHARE")) + 1;
	pMenuPopup->SetMenuItemInfo(0, &mii, FALSE);
	
	mii.dwTypeData = const_cast<LPTSTR>LANG_STR("IDS_VIDEO_WNDSHARE");
	mii.cch = _tcslen(LANG_STR("IDS_VIDEO_WNDSHARE")) + 1;
	pMenuPopup->SetMenuItemInfo(1, &mii, FALSE);
	m_btnScrCap.GetWindowRect(&rcButton);

	int x = rcButton.left - 60;
	int y = rcButton.top - 50;

	pMenuPopup->TrackPopupMenu(TPM_RIGHTBUTTON, x, y, this);
	pMenuPopup->DestroyMenu();
}


void CVideoDlg::CreateMoreMenu()
{
    CRect				rcButton;
    CMenu				Menu;
    MENUITEMINFO		mii;
    UINT                nFlag = 0;

    Menu.LoadMenu(IDR_MENU_MORE);

    CMenu *pMenuPopup = Menu.GetSubMenu(0);

    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_STRING;

    mii.dwTypeData = const_cast<LPTSTR>LANG_STR("IDS_VIDEO_MOREINFO");
    mii.cch = _tcslen(LANG_STR("IDS_VIDEO_MOREINFO")) + 1;
    pMenuPopup->SetMenuItemInfo(0, &mii, FALSE);
    pMenuPopup->SetMenuItemBitmaps(ID_IDR_VIDEOINFO, MF_BYCOMMAND, &m_bitMenuInfo, &m_bitMenuInfo);
    if (m_bShowInfo)
        nFlag = MF_BYCOMMAND | MF_CHECKED;
    else
        nFlag = MF_BYCOMMAND | MF_UNCHECKED;
    pMenuPopup->CheckMenuItem(ID_IDR_VIDEOINFO, nFlag);

    mii.dwTypeData = const_cast<LPTSTR>LANG_STR("IDS_VIDEO_MOREDEVICE");
    mii.cch = _tcslen(LANG_STR("IDS_VIDEO_MOREDEVICE")) + 1;
    pMenuPopup->SetMenuItemInfo(1, &mii, FALSE);
    pMenuPopup->SetMenuItemBitmaps(ID_IDR_DEVICE, MF_BYCOMMAND, &m_bitMenuDevice, &m_bitMenuDevice);

    mii.dwTypeData = const_cast<LPTSTR>LANG_STR("IDS_VIDEO_MOREFILTER");
    mii.cch = _tcslen(LANG_STR("IDS_VIDEO_MOREFILTER")) + 1;
    pMenuPopup->SetMenuItemInfo(2, &mii, FALSE);
    pMenuPopup->SetMenuItemBitmaps(ID_IDR_FILTER, MF_BYCOMMAND, &m_bitMenuFilter, &m_bitMenuFilter);
    if (m_bFilter)
        nFlag = MF_BYCOMMAND | MF_CHECKED;
    else
        nFlag = MF_BYCOMMAND | MF_UNCHECKED;
    pMenuPopup->CheckMenuItem(ID_IDR_FILTER, nFlag);
    
    
    m_btnMore.GetWindowRect(&rcButton);

    int x = rcButton.left - 60;
    int y = rcButton.top - 75;

    pMenuPopup->TrackPopupMenu(TPM_RIGHTBUTTON, x, y, this);
    pMenuPopup->DestroyMenu();
}

void CVideoDlg::OnBnClickedBtnsetup()
{
	m_dlgDevice.ShowWindow(SW_SHOW);
	m_dlgDevice.CenterWindow();
}

void CVideoDlg::OnBnClickedBtnfilter()
{
    IRtcEngine *lpEngine = CAgoraObject::GetEngine();

    if (!m_bFilter){
        load_preprocessing_plugin(lpEngine);
    }
    else{
        unload_preprocessing_plugin(lpEngine);
    }

    m_bFilter = !m_bFilter;
}

void CVideoDlg::OnBnClickedBtnmessage()
{
    BOOL bIsShowing = m_dlgChat.IsWindowVisible();
    CRect   rcChatArea;

    rcChatArea.CopyRect(&m_rcVideoArea);
    rcChatArea.top = rcChatArea.bottom - 150;
    ClientToScreen(&rcChatArea);

    if (bIsShowing) {
        m_dlgChat.ShowWindow(SW_HIDE);
        m_btnMessage.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
    }
    else {
        m_dlgChat.ShowWindow(SW_SHOW);
        m_dlgChat.MoveWindow(&rcChatArea, TRUE);
        m_btnMessage.SwitchButtonStatus(CAGButton::AGBTN_PUSH);
    }
}

void CVideoDlg::OnBnClickedBtnmode()
{
	CAgoraObject *lpAgora = CAgoraObject::GetAgoraObject();
    IRtcEngine *lpRtcEngine = CAgoraObject::GetEngine();

	if (!lpAgora->IsLocalVideoMuted()) {
		lpAgora->MuteLocalVideo(TRUE);
        m_btnMode.SetBackImage(IDB_BTNVIDEO_VIDEO);
        
        CAgoraObject::GetAgoraObject()->EnableLocalRender(FALSE);
        m_wndLocal.SetBackImage(IDB_BACKGROUND_VIDEO, 96, 96);
        m_wndLocal.ShowBackground(TRUE);
	}
	else {
        lpAgora->EnableVideo(TRUE);
		lpAgora->MuteLocalVideo(FALSE);
        CAgoraObject::GetAgoraObject()->EnableLocalRender(TRUE);
        m_btnMode.SetBackImage(IDB_BTNAUDIO_VIDEO);
        m_wndLocal.ShowBackground(FALSE);
	}
    
    m_wndLocal.Invalidate(TRUE);
    m_wndLocal.UpdateWindow();
    
    Invalidate(TRUE);
    UpdateWindow();
}

void CVideoDlg::OnBnClickedBtnaudio()
{
	CAgoraObject *lpAgora = CAgoraObject::GetAgoraObject();

	if (lpAgora->IsLocalAudioMuted()) {
		lpAgora->MuteLocalAudio(FALSE);
		m_btnAudio.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
	}
	else {
		lpAgora->MuteLocalAudio(TRUE);
		m_btnAudio.SwitchButtonStatus(CAGButton::AGBTN_PUSH);
	}
}

void CVideoDlg::OnBnClickedBtncough()
{
	CAgoraObject* lpAgora = CAgoraObject::GetAgoraObject();

	int muteStatus = lpAgora->GetIsMuted();
	if (muteStatus == FALSE) {
		lpAgora->MuteSelf(1);
		m_btnCough.SwitchButtonStatus(CAGButton::AGBTN_PUSH);
		lpAgora->SetIsMuted(1);
	}
	else {
		lpAgora->MuteSelf(0);
		m_btnCough.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
		lpAgora->SetIsMuted(0);
	}
}

LRESULT CVideoDlg::OnEIDJoinChannelSuccess(WPARAM wParam, LPARAM lParam)
{
	LPAGE_JOINCHANNEL_SUCCESS lpData = (LPAGE_JOINCHANNEL_SUCCESS)wParam;
	m_dlgChat.UpdateMessageStream();
	CHANNEL_TYPE chT = (CHANNEL_TYPE)lParam;

	ListWindowRemoveAll(chT);

	delete lpData;
	return 0;
}

LRESULT CVideoDlg::OnEIDReJoinChannelSuccess(WPARAM wParam, LPARAM lParam)
{
	LPAGE_REJOINCHANNEL_SUCCESS lpData = (LPAGE_REJOINCHANNEL_SUCCESS)wParam;
	m_dlgChat.UpdateMessageStream();

	CHANNEL_TYPE chT = (CHANNEL_TYPE)lParam;

	ListWindowRemoveAll(chT);

	delete lpData;
	return 0;
}

LRESULT CVideoDlg::OnEIDFirstLocalFrame(WPARAM wParam, LPARAM lParam)
{
	LPAGE_FIRST_LOCAL_VIDEO_FRAME lpData = (LPAGE_FIRST_LOCAL_VIDEO_FRAME)wParam;

	if (ListWindowGetCount((CHANNEL_TYPE)lParam) == 0)
		ShowMulti();

	delete lpData;

	return 0;
}

LRESULT CVideoDlg::OnEIDFirstRemoteFrameDecoded(WPARAM wParam, LPARAM lParam)
{
	LPAGE_FIRST_REMOTE_VIDEO_DECODED lpData = (LPAGE_FIRST_REMOTE_VIDEO_DECODED)wParam;
	CHANNEL_TYPE chT = (CHANNEL_TYPE)lParam;
	BOOL bFound = FALSE;
	POSITION pos = ListWindowGetHeadPos(chT);

	while (pos != NULL) {
		AGVIDEO_WNDINFO& agvWndInfo = ListWindowGetNextPos(chT, pos);
		if (agvWndInfo.nUID == lpData->uid) {
			bFound = TRUE;
			break;
		}
	}

	if (!bFound) {
		AGVIDEO_WNDINFO agvWndInfo;
		memset(&agvWndInfo, 0, sizeof(AGVIDEO_WNDINFO));
		agvWndInfo.nUID = lpData->uid;
		//agvWndInfo.nWidth = lpData->width;
		//agvWndInfo.nHeight = lpData->height;
		strcpy_s(agvWndInfo.channelID, 64, lpData->channelID);
		ListWindowAddTail(chT, agvWndInfo);
	}

	RebindVideoWnd();

	delete lpData;

	return 0;
}

LRESULT CVideoDlg::OnEIDUserJoined(WPARAM wParam, LPARAM lParam)
{
	LPAGE_USER_JOINED lpData = (LPAGE_USER_JOINED)wParam;
	BOOL bFound = FALSE;
	CHANNEL_TYPE chT = (CHANNEL_TYPE)lParam;
	POSITION pos = ListWindowGetHeadPos(chT);
	CString s(lpData->channelID);

	if (chT == CHANNEL_TYPE::CHANNEL_DEST) return 0;

	while (pos != NULL) {
		AGVIDEO_WNDINFO& agvWndInfo = ListWindowGetNextPos(chT, pos);
		if (agvWndInfo.nUID == lpData->uid) {
			bFound = TRUE;
			break;
		}
	}

	if (!bFound) {
		AGVIDEO_WNDINFO agvWndInfo;
		memset(&agvWndInfo, 0, sizeof(AGVIDEO_WNDINFO));
		agvWndInfo.nUID = lpData->uid;
		strcpy_s(agvWndInfo.channelID, 64, lpData->channelID);
		ListWindowAddTail(chT, agvWndInfo);
	}
	delete lpData;

	//RebindVideoWnd();
	return 0;
}

LRESULT CVideoDlg::OnEIDUserOffline(WPARAM wParam, LPARAM lParam)
{
	LPAGE_USER_OFFLINE lpData = (LPAGE_USER_OFFLINE)wParam;
	CHANNEL_TYPE chT = (CHANNEL_TYPE)lParam;

	POSITION pos = ListWindowGetHeadPos(chT);
	while (pos != NULL) {
		if (ListWindowGetAt(chT, pos).nUID == lpData->uid) {
			ListWindowRemoveAt(chT, pos);
			break;
		}
		ListWindowGetNextPos(chT, pos);
	}
	delete lpData;

	RebindVideoWnd();
	return 0;
}

LRESULT CVideoDlg::OnEIDConnectionLost(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CVideoDlg::OnEIDVideoDeviceChanged(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CVideoDlg::OnRemoteVideoStat(WPARAM wParam, LPARAM lParam)
{
	LPAGE_REMOTE_VIDEO_STAT lpData = (LPAGE_REMOTE_VIDEO_STAT)wParam;
	CHANNEL_TYPE chT = (CHANNEL_TYPE)lParam;

	POSITION posNext = ListWindowGetHeadPos(chT);

	while (posNext != NULL) {
		AGVIDEO_WNDINFO& rWndInfo = ListWindowGetNextPos(chT, posNext);

		if (rWndInfo.nUID == lpData->uid) {
			rWndInfo.nFramerate = lpData->rendererOutputFrameRate;
			rWndInfo.nBitrate = lpData->receivedBitrate;
			rWndInfo.nWidth = lpData->width;
			rWndInfo.nHeight = lpData->height;
			m_wndVideo[rWndInfo.nIndex].SetFrameRateInfo(rWndInfo.nFramerate);
			m_wndVideo[rWndInfo.nIndex].SetBitrateInfo(rWndInfo.nBitrate);
			m_wndVideo[rWndInfo.nIndex].SetVideoResolution(rWndInfo.nWidth, rWndInfo.nHeight);
			break;
		}
	}

	delete lpData;

	return 0;
}

LRESULT CVideoDlg::OnStartRecordingService(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CVideoDlg::OnStopRecordingService(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CVideoDlg::OnStreamMessage(WPARAM wParam, LPARAM lParam)
{
	LPAGE_STREAM_MESSAGE lpData = (LPAGE_STREAM_MESSAGE)wParam;
	TCHAR szMessage[256];
	UserInfo usrInf;

	CAgoraObject::GetEngine()->getUserInfoByUid(lpData->uid, &usrInf);
	int nUTF8Len = lpData->length;

	memset(szMessage, 0, 256 * sizeof(TCHAR));
#ifdef UNICODE
	::MultiByteToWideChar(CP_UTF8, 0, lpData->data, lpData->length, szMessage, 256);
#else
	_tcscpy_s(szMessage, 256, lpData->data);
#endif

	m_dlgChat.AddChatMessage(CString(usrInf.userAccount), szMessage);

	delete[] lpData->data;
	delete lpData;

	return 0;
}

void CVideoDlg::DrawHead(CDC *pDC)
{
	CRect rcTitle;
	CString strTitle;

	if (m_bFullScreen)
		return;

	GetWindowText(strTitle);
	GetClientRect(&rcTitle);
	CBrush *lpDefaultBrush = pDC->SelectObject(&m_brHead);

	rcTitle.bottom = 24;
	pDC->FillRect(&rcTitle, &m_brHead);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0xFF, 0xFF, 0xFF));
	pDC->DrawText(strTitle, &rcTitle, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	pDC->SelectObject(lpDefaultBrush);
}

void CVideoDlg::InitCtrls()
{
	CRect	rcClient;
	int nScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int nScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	
	GetClientRect(&rcClient);

    m_btnMessage.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNMESSAGE_VIDEO);
    m_btnMode.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNMODE_VIDEO);
	m_btnAudio.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNAUDIO_VIDEO);
	m_btnEndCall.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNENDCALL_VIDEO);
	m_btnScrCap.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNSCRCAP_VIDEO);
    m_btnMore.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNMORE_VIDEO); 

	m_btnPublish.Create(NULL, WS_VISIBLE | WS_CHILD | BS_ICON, CRect(0, 0, 1, 1), this, IDC_BTNPUBLISH_VIDEO);

	m_cmbDest.Create(WS_VISIBLE | WS_CHILD | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST, CRect(0, 0, 1, 1), this, IDC_CBXRDEST_VIDEO);
	m_cmbRelay.Create(WS_VISIBLE | WS_CHILD | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST, CRect(0, 0, 1, 1), this, IDC_CBXRELAY_VIDEO);

	m_btnCough.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDB_BTNMCOUGH_VIDEO);

	m_btnShow.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNSCR_VIDEO);
	
	for (int nIndex = 0; nIndex < 4; nIndex++){
		m_wndTransl[nIndex].Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CHILDWINDOW, CRect(0, 0, 192, 200), this, IDC_STATIC);
		m_wndTransl[nIndex].ShowWindow(SW_HIDE);
		m_wndVideo[nIndex].Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, CRect(0, 0, 1, 1), this, IDC_BASEWND_VIDEO + nIndex);
		m_wndVideo[nIndex].SetBackImage(IDB_BACKGROUND_VIDEO, 96, 96, RGB(0x44, 0x44, 0x44));
		m_wndVideo[nIndex].SetFaceColor(RGB(0x58, 0x58, 0x58));
		//m_wndTransl[nIndex].SetBackImage(IDB_BACKGROUND_VIDEO, 96, 96, RGB(0x44, 0x44, 0x44));
	}

	m_wndLocal.Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, CRect(0, 0, 1, 1), this, IDC_BASEWND_VIDEO + 4);
	m_wndLocal.SetBackImage(IDB_BACKGROUND_VIDEO, 96, 96, RGB(0x44, 0x44, 0x44));
	m_wndLocal.SetFaceColor(RGB(0x58, 0x58, 0x58));
	m_wndLocal.SetUID(0);
	m_wndTrLocal.Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CHILDWINDOW, CRect(0, 0, 192, 200), this, IDC_STATIC);
	m_wndTrLocal.SetUID(0);

	m_btnMin.MoveWindow(rcClient.Width() - 72, 1, 22, 22, TRUE);
	m_btnRst.MoveWindow(rcClient.Width() - 48, 1, 22, 22, TRUE);
	m_btnClose.MoveWindow(rcClient.Width() - 24, 1, 22, 22, TRUE);

    m_btnMessage.MoveWindow(rcClient.Width() / 2 - 144, rcClient.Height() - 84, 48, 48, TRUE);
    m_btnMode.MoveWindow(rcClient.Width() / 2 - 24, rcClient.Height() - 84, 48, 48, TRUE);
    m_btnAudio.MoveWindow(rcClient.Width() / 2 + 24, rcClient.Height() - 84, 48, 48, TRUE);
    //m_btnShow.MoveWindow(rcClient.Width() - 72, rcClient.Height() - 84, 48, 48, TRUE);
    m_btnMore.MoveWindow(rcClient.Width() / 2 + 264, rcClient.Height() - 84, 48, 48, TRUE);
    m_btnEndCall.MoveWindow(rcClient.Width()/2 - 340, rcClient.Height() - 60, 128, 53, TRUE);

	m_btnPublish.MoveWindow(rcClient.Width() - 80, rcClient.Height() - 84, 300, 48, TRUE);
	m_cmbDest.MoveWindow(rcClient.Width() - 80, rcClient.Height() - 450, 300, 48, TRUE);
	m_cmbRelay.MoveWindow(rcClient.Width() - 80, rcClient.Height() - 450, 300, 48, TRUE);

    
	m_btnCough.MoveWindow(rcClient.Width() / 2 + 24, rcClient.Height() - 84, 128, 53, TRUE);

	m_wndVideo[0].MoveWindow(0, 24, rcClient.Width(), rcClient.Height() - 96, TRUE);

	m_btnMin.SetBackImage(IDB_BTNMIN, RGB(0xFF, 0x00, 0xFF));
	m_btnRst.SetBackImage(IDB_BTNMAX, RGB(0xFF, 0x00, 0xFF));
	m_btnClose.SetBackImage(IDB_BTNCLOSE, RGB(0xFF, 0x00, 0xFF));

	m_btnScrCap.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnScrCap.EnableFrameEffect(FALSE);
	m_btnScrCap.SetBackImage(IDB_BTNSCRCAP_VIDEO, RGB(0x26, 0x26, 0x26));

    m_btnMessage.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
    m_btnMessage.EnableFrameEffect(FALSE);
    m_btnMessage.SetBackImage(IDB_BTNMSG_VIDEO, RGB(0x26, 0x26, 0x26));

	m_btnPublish.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnPublish.EnableFrameEffect(FALSE);
	m_btnPublish.SetBackImage(IDB_PUBLISH, RGB(0x26, 0x26, 0x26));

	m_btnMode.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnMode.EnableFrameEffect(FALSE);
	m_btnMode.SetBackImage(IDB_BTNAUDIO_VIDEO, RGB(0x26, 0x26, 0x26));

	m_btnAudio.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnAudio.EnableFrameEffect(FALSE);
	m_btnAudio.SetBackImage(IDB_BTNMAUDIO_VIDEO, RGB(0x26, 0x26, 0x26));

	m_btnCough.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnCough.EnableFrameEffect(FALSE);
	m_btnCough.SetBackImage(IDB_COUGHMUTE, RGB(0x26, 0x26, 0x26));
	
	m_btnShow.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnShow.EnableFrameEffect(FALSE);
	m_btnShow.SetBackImage(IDB_BTNFULLSCR_VIDEO, RGB(0x26, 0x26, 0x26));

	m_btnEndCall.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnEndCall.EnableFrameEffect(FALSE);
	m_btnEndCall.SetBackImage(IDB_END_MEETING, RGB(0x26, 0x26, 0x26));

    m_btnMore.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
    m_btnMore.EnableFrameEffect(FALSE);
    m_btnMore.SetBackImage(IDB_BTNMORE_VIDEO, RGB(0x26, 0x26, 0x26));

	//m_btnPublish.SetWindowTextW(_T("Publish"));
}

void CVideoDlg::ShowVideo1()
{
	m_wndLocal.ShowWindow(SW_HIDE);
	m_wndLocal.SetBigShowFlag(FALSE);
	for (int nIndex = 0; nIndex < 4; nIndex++) {
		m_wndVideo[nIndex].ShowWindow(SW_HIDE);
		m_wndVideo[nIndex].SetBigShowFlag(FALSE);
	}

	if (ListWindowGetTotalCount() == 0) {
		m_wndTrLocal.MoveWindow((m_rcVideoArea.Width() / 2) - 400, m_rcVideoArea.bottom - 200, 191, 200, TRUE);
		m_wndLocal.MoveWindow((m_rcVideoArea.Width() / 2) - 395, m_rcVideoArea.bottom - 195, 180, 135, TRUE);
		m_wndTrLocal.SetParent(this);
		m_wndLocal.SetParent(this);
		m_wndTrLocal.ShowWindow(SW_SHOW);
		m_wndLocal.ShowWindow(SW_SHOW);
	}
	else {
		m_wndVideo[0].MoveWindow(&m_rcVideoArea, FALSE);
		m_wndVideo[0].ShowWindow(SW_SHOW);
		m_wndVideo[0].SetBigShowFlag(TRUE);
		m_wndVideo[0].SetParent(this);

		m_wndTrLocal.MoveWindow((m_rcVideoArea.Width() / 2) - 400, m_rcVideoArea.bottom - 200, 191, 200, TRUE);
		m_wndLocal.MoveWindow((m_rcVideoArea.Width() / 2) - 395, m_rcVideoArea.bottom - 195, 180, 135, TRUE);
		m_wndTrLocal.SetParent(this);
		m_wndLocal.SetParent(this);
		m_wndTrLocal.ShowWindow(SW_SHOW);
		m_wndLocal.ShowWindow(SW_SHOW);
	}

	m_nScreenMode = SCREEN_VIDEO1;
	ShowButtonsNormal();
}

void CVideoDlg::ShowVideo4()
{
	m_wndLocal.ShowWindow(SW_HIDE);
	m_wndLocal.SetBigShowFlag(FALSE);
	m_wndTrLocal.ShowWindow(SW_HIDE);
	for (int nIndex = 0; nIndex < 4; nIndex++) {
		m_wndTransl[nIndex].SetParent(this);
		m_wndTransl[nIndex].ShowWindow(SW_HIDE);
		m_wndVideo[nIndex].ShowWindow(SW_HIDE);
		m_wndVideo[nIndex].SetBigShowFlag(FALSE);
		m_wndVideo[nIndex].SetParent(this);
	}

	(&m_wndVideo[0])->ShowWindow(SW_SHOW);
	(&m_wndVideo[0])->MoveWindow(&m_rcVideoArea, TRUE);
	(&m_wndVideo[0])->SetBigShowFlag(TRUE);

	m_wndTrLocal.MoveWindow((m_rcVideoArea.Width() / 2) - 400, m_rcVideoArea.bottom - 200, 191, 200, TRUE);
	m_wndTrLocal.SetParent(this);
	m_wndLocal.MoveWindow((m_rcVideoArea.Width() / 2) - 395, m_rcVideoArea.bottom - 195, 180, 135, TRUE);
	m_wndLocal.SetParent(this);
	m_wndTrLocal.ShowWindow(SW_SHOW);
	m_wndLocal.ShowWindow(SW_SHOW);

	for (int nIndex = 0; nIndex < 4; nIndex++) {
		int nXPos = (m_rcVideoArea.Width() / 2) - 400 + (204 * nIndex);
		int nYPos = m_rcVideoArea.bottom - 200;
		if (!m_wndVideo[nIndex].IsBigShow()) {
			if (m_wndVideo[nIndex].GetUID() != 0) {
				m_wndTransl[nIndex].MoveWindow(nXPos, nYPos, 191, 200, TRUE);
				m_wndVideo[nIndex].MoveWindow(nXPos + 5, nYPos + 5, 180, 135, TRUE);
				m_wndTransl[nIndex].SetUID(m_wndVideo[nIndex].GetUID());
				m_wndVideo[nIndex].IsHidden();
				m_wndTransl[nIndex].ShowWindow(SW_SHOW);
				m_wndVideo[nIndex].ShowWindow(SW_SHOW);
				m_wndTransl[nIndex].InitCtrls();
				m_wndTransl[nIndex].ResetButtons();
			}
		}
	}
	m_nScreenMode = SCREEN_VIDEO4;

	ShowButtonsNormal();
}

void CVideoDlg::ShowMulti()
{
	int nLocalIndex = 0;

	m_wndLocal.ShowWindow(SW_HIDE);
	m_wndLocal.SetBigShowFlag(FALSE);
	m_wndTrLocal.ShowWindow(SW_HIDE);
	for (int nIndex = 0; nIndex < 4; nIndex++) {
		m_wndTransl[nIndex].SetParent(this);
		m_wndTransl[nIndex].ShowWindow(SW_HIDE);
		m_wndVideo[nIndex].ShowWindow(SW_HIDE);
		m_wndVideo[nIndex].SetBigShowFlag(FALSE);
		m_wndVideo[nIndex].SetParent(this);
	}

	m_lpBigShowed = &m_wndVideo[0];

	m_lpBigShowed->ShowWindow(SW_SHOW);
	m_lpBigShowed->MoveWindow(&m_rcVideoArea, TRUE);
	//m_lpBigShowed->SetParent(this);
	m_lpBigShowed->SetBigShowFlag(TRUE);



	for (int nIndex = 0; nIndex < 4; nIndex++) {
		int nXPos = (m_rcVideoArea.Width() / 2) - 402 + (204 * nLocalIndex);
		int nYPos = m_rcVideoArea.bottom - 200;

		if (!m_wndVideo[nIndex].IsBigShow()) {
			if (m_wndVideo[nIndex].GetUID() != 0) {
				m_wndTransl[nIndex].MoveWindow(nXPos, nYPos, 191, 200, TRUE);
				m_wndVideo[nIndex].MoveWindow(nXPos + 5, nYPos + 5, 180, 135, TRUE);
				m_wndTransl[nIndex].SetUID(m_wndVideo[nIndex].GetUID());
				m_wndVideo[nIndex].IsHidden();
				m_wndTransl[nIndex].ShowWindow(SW_SHOW);
				m_wndVideo[nIndex].ShowWindow(SW_SHOW);
				m_wndTransl[nIndex].InitCtrls();
				m_wndTransl[nIndex].ResetButtons();
				nLocalIndex++;
			}
		}
		else {
			m_wndTrLocal.MoveWindow(nXPos, nYPos, 191, 200, TRUE);
			m_wndTrLocal.SetParent(this);
			m_wndLocal.MoveWindow(nXPos + 5, nYPos + 5, 180, 135, TRUE);
			m_wndLocal.SetParent(this);
			m_wndTrLocal.ShowWindow(SW_SHOW);
			m_wndLocal.ShowWindow(SW_SHOW);
			nLocalIndex++;
		}
	}

	m_nScreenMode = SCREEN_VIDEOMULTI;

	ShowButtonsNormal();
}

BOOL CVideoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:
	m_dlgDevice.Create(CDeviceDlg::IDD, this);
	m_dlgDevice.EnableDeviceTest(FALSE);

	m_dlgScreenCapture.Create(CAGScreenCaptureDlg::IDD, this);
	m_dlgDesktopCapture.Create(CAGDesktopCaptureDlg::IDD, this);
    m_dlgChat.Create(CChatDlg::IDD, this);
	InitCtrls();
    
    m_bitMenuInfo.LoadBitmap(IDB_MENU_INFO);
    m_bitMenuDevice.LoadBitmap(IDB_MENU_DEVICE);
    m_bitMenuFilter.LoadBitmap(IDB_MENU_FILTER);

	return TRUE;  // return TRUE unless you set the focus to a control
}


void CVideoDlg::RebindVideoWnd()
{
	if (m_wndVideo[0].GetSafeHwnd() == NULL || m_wndLocal.GetSafeHwnd() == NULL)
		return;


	POSITION pos = m_listWndInfoHost.GetHeadPosition();
	if (pos != NULL) {
		VideoCanvas canvas;
		canvas.renderMode = RENDER_MODE_FIT;
		AGVIDEO_WNDINFO& agvWndInfo = m_listWndInfoHost.GetAt(pos);

		canvas.uid = agvWndInfo.nUID;
		strcpy_s(canvas.channelId, 64, agvWndInfo.channelID);

		canvas.view = m_wndVideo[0].GetSafeHwnd();
		agvWndInfo.nIndex = 0;

		CAgoraObject::GetEngine()->setupRemoteVideo(canvas);
		m_wndVideo[0].SetUID(canvas.uid);
		m_wndVideo[0].SetVideoResolution(agvWndInfo.nWidth, agvWndInfo.nHeight);
		m_wndVideo[0].SetFrameRateInfo(agvWndInfo.nFramerate);
		m_wndVideo[0].SetBitrateInfo(agvWndInfo.nBitrate);
	}
	else
		m_wndVideo[0].SetUID(0);

	pos = m_listWndInfoDest.GetHeadPosition();
	for (int nIndex = 1; nIndex < 4; nIndex++) {
		if (pos != NULL) {
			AGVIDEO_WNDINFO& agvWndInfo = m_listWndInfoDest.GetNext(pos);
			VideoCanvas canvas;
			canvas.renderMode = RENDER_MODE_FIT;

			canvas.uid = agvWndInfo.nUID;
			strcpy_s(canvas.channelId, 64, agvWndInfo.channelID);

			canvas.view = m_wndVideo[nIndex].GetSafeHwnd();
			agvWndInfo.nIndex = nIndex;

			int ret = CAgoraObject::GetEngine()->setupRemoteVideo(canvas);
			m_wndVideo[nIndex].SetUID(canvas.uid);
			m_wndVideo[nIndex].SetVideoResolution(agvWndInfo.nWidth, agvWndInfo.nHeight);
			m_wndVideo[nIndex].SetFrameRateInfo(agvWndInfo.nFramerate);
			m_wndVideo[nIndex].SetBitrateInfo(agvWndInfo.nBitrate);
		}
		else
			m_wndVideo[nIndex].SetUID(0);
	}

	//if (m_nScreenMode != SCREEN_VIDEOMULTI) {
	//	if (ListWindowGetTotalCount() <= 1)
	//		ShowVideo1();
	//	else if (ListWindowGetTotalCount() > 1 && ListWindowGetTotalCount() < 4)
	//		ShowVideo4();
	//	else
	//		ShowMulti();
	//}
	//else
	ShowMulti();
}

void CVideoDlg::UpdateVideoWnd(uid_t uid, bool mute)
{
	POSITION pos = m_listWndInfoDest.GetHeadPosition();
	for (int nIndex = 1; nIndex < 4; nIndex++) {
		if (pos != NULL) {
			AGVIDEO_WNDINFO& agvWndInfo = m_listWndInfoDest.GetNext(pos);
			if (agvWndInfo.nUID == uid)
				m_wndVideo[agvWndInfo.nIndex].HideWnd(mute);
		}
	}
}

BOOL CVideoDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN){
		switch (pMsg->wParam){
		case VK_RETURN:
			return FALSE;
		case VK_ESCAPE:
			if (m_bFullScreen)
				OnBnClickedBtnfullscr();
			
			return FALSE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

LRESULT CVideoDlg::OnShowModeChanged(WPARAM wParam, LPARAM lParam)
{
	//if (m_nScreenMode == SCREEN_VIDEOMULTI) {
	//	if (ListWindowGetTotalCount() <= 1)
	//		ShowVideo1();
	//	else if (ListWindowGetTotalCount() > 1 && ListWindowGetTotalCount() < 4)
	//		ShowVideo4();
	//}
	//else {
	//	m_lpBigShowed = (CAGVideoWnd*)wParam;
	//	ShowMulti();
	//}
	ShowMulti();
	return 0;
}

LRESULT CVideoDlg::OnShowBig(WPARAM wParam, LPARAM lParam)
{
	CRect rcNextBig;
	CRect rcCurBig;

	if (m_nScreenMode != SCREEN_VIDEOMULTI)
		return 0;

	CAGVideoWnd *lpNextBigWnd = (CAGVideoWnd *)wParam;
	if (lpNextBigWnd == m_lpBigShowed)
		return 0;

	ShowMulti();
	m_lpBigShowed = lpNextBigWnd;

	return 0;
}

LRESULT CVideoDlg::OnWindowShareStart(WPARAM wParam, LPARAM lParam)
{
    HWND hShareWnd = (HWND)wParam;
    if (hShareWnd == GetSafeHwnd()) {
        CAgoraObject::GetAgoraObject()->EnableLocalRender(FALSE);
        m_wndLocal.Invalidate(TRUE);
    }

	CAgoraObject::GetAgoraObject()->EnableScreenCapture((HWND)wParam, 15, NULL, TRUE);
	m_btnScrCap.SwitchButtonStatus(CAGButton::AGBTN_PUSH);

	return 0;
}

LRESULT CVideoDlg::OnDesktopShareStart(WPARAM wParam, LPARAM lParam)
{
	LPDESKTOP_SHARE_PARAM	lpDesktopShareParam = (LPDESKTOP_SHARE_PARAM)(wParam);
	CRect rcRegion;

	CAgoraObject::GetAgoraObject()->EnableVideo();

	rcRegion.left = lpDesktopShareParam->nX;
	rcRegion.top = lpDesktopShareParam->nY;
	rcRegion.right = rcRegion.left + lpDesktopShareParam->nWidth;
	rcRegion.bottom = rcRegion.top + lpDesktopShareParam->nHeight;

	CAgoraObject::GetAgoraObject()->EnableScreenCapture(NULL, lpDesktopShareParam->nFPS, &rcRegion, TRUE);
	//	Sleep(1000);
	//	CAgoraObject::GetAgoraObject()->SetVideoProfileEx(lpDesktopShareParam->nWidth, lpDesktopShareParam->nHeight, lpDesktopShareParam->nFPS, lpDesktopShareParam->nBitrate);

	m_btnScrCap.SwitchButtonStatus(CAGButton::AGBTN_PUSH);

	return 0;
}

void CVideoDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow && GetSafeHwnd() != NULL)
		RebindVideoWnd();
}


void CVideoDlg::OnMove(int x, int y)
{
    CDialogEx::OnMove(x, y);

    CRect rcChatBox;

    rcChatBox.SetRect(x, y + m_rcVideoArea.Height() - 126, x + m_rcVideoArea.Width(), y + m_rcVideoArea.Height() + 24);

    if (::IsWindow(m_dlgChat.GetSafeHwnd()) && m_dlgChat.IsWindowVisible()){
        m_dlgChat.MoveWindow(&rcChatBox);
    }
}
