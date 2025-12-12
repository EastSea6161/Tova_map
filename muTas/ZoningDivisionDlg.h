#pragma once


// KZoningDivisionDlg 대화 상자입니다.
#include "ZoneDivisionInfo.h"
#include "IMapFeedback.h"
#include <fstream>
#include <iostream>
//#include "StdioFileEx.h"
//^^ #include "IOColumns.h"
//^#include "ResizeDialogEx.h"
#include "ImChampMultiIDFind.h"
//^^ #include "Observer.h"
#include "ShortestPathBase.h"
#include "afxwin.h"

class KTarget;
class KMapView;
struct TLinkFTNodeID;


enum KEMInputError
{
    KEMInputErrorMap,
    KEMInputErrorRate,
    KEMInputOK
};


class KZoningDivisionDlg : public KDialogEx, public KShortestPathBase, public IMapZoning, public ImChampModelessFeedback, public KTableObserver
{
	DECLARE_DYNAMIC(KZoningDivisionDlg)

public:
	KZoningDivisionDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KZoningDivisionDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4410_ZoningDivisionDlg };
	
protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
    KTarget*  m_pTarget;
	KTarget*  m_pRunningTarget;	// 여러 시나리오 처리 기능이 추가되면서, 현재 처리중인 Target을 저장, 실행 전 running target을 먼저 설정 해 줘야 한다.
    KMapView* m_pMapView;
    CXTPOfficeBorder<CXTPReportControl,false> m_wndOriginZoneReportCtrl;
    CXTPOfficeBorder<CXTPReportControl,false> m_wndDivisionZoneReportCtrl;
	CImageList          m_imageListSelectIcon;

private:
    std::map<Integer, KZoningOriginZoneInfoPtr> m_mapZoneDivisionInfo;
	std::set<Integer> m_setDeleteLinkID;
	Integer           m_nxSelectOrgZoneID;
	KImChampMultiIDFindPtr m_spImTasMultiIDFind;


protected:
	afx_msg void OnReportItemClick         (NMHDR*  pNMHDR, LRESULT* pResult);
    afx_msg void OnOriginSelectChanged     (NMHDR*  pNMHDR, LRESULT* pResult);
    afx_msg void OnOriginValueChanged      (NMHDR*  pNMHDR, LRESULT* pResult);
    afx_msg void OnDivisionValueChanged    (NMHDR*  pNMHDR, LRESULT* pResult);
    afx_msg void OnDivisionItemButtonClick (NMHDR * pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedSelectOriginZone();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
			void SelectNodeList();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

public:
    void SetTarget(KTarget* a_pTarget)
    {
        m_pTarget = a_pTarget;
    }
    void SetMapView(KMapView* a_pMapView)
    {
        m_pMapView = a_pMapView;
    }

private:
    void DisplayOriginalZone();
	void UpdateDivisionZone(CXTPReportRecord* a_pRecord);
    void DisplayDivisionZone(Integer a_nxOriginalZoneID);
    void UpdateNote();
private:
    void DrawZoneAccess(Integer &a_nxZoneID, bool a_bRefresh);
    std::vector<__int64> ZoneNode(__int64 nxZoneID);
private:
	CXTPReportRecord*  GetSelectedOriginRecord();
    KEMInputError IsInputDone(KZoningOriginZoneInfoPtr a_spZoningOriginZoneInfoPtr);

public:
    void ReceivedZoneInfoFromMap(TxPointPtr a_spNodeGeometry, TxPolygonPtr a_spDemarcationGeometry);

	static unsigned __stdcall DivisionZoneThreadCaller(void* pParam);
	static void AddStatusMessage(CString a_strMsg);
	static void ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg);
	bool        RunDivisionGeneration();
	
	static bool m_bRunDivisionSucceess;
	int         m_nResultCode;
private:
	void DrawAddedZone(const std::vector<KZoneDivisionInfoPtr> &vecZoneDivisionInfo, bool a_bRefresh);

protected:    
    virtual BOOL OnInitDialog();
protected:
    afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnBnClickedBtnMultiScenario();

