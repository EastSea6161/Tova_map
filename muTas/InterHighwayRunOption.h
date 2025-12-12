#pragma once

#include "DBaseAssignment.h"

class KTarget;

// KInterHighwayRunOption 대화 상자입니다.

class KInterHighwayRunOption : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KInterHighwayRunOption)

public:
	KInterHighwayRunOption(KTarget* a_pTarget, int& a_nChkTurn, TInterRunOptionInfo& a_oRunOption, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterHighwayRunOption();

	enum { IDD = IDD_6872_Inter_HighwayRunOption };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

protected:
	KTarget* m_pTarget;
	int& m_nChkTurn;
	TInterRunOptionInfo& m_oRunOption;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndModeReport;

protected:
	void InitReportHeader(void);
	void UpdateReportData(std::map<int, KInterOccupancePCEMode> a_mapInterFOPMode);

	void SaveFInitialVolume(void);
	void ResizeComponet(void);

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnReportCheckItemMode(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedButtonDef();
	afx_msg void OnBnClickedCheckFreight();
public:
	afx_msg void OnBnClickedCheckRegionBus();
};
