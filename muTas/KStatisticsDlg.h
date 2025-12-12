/**
 * @file KStatisticsDlg.h
 * @brief DB Management 통계
 * @author nomplus2@qbicware.com
 * @date 2011.08.28
 * @remark 
 */

#pragma once

//^^ #include "IOTable.h"
#include "TStatisticsInfo.h"
class KIOColumns;
class KIOColumn;

/**
* @brief DB Management 통계
* @version 2.0
* @author jyk@qbicware.com
* @date 2012.05.04
*/
class KStatisticsDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KStatisticsDlg)

public:
	KStatisticsDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KStatisticsDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4140_TableStatisticsDlg };

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

	void SetNormalInfo(std::vector< TSingleNormalInfo >        &a_vecNormalInfo)
	{
		m_vecNormalInfo = a_vecNormalInfo;
	}

	void SetGroupInfo(std::vector< TSingleGroupInfo  >         &a_vecGroupInfo)
	{
		m_vecGroupInfo = a_vecGroupInfo;
	}

	void SetNormalExpressionInfo(std::vector< TSingleExpressionInfo >     &a_vecExpressionInfo)
	{
		m_vecExpressionInfo = a_vecExpressionInfo;
	}


	void SetGroupExpressionInfo(std::vector< TSingleGroupExpressionInfo> &a_vecGroupExpressionInfo)
	{
		m_vecGroupExpressionInfo = a_vecGroupExpressionInfo;
	}


private:
    KIOTable*               m_pTable ;
	KIOColumns*    m_pColumns;
    CString                 m_strFilter;
	KIOColumn*              m_pGroupColumn;

	int                     m_nRecordCount;
	int                     m_nDataCount;

	std::vector< TSingleNormalInfo >         m_vecNormalInfo;
	std::vector< TSingleExpressionInfo >     m_vecExpressionInfo;
	std::vector< TSingleGroupInfo  >         m_vecGroupInfo;
	std::vector< TSingleGroupExpressionInfo> m_vecGroupExpressionInfo;

private:
    /**
    * @brief Report Control Header 초기화
    */
    void InitReportHeader();
    /**
    * @brief 통계정보 Display (ver. Normal)
    */
    void InitReportNormalHeader();

    /**
    * @brief 통계정보 Display (ver. Group)
    */
    void InitReportGroupHeader();
	
	/**
    * @brief 통계정보 Display
    */
	void InitReportData();

    /**
    * @brief 통계정보 Display (ver. Normal)
    */
    void InitReportNormalData();

    /**
    * @brief 통계정보 Display (ver. Group)
    */
    void InitReportGroupData();


	void SetRecordGrid(CXTPReportRecord* a_pRecord, LPCTSTR a_lpcszName, KDevideDataInterval &a_oDevideDataInterval);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	static unsigned __stdcall ExportThreadCaller(void* p);
	void ExportData(void);

	void WriteRecord(CXTPReportRecord* a_pRecord, CStdioFileEx& of);

private:
	CString m_strFile;
	CString m_strErrMsg;

protected:
    /**
    * @brief 다이얼로그 초기화
    */
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonExport();

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;

};
