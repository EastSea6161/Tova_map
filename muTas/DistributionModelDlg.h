/**
 * @file DistributionModelDlg.h
 * @brief KDistributionModelDlg 선언 파일
 * @author nomplus2@qbicware.com
 * @date 2011.05.19
 * @remark
 */


#pragma once

#include "afxwin.h"
#include "model_common.h"
#include "MarkupStatic.h"
#include "../kmzModel/KModelInclude.h"
#include "DBaseDefaultDistribution.h"
#include <fstream>
#include <iostream>
#include <direct.h>

/* Forward declarations */
class KDistributionInfo;
class KDistributionZoneData;
class KDistributionZoneDataCollection;
class KIOTable;
class KPAPurpose;
class KPurpose;
class KTarget;
class KMultiID;
class KMutiIDTraits;
class KODKey;
class KSTParaColumn;
class KIOColumns;
class KXmlManager;
class TiXmlElement;
class KIOColumn;

/**
 * @brief 분포 모형을 수행하는 Dialog class
 * @remark
 * @warning
 * @version 1.0
 * @author nomplus2@qbicware.com
 * @date 2011.05.19
 */
class KDistributionModelDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KDistributionModelDlg)


public:
    static void ReceiveModelMsg(CAtlString strMsg=_T(""));
    static void ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg);
    void  RunDistributionFortran();

public:
	KDistributionModelDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDistributionModelDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6200_DistrbutionModelDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public :

	/**
	* @brief 분포모형을 수행 할 Target year를 설정한다.
	* @param[in] pTarget - Target year 정보
	*/
	void SetTarget(KTarget* pTarget);

	/**
	* @brief 분포모형에서 사용 될 Zone table 정보를 설정한다.
	* @param[in] pZoneTable - Zone table 정보
	*/
	void SetZoneTable(KIOTable* pZoneTable);

	/**
	* @brief OD의 종류를 반환한다.
	* @return KEMODType - OD의 종류
	*/
	KEMDODType ODType(void);

	/**
	* @brief 분포모형이 구동되는 Thread function
	* @param[in] pParem - Thread funciton의 parameter
	*/
	static unsigned __stdcall DistributiontModelThreadCaller(void* pParam);
    
protected :

	CXTPButton m_btnModel;
	CXTPButton m_btnInput;
	CXTPButton m_btnBaseOD;
	CXTPButton m_btnVariable;
	CXTPButton m_btnCriteria;
	CXTPButton m_btnOutputDesign;
    CXTPHyperLink m_hyperLink;
	CXTPButton m_btnViewTable;
	CButton    m_btnRun;

    CMarkupStatic m_uiBackImage;
    ZArrowStatic m_uiArrow1;
    ZArrowStatic m_uiArrow2;
    ZArrowStatic m_uiArrow3;
    ZArrowStatic m_uiArrow4;
    ZArrowStatic m_uiArrow5;
    ZArrowStatic m_uiArrow6;

	KTarget*   m_pTarget;
	KIOTable*  m_pZoneTable;
	KEMDODType m_emODType;
	int        m_nFratarIteration;
	double     m_dFratarGap;
	int        m_nGravityIteration;
	double     m_dGravityError;

	bool       m_bUseFratar;
	bool       m_bUseGravity;

	std::list<KPAPurpose*>            m_PAPurposeList;
	std::map<int, KDistributionInfo*> m_purposeInfoMap;
    std::list<KSTParaColumn>          m_paraODColumns;

	std::map<int, KDistributionParaInfo>	m_mapDefaultParameter;

    /**
    * @brief Parameter OD Column 로드
    */
    void initParaODColumnNames();
	
	/**
    * @brief Parameter OD Column 로드
    */
	bool CheckparaODColumn(KIOColumn* a_pColumn);


    /**
    * @brief 목적 정보 로드
    */
	void initPurposeList(void);

    /**
    * @brief 목적 정보 초기화
    */
	void initPurposeInfos(void);

    /**
    * @brief 목적 정보 Clear
    */
	void clearPurposeInfos(void);

    /**
    * @brief 분포 정보 Clear
    */
	void clearDistributionInfo(void);
        
    /**
    * @brief 분포 모형 구동
    */
    void runDistributionModel(void);

    /**
    * @brief Fratar 모형 구동
    * @param[in] pDInfo - 모형 Input 정보
    * @param[out] resultRecord - 결과 저장 Record Set
    * @param[in] index - Record Set 컬럼 인덱스
    */
    bool runFratar(KDistributionInfo* pDInfo, KODKeyDoubleRecords& resultRecord, size_t index);

    /**
    * @brief Fratar 모형 구동
    * @param[in] pDInfo - 모형 Input 정보
    */
	bool runGravity(KDistributionInfo* pDInfo);

	typedef CAtlArray<KDistributionZoneData*>	        KTDZoneArray;
	typedef CAtlMap<KMultiID, double, KMutiIDTraits>	KTDBaseODMap;

    /**
    * @brief 존데이터 로딩
    * @param[in] pDInfo - 모형 Input 정보
    * @param[out] zoneData - 존 데이터 Record Set
    */
	bool prepareZoneData(KDistributionInfo* pDInfo, KTDZoneArray& zoneData);

    /**
    * @brief 존데이터 로딩
    * @param[in] pDInfo - 모형 Input 정보
    * @param[out] zoneData - 존 데이터 Record Set
    */
    bool prepareZoneData(KDistributionInfo* pDInfo, KDistributionZoneDataCollection& zoneData);

    /**
    * @brief 존데이터 Clear
    * @param[out] zoneData - 존 데이터 Record Set
    */
	void releaseZoneData(KTDZoneArray& zoneData);

    /**
    * @brief Base OD 데이터 로딩
    * @param[in] pDInfo - 모형 Input 정보
    * @param[out] baseODMap - Base OD 데이터 Record Set
    */
	bool prepareBaseOD(KDistributionInfo* pDInfo, KTDBaseODMap& baseODMap);

    /**
    * @brief Base OD 데이터 로딩
    * @param[in] pDInfo - 모형 Input 정보
    * @param[out] baseODMap - Base OD 데이터 Record Set
    */
    bool prepareBaseOD(KDistributionInfo* pDInfo, std::map<KODKey, double>& baseODMap);
	

    
    /**
    * @brief 모형 구동 결과 정보 Update
    * @param[in] pDInfo - 모형 Input 정보
    * @param[in] strStartDate - 모형 구동 시작시간
    * @param[in] strEndDate - 모형 구동 종료시간
    */
	bool recordDistributionResult(KDistributionInfo* pDInfo, LPCTSTR strStartDate, LPCTSTR strEndDate);
	
    /**
    * @brief Control Redraw
    */
    void KRedrawWindow();

