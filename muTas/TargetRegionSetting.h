#pragma once
//^#include "ResizeDialogEx.h"
#include "DBaseInterModal.h"
#include "ImChampMapSelectionReceiver.h"


class KTarget;
class KMapView;
class TCoordinate;

// KTargetRegionSetting 대화 상자입니다.

class KTargetRegionSetting : public KResizeDialogEx, public ImChampMapSelectionReceiver, public ImTasTargetRegionReceiver
{
	DECLARE_DYNAMIC(KTargetRegionSetting)

public:
	KTargetRegionSetting(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTargetRegionSetting();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6810_TargetRegionSetting };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	int m_nSelectTRCode;

	KTarget* m_pTarget;
	KMapView* m_pMapView;

	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrlTRegion;

	std::map<int, CString> m_mapTargetRegionCode;

	std::map<int, TTargetRegionInfo> m_mapTargetRegionInfo;

	std::map<Integer, TTargetRegionNode> m_mapNodeTargetInfo;
	std::map<int, int> m_mapRegionCount;

private:
    //std::vector<TCoordinate> m_vecTargetRegionCoordinate;

protected:
	void InitAddEditEnableFalseButton(void);

	void InitAddEditEnalbeTrueButton(void);

	void GetTargetRegionCode(void);

	void InitReportHeaderTR(void);

	void UpdateReportTR(void);

	void GetMaxCode(int& a_nMaxCode);

	void ClearReportTR(void);

	void DrawTargetZoneNode(bool a_bRefresh);

	void DrawExcludeNode(bool a_bRefresh);

	void SelectZoneNodeSymbol(bool a_bRefresh);

	void AddInZoneFromMap(std::vector<Integer> a_vecNodeID);

	void ClearAllMap(bool a_bRefresh);

	void UpdateTargetRegionCode(void);

	void UpdateSelectedNodeCount(void);

	void UpdatePolygonVertexInfo(void);

	void ResizeComponent(void);

	void Run(void);

	void GetRegionNodeCount(void);

	void DeleteRegionCodebyMemory(int& a_nCode);

private:
	void MapSelectionFeedback(std::vector<Integer> a_vecSelection);
	void MapSelectionFeedback(Integer a_nxNodeID);
	void MapSelectionNoneFeedback();
    void TargetRegionFeedback(std::vector<Integer> a_vecSelection, std::vector<TCoordinate> a_vecCoordinate);

public:
	static unsigned __stdcall TargetRegionThreadCaller(void* p);
	static void AddStatusMessage(CString a_strMsg);

private:
	virtual BOOL OnInitDialog();

	afx_msg void OnRadioClicked(UINT nID);
	
	afx_msg void OnBnClickedButtonAddRegion();
	afx_msg void OnBnClickedButtonDelRegion();
	
	afx_msg void OnReportCheckItemTR(NMHDR* pNotifyStruct, LRESULT* pResult);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	afx_msg void OnBnClickedButtonPoly();
	afx_msg void OnBnClickedButtonPolyEdit();
	afx_msg void OnBnClickedButtonAreacode();
private:
    std::map<int, std::set<__int64>> m_mapTransitLine;
    std::map<int, std::set<__int64>> m_mapTransitStn;
private:
    void LoadInitData();
private:
    void DrawStnRadius(bool bRefresh);
public:
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedCheck1();
    afx_msg void OnBnClickedCheck2();
    afx_msg void OnBnClickedCheck3();
};

typedef std::shared_ptr<KTargetRegionSetting> KTargetRegionSettingPtr;