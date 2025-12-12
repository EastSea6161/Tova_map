#pragma once


// KZoningIntegrationDlg 대화 상자입니다.
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


class KTarget;
class KMapView;
struct TLinkFTNodeID;


enum KEMNewInputError
{
	KEMNewInputErrorMap,
	KEMNewInputErrorZoneList,
	KEMNewInputErrorZoneShort,
	KEMNewInputOK
};

class KZoningIntegrationDlg : public KDialogEx, public KShortestPathBase, public IMapZoning, public ImChampModelessFeedback, public KTableObserver
{
	DECLARE_DYNAMIC(KZoningIntegrationDlg)

public:
	KZoningIntegrationDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KZoningIntegrationDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4410_ZoningIntegrationDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

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
	KTarget*  m_pTarget;
	KTarget*  m_pRunningTarget;
	KMapView* m_pMapView;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndAAAZoneReportCtrl;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndBBBZoneReportCtrl;
	CImageList          m_imageListSelectIcon;
	Integer   m_nxTempMaxID;
	Integer   m_nxSelectedTempID;

	std::map<Integer, KZoneDivisionInfoPtr>     m_mapZoneIntegrationInfo;
	std::set<Integer> m_setDeleteLinkID;
	KImChampMultiIDFindPtr m_spImTasMultiIDFind;

	std::set<Integer> m_setSelectedZone;

private:
	void DisplayAZone();
	void UpdateNote  ();
	void DisplayBZone(Integer &a_nxTempID);
	void DrawZoneAccess(std::vector<Integer> &a_vecZoneID);
	KEMNewInputError IsInputDone ( KZoneDivisionInfoPtr a_spZoningIntegrationZoneInfoPtr );

public:
	void ReceivedZoneInfoFromMap(TxPointPtr a_spNodeGeometry, TxPolygonPtr a_spDemarcationGeometry);

	static unsigned __stdcall IntegrationZoneThreadCaller(void* pParam);
	static void AddStatusMessage(CString a_strMsg);
	static void ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg);
	bool        RunIntegrateGeneration();

	static bool m_bRunIntegrationSucceess;
	int         m_nResultCode;

private:
	void DrawAddedZone(KZoneDivisionInfoPtr spZoneDivisionInfo);

protected:
	// Zoning dll
	typedef int (*FZONING)(void *pParam, void* pCallback);
	FZONING ZoningProcPtr;

private:
	void    InitReportHeader();
	bool    InputCheck       ( CString& ar_strError );
	void    GenerateNewZoneID();
	
	/**************************************************************************/
	/* 2019.01.18 수정 : 여러 시나리오 처리기능 추가에 따른 변경                     */
	//void    SaveGeometry();
	void    SaveGeometry(KMapView* pMapView);
	void    SaveGeometry(KDBaseConPtr spDBConnection);
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
	bool    ArgumentWriteBase       (CStdioFileEx& of);
	bool    ArgumentWriteIntegration(CStdioFileEx& of);
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

	bool    UpdateIntegrationSystemInfo();
	void    UpdateZoneSystemData   (KDBaseConPtr a_spDBaseConnection);
	void    UpdateSedSystemData    (KDBaseConPtr a_spDBaseConnection);
	size_t  ConvertZoneListToVector(CString a_strZoneList, std::vector<Integer> &a_vecOriginID);
	void    NotifyRelationTable();
	void    NotifyZoneRelation();
	void    NotifyLinkRelation();
	void    ClearDrawInfo();
	CString ConvertSavePath( CString a_strFileName );
	size_t  ConvertZoneIntegrationInfoToZoneOriginSet(std::set<Integer> &a_setOriginID);

protected:
	virtual void ModelessOKFeedback(UINT nDialogID);
	virtual void ModelessCancleFeedback(UINT nDialogID);

public:
	void       InitializeNode       ( void );
	void       ActivatedFrameWndNotifyProcess();
	void       DeActivatedFrameWndNotifyProcess();
private:
	bool       GetZoneNodeInfo(void);

private:
	virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);

private:
	KIOColumns m_oZoneColumns;
	KIOColumns m_oSedVectorColumns;
	KIOColumns m_oModeODColumns;
	KIOColumns m_oPurposeODColumns;
	KIOColumns m_oParameterODColumns;

	/**************************************************************************/
	/* 2019.01.18 수정 : 존 연결로 자동생성 기능 추가에 따른 변경                    */
	BOOL m_bAutoGenZoneConn;	// 존 연결로 자동 생성 여부(TRUE : 선택, FALSE : 선택되지 않음)
	/**************************************************************************/

protected:    
	virtual BOOL OnInitDialog();

	afx_msg void OnIntegrationSelectChanged   (NMHDR*  pNMHDR, LRESULT* pResult);
			void RefreshRelationRecord(CXTPReportRecord* a_pRecordA);
	afx_msg void OnIntegrationValueChanged    (NMHDR*  pNMHDR, LRESULT* pResult);
	afx_msg void OnIntegrationItemButtonClick (NMHDR * pNMHDR, LRESULT* pResult);
	afx_msg void OnReportItemClick            (NMHDR*  pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnAddNewZone();
	afx_msg void OnBnClickedBtnDelZone();
	afx_msg LRESULT OnOriginZoneEdit(WPARAM a_wParam, LPARAM a_lParam);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnBnClickedBtnMultiScenario();

};

typedef std::shared_ptr<KZoningIntegrationDlg> KZoningIntegrationDlgPtr;