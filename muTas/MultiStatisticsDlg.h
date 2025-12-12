/**
 * @file KMultiStatisticsDlg.h
 * @brief DB Management multi 통계
 * @author jyk@qbicware.com
 * @date 2012.04.27
 * @remark 
 */
#pragma once

#include "afxwin.h"
//^^ #include "IOTable.h"
#include "TStatisticsInfo.h"
#include <fstream>
#include <iostream>
//#include "StdioFileEx.h"

class KIOColumns;
class KIOColumn;
class KGRegressionExpression;


/**
* @brief DB Management multi 통계
* @version 1.0
* @author jyk@qbicware.com
* @date 2012.04.27
*/
class KMultiStatisticsDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(KMultiStatisticsDlg)

public:
	KMultiStatisticsDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KMultiStatisticsDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4145_TableMultiStatisticsDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
    void SetInitValue(KIOTable* pTable, KIOColumns* pColumns, CString strFilter=_T("")) 
    { 
		m_pTable        = pTable; 
		m_pColumns      = pColumns;
		m_strFilter     = strFilter;
	}

	void SetGroupColumn( KIOColumn* a_pGroupColumn )
	{
		m_pGroupColumn = a_pGroupColumn;
	}


	void SetMultiColumn( std::vector<TMultiColumn> &a_vecMultiColumn )
	{
		m_vecMultiColumn = a_vecMultiColumn;
	}


	void SetGroupTypeCount(const int &a_nGroupFieldTypeCount)
	{
		m_nGroupFieldTypeCount = a_nGroupFieldTypeCount;
	}


	void SetStatisticMainData(std::vector<TStatisticResultMain> &a_vecStatisticMain)
	{
		m_vecStatisticMain = a_vecStatisticMain;
	}


	void SetStatisticPartData(std::map<int, std::vector<TStatisticResultPart>> &a_mapStatisticPart)
	{
		m_mapStatisticPart = a_mapStatisticPart;
	}

private:
    KIOTable*                 m_pTable ;
	KIOColumns*      m_pColumns;
    CString                   m_strFilter;
	KIOColumn*                m_pGroupColumn;
	std::vector<TMultiColumn> m_vecMultiColumn;

	std::vector<TStatisticResultMain>                m_vecStatisticMain;
	std::map<int, std::vector<TStatisticResultPart>> m_mapStatisticPart;

	int m_nGroupFieldTypeCount;

private:
    /**
    * @brief Report Control Header 초기화
    */
    void InitReportHeader();

    /**
    * @brief 통계정보 Main Header Display (ver. Normal)
    */
    void InitMainReportNormalHeader();

    /**
    * @brief 통계정보 Part Header Display (ver. Normal)
    */
    void InitPartReportNormalHeader();

    /**
    * @brief 통계정보 Main Header Display (ver. Group)
    */
    void InitMainReportGroupHeader();

    /**
    * @brief 통계정보 Part Header Display (ver. Group)
    */
    void InitPartReportGroupHeader();

	/**
    * @brief 통계정보 Display
    */
	void InitReportData();

    /**
    * @brief 통계정보 Main Display (ver. Normal)
    */
    void InitMainReportNormalData();

    /**
    * @brief 통계정보 Part Display (ver. Normal)
    */
    void InitPartReportNormalData();

    /**
    * @brief 통계정보 Main Display (ver. Group)
    */
    void InitMainReportGroupData();

    /**
    * @brief 통계정보 Part Display (ver. Group)
    */
    void InitPartReportGroupData();


	void SetMainNormalRecordGrid(CXTPReportRecord* a_pRecord, LPCTSTR a_lpcszName, std::vector<double> &a_vecData, TMultiColumn &a_oTMultiColumn);


	void SetMainGroupRecordGrid(CXTPReportRecord* a_pRecord, LPCTSTR a_lpcszName, std::vector<double> &a_vecData);


	void SetPartRecordGrid(CXTPReportRecord* a_pRecord, LPCTSTR a_lpcszName, std::vector<double> &a_vecData);


	/**
    * @brief 결과 데이터 사이즈 체크 (ver. Normal)
    */
	bool ValidNormalResultDataSize();

    /**
    * @brief 결과 데이터 사이즈 체크 (ver. Group)
    */
	bool ValidGroupResultDataSize();

private:
	static unsigned __stdcall ExportThreadCaller(void* p);
	
	void ExportData();
	void ExportMainReport(CStdioFileEx& of);
	void ExportPartReport(CStdioFileEx& of);
	
	void WriteRecord(CXTPReportRecord* a_pRecord, CStdioFileEx& of);

private:
	CString m_strFile;
	CString m_strErrMsg;


protected:
    CXTPOfficeBorder<CXTPReportControl,false> m_ReportMainCtrl;
	CXTPOfficeBorder<CXTPReportControl,false> m_ReportPartCtrl;

protected:

    /**
    * @brief 다이얼로그 초기화
    */
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
	afx_msg void OnMainReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedButtonExport();
};
