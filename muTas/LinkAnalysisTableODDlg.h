#pragma once
#include "LinkAnalysisInfo.h"
//^#include "ResizeDialogEx.h"

// KLinkAnalysisTableODDlg 대화 상자입니다.

class KLinkAnalysisTableODDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KLinkAnalysisTableODDlg)

public:
	KLinkAnalysisTableODDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KLinkAnalysisTableODDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5090_LinkAnalysisTableODDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	std::vector<TLinkODVolume> m_vecLinkODVolume;

protected:
	void InitReportHeader();
public:
	void InitReportData();
	void ClearReportData();

public:
	virtual BOOL OnInitDialog();

	void SetLinkODVolume(std::vector<TLinkODVolume> a_vecLinkODVolume)
	{
		m_vecLinkODVolume = a_vecLinkODVolume;
	}

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
};
