#pragma once
#include "AGButton.h"
#include "AGEdit.h"
#include "AGComboBox.h"
#include "DeviceDlg.h"
#include "afxwin.h"

class CEnterApplication : public CDialogEx
{
	DECLARE_DYNAMIC(CEnterApplication)


public:
	CEnterApplication(CWnd* pParent = NULL);
	virtual ~CEnterApplication();

	//CString GetChannelName();

	//void SetVideoString(LPCTSTR lpVideoString);
	//void CleanEncryptionSecret();
	//void SetCtrlPos();

	enum { IDD = IDD_ENTERAPPLICATION_DIALOG };

protected:
	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	//virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnPaint();
	//afx_msg void OnBnClickedBtntestChannel();
	afx_msg void OnBnClickedBtnjoinChannel();
	//afx_msg void OnBnClickedBtnsetChannel();
	afx_msg void OnBnClickedBtnmin();
	afx_msg void OnBnClickedBtnclose();
	afx_msg LRESULT OnNcHitTest(CPoint point);

	DECLARE_MESSAGE_MAP()

protected:
	void InitCtrls();
	void DrawClient(CDC* lpDC);

private:
	//CAGEdit			m_ctrChannel;
	//CAGEdit			m_ctrUserName;
	//CAGComboBox     m_cmbEncType;
	//CAGButton		m_btnTest;
	CAGButton		m_btnJoin;
	//CAGButton		m_btnSetup;
	CAGButton		m_btnMin;
	CAGButton		m_btnClose;

	//CFont			m_ftEncy;
	CFont			m_ftHead;
	CFont			m_ftDesc;
	//CFont			m_ftBtn;

	CPen            m_penFrame;

	//CDeviceDlg		m_dlgDevice;
};
