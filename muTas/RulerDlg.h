#pragma once

#include "ImChampMapSelectionReceiver.h"

class  KTarget;
class  KMapView;
struct TRulerInfo;
// KRulerDlg 대화 상자입니다.

class KRulerDlg : public KResizeDialogEx, public ImChampMapSelectionReceiver
{
	DECLARE_DYNAMIC(KRulerDlg)

public:
	KRulerDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KRulerDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_3020_RulerDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	void InitCombo(void);
	void InitResizeCtrl(void);

	void InitReportHeader(void);
	void InitReportData(void);
	void AddReportData(Integer a_nxLinkID);

	void GetRulerData();
	void DrawLinkThemes();

	void UpdateInformation(void);

private:
	void MapSelectionFeedback(Integer a_nxID);
	void MapSelectionFeedback(std::vector<Integer> a_vecSelection);

private:
	std::vector<Integer>    m_vecLinkID;
	std::vector<TRulerInfo> m_vecRulerInfo;
	bool m_bIsSingleSelect;

private:
	KTarget*  m_pTarget;
	KMapView* m_pMapView;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonMap();

private:
	CComboBox         m_cboColumn;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
	
public:
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedRadio(UINT nID);
};

typedef std::shared_ptr<KRulerDlg> KRulerDlgPtr;
