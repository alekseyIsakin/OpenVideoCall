#include "StdAfx.h"
#include "AgoraObject.h"
#include "AGResourceVisitor.h"

#include <stdio.h>
#include "AGJson.h"

CAgoraObject *CAgoraObject::m_lpAgoraObject = NULL;
IRtcEngine *CAgoraObject::m_lpAgoraEngine = NULL;
CAGEngineEventHandler CAgoraObject::m_EngineEventHandler;
CString   CAgoraObject::m_strVendorKey;

CAgoraObject::CAgoraObject(void)
	: m_dwEngineFlag(0)
	, m_bVideoEnable(FALSE)
	, m_bLocalAudioMuted(FALSE)
	, m_bScreenCapture(FALSE)
	, m_bEchoTest(FALSE)
	, m_nSelfUID(0)
{
	m_strChannelName.Empty();
	m_bLocalVideoMuted = FALSE;
}

CAgoraObject::~CAgoraObject(void)
{
}

CString CAgoraObject::LoadAppID()
{
    TCHAR szFilePath[MAX_PATH];
    CString strAppID(APP_ID);
    if (!strAppID.IsEmpty())
        return strAppID;

    ::GetModuleFileName(NULL, szFilePath, MAX_PATH);
    LPTSTR lpLastSlash = _tcsrchr(szFilePath, _T('\\'));

    if (lpLastSlash == NULL)
        return strAppID;

    SIZE_T nNameLen = MAX_PATH - (lpLastSlash - szFilePath + 1);
    _tcscpy_s(lpLastSlash + 1, nNameLen, _T("AppID.ini"));

    if (!PathFileExists(szFilePath)) {
        HANDLE handle = CreateFile(szFilePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, 0, NULL);
        CloseHandle(handle);
    }

    TCHAR szAppid[MAX_PATH] = { 0 };
    ::GetPrivateProfileString(_T("AppID"), _T("AppID"), NULL, szAppid, MAX_PATH, szFilePath);
    if (_tcslen(szAppid) == 0) {
        ::WritePrivateProfileString(_T("AppID"), _T("AppID"), _T(""), szFilePath);
        ::ShellExecute(NULL, _T("open"), szFilePath, NULL, NULL, SW_MAXIMIZE);
    }

    strAppID = szAppid;

    return strAppID;
}


CString CAgoraObject::GetSDKVersion()
{
	int nBuildNumber = 0;
	const char *lpszEngineVer = getAgoraRtcEngineVersion(&nBuildNumber);

	CString strEngineVer;

#ifdef UNICODE
	::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, lpszEngineVer, -1, strEngineVer.GetBuffer(256), 256);
	strEngineVer.ReleaseBuffer();
#endif

	return strEngineVer;
}

CString CAgoraObject::GetSDKVersionEx()
{
	int nBuildNumber = 0;
	const char *lpszEngineVer = getAgoraRtcEngineVersion(&nBuildNumber);

	CString strEngineVer;
	CString strVerEx;
	SYSTEMTIME sysTime;

#ifdef UNICODE
	::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, lpszEngineVer, -1, strEngineVer.GetBuffer(256), 256);
	strEngineVer.ReleaseBuffer();
#else
	strEngineVer = lpszEngineVer;
#endif

	::GetLocalTime(&sysTime);
	strVerEx.Format(_T("V%s, Build%d, %d/%d/%d, V%s"), strEngineVer, nBuildNumber, sysTime.wYear, sysTime.wMonth, sysTime.wDay, strEngineVer);

	return strVerEx;
}

IRtcEngine *CAgoraObject::GetEngine()
{
	if(m_lpAgoraEngine == NULL)
		m_lpAgoraEngine = (IRtcEngine *)createAgoraRtcEngine();

	return m_lpAgoraEngine;
}

CAgoraObject *CAgoraObject::GetAgoraObject(LPCTSTR lpVendorKey)
{
	if(m_lpAgoraObject == NULL)
		m_lpAgoraObject = new CAgoraObject();

	if(m_lpAgoraEngine == NULL)
		m_lpAgoraEngine = (IRtcEngine *)createAgoraRtcEngine();

	// ���VendorKeyΪ����ֱ�ӷ��ض���
	if (lpVendorKey == NULL)
		return m_lpAgoraObject;

	RtcEngineContext ctx;

	ctx.eventHandler = &m_EngineEventHandler;

#ifdef UNICODE
	char szVendorKey[128];

	::WideCharToMultiByte(CP_ACP, 0, lpVendorKey, -1, szVendorKey, 128, NULL, NULL);
	ctx.appId = szVendorKey;
#else
	ctx.appId = lpVendorKey;
#endif

	m_lpAgoraEngine->initialize(ctx);
	if (lpVendorKey != NULL)
		m_strVendorKey = lpVendorKey;

	return m_lpAgoraObject;
}