private:

    /**
    * @brief Argument 파일 생성
    */
    bool CreateArgument();

    /**
    * @brief Argument Write
    * @param[in] of - File
    */
    bool ArgumentWriteODType    ( CStdioFileEx& of );

    /**
    * @brief Argument Write
    * @param[in] of - File
    */
    bool ArgumentWriteModel     ( CStdioFileEx& of );

    /**
    * @brief Argument Write
    * @param[in] of - File
    * @param[in] zoneColumnCollection - Zone Column Collection
    */
    bool ArgumentWriteInput     ( CStdioFileEx& of, KIOColumns& zoneColumnCollection );

    /**
    * @brief Argument Write
    * @param[in] of - File
    * @param[in] baseODColumnCollection - Base OD Column Collection
    */
    bool ArgumentWriteBaseOD    ( CStdioFileEx& of, KIOColumns& baseODColumnCollection );

    /**
    * @brief Argument Write
    * @param[in] of - File
    * @param[in] paraODColumnCollection - Parameter OD Column Collection
    */
    bool ArgumentWriteImpedance ( CStdioFileEx& of, KIOColumns& paraODColumnCollection );

    /**
    * @brief Argument Write
    * @param[in] of - File
    */
    bool ArgumentWriteStopping  ( CStdioFileEx& of );


    /**
    * @brief Argument Input File Write
    * @param[in] zoneColumnCollection - Zone Column Collection
    */
    bool ArgumentWriteFileZone  ( KIOColumns& zoneColumnCollection   );

    /**
    * @brief Argument Input File Write
    * @param[in] baseODColumnCollection - Base OD Column Collection
    */
    bool ArgumentWriteFileBaseOD( KIOColumns& baseODColumnCollection );

    /**
    * @brief Argument Input File Write
    * @param[in] paraODColumnCollection - Parameter OD Column Collection
    */
    bool ArgumentWriteFileParaOD( KIOColumns& paraODColumnCollection );

    /**
    * @brief columnCollection에 컬럼 추가
    * @param[in] columnCollection - KIOColumnCollection
    * @param[int] strColumn - 컬럼 이름 
    * @return int - columnCollection Index
    */
    int  AddColumn ( KIOColumns& columnCollection, CString strColumn );

    /**
    * @brief 분포모형 결과 정보 저장
    * @param[in] timeStart - 모형 구동 시간
    */
    bool WriteResult(CTime& timeStart);

	/**
    * @brief 'Run' 버튼 활성화 비활성화 컨트롤
    */  
	void RunButtonControl();

	//Default 설정
	void InitDefultFunction( KDistributionInfo* a_pDInfo );
	void InitDistributionParaInfo( void );

public:
    /**
    * @brief 다이얼로그 초기화
    */
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedModel();
	afx_msg void OnBnClickedInput();
	afx_msg void OnBnClickedBaseod();
	afx_msg void OnBnClickedVariable();
	afx_msg void OnBnClickedCriteria();
	afx_msg void OnBnClickedOutputdesign();
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedOutputtable();
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);


private:
	int  m_nResultCode;
public:
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	static void AddStatusMessage(CString a_strMsg);
};