private:
	void    InitReportHeader();
    bool    InputCheck       (CString& ar_strError);
    void    GenerateNewZoneID();
    void    GenerateDivisionInfo();

	/**************************************************************************/
	/* 2019.01.15 수정 : 여러 시나리오 처리기능 추가에 따른 변경                     */
	//void    SaveGeometry();
	void    SaveGeometry(KMapView* pMapView);
	void	SaveGeometry(KDBaseConPtr a_spDBaseConnection);
	/**	
	/*  생성된 zone connect link 결과 파일을 읽어서 DB에 저장하고,
	/*	Geometry 생성을 위해 std::vector<TLinkFTNodeID>의 참조에 linkId, fromNodeId, toNodeId 정보를 저장
	**/
	void	InsertZoneConnectLink(KDBaseConPtr a_spDBaseConnection, std::vector<TLinkFTNodeID>& arrZoneConnectLink);

	/**
	/*	arrZoneConnectLink에 저장된 링크정보로 Link Geometry를 생성하여 저장
	**/
	void	SaveZoneConnectLinkGeometry(std::vector<TLinkFTNodeID>& arrZoneConnectLink, KMapView* pMapView);

	/**
	/*	arrZoneConnectLink에 저장된 링크정보로 Link Geometry를 생성하여 저장
	**/
	void	SaveZoneConnectLinkGeometry(std::vector<TLinkFTNodeID>& arrZoneConnectLink, KDBaseConPtr a_spDBaseConnection);
	/**************************************************************************/
	void    SaveGeometryCache();
	bool    ReadOutPutFile();
	bool    CreateArgument();
	bool    ClearFiles();
	bool    ArgumentWriteBase    (CStdioFileEx& of);
	bool    ArgumentWriteDivision(CStdioFileEx& of);


	int     InitZoneCount();
	int     GetDoubleTypeColumnCount(CString a_strTableName);
	void    GetColumnInfo        (CString a_strTableName, KIOColumns &a_oColumns);
	void    UpsertZoneData       (KDBaseConPtr a_spDBaseConnection);
	void    UpsertSedData        (KDBaseConPtr a_spDBaseConnection);
	void    UpsertNodeSystemData (KDBaseConPtr a_spDBaseConnection);
	void    DeleteLinkData       (KDBaseConPtr a_spDBaseConnection);
	void    DeleteTransitData    (KDBaseConPtr a_spDBaseConnection, std::set<Integer> &a_setDeleteLinkID);
	void    UpsertPurposeODData  (KDBaseConPtr a_spDBaseConnection);
	void    UpsertModeODData     (KDBaseConPtr a_spDBaseConnection);
	void    UpsertParameterODData(KDBaseConPtr a_spDBaseConnection);
	void    DeleteAccessData(KDBaseConPtr a_spDBaseConnection);

	bool    UpdateDivisionSystemInfo();
	void    UpdateZoneSystemData (KDBaseConPtr a_spDBaseConnection);
	void    UpdateSedSystemData  (KDBaseConPtr a_spDBaseConnection);
	void    NotifyRelationTable();
	void    NotifyZoneRelation();
	void    NotifyLinkRelation();
	void    ClearDrawInfo(bool a_bRefresh);
	CString ConvertSavePath( CString a_strFileName );



protected:
	virtual void ModelessOKFeedback(UINT nDialogID);
	virtual void ModelessCancleFeedback(UINT nDialogID);

public:
	void       InitializeNode       ( void );
	void       ActivatedFrameWndNotifyProcess();
	void       DeActivatedFrameWndNotifyProcess();
private:
	bool       GetZoneNodeInfo(void);
	CString    GetNodeName(std::map<Integer, CString> &a_mapZoneNodeData, Integer a_nxID);

private:
	virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);
	
private:
	KIOColumns m_oZoneColumns;
	KIOColumns m_oSedVectorColumns;
	KIOColumns m_oModeODColumns;
	KIOColumns m_oPurposeODColumns;
	KIOColumns m_oParameterODColumns;
	
	std::set<CString>   m_setSearchedID;
	CString             m_strSearchedIDs;

	CBrush              m_backBrush;
	BOOL m_bAutoGenZoneConn;	// 존 연결로 자동 생성 여부(TRUE : 선택, FALSE : 선택되지 않음)
	virtual void DoDataExchange(CDataExchange* pDX);
protected:
	CButton m_btn_multi;
};

typedef std::shared_ptr<KZoningDivisionDlg> KZoningDivisionDlgPtr;