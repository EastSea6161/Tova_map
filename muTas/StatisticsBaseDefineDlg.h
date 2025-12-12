/**
 * @file KStatisticsBaseDefineDlg.h
 * @brief DB Management 통계
 * @author jyk@qbicware.com
 * @date 2012.04.23
 * @remark 
 */

#pragma once
#include "afxwin.h"
#include "TStatisticsInfo.h"

class KIOTable;
class KTarget;
class KIOColumns;
class KIOColumn;
class KGRegressionExpression;
class KDevideDataInterval;


// KStatisticsBaseDefineDlg 대화 상자입니다.

class KStatisticsBaseDefineDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(KStatisticsBaseDefineDlg)

public:
	KStatisticsBaseDefineDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KStatisticsBaseDefineDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4141_StatisticsBaseDefineDlg };

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

	KIOColumns* GetSelectedColumns()
	{
		return m_pSelectedColumns;
	}

	static unsigned __stdcall StatisticThreadCaller(void* p);

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


private:
	KIOTable*               m_pTable;
	KTarget*			    m_pTarget;
	CString                 m_strFilter;
	KIOColumns*    m_pSelectedColumns;
	KGRegressionExpression* m_pExpression;
	
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
	void GetSourceData();
	void GetNormalSourceData();
	void GetGroupSourceData();
	bool GetNormalData(KIOColumn* pColumn, std::vector<double> &a_vecData);
	bool GetGroupData( KIOColumn* a_pColumn, std::map<Integer, std::vector<double>> &a_mapSourceData);
	bool GetExpressionNormalData( std::vector<double> &a_vecData );
	bool GetExpressionGroupData( std::map<Integer, std::vector<double>> &a_mapExpressionData );
protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
	CString           m_strFunction;
	BOOL              m_bChkZero;
	BOOL              m_bChkGroup;
	CComboBox         m_cboGroup;

	KIOColumn* m_pGroupColumn;

	std::vector< TSingleNormalInfo >         m_vecNormalInfo;
	std::vector< TSingleGroupInfo  >         m_vecGroupInfo;
	std::vector< TSingleExpressionInfo >     m_vecExpressionInfo;
	std::vector< TSingleGroupExpressionInfo> m_vecGroupExpressionInfo;
};
