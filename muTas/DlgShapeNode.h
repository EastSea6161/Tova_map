#pragma once
#include "resource.h"
#include "afxwin.h"
#include "CustomReportRecordCtrl.h"
#include "DBaseImpExpShapeFile.h"

// KDlgShapeNode 대화 상자입니다.

class ShapeFileReader;
class KTarget;

class QBicTxtWriter;

class KDlgShapeNode : public KWhiteDlgEx
{
	DECLARE_DYNAMIC(KDlgShapeNode)

public:
	KDlgShapeNode(KTarget* a_pTarget, KIOTable* a_pTable, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDlgShapeNode();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_201_04_02_IMPORT_SHAPE_NODE };

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

private:
	int          m_nShapeIndexOfID;
	std::map<Integer, TDBLayerInfo> m_mapPointShapeData;

	CString      m_strKeyFieldName;

	CString      m_strErrFile;
    std::map<int, int> m_mapReadNodeType; //Node Type 별 개수

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
    TxFeatureLayerPtr m_spFeatureLayer;
    TxFeatureLayerPtr m_spZoneLayer;
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
	void         ImportShapeFileData(ThreadPara* pPara, ShapeFileReader &a_oShapeFile, const bool a_bErrFile, QBicTxtWriter &ofErr);
	void         LoadShapeData(KDBaseConPtr a_spDBConnection, LPCTSTR lpszLayerTable, std::map<Integer, TDBLayerInfo> &a_mapShapeData);
	void         AddStatusMessage( CString a_strMsg );
	void         ErrorFileOpen();

	void         LoadLayerInfo(KTarget* a_pTarget, ITxMapServerPtr &spMapServer, TxFeatureLayerPtr &spFeatureLayer, TxFeatureLayerPtr &spZoneLayer, bool bInitExtent=true);

private:
	int          GetSelectedEncoding();
	void         InitComboEncoding();
	
};
