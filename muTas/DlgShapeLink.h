#pragma once
#include "CustomReportRecordCtrl.h"
#include "DBaseImpExpShapeFile.h"
#include "afxwin.h"
//#include "NetworkShapeInsert.h"

class KTarget;

class ShapeFileReader;
class QBicTxtWriter;

// KDlgShapeLink 대화 상자입니다.

class KDlgShapeLink : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KDlgShapeLink)

public:
	KDlgShapeLink(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDlgShapeLink();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_4200_LinkShapeImportDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget*	m_pTarget;
	KIOTable*	m_pTable;

	int          m_nGeoType;

	std::map<long, CString> m_mapShapeField;

	Integer m_nxExistCount;
	Integer m_nxNewCount;

	std::map<Integer, TxPoint>	m_mapNodeRecord;
	const double m_dOneWayOffset;

	CString      m_strErrFile;
    std::map<int, int> m_mapReadLinkType; //Link Type 별 개수

protected:
	CXTBrowseEdit		m_editShapeFile;
	CComboBox			m_cboOneField;
	CComboBox           m_cboEncoding;

	CXTPOfficeBorder<CXTPReportControl,false>		m_wndReportForward;
	CXTPOfficeBorder<CXTPReportControl,false>		m_wndReportReverse;
	CXTPOfficeBorder<KCustomReportRecordCtrl,false> m_wndReportDrag;
	CXTPOfficeBorder<CXTPReportControl,false>		m_wndReportPreview;

protected:
	std::vector<KImportShapeMatching> m_vecForShapeMatting;
	std::vector<KImportShapeMatching> m_vecRevShapeMatting;

private:
    TxFeatureLayerPtr m_spFeatureLayer;

protected:
	void ResizeComponent();

protected:
	bool LoadPreViewShapeField();
	void OpenShapeFile(CString a_strShapeFile, ShapeFileReader &a_oShapeFile);

private:
	ThreadResultInfo RThreadInfo;
	static unsigned __stdcall ThreadRun(void* p); 
	void Execute(ThreadPara* pPara);

	void IniteOneWayCombo();

	void IniteReportHeaderForward();
	void IniteReportHeaderReverse();
	void IniteReportHeaderDrag();
	void IniteReportHeaderPreView();

	void UpdateReportForward();
	void UpdateReportReverse();
	void UpdateReportDrag();
	void UpdateReportPreview();

protected:
	bool InValidForColumnData();
	bool InValidRevColumnData();
	bool CheckForwardReverseField();

	void SaveForwardFeatureData(ThreadPara* pPara, ShapeFileReader &a_oShapeFile, const bool a_bErrFile, QBicTxtWriter &ofErr, int &a_nInsertCnt, std::set<int> &a_setFwrFailedEntity);
	void SaveReverseFeatureData(ThreadPara* pPara, ShapeFileReader &a_oShapeFile, const bool a_bErrFile, QBicTxtWriter &ofErr, int &a_nInsertCnt, std::set<int> &a_setRvrFailedEntity);
	bool IsFlipVertex(Integer a_nxLinkID, Integer a_nxFNodeID, TxPolyline &a_oPolyLine);

	void AddStatusMessage( CString a_strMsg );

	void         LoadLayerInfo(KTarget* a_pTarget, ITxMapServerPtr &spMapServer, TxFeatureLayerPtr &spFeatureLayer, bool bInitExtent=true);

private:
	int          GetSelectedEncoding();
	void         InitComboEncoding();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioType( UINT nID );
	afx_msg void OnEnChangeEditBrowse();
		void RefreshInfo();
	afx_msg void OnBnClickedCheckId();
	afx_msg void OnReportRecordsDroppedFor(NMHDR *pNotifyStruct, LRESULT *pResult);
	afx_msg void OnReportRecordsDroppedRev(NMHDR *pNotifyStruct, LRESULT *pResult);
	afx_msg void OnReportRecordsDroping(NMHDR *pNotifyStruct, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
		bool InvalidateInputInfo();
		void ErrorFileOpen();
        void ImportShapeFileData(ThreadPara* pPara, ShapeFileReader &a_oShapeFile, const bool a_bErrFile, QBicTxtWriter &ofErr);
        afx_msg void OnCbnSelchangeCombo4();

	afx_msg void OnBnClickedCancel();
};