void CAgoraObject::CloseAgoraObject()
{
	if(m_lpAgoraEngine != NULL)
		m_lpAgoraEngine->release();

	if(m_lpAgoraObject != NULL)
		delete m_lpAgoraObject;

	m_lpAgoraEngine = NULL;
	m_lpAgoraObject = NULL;
}

void CAgoraObject::SetMsgHandlerWnd(HWND hWnd)
{
	m_EngineEventHandler.SetMsgReceiver(hWnd);

	m_channelHostEventHandler.setMsgHandler		(hWnd);
	m_channelDestEventHandler.setMsgHandler		(hWnd);
	m_channelSrcEventHandler.setMsgHandler		(hWnd);
	m_channelTranslEventHandler.setMsgHandler	(hWnd);
}

HWND CAgoraObject::GetMsgHandlerWnd()
{
	return m_EngineEventHandler.GetMsgReceiver();
}


void CAgoraObject::SetNetworkTestFlag(BOOL bEnable)
{
	if(bEnable)
		m_dwEngineFlag |= AG_ENGFLAG_ENNETTEST;
	else
		m_dwEngineFlag &= (~AG_ENGFLAG_ENNETTEST);
}

BOOL CAgoraObject::GetNetworkTestFlag()
{
	return (m_dwEngineFlag & AG_ENGFLAG_ENNETTEST) != 0;
}

void CAgoraObject::SetEchoTestFlag(BOOL bEnable)
{
	if(bEnable)
		m_dwEngineFlag |= AG_ENGFLAG_ECHOTEST;
	else
		m_dwEngineFlag &= (~AG_ENGFLAG_ECHOTEST);
}

BOOL CAgoraObject::GetEchoTestFlag()
{
	return (m_dwEngineFlag & AG_ENGFLAG_ECHOTEST) != 0;
}

void CAgoraObject::SetSpeakerphoneTestFlag(BOOL bEnable)
{
	if(bEnable)
		m_dwEngineFlag |= AG_ENGFLAG_SPKPHTEST;
	else
		m_dwEngineFlag &= (~AG_ENGFLAG_SPKPHTEST);
}

BOOL CAgoraObject::GetSpeakerphoneTestFlag()
{
	return (m_dwEngineFlag & AG_ENGFLAG_SPKPHTEST) != 0;
}

void CAgoraObject::SetMicrophoneTestFlag(BOOL bEnable)
{
	if(bEnable)
		m_dwEngineFlag |= AG_ENGFLAG_MICPHTEST;
	else
		m_dwEngineFlag &= (~AG_ENGFLAG_MICPHTEST);
}

BOOL CAgoraObject::GetMicrophoneTestFlag()
{
	return (m_dwEngineFlag & AG_ENGFLAG_MICPHTEST) != 0;
}


void CAgoraObject::SetVideoTestFlag(BOOL bEnable)
{
	if (bEnable)
		m_dwEngineFlag |= AG_ENGFLAG_VIDEOTEST;
	else
		m_dwEngineFlag &= (~AG_ENGFLAG_VIDEOTEST);
}

BOOL CAgoraObject::GetVideoTestFlag()
{
	return (m_dwEngineFlag & AG_ENGFLAG_VIDEOTEST) != 0;
}

