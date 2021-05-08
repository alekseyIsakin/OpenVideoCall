#pragma once

#include "StdAfx.h"
#include "AGEventDef.h"

enum class CHANNEL_TYPE {
	CHANNEL_SRC = 255,
	CHANNEL_TRANSL,
	CHANNEL_DEST
};

class AGChannelEventHandler 
	:public agora::rtc::IChannelEventHandler
{
private:
	HWND m_hMsgHanlder;
	CHANNEL_TYPE m_channelType;

public:

	virtual void SetChannelType(CHANNEL_TYPE type)
	{
		m_channelType = type;
	}

	virtual void setMsgHandler(HWND msgHandler)
	{
		this->m_hMsgHanlder = msgHandler;
	}
	virtual void onChannelWarning(IChannel* rtcChannel, int warn, const char* msg) {
	}

	virtual void onChannelError(IChannel* rtcChannel, int err, const char* msg) {
	}

	virtual void onJoinChannelSuccess(IChannel* rtcChannel, uid_t uid, int elapsed) {

		LPAGE_JOINCHANNEL_SUCCESS lpData = new AGE_JOINCHANNEL_SUCCESS;

		int nChannelLen = strlen(rtcChannel->channelId()) + 1;

		lpData->channel = new char[nChannelLen];
		lpData->uid = uid;
		lpData->elapsed = elapsed;

		strcpy_s(lpData->channel, nChannelLen, rtcChannel->channelId());

		if (m_hMsgHanlder != NULL)
			::PostMessage(m_hMsgHanlder, WM_MSGID(EID_JOINCHANNEL_SUCCESS), (WPARAM)lpData, (LPARAM)m_channelType);
		}

	virtual void onRejoinChannelSuccess(IChannel* rtcChannel, uid_t uid, int elapsed) {
		LPAGE_JOINCHANNEL_SUCCESS lpData = new AGE_JOINCHANNEL_SUCCESS;

		int nChannelLen = strlen(rtcChannel->channelId()) + 1;

		lpData->channel = new char[nChannelLen];
		lpData->uid = uid;
		lpData->elapsed = elapsed;

		strcpy_s(lpData->channel, nChannelLen, rtcChannel->channelId());

		if (m_hMsgHanlder != NULL)
			::PostMessage(m_hMsgHanlder, WM_MSGID(EID_REJOINCHANNEL_SUCCESS), (WPARAM)lpData, (LPARAM)m_channelType);
	}

	virtual void onLeaveChannel(IChannel* rtcChannel, const RtcStats& stats) {

		LPAGE_LEAVE_CHANNEL lpData = new AGE_LEAVE_CHANNEL;

		memcpy(&lpData->rtcStat, &stat, sizeof(RtcStats));

		if (m_hMsgHanlder != NULL)
			::PostMessage(m_hMsgHanlder, WM_MSGID(EID_LEAVE_CHANNEL), (WPARAM)lpData, (LPARAM)m_channelType);
	}

	virtual void onRemoteAudioStats(IChannel* rtcChannel, const RemoteAudioStats & rAudioStas)
	{
		LPAGE_AUDIO_QUALITY lpData = new AGE_AUDIO_QUALITY;

		lpData->uid = rAudioStas.uid;
		lpData->quality = rAudioStas.quality;
		lpData->delay = rAudioStas.networkTransportDelay;
		// lpData->lost = lost;

		if (m_hMsgHanlder != NULL)
			::PostMessage(m_hMsgHanlder, WM_MSGID(EID_AUDIO_QUALITY), (WPARAM)lpData, (LPARAM)m_channelType);
	}

	virtual void onUserJoined(IChannel* rtcChannel, uid_t uid, int elapsed) {
			LPAGE_USER_JOINED lpData = new AGE_USER_JOINED;

			lpData->uid = uid;
			lpData->elapsed = elapsed;

			CString s(rtcChannel->channelId());
			strcpy_s(lpData->channelID, 64, rtcChannel->channelId());

			if (m_hMsgHanlder != NULL)
				::PostMessage(m_hMsgHanlder, WM_MSGID(EID_USER_JOINED), (WPARAM)lpData, (LPARAM)m_channelType);
	}

	virtual void onUserOffline(IChannel* rtcChannel, uid_t uid, USER_OFFLINE_REASON_TYPE reason) {
		LPAGE_USER_OFFLINE lpData = new AGE_USER_OFFLINE;

		lpData->uid = uid;
		lpData->reason = reason;

		if (m_hMsgHanlder != NULL)
			::PostMessage(m_hMsgHanlder, WM_MSGID(EID_USER_OFFLINE), (WPARAM)lpData, 0);
	}

	virtual void onConnectionLost(IChannel* rtcChannel) {
	}

	virtual void onFirstRemoteVideoFrame(uid_t uid, int width, int height, int elapsed)
	{
	}


	virtual void onRequestToken(IChannel* rtcChannel) {
	}
	virtual void onTokenPrivilegeWillExpire(IChannel* rtcChannel, const char* token) {

	}

	virtual void onRtcStats(IChannel* rtcChannel, const RtcStats& stats) {

	}

	virtual void onNetworkQuality(IChannel* rtcChannel, uid_t uid, int txQuality, int rxQuality) {

	}

	virtual void onRemoteVideoStats(IChannel* rtcChannel, const RemoteVideoStats& stats) {

		LPAGE_REMOTE_VIDEO_STAT lpData = new AGE_REMOTE_VIDEO_STAT;

		lpData->uid = stats.uid;
		lpData->delay = stats.delay;
		lpData->width = stats.width;
		lpData->height = stats.height;
		
		strcpy_s(lpData->channelID, 64, rtcChannel->channelId());

		lpData->rendererOutputFrameRate = stats.rendererOutputFrameRate;
		lpData->receivedBitrate = stats.receivedBitrate;
		lpData->decoderOutputFrameRate = stats.decoderOutputFrameRate;

		if (m_hMsgHanlder != NULL)
			::PostMessage(m_hMsgHanlder, WM_MSGID(EID_REMOTE_VIDEO_STAT), (WPARAM)lpData, (LPARAM)m_channelType);
	}
	virtual void onRemoteAudioStateChanged(IChannel* rtcChannel, uid_t uid, REMOTE_AUDIO_STATE state, REMOTE_AUDIO_STATE_REASON reason, int elapsed) {

	}
	virtual void onAudioPublishStateChanged(IChannel* rtcChannel, STREAM_PUBLISH_STATE oldState, STREAM_PUBLISH_STATE newState, int elapseSinceLastState) {

	}
	virtual void onVideoPublishStateChanged(IChannel* rtcChannel, STREAM_PUBLISH_STATE oldState, STREAM_PUBLISH_STATE newState, int elapseSinceLastState) {

	}
	virtual void onAudioSubscribeStateChanged(IChannel* rtcChannel, uid_t uid, STREAM_SUBSCRIBE_STATE oldState, STREAM_SUBSCRIBE_STATE newState, int elapseSinceLastState) {

	}
	virtual void onVideoSubscribeStateChanged(IChannel* rtcChannel, uid_t uid, STREAM_SUBSCRIBE_STATE oldState, STREAM_SUBSCRIBE_STATE newState, int elapseSinceLastState) {

	}
	virtual void onActiveSpeaker(IChannel* rtcChannel, uid_t uid) {

	}
	virtual void onVideoSizeChanged(IChannel* rtcChannel, uid_t uid, int width, int height, int rotation) {

	}
	virtual void onRemoteVideoStateChanged(IChannel* rtcChannel, uid_t uid, REMOTE_VIDEO_STATE state, REMOTE_VIDEO_STATE_REASON reason, int elapsed) {
		if (state == REMOTE_VIDEO_STATE_STARTING) 
		{
			LPAGE_FIRST_REMOTE_VIDEO_DECODED lpData = new AGE_FIRST_REMOTE_VIDEO_DECODED;

			lpData->uid = uid;
			lpData->width = 192;
			lpData->height = 108;
			lpData->elapsed = elapsed;
			strcpy_s(lpData->channelID, 64, rtcChannel->channelId());

			if (m_hMsgHanlder != NULL)
				::PostMessage(m_hMsgHanlder, WM_MSGID(EID_FIRST_REMOTE_VIDEO_DECODED), (WPARAM)lpData, (LPARAM)m_channelType);
		}

	}
	virtual void onStreamMessage(IChannel* rtcChannel, uid_t uid, int streamId, const char* data, size_t length) {
		LPAGE_STREAM_MESSAGE lpData = new AGE_STREAM_MESSAGE;

		lpData->uid = uid;
		lpData->streamId = streamId;
		lpData->data = new char[length];
		lpData->length = length;

		memcpy_s(lpData->data, length, data, length);

		if (m_hMsgHanlder != NULL)
			::PostMessage(m_hMsgHanlder, WM_MSGID(EID_STREAM_MESSAGE), (WPARAM)lpData, (LPARAM)m_channelType);
	}
	virtual void onStreamMessageError(IChannel* rtcChannel, uid_t uid, int streamId, int code, int missed, int cached) {

	}
	virtual void onChannelMediaRelayStateChanged(IChannel* rtcChannel, CHANNEL_MEDIA_RELAY_STATE state, CHANNEL_MEDIA_RELAY_ERROR code) {

	}
	virtual void onChannelMediaRelayEvent(IChannel* rtcChannel, CHANNEL_MEDIA_RELAY_EVENT code) {

	}
	virtual void onRtmpStreamingStateChanged(IChannel* rtcChannel, const char* url, RTMP_STREAM_PUBLISH_STATE state, RTMP_STREAM_PUBLISH_ERROR errCode) {

	}
	virtual void onRtmpStreamingEvent(IChannel* rtcChannel, const char* url, RTMP_STREAMING_EVENT eventCode) {

	}
	virtual void onTranscodingUpdated(IChannel* rtcChannel) {

	}

	virtual void onStreamInjectedStatus(IChannel* rtcChannel, const char* url, uid_t uid, int status) {

	}

	virtual void onLocalPublishFallbackToAudioOnly(IChannel* rtcChannel, bool isFallbackOrRecover) {

	}

	virtual void onRemoteSubscribeFallbackToAudioOnly(IChannel* rtcChannel, uid_t uid, bool isFallbackOrRecover) {

	}

	virtual void onConnectionStateChanged(IChannel* rtcChannel,
		CONNECTION_STATE_TYPE state,
		CONNECTION_CHANGED_REASON_TYPE reason) {

	}
};