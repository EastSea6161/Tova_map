/**
 * @file SelectByAttributeDlg.h
 * @brief KSelectByAttributeDlg 선언
 * @author 
 * @date 2010.09.08
 * @remark
 */


#pragma once
#include "afxwin.h"
#include "afxcmn.h"



/* Forward declarations */
class KIOTable;



/**
 * @brief Talbe의 data를 선택하기 위한 Query의 where절을 생성하는 다이얼로그
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2010.09.08
 */
class KSelectByAttributeDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KSelectByAttributeDlg)

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_4120_SelectByAttributeDlg };


	/**************************************
	* Constructors / Destructor           *
	**************************************/
public:
	KSelectByAttributeDlg(KIOTable* a_pIOTable, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KSelectByAttributeDlg();

	/**************************************
	* Operations                          *
	**************************************/
protected :
	void addTextToEdit( const CString& strText );
	
	void ConvertFieldName( CXTPReportControl& wndReport, CString& strQuery );
	void InitFieldReport( void );
	void UpdateFieldRecord( void );
	void InitUniqueReport( void );
	void UpdateUniqueRecord( const CString& strSQL );

public:
	void SetFilter(CString a_strFilter);
	

	/**************************************
	* Overrides                           *
	**************************************/
public :
	virtual BOOL OnInitDialog();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


	/**************************************
	* Properties / Variables              *
	**************************************/
public :
	CEdit m_editQuery;

	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReportFiled;
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReportUnique;

protected :
	KIOTable* m_pIOTable;
	CString   m_strFilter;
	CString   m_strOrignalFilter;


	/**************************************
	* Message handlers                    *
	**************************************/
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton0();
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

	afx_msg void OnReportFieldDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReportUniqueDblClick(NMHDR* pNMHDR, LRESULT* pResult);
};
