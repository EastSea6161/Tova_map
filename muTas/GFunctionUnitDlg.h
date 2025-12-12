/**
 * @file GFunctionUnitDlg.h
 * @brief KGFunctionUnitDlg 선언 파일
 * @author 
 * @date 2011.04.29
 * @remark
 */


#pragma once

class KTarget;

#include "afxwin.h"


/**
 * @brief 발생 모델에서 Unit 모형의 수식입력 dialog class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.04.29
 */
class KGFunctionUnitDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KGFunctionUnitDlg)

public:
	KGFunctionUnitDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KGFunctionUnitDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_6140_GFunctionUnitDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public :

	void SetTarget(KTarget* a_pTarget);

	/**
	* @brief Unit 모형 수식의 상수값을 반환한다.
	* @return double - 수식의 상수값
	*/
	double Number(void);

	/**
	* @brief Unit 모형 수식의 상수값을 설정한다.
	* @param[in] double - dNumber - 상수값
	*/
	void Number(double a_dNumber);

	/**
	* @brief Unit 모형의 입력에 사용 될 Column의 명칭을 반환한다.
	* @return LPCTSTR - Column의 명칭
	*/
	LPCTSTR FieldName(void);

	/**
	* @brief Unit 모형의 입력에 사용 될 Column의 명칭을 설정한다.
	* @param[in] strFieldName - Column의 명칭
	*/
	void FieldName(LPCTSTR a_strFieldName);

	/**
	* @brief Unit 모형의 입력에 사용 될 Column의 표출 명칭을 반환한다.
	* @return LPCTSTR - Column의 표출 명칭
	*/
	LPCTSTR FieldCaption(void);

	/**
	* @brief Unit 모형의 입력에 사용 될 Column의 표출 명칭을 설정한다.
	* @param[in] strFieldCaption - Column의 표출 명칭
	*/
	void FieldCaption(LPCTSTR a_strFieldCaption);

protected:
	/**
	* @brief SED 컬럼 정보 설정
	*/
	void initList(void);


protected :
	KTarget*	m_pTarget;
	double		m_dNumber;
	CString		m_strFieldName;
	CString		m_strFieldCaption;
	CListBox	m_lstField;
	ZNumberEdit m_edtNumber;
    
public:
    /**
	* @brief 다이얼로그 초기화
	*/
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnDblclkList1();
};
