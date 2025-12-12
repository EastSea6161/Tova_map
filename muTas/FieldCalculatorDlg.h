/**
 * @file FieldCalculatorDlg.h
 * @brief KFieldCalculatorDlg 선언
 * @author 
 * @date 2010.09.08
 * @remark
 */


#pragma once

#include "Resource.h"
#include "FormulaExpression.h"

/* Forward declarations */
class KIOTable;

using namespace std;

/**
 * @brief Table의 Field를 계산한다.
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2010.09.08
 */
class KFieldCalculatorDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KFieldCalculatorDlg)
public:
	KFieldCalculatorDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KFieldCalculatorDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_4130_FieldCalculatorDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
    CString m_strInitFilter;

public :
	inline void SetTable(KIOTable* pTable, CString a_strFilter)
	{
		m_pTable = pTable;
        m_strInitFilter = a_strFilter;
	}

	inline CString GetCalculateString(void)
	{
		return m_strCalculate;
	}

private:
	void SetResizeCtrl(void);
	void InitCalculateReport(void);
	void UpdateCalculateRecord(void);

	void InitOperatorReport(void);
	void UpdateOperatorRecord(void);
	
	void InitWhereReport(void);
	void UpdateWhereRecord(void);

	void InitUniqueReport(void);
	void UpdateUniqueRecord(void);

	void InitFieldCombo(void);

	void GetCountUniqueRecord(KIOTable* a_pTable, CString a_strQuery, int& ar_count);

	static unsigned __stdcall FieldCalculatorThreadCaller(void* p);

	static unsigned __stdcall GetUniqueValThreadCaller(void* p);
	static void               AddStatusMessage(CString a_strMsg);
	
	void Apply(void);

private:
	KFormulaExpression m_oExpression;

private:
	bool    m_bResultOK;
	bool    m_bResultGetUniqueValThread;
	CString m_strUniqueValQuery;

	map<__int64, KSDoubleRecord*> m_mapResultRecord;

protected :
	void    addTextToEdit( CEdit& edit, const CString& strText );
	void    convertFieldName( CListCtrl& listCtrl, CString& strSQL );
	CString ConvertCaptionName(CString strCaptionSQL);

	void    ConvertFieldName(CXTPReportControl& wndReport, CString& strQuery);

	void    DeallocateMap(void);


public :
	bool		m_bUpdated;
	CEdit		m_editCalculate;
	CEdit		m_editQuery;

protected :
	KIOTable* m_pTable;
	CString m_strCalculate;
	
public :
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonUniqueVal();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	afx_msg void OnReportCalculateDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReportOperatorDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReportWhereDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReportUniqueDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	
private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndCalculateReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndOperatorReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndWhereReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndUniqueReport;

	CComboBox m_cboField;
};
