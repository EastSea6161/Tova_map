#pragma once

//#include "StdioFileEx.h"
//^#include "ResizeDialogEx.h"

#include "TableImpEmmeNodeSubDlg.h"
#include "TableImpEmmeLinkSubDlg.h"

// KTableImpEmmeNetworkDlg 대화 상자입니다.
class KTarget;

class KTableImpEmmeNetworkDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KTableImpEmmeNetworkDlg)

public:
	KTableImpEmmeNetworkDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTableImpEmmeNetworkDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_4167_TableImpEmmeNetworkDlg };

	typedef std::vector< CString > CSVRow;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void SetImportFile(CString a_strImpFile);
	int  GetStatus(void);
	KIOColumns* m_pVDFColumns;

private:
	void UpdateBtnState(void);

	bool NotifyTableChange(void);

	void InitNodeFixColumn(void);
	void InitLinkFixColumn(void);

	void Text2BinaryNode(void);
	void Text2BinaryLink(void);

	void LoadLayerInfo(KTarget* a_pTarget, ITxMapServerPtr &spMapServer, TxFeatureLayerPtr &spNodeLayer, TxFeatureLayerPtr &spLinkLayer, TxFeatureLayerPtr &spZoneLayer, bool bInitExtent=true);
	void ImportNodeData(TxFeatureLayerPtr spNodeLayer, TxFeatureLayerPtr spZoneLayer);
	void ImportLinkData(TxFeatureLayerPtr spLinkLayer);

	void Run(void);

	void CSV2BinaryNode( int a_nCurrentLine, CSVRow a_row, std::ofstream& ofs, CStdioFileEx& ofErr );
	void CSV2BinaryLink( int a_nCurrentLine, CSVRow a_row, std::ofstream& ofs, CStdioFileEx& ofErr );

	void WriteOutputStream( CString a_strImportValue, KEMIODataType a_emDataType, std::ofstream& ofs );
	void ParseCSVLineString( CString& strLine, CSVRow& row );

	static unsigned __stdcall BinaryWriteThreadCaller(void* p);
	void BinaryWrite(void);

	static unsigned __stdcall ImpEmmeThreadCaller(void* p);
	static void               AddStatusMessage(CString a_strMsg); 

	void RemoveDoubleSpace(CString& strLine);

private:
	KTarget* m_pTarget;
	CString  m_strImpFile;
	int      m_nPage;		//-1 : Select Import File, 0 - Node Define, 1 - Link Define;
	int      m_nStatus;		// 0 = OK, 1 = Cancel, 2 = back
	bool     m_bResult;		//thread 결과
	int      m_nNodeDataCount;
	int      m_nLinkDataCount;
	TCHAR    m_cSeparator;
	int      m_nStartLinkID;

	KIOColumns* m_pNodeColumns;
	KIOColumns* m_pLinkColumns;

	KTableImpEmmeNodeSubDlg m_oNodeSubDlg;
	KTableImpEmmeLinkSubDlg m_oLinkSubDlg;

	std::map<Integer, TxPoint>	m_mapNodeRecord; // Import Node를 통해 수집 한다. (node 테이블은 delete가 되기 때문에)

	std::set<int> m_setFnID;
	CString       m_strErrMsg;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedNext();
	afx_msg void OnBnClickedBack();
	afx_msg void OnBnClickedOk();
	
};
