#pragma once

//^#include "ResizeDialogEx.h"
#include "ImChampMapSelectionReceiver.h"
//#include "StdioFileEx.h"
//^^ #include "IOColumns.h"

class KMode;
class KPurpose;

class KTarget;
class KMapView;
class KProject;

struct TNodeFixedColumn;
struct TLinkFixedColumn;
struct TTurnFixedColumn;

using namespace std;

enum KEMSubODNodeType
{
	KEMSubODNodeOuter = 0,	// 경계 밖 외부 노드
	KEMSubODNodeInner = 1,	// 경계 안 내부 노드
	KEMSubODNodeCross = 2	// 경계 링크와 연결된 외부 노드
};

enum KEMSubODLinkType
{
	KEMSubODLinkFullOut = 0,		// F/T 모두 외부인 링크
	KEMSubODLinkOnlyFNodeOut = 1,	// FNode만  외부인 링크
	KEMSubODLinkOnlyTNodeOut = 2,	// TNode만  외부인 링크
	KEMSubODLinkInner = 3			// F/T 모두 내부인 링크
};

class KSubNetworkDlg : 
	public KResizeDialogEx, public ImChampMapSelectionReceiver
{
	DECLARE_DYNAMIC(KSubNetworkDlg)

public:
	KSubNetworkDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KSubNetworkDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4700_SubNetworkDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	void    ValidateInput(void);
	CString ConvertSavePath(CString a_strFileName);
	void    InitNetworkInfo(void);
	void    CalcNetworkSubFiled(vector<TReceiveValue>& ar_vecReceiveValue);
	void    InitInfoCtrl(void);
	void    UpdateInfoCtrl(void);
	void    InitModeCtrl(void);
	void    UpdateModeCtrl(void);

	ThreadResultInfo RThreadInfo;
	static unsigned __stdcall SubNetworkThreadCaller(void* p);
	static void               ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg);
	static void AddStatusMessage(CString a_strMsg);

	static int CALLBACK BrowseCallBackProc( HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lPData );

	void Run(ThreadPara* pPara);
	bool CreateArgument(void);
	void WriteTextArgument(CStdioFileEx& of);
	void CreateSubNetwork(ThreadPara* pPara);
	void CreateSubGeometry(ThreadPara* pPara);
		void LoadSubNodeID(KDBaseConPtr spDBase, std::vector<Integer> &a_vecNodeID);
		void LoadSubLinkID(KDBaseConPtr spDBase, std::vector<Integer> &a_vecLinkID);
		void LoadSubZoneID(KDBaseConPtr spDBase, std::vector<Integer> &a_vecZoneID);
		void LoadLayerInfo(KTarget* a_pTarget, OUT ITxMapServerPtr &spMapServer, TxFeatureLayerPtr &spNodeLayer, TxFeatureLayerPtr &spLinkLayer, TxFeatureLayerPtr &spZoneLayer, bool bInitExtent=true);

private:
	KProject* m_pSubProject;
	KTarget*  m_pTarget;
	KMapView* m_pMapView;
	
	KIOColumns m_oUserColumnsLink;
	
	set<__int64>        m_mapZoneNode;
	map<__int64, int>   m_mapNodeSubField;
	map<__int64, int>   m_mapLinkSubField;

	map<int, CString>   m_mapModeInfo;

	int     m_nRunOption;
	int     m_nZoneThrough;
	int     m_nResultCode;	    //kluncher Result Code;
	
	CString m_strPrjName;
	CString m_strLocation;

	vector<KMode*>    m_vecMode;
	vector<KPurpose*> m_vecPurpose;
	
	map<Integer, TNodeFixedColumn> m_mapNodeFixedInfo;
	map<Integer, TLinkFixedColumn> m_mapLinkFixedInfo;
	map<Integer, TTurnFixedColumn> m_mapTurnFixedInfo;

	bool m_bSelectSubArea;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonPolygon();
	afx_msg void OnClickedPrjbtnLocation();

	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheck1();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);

private:
	void MapSelectionFeedback(Integer a_nxNodeID){};
	//void MapSelectionNoneFeedback(){};
	void MapAnalysisFeedback(vector<TReceiveValue> a_vecReceiveValue);
	
private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrlInfo;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrlMode;
};

typedef std::shared_ptr<KSubNetworkDlg> KSubNetworkDlgPtr;