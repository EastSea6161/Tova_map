#pragma once

//^#include "ResizeDialogEx.h"
#include "ImChampMultiIDFind.h"
#include "DBaseInterModal.h"
#include "TDrawNodeInfo.h"

class KMapView;
class KTarget;

// KGlobalTermialSettingDlg 대화 상자입니다.

class KGlobalTerminalSettingDlg : public KResizeDialogEx, public ImChampModelessFeedback, public KTableObserver
{
	DECLARE_DYNAMIC(KGlobalTerminalSettingDlg)

public:
	KGlobalTerminalSettingDlg(KTarget* a_pTargt, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KGlobalTerminalSettingDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_6815_GlobalTerminalSettingDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	KTarget*			m_pTarget;
	KMapView*			m_pMapView;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;

	CXTPReportColumn *m_pColumn;
	CXTPReportRecordItem* m_pItem;

	std::map<Integer, TInterTerminalNodeInfo> m_mapAllNodeTypeInfo;

	std::vector<KIDCaption> m_vecKIDTransitComplex;
	std::vector<KIDCaption> m_vecKIDInterRegionRail;
	std::vector<KIDCaption> m_vecKIDBusTerminal;
	std::vector<KIDCaption> m_vecKIDAirTerminal;
	std::vector<KIDCaption> m_vecKIDMarinPort;

	std::vector<Integer> m_vecTransitComplexID;
	std::vector<Integer> m_vecInterRegionRailID;
	std::vector<Integer> m_vecBusTerminalID;
	std::vector<Integer> m_vecAirTerminalID;
	std::vector<Integer> m_vecMarinPortID;
	std::map<Integer,CString> m_mapNodeName;

protected:
	CXTPColorPicker     m_cpTransitComplexColor;
	CXTPColorPicker     m_cpInterRailColor;
	CXTPColorPicker     m_cpBusTerminalColor;
	CXTPColorPicker     m_cpAirTerminalColor;
	CXTPColorPicker     m_cpMarinPortColor;

	COLORREF            m_clrTransitComplex;
	COLORREF            m_clrInterRailColor;
	COLORREF            m_clrBusTerminalColor;
	COLORREF            m_clrAirTerminal;
	COLORREF            m_clrMarinPort;

public:
	void         ActivatedFrameWndNotifyProcess();
	void         DeActivatedFrameWndNotifyProcess();

protected:

	void ResizeComponent();

	void InitReportHeader(void);
	void UpdateReportData(void);
	void MathingCode(void);
	void ClickedButton(std::vector<Integer> a_vecID, std::vector<KIDCaption> a_vecKIDCaption, CString a_strSelect);
	void ResultUpdateValue(CString a_strCaption, std::vector<Integer>& a_vecTerminalID);

	void ClearReportCheck(void);
	void DrawZoneTerminal();
	void DrawZoneDemacation(int a_nTRCode);
	void DrawTermianlNode(std::vector<Integer> a_vecTerminalID, COLORREF a_clrTerminal, std::vector<TDrawNodeInfo>& ar_vecDrawNodeInfo);

	void ClearAllMap();

private:
    KImChampMultiIDFindPtr m_spImTasMultiIDFind;
private:
    virtual void ModelessOKFeedback(UINT nDialogID);
	virtual void ModelessCancleFeedback(UINT nDialogID);
private:
	virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg LRESULT OnExpressionEdit(WPARAM a_wParam, LPARAM a_lParam);
	afx_msg void OnItemButtonClick(NMHDR * a_pNotifyStruct, LRESULT* a_pResult);
	afx_msg void OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckTc();
	afx_msg void OnBnClickedCheckIrrs();
	afx_msg void OnBnClickedCheckBt();
	afx_msg void OnBnClickedCheckAt();
	afx_msg void OnBnClickedCheckMp();
	afx_msg void OnBnClickedButtonTr();
	afx_msg void OnBnClickedButtonIrrs();
	afx_msg void OnBnClickedButtonBt();
	afx_msg void OnBnClickedButtonAt();
	afx_msg void OnBnClickedButtonMp();
	afx_msg void OnBnClickedCheckLabel();

	afx_msg void OnSelEndOkTR();
	afx_msg void OnSelEndOkIRRS();
	afx_msg void OnSelEndOkBT();
	afx_msg void OnSelEndOkAT();
	afx_msg void OnSelEndOkMP();
};
typedef std::shared_ptr<KGlobalTerminalSettingDlg> KGlobalTerminalSettingPtr;