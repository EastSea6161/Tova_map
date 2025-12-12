#pragma once
#include "resource.h"
#include "afxwin.h"
#include "CustomReportRecordCtrl.h"
#include "DBaseImpExpShapeFile.h"

// KDlgShapeDemarcation 대화 상자입니다.

class ShapeFileReader;
class KTarget;

class QBicTxtWriter;

class KDlgShapeDemarcation : public KWhiteDlgEx
{
	DECLARE_DYNAMIC(KDlgShapeDemarcation)

public:
	KDlgShapeDemarcation(KTarget* a_pTarget, KIOTable* a_pTable, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDlgShapeDemarcation();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_201_04_01_IMPORT_SHAPE_DEMARCATION };

	struct TDBLayerInfo
	{
		Integer nxTxID;
		double  dAvgExtent;
		double  dMinX;
		double  dMinY;
		double  dMaxX;
		double  dMaxY;

		int     nVersion;
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	KIOTable*    m_pTable;
	KTarget*     m_pTarget;

private:
	int          m_nGeoType;

	std::map<long, CString>           m_mapShapeField;
	std::vector<KImportShapeMatching> m_vecShapeMatching;
	std::vector<KImportShapeMatching> m_vecShapeMatching_Normal;
	std::vector<KImportShapeMatching> m_vecShapeMatching_PK;

private:
	int          m_nShapeIndexOfID;
	std::map<Integer, TDBLayerInfo> m_mapPointShapeData;

	CString      m_strKeyFieldName;

	CString      m_strErrFile;

private:
	void         SetResizeComponent();

	void         InitControl();
	void         InitReportHeaderDefine();
	void         InitReportHeaderDrag();

	void         UpdateReportDataDefine();
	void         UpdateReportDataDrag();
	void         UpdateReportDataPreview();

	bool         LoadPreViewShapeField();
	void         OpenShapeFile(CString a_strShapeFile, ShapeFileReader &a_oShapeFile);

private:
	ThreadResultInfo RThreadInfo;
	static unsigned __stdcall ThreadRun(void* p); 
	void Execute(ThreadPara* pPara);

private:
	CXTPOfficeBorder<KCustomReportRecordCtrl,false> m_ctrlReportDrag;
	CXTPOfficeBorder<CXTPReportControl,false>       m_ctrlReportDefine;
	CXTPOfficeBorder<CXTPReportControl,false>       m_ctrlReportPreview;

	CXTBrowseEdit m_editShapeFile;
	CComboBox     m_cboEncoding;

private:
	virtual BOOL OnInitDialog();

	afx_msg void OnEnChangeEditBrowse();
	void RefreshInfo();
	afx_msg void OnReportRecordsDropped(NMHDR *pNotifyStruct, LRESULT *pResult);
	afx_msg void OnReportRecordsDroping(NMHDR *pNotifyStruct, LRESULT *pResult);

	afx_msg void OnBnClickedButtonRun();
	afx_msg void OnCbnSelchangeCombo4();

	bool         InvalidateInputInfo();
	bool         InValidColumnData();

	bool         GetShapeIndexOfID(OUT int &a_nShapeIndex_ID, CString &a_strKeyFieldName);
	void         UpdateShapeFileData(ShapeFileReader &a_oShapeFile, const bool a_bErrFile, QBicTxtWriter &ofErr);
	void         LoadShapeData(KDBaseConPtr a_spDBConnection, LPCTSTR lpszLayerTable, std::map<Integer, TDBLayerInfo> &a_mapShapeData);
	void         AddStatusMessage( CString a_strMsg );
	void         ErrorFileOpen();

	void         LoadLayerInfo(KTarget* a_pTarget, ITxMapServerPtr &spMapServer, TxFeatureLayerPtr &spZoneLayer, bool bInitExtent=true);

public:
	static bool SpatialCalculateZoneID(CWnd* a_pParent, KTarget* a_pTarget);    
	static void PolygonToPoint( ITxFeatureLayerPtr a_spPolygonFeatureLayer, ITxFeatureLayerPtr a_spPointFeatureLayer, 
								OUT std::map<Integer, std::vector<Integer>> &ar_mapMappingInfo );
private:
	int          GetSelectedEncoding();
	void         InitComboEncoding();

};
