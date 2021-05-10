#pragma once

#include <IAgoraRtcEngine.h>
#include <IAgoraRtcChannel.h>
#include "AGEngineEventHandler.h"
#include "AGChannelEventHandler.h"
#include "video_preprocessing_plugin.h"

#include <string>
#include "Tokens.h"

#define AVC_VER _T("V1.12.0, Build234, 08/14/2017, SDK1.12 .0")

using namespace agora::rtc;


// ������λ
#define AG_ENGFLAG_ENNETTEST	0x00000001
#define AG_ENGFLAG_ECHOTEST		0x00000002
#define AG_ENGFLAG_SPKPHTEST	0x00000004
#define AG_ENGFLAG_MICPHTEST	0x00000008
#define AG_ENGFLAG_VIDEOTEST	0x00000010


#define AG_CODEC_E264	0x00000000
#define AG_CODEC_EVP	0x00000001
#define AG_CODEC_VP8	0x00000002

/* NOTE:
	PLEASE KEEP THIS App ID IN SAFE PLACE
    Get your own App ID at https://dashboard.agora.io/
    After you entered the App ID, remove <##> outside of Your App ID
    APP_ID _T("<YOUR_APP_ID>")
    Obtain a temp Access Token at https://dashboard.agora.io
    You will need to deploy your own token server for production release
    Leave this value empty if Security keys/Token is not enabled for your project
    APP_TOKEN "<YOUR TOKEN>"
*/
#define APP_ID _T("8f5f3639ee4941238369d5ecbcecad14")
#define APP_TOKEN ""

class CAgoraObject
{
public:
	~CAgoraObject(void);

	static CString LoadAppID();

	void SetNetworkTestFlag(BOOL bEnable);
	BOOL GetNetworkTestFlag();

	void SetEchoTestFlag(BOOL bEnable);
	BOOL GetEchoTestFlag();

	void SetSpeakerphoneTestFlag(BOOL bEnable);
	BOOL GetSpeakerphoneTestFlag();

	void SetMicrophoneTestFlag(BOOL bEnable);
	BOOL GetMicrophoneTestFlag();

	void SetVideoTestFlag(BOOL bEnable);
	BOOL GetVideoTestFlag();

	void SetMsgHandlerWnd(HWND hWnd);
	HWND GetMsgHandlerWnd();

	BOOL SetLogFilePath(LPCTSTR lpLogPath = NULL);

	BOOL JoinChannel(LPCTSTR lpChannelName, UINT nUID = 0, LPCSTR lpChannelToken = NULL);
	BOOL LeaveCahnnel();

	BOOL JoinChannelSrc(LPCTSTR channel, LPCSTR token, UINT nUID, LPCSTR info = "");
	BOOL JoinChannelDest(LPCTSTR channel, LPCSTR token, UINT nUID, LPCSTR info = "");
	BOOL JoinChannelTransl(LPCTSTR channel, LPCSTR token, UINT nUID, LPCSTR info = "");

	BOOL LeaveDestChannel();
	BOOL LeaveSrcChannel();
	BOOL LeaveTranslChannel();

	BOOL IsPublish() { return m_channelDestPublish; }

	BOOL RegistrLocalAccount(CString userAccount);

	CString GetChanelName();
	CString GetCallID();
	CString GetVendorKey() { return m_strVendorKey; };

	void SetSelfUID(UINT nUID) { m_nSelfUID = nUID; };
	UINT GetSelfUID() { return m_nSelfUID; };

	const char* GetSelfAccount() { return m_selfAccount; };
	void SetSelfAccount(const char*acc) { strcpy_s(m_selfAccount, 256, acc); }

	BOOL EnableVideo(BOOL bEnable = TRUE);
	BOOL IsVideoEnabled();

	BOOL EnableScreenCapture(HWND hWnd, int nCapFPS = 15, LPCRECT lpCapRect = NULL, BOOL bEnable = TRUE, int nBitrate = 0);
	BOOL IsScreenCaptureEnabled();

	BOOL MuteLocalAudio(BOOL bMuted = TRUE);
	BOOL IsLocalAudioMuted();

	BOOL MuteLocalVideo(BOOL bMuted = TRUE);
	BOOL IsLocalVideoMuted();

	BOOL EnableAudioRecording(BOOL bEnable, LPCTSTR lpFilePath);

	BOOL EnableNetworkTest(BOOL bEnable);

	BOOL EnableEchoTest(BOOL bEnable = TRUE);
	BOOL IsEchoTesting() { return m_bEchoTest; };

	BOOL LocalVideoPreview(HWND hVideoWnd, BOOL bPreviewOn = TRUE);

	BOOL SetLogFilter(UINT logFilterType, LPCTSTR lpLogPath);

    BOOL SetEncryptionSecret(LPCTSTR lpKey, int nEncryptType = 0);

	BOOL EnableEncryption(bool enabled, const EncryptionConfig& config);

    BOOL EnableLocalRender(BOOL bEnable);

    int CreateMessageStream();
    BOOL SendChatMessage(int nStreamID, LPCTSTR lpChatMessage);

	static IRtcEngine *GetEngine();
	
	static CString GetSDKVersion();
	static CString GetSDKVersionEx();

	static BOOL EnableWhiteboardVer(BOOL bEnable);
	static BOOL EnableWhiteboardFeq(BOOL bEnable);

    void SetDefaultParameters();

	std::string GetToken();

	Tokens GetComplexToken();
	void SetComplexToken(Tokens token);

	int TogglePublishChannel(CHANNEL_TYPE channel);
protected:
	CAgoraObject(void);

	std::vector<int> CollectorUID;

private:

	DWORD	m_dwEngineFlag;
	static  CAgoraObject	*m_lpAgoraObject;
	static	IRtcEngine	    *m_lpAgoraEngine;
	static	CString			m_strVendorKey;

	IChannel* m_channelSrc;
	IChannel* m_channelDest;
	IChannel* m_channelTransl;
	
	AGChannelEventHandler m_channelSrcEventHandler;
	AGChannelEventHandler m_channelDestEventHandler;
	AGChannelEventHandler m_channelTranslEventHandler;
	
	BOOL		m_channelSrcJoin		= false;
	BOOL		m_channelDestJoin		= false;
	BOOL		m_channelTranslJoin		= false;

	BOOL		m_channelDestPublish	= false;
	BOOL		m_channelTranslPublish	= false;

	uid_t		m_nSelfUID;
	char		m_selfAccount[256];
	CString		m_strChannelName;
	BOOL		m_bVideoEnable;

	BOOL		m_bLocalAudioMuted;
	BOOL		m_bLocalVideoMuted;
	BOOL		m_bEchoTest;

	BOOL		m_bScreenCapture;
	
//	int			m_nCodecType;
	Tokens m_token;
	uid_t m_hostUID;

	bool IsMuted;
public:
	static CAgoraObject *GetAgoraObject(LPCTSTR lpVendorKey = NULL);
	static void CloseAgoraObject();

	static CAGEngineEventHandler m_EngineEventHandler;

	//std::vector<int> CollectAllUID();

	void MuteAllAudio(int mute);
	void MuteSelf(int mute);
	int SwitchMute();
	int GetIsMuted();
	void ClearUID();

	IChannel* GetChanelTranslator();
};
