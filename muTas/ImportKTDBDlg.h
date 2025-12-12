/**
 * @file ImportKTDBDlg.h
 * @brief KImportKTDBDlg 선언
 * @author 
 * @date 2012.01.17
 * @remark
 */


#pragma once

#include "resource.h"
#include "kmz_common.h"

/* forward declarations */
class KProject;
class KScenario;
class KPurpose;
class KMode;
class KTarget;

typedef std::map<int, int> KTDDataYearMap;			// <column index, year> 
struct TCheckDataInfoByRow							// row 한 줄에 대한 체크된 정보
{
	int nTotalCheckedCnt;
	KTDDataYearMap mapCheckedDataYear;
	int nTargetYear;
	KEMImportKTDB emImportKTDB;
};
typedef std::map<int, TCheckDataInfoByRow*> KTDCheckDataMap;	// <row index, TCheckDataInfoByRow>

#define DATA_CHECK		_T("v")
#define DATA_UNCHECK	_T("")

/**
 * @brief 
 * @remark
 * @warning
 * @version 1.1
 * @author jyk@qbicware.com
 * @date 2012.01.17
 */
class KImportKTDBDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KImportKTDBDlg)

	enum { IDD = IDD_2000_ImportKTDBDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV supportE
	
	DECLARE_MESSAGE_MAP()

public:
	KImportKTDBDlg( CWnd* pParent = NULL);   // standard constructor
	virtual ~KImportKTDBDlg();

    void SetScenario(KScenario* pScenario);

	void SetSingleTargetMode(KTarget* a_pTarget);
	/**
	* @brief 체크된 network 정보를 반환 한다.
	* @return m_mapCheckNetworkData - 체크된 network 정보
	*/
	inline KTDCheckDataMap GetCheckNetworkData()
	{
		return m_mapCheckNetworkData;
	}

	/**
	* @brief 체크된 OD 정보를 반환 한다.
	* @return m_mapCheckODData - 체크된 OD 정보
	*/
	inline KTDCheckDataMap GetCheckODData()
	{
		return m_mapCheckODData;
	}

	/**
	* @brief 선택한 Analysis 정보를 반환 한다.
	* @return m_strAnalysisArea - 선택한 AnalysisArea 정보
	*/
	inline const CString GetAnalysisArea()
	{
		return m_strAnalysisArea;
	}

	
	std::map<CString, int> GetTotalODSelectedYears()
	{
		return m_mapTotalODSelectedYear;
	}


	std::map<CString, int> GetTotalNetworkSelectedYears()
	{
		return m_mapTotalNetworkSelectedYear;
	}


	std::vector<KPurpose*> GetPurpose()
	{
		return m_vecPurpose;
	}


	std::vector<KMode*> GetMode()
	{
		return m_vecMode;
	}

	std::map< unsigned int, KTarget* > GetImportTargetMap()
	{
		return m_mapTarget;
	}

protected :
	
	void InitAnalysisArea( void );
	
	void InitNetworkListControl( void );
	
	void InitODDataListControl( void );
	
	void UpdateNetworkListControl( void );
	
	void UpdateODDataListControl( void );
	
	void GetNetworkCheckInfo( CXTPReportRecord*    a_pRecord,         int            a_nTartgetYear, const std::list< CString > &a_dataYearList,	// param[in]
							  TCheckDataInfoByRow* a_pTCheckDataInfo, KEMImportKTDB& a_emImportKTDB );															

	void GetODCheckInfo( CXTPReportRecord*    a_pRecord,		 int			a_nTartgetYear,	  const std::list< CString > &a_dataYearList,		// param[in]
						 TCheckDataInfoByRow* a_pTCheckDataInfo, KEMImportKTDB& a_emImportKTDB);													// param[out]
	
	KEMImportKTDB GetInterExtra( std::vector<int>           &a_vecHighYear,  std::vector<int>           &a_vecLowYear,
								 std::vector<int>::iterator &a_itIndexStart, std::vector<int>::iterator &a_itIndexEnd);
	
	void SetDataCheck(TCheckDataInfoByRow* a_pTCheckDataInfo, CXTPReportRecord* pRecord);
	
	CString GetStatus(KEMImportKTDB a_emImportKTDB);
	
	KEMImportKTDB SetODDataStatus(    TCheckDataInfoByRow* a_pTCheckData);
	
	void MakeODSelectedYearList(      KTDDataYearMap &a_mapCheckedDataYear);

	void MakeNetworkSelectedYearList( KTDDataYearMap &a_mapCheckedDataYear);

public:
	std::vector<CString>	m_vecTargetYears;
	std::vector<KPurpose*>	m_vecPurpose;
	std::vector<KMode*>		m_vecMode;
	CString					m_strAnalysisArea;

protected:
	KScenario*				  m_pScenario;
	KTDCheckDataMap			  m_mapCheckODData;
	KTDCheckDataMap			  m_mapCheckNetworkData;
	KTDDataYearMap			  m_mapDataYear;
	std::map<CString, int>	  m_mapTotalODSelectedYear;			// 보간 , 보외시 선택한 '년도'들,,,
	std::map<CString, int>	  m_mapTotalNetworkSelectedYear;	// network 선택한 '년도'를,,,

	KTarget*                  m_pTargetSingle;
	bool                      m_bTargetSingle;
	std::map< unsigned int, KTarget* > m_mapTarget;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedPrjchkImportnewtork();
	afx_msg void OnClickedPrjchkImportod();
	afx_msg void OnNetworkDataClick(NMHDR * a_pNotifyStruct, LRESULT * a_result);
	afx_msg void OnODDataClick(NMHDR * a_pNotifyStruct, LRESULT * a_result);
	afx_msg void OnBnClickedPrjbtnNetworkdefault();
	afx_msg void OnBnClickedPrjbtnODdefault();
	afx_msg void OnBnClickedOk();

public:
	CStatic m_stcScenarioName;

protected :
	CXTPOfficeBorder<CXTPReportControl,false> m_reportNetwork;
	CXTPOfficeBorder<CXTPReportControl,false> m_reportOD;
public:
	afx_msg void OnBnClickedCancel();
};
