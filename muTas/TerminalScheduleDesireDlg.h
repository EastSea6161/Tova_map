#pragma once
#include "afxwin.h"
//^#include "ResizeDialogEx.h"
#include "ShortestPathBase.h"
#include "ImChampIDFind.h"
#include "TerminalScheduleDesire.h"
#include "TUserGeometryInfo.h"
#include "TDrawNodeInfo.h"
#include "PathVolumeInfo.h"

class KTarget;
class KMapView;
// KTerminalScheduleDesireDlg 대화 상자입니다.

class KTerminalScheduleDesireDlg : public KResizeDialogEx, public KShortestPathBase, public ImChampModelessFeedback
{
	DECLARE_DYNAMIC(KTerminalScheduleDesireDlg)

public:
	KTerminalScheduleDesireDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTerminalScheduleDesireDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6880_TerminalScheduleDesire };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void         Initialize();
	void         ActivatedFrameWndNotifyProcess();
	void         DeActivatedFrameWndNotifyProcess();

protected:
	void         LoadRegTransitType(std::map<int, CString> &a_mapRegTransitType);
	void         InitComboMode     (std::map<int, CString> &a_mapRegTransitType);
	bool         GetOriginTerminalInfo   (void);
	void         GetPassengerNumField(void);

protected:
	void         InitReportHeader();
	void         ResizeComponent();

	void         SetNodeViewData(CString a_strZoneID, CString &a_strNodeViewData, KIDCaption &a_oIDCaption);
	void         ConvertEmptyNodeName(CString &a_strNodeName);
	void         ClickedEditStartNode();
	void         SelectStartNodeList();
	Integer      GetSelStartNodeId();

private:
	virtual void ModelessOKFeedback(UINT nDialogID);
	virtual void ModelessCancleFeedback(UINT nDialogID);
	void         GetDesTerminalList();
	void         UpdateReportData(std::vector<TDesTerminalInfo> &a_vecDesTerminalInfo);

protected:
	KTarget*     m_pTarget;
	KMapView*    m_pMapView;
	CString      m_strTripFieldName;

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonStartNodeSearch();
	afx_msg void OnCbnSelchangeComboMode();
	afx_msg void OnBnClickedCheckDestinationAll();
	afx_msg void OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	void         UpdateMinMax();
	afx_msg void OnBnClickedButtonDraw();
			void DoneDraw(bool a_bShowErrMsg);
	bool         UserDrawInputCheck(bool a_bShowErrMsg);
	bool         IsCheckedDesTerminal();
	void         GetDrawVolumeInfo(TPathVolumeSettingInfo &a_oSettingInfo, std::vector<TUserLineInfo> &a_vecDrawUserLineInfo, std::vector<TDrawNodeInfo> &a_vecDrawNodeInfo);
	void         Node2XYCoordinate(KTarget* pTarget, KODKey oODKey, std::vector<TMapPoint>& a_vecMapPoint );
	void         CalculateValue2Width(double a_dTrip, TPathVolumeSettingInfo &a_oSettingInfo, double& dWidth );
	afx_msg void OnBnClickedCheckLabel();
	afx_msg void OnBnClickedButtonClear();
	void         ClearMapDraw(bool a_bRefresh);
	afx_msg void OnBnClickedCancel();

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
	CComboBox           m_cboMode;
	KIDCaption          m_oStartNodeIdCaption;
	CXTPColorPicker     m_cpLineColor;
	COLORREF            m_clrLine;

	KImChampIDFindPtr     m_spImTasIDFind;
};

typedef std::shared_ptr<KTerminalScheduleDesireDlg> KTerminalScheduleDesireDlgPtr;