BOOL CAgoraObject::SetLogFilePath(LPCTSTR lpLogPath)
{
	ASSERT(m_lpAgoraEngine != NULL);

	CHAR szLogPathA[MAX_PATH];
	CHAR szLogPathTrans[MAX_PATH];

	int ret = 0;
	RtcEngineParameters rep(*m_lpAgoraEngine);

	if (::GetFileAttributes(lpLogPath) == INVALID_FILE_ATTRIBUTES) {
		::GetModuleFileNameA(NULL, szLogPathA, MAX_PATH);
		LPSTR lpLastSlash = strrchr(szLogPathA, '\\')+1;
		strcpy_s(lpLastSlash, 64, "AgoraSDK.log");
	}
	else {
#ifdef UNICODE
		::WideCharToMultiByte(CP_UTF8, 0, lpLogPath, -1, szLogPathA, MAX_PATH, NULL, NULL);
#else
		::MultiByteToWideChar(CP_UTF8, 0, lpLogPath, -1, (WCHAR *)szLogPathA, MAX_PATH, NULL, NULL);
#endif
	}

	CAGResourceVisitor::TransWinPathA(szLogPathA, szLogPathTrans, MAX_PATH);

	ret = rep.setLogFile(szLogPathTrans);

	return ret == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::JoinChannel(LPCTSTR lpChannelName, UINT nUID, LPCSTR lpChannelToken)
{
	int nRet = 0;

	//m_lpAgoraEngine->setVideoProfile(VIDEO_PROFILE_LANDSCAPE_480P_4, false);
#ifdef UNICODE
	CHAR szChannelName[128];

	::WideCharToMultiByte(CP_ACP, 0, lpChannelName, -1, szChannelName, 128, NULL, NULL);
	nRet = m_lpAgoraEngine->joinChannel(lpChannelToken, szChannelName, NULL, nUID);
#else
	nRet = m_lpAgoraEngine->joinChannel(lpChannelToken, lpChannelName, NULL, nUID);
#endif

	if (nRet == 0)
		m_strChannelName = lpChannelName;
	
	return nRet == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::LeaveCahnnel()
{
	int nRet = m_lpAgoraEngine->leaveChannel();
	return nRet == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::JoinChannelHost(LPCTSTR lpChannelName, LPCSTR token, UINT nUID, LPCSTR info)
{
	int ret = 0;

#ifdef UNICODE
	CHAR szChannelName[128];

	::WideCharToMultiByte(CP_ACP, 0, lpChannelName, -1, szChannelName, 128, NULL, NULL);
	m_channelHost = static_cast<IRtcEngine2*>(CAgoraObject::GetEngine())->createChannel(szChannelName);
#else
	m_channelHost = static_cast<IRtcEngine2*>(CAgoraObject::GetEngine())->createChannel(CT2A(lpChannelName));
#endif

	m_channelHostEventHandler.SetChannelType(CHANNEL_TYPE::CHANNEL_HOST);
	m_channelHost->setClientRole(CLIENT_ROLE_AUDIENCE);
	m_channelHost->setChannelEventHandler(&m_channelHostEventHandler);

	ChannelMediaOptions options;
	options.autoSubscribeAudio = 1;
	options.autoSubscribeVideo = 1;
	ret = m_channelHost->joinChannel(token, info, nUID, options);
	//ret = m_channelHost->joinChannelWithUserAccount(token, m_selfAccount, options);

	m_channelHostJoin = ret == 0 ? true : false;
	return 0 == ret;
}

BOOL CAgoraObject::JoinChannelSrc(LPCTSTR lpChannelName, LPCSTR token, UINT nUID, LPCSTR info)
{
	int ret = 0;

#ifdef UNICODE
	CHAR szChannelName[128];

	::WideCharToMultiByte(CP_ACP, 0, lpChannelName, -1, szChannelName, 128, NULL, NULL);
	m_channelSrc = static_cast<IRtcEngine2*>(CAgoraObject::GetEngine())->createChannel(szChannelName);
#else
	m_channelSrc = static_cast<IRtcEngine2*>(CAgoraObject::GetEngine())->createChannel(CT2A(lpChannelName));
#endif

	m_channelSrcEventHandler.SetChannelType(CHANNEL_TYPE::CHANNEL_SRC);
	m_channelSrc->setClientRole(CLIENT_ROLE_AUDIENCE);
	m_channelSrc->setChannelEventHandler(&m_channelSrcEventHandler);

	ChannelMediaOptions options;
	options.autoSubscribeAudio = 1;
	options.autoSubscribeVideo = 0;
	ret = m_channelSrc->joinChannel(token, info, nUID, options);
	//ret = m_channelSrc->joinChannelWithUserAccount(token, m_selfAccount, options);

	m_channelSrcJoin = ret == 0 ? true : false;
	return 0 == ret;
}

BOOL CAgoraObject::JoinChannelDest(LPCTSTR lpChannelName, LPCSTR token, UINT nUID, LPCSTR info)
{
	int ret = 0;

#ifdef UNICODE
	CHAR szChannelName[128];

	::WideCharToMultiByte(CP_ACP, 0, lpChannelName, -1, szChannelName, 128, NULL, NULL);
	m_channelDest = static_cast<IRtcEngine2*>(CAgoraObject::GetEngine())->createChannel(szChannelName);
#else
	m_channelDest = static_cast<IRtcEngine2*>(CAgoraObject::GetEngine())->createChannel(CT2A(lpChannelName));
#endif

	ChannelMediaOptions options;
	options.autoSubscribeAudio = 1;
	options.autoSubscribeVideo = 1;
	
	m_channelDestEventHandler.SetChannelType(CHANNEL_TYPE::CHANNEL_DEST);

	m_channelDest->setClientRole(CLIENT_ROLE_BROADCASTER);
	m_channelDest->setChannelEventHandler(&m_channelDestEventHandler);
	ret = m_channelDest->joinChannel(token, info, nUID, options);
	//ret = m_channelDest->joinChannelWithUserAccount(token, m_selfAccount, options);

	m_channelDestJoin = ret == 0 ? true : false;
	return 0 == ret;
}

BOOL CAgoraObject::JoinChannelTransl(LPCTSTR lpChannelName, LPCSTR token, UINT nUID, LPCSTR info)
{
	int ret = 0;

#ifdef UNICODE
	CHAR szChannelName[128];

	::WideCharToMultiByte(CP_ACP, 0, lpChannelName, -1, szChannelName, 128, NULL, NULL);
	m_channelTransl = static_cast<IRtcEngine2*>(CAgoraObject::GetEngine())->createChannel(szChannelName);
#else
	m_channelTransl = static_cast<IRtcEngine2*>(CAgoraObject::GetEngine())->createChannel(CT2A(lpChannelName));
#endif

	m_channelTranslEventHandler.SetChannelType(CHANNEL_TYPE::CHANNEL_TRANSL);
	m_channelTransl->setClientRole(CLIENT_ROLE_BROADCASTER);
	m_channelTransl->setChannelEventHandler(&m_channelTranslEventHandler);

	int data;
	m_channelTransl->createDataStream(&data, true, true);

	ChannelMediaOptions options;
	options.autoSubscribeAudio = 1;
	options.autoSubscribeVideo = 1;
	
	//ret = m_channelTransl->joinChannel(token, info, nUID, options);
	ret = m_channelTransl->joinChannelWithUserAccount(token, m_selfAccount, options);
	m_channelTranslJoin = ret == 0 ? true : false;

	m_channelTranslPublish = ret == 0 ? true : false;

	return 0 == ret;
}

BOOL CAgoraObject::LeaveHostChannel()
{
	int ret = -1;
	if (m_channelHost != NULL)
	{
		if (m_channelHostJoin)	  ret = m_channelHost->leaveChannel();
		m_channelHostJoin = false;
	}
	return 0 == ret;
}
BOOL CAgoraObject::LeaveDestChannel() 
{
	int ret = -1;
	if (m_channelDest != NULL)
	{
		if (m_channelDestPublish) ret = m_channelDest->unpublish();
		if (m_channelDestJoin)	  ret = m_channelDest->leaveChannel();
		
		m_channelDestPublish	= false;
		m_channelDestJoin		= false;
	}
	return 0 == ret;
}
BOOL CAgoraObject::LeaveSrcChannel()
{
	int ret = -1;
	if (m_channelSrc != NULL)
	{
		if (m_channelSrcJoin) ret = m_channelSrc->leaveChannel();
		m_channelSrcJoin = false;
	}
	return 0 == ret;
}

BOOL CAgoraObject::LeaveTranslChannel() 
{
	int ret = -1;
	if (m_channelTransl != NULL)
	{
		if (m_channelTranslPublish) ret = m_channelTransl->unpublish();
		if (m_channelTranslJoin)	ret = m_channelTransl->leaveChannel();
		
		m_channelTranslPublish	= false;
		m_channelTranslJoin		= false;
	}
	return 0 == ret;
}
CString CAgoraObject::GetChanelName()
{
	return m_strChannelName;
}

CString CAgoraObject::GetCallID()
{
	agora::util::AString uid;
	CString strUID;

	m_lpAgoraEngine->getCallId(uid);

#ifdef UNICODE
	::MultiByteToWideChar(CP_ACP, 0, uid->c_str(), -1, strUID.GetBuffer(128), 128);
	strUID.ReleaseBuffer();
#else
	strUID = uid->c_str();
#endif

	return strUID;
}

BOOL CAgoraObject::EnableVideo(BOOL bEnable)
{
	int nRet = 0;

	if (bEnable)
		nRet = m_lpAgoraEngine->enableVideo();
	else
		nRet = m_lpAgoraEngine->disableVideo();

	if (nRet == 0)
		m_bVideoEnable = bEnable;

	return nRet == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::IsVideoEnabled()
{
	return m_bVideoEnable;
}

BOOL CAgoraObject::SelfUIDCheck(uid_t uid)
{
	return m_nSelfUID == uid;
}

BOOL CAgoraObject::RegistrLocalAccount(CString userAccount)
{
	CHAR userAcc[255];
	UserInfo usrInfo;
	int ret = -1;

	CT2A userA(userAccount);
	strcpy_s(userAcc, 255, userA);

	CAgoraObject::GetEngine()->getUserInfoByUserAccount(userAcc, &usrInfo);
	
	if (SelfUIDCheck(usrInfo.uid) || usrInfo.uid == 0)
		ret = CAgoraObject::GetEngine()->registerLocalUserAccount(CT2A(APP_ID), userAcc);

	return 0 == ret;
}

BOOL CAgoraObject::EnableScreenCapture(HWND hWnd, int nCapFPS, LPCRECT lpCapRect, BOOL bEnable, int nBitrate)
{
	ASSERT(m_lpAgoraEngine != NULL);

	int ret = 0;
	RtcEngineParameters rep(*m_lpAgoraEngine);

	agora::rtc::Rectangle rcCap;
	ScreenCaptureParameters capParam;
	capParam.bitrate = nBitrate;
	capParam.frameRate = nCapFPS;

	if (bEnable) {
		if (lpCapRect == NULL){
			RECT rc;
			if (hWnd){
				GetWindowRect(hWnd, &rc);
				capParam.dimensions.width = rc.right - rc.left;
				capParam.dimensions.height = rc.bottom - rc.top;
				ret = m_lpAgoraEngine->startScreenCaptureByWindowId(hWnd, rcCap, capParam);
			}
			else{
				GetWindowRect(GetDesktopWindow(), &rc);
				agora::rtc::Rectangle screenRegion = { rc.left, rc.right, rc.right - rc.left, rc.bottom - rc.top };
				capParam.dimensions.width = rc.right - rc.left;
				capParam.dimensions.height = rc.bottom - rc.top;
				ret = m_lpAgoraEngine->startScreenCaptureByScreenRect(screenRegion, rcCap, capParam);
			}
			//startScreenCapture(hWnd, nCapFPS, NULL, nBitrate);
		}
		else {
			capParam.dimensions.width = lpCapRect->right - lpCapRect->left;
			capParam.dimensions.height = lpCapRect->bottom - lpCapRect->top;

			rcCap.x = lpCapRect->left;
			rcCap.y = lpCapRect->top;
			rcCap.width = lpCapRect->right - lpCapRect->left;
			rcCap.height = lpCapRect->bottom - lpCapRect->top;

			if (hWnd)
				ret = m_lpAgoraEngine->startScreenCaptureByWindowId(hWnd, rcCap, capParam);
			else{
				
				agora::rtc::Rectangle screenRegion = rcCap;
				ret = m_lpAgoraEngine->startScreenCaptureByScreenRect(screenRegion, rcCap, capParam);
			}
		}
	}
	else
		ret = m_lpAgoraEngine->stopScreenCapture();

	if (ret == 0)
		m_bScreenCapture = bEnable;

	return ret == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::IsScreenCaptureEnabled()
{
	return m_bScreenCapture;
}

BOOL CAgoraObject::MuteLocalAudio(BOOL bMuted)
{
	ASSERT(m_lpAgoraEngine != NULL);

	RtcEngineParameters rep(*m_lpAgoraEngine);

	int ret = rep.muteLocalAudioStream((bool)bMuted);
	if (ret == 0)
		m_bLocalAudioMuted = bMuted;

	return ret == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::IsLocalAudioMuted()
{
	return m_bLocalAudioMuted;
}


BOOL CAgoraObject::MuteLocalVideo(BOOL bMuted)
{
	ASSERT(m_lpAgoraEngine != NULL);

	RtcEngineParameters rep(*m_lpAgoraEngine);

	int ret = rep.muteLocalVideoStream((bool)bMuted);
	if (ret == 0)
		m_bLocalVideoMuted = bMuted;

	return ret == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::IsLocalVideoMuted()
{
	return m_bLocalVideoMuted;
}


BOOL CAgoraObject::EnableAudioRecording(BOOL bEnable, LPCTSTR lpFilePath)
{
	int ret = 0;

	RtcEngineParameters rep(*m_lpAgoraEngine);

	if (bEnable) {
#ifdef UNICODE
		CHAR szFilePath[MAX_PATH];
		::WideCharToMultiByte(CP_ACP, 0, lpFilePath, -1, szFilePath, MAX_PATH, NULL, NULL);
		ret = rep.startAudioRecording(szFilePath, AUDIO_RECORDING_QUALITY_HIGH);
#else
		ret = rep.startAudioRecording(lpFilePath);
#endif
	}
	else
		ret = rep.stopAudioRecording();

	return ret == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::EnableNetworkTest(BOOL bEnable)
{
	int ret = 0;

	
	if (bEnable)
		ret = m_lpAgoraEngine->enableLastmileTest();
	else
		ret = m_lpAgoraEngine->disableLastmileTest();

	return ret == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::EnableEchoTest(BOOL bEnable)
{
	int ret = 0;

	if (bEnable)
		ret = m_lpAgoraEngine->startEchoTest();
	else
		ret = m_lpAgoraEngine->stopEchoTest();

	if (ret == 0)
		m_bEchoTest = bEnable;

	return ret == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::LocalVideoPreview(HWND hVideoWnd, BOOL bPreviewOn)
{
	int nRet = 0;

	if (bPreviewOn) {
		VideoCanvas vc;

		vc.uid = 0;
		vc.view = hVideoWnd;
		vc.renderMode = RENDER_MODE_TYPE::RENDER_MODE_HIDDEN;

		m_lpAgoraEngine->setupLocalVideo(vc);
		nRet = m_lpAgoraEngine->startPreview();
	}
	else
		nRet = m_lpAgoraEngine->stopPreview();

	return nRet == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::SetLogFilter(UINT logFilterType, LPCTSTR lpLogPath)
{
	int nRet = 0;
	RtcEngineParameters rep(*m_lpAgoraEngine);

	nRet = rep.setLogFilter(logFilterType);

#ifdef UNICODE
	CHAR szFilePath[MAX_PATH];
	::WideCharToMultiByte(CP_ACP, 0, lpLogPath, -1, szFilePath, MAX_PATH, NULL, NULL);
	nRet |= rep.setLogFile(szFilePath);
#else
	nRet |= rep.setLogFile(lpLogPath);
#endif

	return nRet == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::SetEncryptionSecret(LPCTSTR lpKey, int nEncryptType)
{
	CHAR szUTF8[MAX_PATH];

#ifdef UNICODE
	::WideCharToMultiByte(CP_UTF8, 0, lpKey, -1, szUTF8, MAX_PATH, NULL, NULL);
#else
	WCHAR szAnsi[MAX_PATH];
	::MultiByteToWideChar(CP_ACP, 0, lpKey, -1, szAnsi, MAX_PATH);
	::WideCharToMultiByte(CP_UTF8, 0, szAnsi, -1, szUTF8, MAX_PATH, NULL, NULL);
#endif
    switch (nEncryptType)
    {
    case 0:
        m_lpAgoraEngine->setEncryptionMode("aes-128-xts");
    	break;
    case 1:
        m_lpAgoraEngine->setEncryptionMode("aes-256-xts");
        break;
    default:
        m_lpAgoraEngine->setEncryptionMode("aes-128-xts");
        break;
    }
	int nRet = m_lpAgoraEngine->setEncryptionSecret(szUTF8);

	return nRet == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::EnableEncryption(bool enabled, const EncryptionConfig & config)
{
	int nRet = m_lpAgoraEngine->enableEncryption(enabled, config);
	return nRet == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::EnableLocalRender(BOOL bEnable)
{
    int nRet = 0;

    //if (bEnable)
    //    nRet = m_lpAgoraEngine->setParameters("{\"che.video.local.render\":true}");
    //else
    //    nRet = m_lpAgoraEngine->setParameters("{\"che.video.local.render\":false}");

    return nRet == 0 ? TRUE : FALSE;
}

int CAgoraObject::CreateMessageStream()
{
    int nDataStream = 0;

	if (m_channelTransl != NULL)
		m_channelTransl->createDataStream(&nDataStream, true, true);

    return nDataStream;
}

BOOL CAgoraObject::SendChatMessage(int nStreamID, LPCTSTR lpChatMessage)
{
    _ASSERT(nStreamID != 0);
    int nMessageLen = _tcslen(lpChatMessage);
    _ASSERT(nMessageLen < 128);

	CHAR szUTF8[255];

#ifdef UNICODE
	int nUTF8Len = ::WideCharToMultiByte(CP_UTF8, 0, lpChatMessage, nMessageLen, szUTF8, 255, NULL, NULL);
#else
	int nUTF8Len = ::MultiByteToWideChar(CP_UTF8, lpChatMessage, nMessageLen, szUTF8, 255);
#endif

	CString s(m_channelTransl->channelId());
    int nRet = m_channelTransl->sendStreamMessage(nStreamID, szUTF8, nUTF8Len);

    return nRet == 0 ? TRUE : FALSE;
}


BOOL CAgoraObject::EnableWhiteboardVer(BOOL bEnable)
{
	// HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Internet Explorer\MAIN\FeatureControl\FEATURE_BROWSER_EMULATION
	HKEY hKey = NULL;

	LSTATUS lStatus = ::RegCreateKeyEx(HKEY_CURRENT_USER, _T("SOFTWARE\\Wow6432Node\\Microsoft\\Internet Explorer\\MAIN\\FeatureControl\\FEATURE_BROWSER_EMULATION"), 0, REG_OPTION_NON_VOLATILE
		, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);

	if (lStatus != ERROR_SUCCESS)
		return FALSE;

	DWORD dwIEVer = 11001;

	if (bEnable)
		lStatus = ::RegSetValueEx(hKey, _T("AgoraVideoCall.exe"), 0, REG_DWORD, (const BYTE*)&dwIEVer, sizeof(DWORD));
	else
		lStatus = ::RegDeleteKeyValue(hKey, NULL, _T("AgoraVideoCall.exe"));

	::RegCloseKey(hKey);

	return lStatus == ERROR_SUCCESS ? TRUE : FALSE;
}

BOOL CAgoraObject::EnableWhiteboardFeq(BOOL bEnable)
{
	// HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Internet Explorer\MAIN\FeatureControl\FEATURE_BROWSER_EMULATION
	HKEY hKey = NULL;
	
	LSTATUS lStatus = ::RegCreateKeyEx(HKEY_CURRENT_USER, _T("SOFTWARE\\Wow6432Node\\Microsoft\\Internet Explorer\\MAIN\\FeatureControl\\FEATURE_MANAGE_SCRIPT_CIRCULAR_REFS"), 0, REG_OPTION_NON_VOLATILE
		, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);

	if (lStatus != ERROR_SUCCESS)
		return FALSE;

	DWORD dwValue = 1;

	if (bEnable)
		lStatus = ::RegSetValueEx(hKey, _T("AgoraVideoCall.exe"), 0, REG_DWORD, (const BYTE*)&dwValue, sizeof(DWORD));
	else
		lStatus = ::RegDeleteKeyValue(hKey, NULL, _T("AgoraVideoCall.exe"));

	::RegCloseKey(hKey);

	return lStatus == ERROR_SUCCESS ? TRUE : FALSE;
}


void CAgoraObject::SetDefaultParameters()
{
    CAGJson m_agJson;
    std::map<std::string, std::string> mapStringParamsters;
    std::map<std::string, bool> mapBoolParameters;
    std::map<std::string, int> mapIntParameters;
    std::map<std::string, std::string> mapObjectParameters;
    if (m_agJson.GetParameters(mapStringParamsters, mapBoolParameters, mapIntParameters, mapObjectParameters)) {
        AParameter apm(m_lpAgoraEngine);
        for (auto iter = mapBoolParameters.begin();
            iter != mapBoolParameters.end(); ++iter) {
            apm->setBool(iter->first.c_str(), iter->second);
        }
        for (auto iter = mapStringParamsters.begin();
            iter != mapStringParamsters.end(); ++iter) {
            apm->setString(iter->first.c_str(), iter->second.c_str());
        }
        for (auto iter = mapIntParameters.begin();
            iter != mapIntParameters.end(); ++iter) {
            apm->setInt(iter->first.c_str(), iter->second);
        }

        for (auto iter = mapObjectParameters.begin();
            iter != mapObjectParameters.end(); ++iter) {
            apm->setObject(iter->first.c_str(), iter->second.c_str());
        }
    }
}


std::string CAgoraObject::GetToken()
{
    std::string token(APP_TOKEN);
    if (!token.empty())
        return token;

    TCHAR szFilePath[MAX_PATH];
    ::GetModuleFileName(NULL, szFilePath, MAX_PATH);
    LPTSTR lpLastSlash = _tcsrchr(szFilePath, _T('\\'));

    if (lpLastSlash == NULL)
        return token;

    SIZE_T nNameLen = MAX_PATH - (lpLastSlash - szFilePath + 1);
    _tcscpy_s(lpLastSlash + 1, nNameLen, _T("AppID.ini"));


    TCHAR szToken[MAX_PATH] = { 0 };
    char temp[MAX_PATH] = { 0 };
    ::GetPrivateProfileString(_T("AppID"), _T("AppToken"), NULL, szToken, MAX_PATH, szFilePath);
    ::WideCharToMultiByte(CP_UTF8, 0, szToken, -1, temp, 128, NULL, NULL);

    return temp;
}

Tokens CAgoraObject::GetComplexToken()
{
	if (!m_token.isEmptyToken())
		return m_token;
	return Tokens();
}

void CAgoraObject::SetComplexToken(Tokens token)
{
	m_token = token;
}

void CAgoraObject::MuteAllAudio(int mute) //Mutes all clients
{
	GetChannelTranslator()->muteAllRemoteAudioStreams(mute);
}

void CAgoraObject::MuteClient(LPARAM id, int mute) //Searches id and mutes client
{
	GetChannelTranslator()->muteRemoteAudioStream(id, mute);
}

void CAgoraObject::MuteClient(int id, int mute) //Mutes provided client by its id
{
	GetChannelTranslator()->muteRemoteAudioStream(id, mute);
}


void CAgoraObject::MuteSelf(int mute) //Mutes local audio
{
	RtcEngineParameters rep(this->GetEngine());

	rep.muteLocalAudioStream(mute);
}

void CAgoraObject::ClearUID()
{
	CollectorUID.clear();
}

IChannel* CAgoraObject::GetChannelTranslator()
{
	return m_channelTransl;
}

uid_t CAgoraObject::GetHostUID()
{
	return m_hostUID;
}

int CAgoraObject::SwitchMute()
{
	if (IsMuted == 1)
	{
		IsMuted = 0;
		MuteAllAudio(0);
		MuteSelf(1);
	}
	else
	{
		IsMuted = 1;
		MuteAllAudio(1);
		MuteSelf(0);
	}

	return IsMuted;
}

int CAgoraObject::GetIsMuted()
{
	return IsMuted;
}

void CAgoraObject::SetIsMuted(bool mute)
{
	IsMuted = mute;
}

int CAgoraObject::TogglePublishChannel(CHANNEL_TYPE channel)
{
	int ret = -1;

	if (m_channelTranslPublish)
	{
		ret = m_channelTransl->unpublish();
		m_channelTranslPublish = 0;
	}

	if (m_channelDestPublish)
	{
		ret = m_channelDest->unpublish();
		m_channelDestPublish = 0;
	}
	ret = -1;
	switch (channel)
	{
	case CHANNEL_TYPE::CHANNEL_TRANSL:
		if (m_channelTransl != NULL && m_channelTranslJoin)	ret = m_channelTransl->publish();
		m_channelTranslPublish = (0 == ret);
		break;
	case CHANNEL_TYPE::CHANNEL_DEST:
		if (m_channelDest != NULL && m_channelDestJoin)		ret = m_channelDest->publish();
		m_channelDestPublish = (0 == ret);
		MuteLocalAudio(0);
		break;
	case CHANNEL_TYPE::CHANNEL_SRC:
		break;
	default:
		break;
	}

	MuteLocalVideo(IsPublish());
	MuteAllAudio(IsPublish());

	return 0 == ret;
}

void CAgoraObject::SetHostUID(uid_t uid)
{
	m_hostUID = uid;
}