#pragma once
#include "AGButton.h"

class CAGBckWnd : public CWnd
{
	DECLARE_DYNAMIC(CAGBckWnd)

public:
	CAGBckWnd();
	virtual ~CAGBckWnd();

	void ShowTips(BOOL bShow = TRUE);
	void SetVideoResolution(int nWidth, int nHeight);
	void SetFrameRateInfo(int nFPS);
	void SetBitrateInfo(int nBitrate);
	void InitCtrls();
	void SetUID(int nUID);
	BOOL SetBackImage(UINT nID, UINT nWidth, UINT nHeight, COLORREF crMask);

protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMuteClick();
	afx_msg void OnVideoMuteClick();
	afx_msg LRESULT OnResize(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

private:
	BOOL		m_bShowTip;
	UINT		m_nUID;

	COLORREF	m_crBackColor;
	CImageList		m_imgBackGround;

	int		m_nWidth;
	int		m_nHeight;
	int		m_nFps;
	int		m_nBitrate;

	CBrush	m_brBack;

	CAGButton		m_btnEnableAudio;
	CAGButton		m_btnShowVid;
	BOOL ctr_Created;
	BOOL bMuted;
	BOOL bHidden;
	BOOL CtrlMode;
};