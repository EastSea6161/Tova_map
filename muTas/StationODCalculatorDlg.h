#pragma once
#include "ImChampMultiIDFind.h"
#include "BulkDBaseTransit.h"
#include "MultiModalResultInfo.h"

class KTarget;
class KMapView;
class KStationVectorVRecord;
class KStationMatrixVRecord;

// KStationODCalculatorDlg 대화 상자입니다.

class KStationODCalculatorDlg : public KResizeDialogEx, public ImChampModelessFeedback
{
	DECLARE_DYNAMIC(KStationODCalculatorDlg)

public:
	KStationODCalculatorDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KStationODCalculatorDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_5087_StationODCalculatorDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	void SetViewTransit(Integer a_nxTransitID);
	void ActivatedFrameWndNotifyProcess();
	void DeActivatedFrameWndNotifyProcess();
	void ResizeComponent(void);

private:
    std::set<__int64> m_setTransit;
protected:
	int			m_nReadOutResult;

	KTarget*	m_pTarget;
	KMapView*	m_pMapView;

	KImChampMultiIDFindPtr m_spImTasMultiIDFind;

	/*Integer m_nxTransitID;*/

	std::vector<KIDCaption>				m_vecLineIDCaption;
	std::map<Integer, TTransitModeName> m_mapLineData;

    //★ 전체 존 ID
	std::set<Integer> m_setAllZoneID;
    
    //★ 정류장 OD
	std::map<KODKey, double> m_mapStnOD;

    //★ 정류장 정보
    std::map<__int64, std::map<int, TStationODInfo>> m_mapRouteNodeSeq;

private:
	void VectorReport(void);
	void MatrixReport(void);

	void GetStatisticsSum_Oi(KSDoubleRecord& a_oRecordSum_Oi);
	void GetStatisticsSum_Di(KSDoubleRecord& a_oRecordSum_Oi);

private:
	KSDoubleRecordArray*   m_pRecordArray;
	KStationVectorVRecord* m_pVectorRecord;
	KStationMatrixVRecord* m_pMatrixRecord;

	std::vector<__int64> m_vecOrigin;
	std::vector<__int64> m_vecDest;
protected:
	virtual void ModelessOKFeedback(UINT nDialogID);
	virtual void ModelessCancleFeedback(UINT nDialogID);

protected:
	void SetTransitIDCaption();

protected:
	bool ReadOutputFile();
	bool ReadBusPathFile(CString strFileName);
	bool ReadRailPathFile(CString strFileName);
	bool ReadBusRailPathFile(CString strFileName);

protected:
	static unsigned __stdcall SearchStationThreadCaller(void* pParam);
	static void AddStatusMessage(CString a_strMsg);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonFind();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedRadioType( UINT nID );

private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;

private:
    afx_msg void OnBnClickedButton3();

private:
    CString m_strFile;         //파일경로
    CString m_strErrMsg;       // Export Error Msg
    bool    m_bExportThreadOK; // Export 성공 여부
    bool    m_bEnableExport;
private:
    static unsigned __stdcall ExportDataThreadCaller(void* pParam);
    void ExportData();
public:
	afx_msg void OnBnClickedCancel();
	void CloseWindowProc();
};
typedef std::shared_ptr<KStationODCalculatorDlg> KStationODCalculatorDlgPtr;