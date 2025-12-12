/**
 * @file KMultiStatisticsBaseDefineDlg.h
 * @brief DB Management Multi 통계
 * @author jyk@qbicware.com
 * @date 2012.04.26
 * @remark 
 */

#pragma once
#include "afxwin.h"
#include "TStatisticsInfo.h"
#include <fstream>
#include <iostream>
//#include "StdioFileEx.h"

class KIOTable;
class KTarget;
class KIOColumns;
class KIOColumn;
class KGRegressionExpression;

// KMultiStatisticsBaseDefineDlg 대화 상자입니다.

class KMultiStatisticsBaseDefineDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KMultiStatisticsBaseDefineDlg)

public:
	KMultiStatisticsBaseDefineDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KMultiStatisticsBaseDefineDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4146_MultiStatisticsBaseDefineDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	void SetTable(KIOTable* a_pTable) 
	{ 
		m_pTable = a_pTable;
	}

	void SetTarget(KTarget* a_pTarget)
	{
		m_pTarget = a_pTarget;
	}

	void SetFilter(CString &a_strFilter)
	{
		m_strFilter = a_strFilter;
	}

	static unsigned __stdcall StatisticThreadCaller(void* p);
	static void ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg);
	static void AddStatusMessage(CString a_strMsg);

	static bool m_bRunStatisticSucceess;

protected:
	// Statistics dll
	HINSTANCE m_hStatistics;
	typedef int (*STATISTICALANALYSIS)(void *pParam, void* pCallback);
	STATISTICALANALYSIS StatisticsProcPtr;

private:
	/**
	* @brief Report Control Header 초기화
	*/
	void InitReportHeader();

	/**
	* @brief 통계정보 Display
	*/
	void InitReportData();

	/**
	* @brief Group Filed Combo Box Data 초기화
	*/
	void InitGroupFiled();
	

	void CheckboxControl(BOOL a_bCheck);

	/**
	* @brief component resize seting
	*/
	void SetResizeComponent();

	/**
	* @brief Group Editbox 활성화 설정
	*/
	void DisableGroupFiled(BOOL a_bDisable);

	/**
	* @brief 현재 Group Filed 정보 반환
	*/
	KIOColumn* GetCurGroupField(void);


	void InputGridComboData(	CXTPReportRecordItemEditOptions* a_pBaseEditOptions, CXTPReportRecordItemEditOptions* a_pCompareEditOptions );


	void AddReportData();


	int GroupFiledTypeCount(KIOColumn* a_pGroupColumn);


	bool GetStatisticsData();


	bool CreateArgument();


	bool ClearFiles();


	bool WriteStatisticsArgument(CStdioFileEx& of);


	bool BulkSelect(KDBaseConPtr spDBaseConnection);


	bool ReadOutPutFile( void );


	bool ReadNormalOutPutFile( void );


	bool ReadGroupOutPutFile( void );


	bool GetExpressionGroupData(  std::map<Integer, std::vector<double>> &a_mapExpressionData );


	bool GetExpressionNormalData( std::vector<double> &a_vecData );

private:
	KIOTable*               m_pTable;
	KTarget*			    m_pTarget;
	CString                 m_strFilter;
	KGRegressionExpression* m_pExpression;
	
	KIOColumn*                m_pGroupColumn;
	std::vector<TMultiColumn> m_vecMultiColumn;
	int                       m_nGroupFieldTypeCount;
	KIOColumns*      m_pSelectedColumns;

	std::vector<TStatisticResultMain>                m_vecStatisticMain;
	std::map<int, std::vector<TStatisticResultPart>> m_mapStatisticPart;

protected:

    /**
    * @brief 다이얼로그 초기화
    */
    virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnFunction();
	afx_msg void OnSelectCancel();
	afx_msg void OnSelectAll();
	afx_msg void OnReportColumnRButtonClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedCheckGroupFiled();
	afx_msg void OnBnClickedButtonAdd();

	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);

private:
	int               m_nResultCode;

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
	CString           m_strFunction;
	BOOL              m_bChkZero;
	BOOL              m_bChkGroup;
	CComboBox         m_cboGroup;
};
