#pragma once
#include "ImChampMapSelectionReceiver.h"

class KTarget;
class KMapView;
class TDrawNodeInfo; 

struct TCodeZoneID
{
	std::set<Integer> setZoneID;
	CString strZoneIDs;
};

struct TCodeNodePolygon
{
	std::vector<TCoordinate> vecCoordinate;
	std::vector<TDrawNodeInfo> vecDrawNodeInfo;
};

// KZoneODGroupGenerationDlg 대화 상자입니다.

class KZoneODGroupGenerationDlg : public KResizeDialogEx, public ImTasTargetRegionReceiver, public KTableObserver
{
	DECLARE_DYNAMIC(KZoneODGroupGenerationDlg)

public:
	KZoneODGroupGenerationDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KZoneODGroupGenerationDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_2198_ZONEGROUPFORODGROUP_GEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	KTarget*     m_pTarget;
	KMapView*    m_pMapView;
	int          m_nRunResult;

	std::map<Integer, int> m_mapZoneIdCode;
	CXTPReportRecord*      m_pSelectRecord;
	std::set<Integer>      m_setZoneID;
	std::map<int, TCodeNodePolygon> m_mapCodePolygonInfo;

public:
	void         ActivatedFrameWndNotifyProcess();
	void         DeActivatedFrameWndNotifyProcess();
	void         InitializeStatus();

protected:
	void         InitReportHeader();
	void         LoadCodeGroup();
	void         LoadReportData();
	void         LoadOutColumnName();
	void         ResizeComponent();

	void         ReleasePreZoneID  (CString &a_strZoneIDs);
	void         ConvertCodeZoneSet(std::map<Integer, int> &a_mapZoneIdCode, std::map<int, TCodeZoneID> &a_mapCodeZoneIdSet);
	void         UpdateReportData  (std::map<int, TCodeZoneID> &a_mapCodeZoneIdSet);
	void         UpdateReport      (CXTPReportRecord* a_pSelectedRecord, std::set<Integer> &a_setSelectedZone);
	void         TempCodePolygonInfo(CXTPReportRecord* a_pSelectedRecord, std::vector<TCoordinate> &a_vecCoordinate, std::vector<TDrawNodeInfo> &a_vecDrawNodeInfo);

private:
	void         TargetRegionFeedback(std::vector<Integer> a_vecSelection, std::vector<TCoordinate> a_vecCoordinate);
	void         ClearAllMap( void );
	bool         InputCheck();
	KIOColumn*   AddZoneCodeColumn( int a_nCodeGroup, CString a_strColumnName );
	void         UpdateZoneTable( KIOColumn* a_pColumn, std::map<Integer, int>& ar_mapValue );
	void         UpdateResultTable(KIOColumn* a_pColumn, int a_nGroupCode);

	static unsigned __stdcall ThreadCaller(void* p); 
	void         Run();

private:
	virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);

private:
	CComboBox    m_cboGroupName;
	CComboBox    m_cboOutColumnName;

	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReport;    

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnItemButtonClick (NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedRadio(UINT nID);
	        void ControlRadio();
	afx_msg void OnCbnSelchangeCboGroupName();
	afx_msg void OnBnClickedButtonModify();      
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};

typedef std::shared_ptr<KZoneODGroupGenerationDlg> KZoneODGroupGenerationDlgPtr;