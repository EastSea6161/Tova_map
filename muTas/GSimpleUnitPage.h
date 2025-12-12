/**
 * @file GSimpleUnitPage.h
 * @brief KGSimpleUnitPage 선언 파일
 * @author 
 * @date 2011.05.27
 * @remark
 */
#pragma once


#include "afxwin.h"
#include "GFormulaDefaultDlg.h"
#include "DBaseFGenerationModel.h"

/* Forward declarations */
class KGenerationInfo;
class KGUnitExpression;
class KTarget;

/**
 * @brief 발생 모델에서 Zone group이 설정되지 않은 Unit 모형의 수식입력 page class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.04.27
 */

class KGSimpleUnitPage : public CXTPPropertyPage
{
	DECLARE_DYNAMIC(KGSimpleUnitPage)

public:
	KGSimpleUnitPage(KGFormulaDefaultDlgPtr* a_spKGFormulaDefaultDlg);
	virtual ~KGSimpleUnitPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6110_GSimpleUnitPage };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public :
	/**
	* @brief 발생모형 정보를 설정한다.
	* @param[in] pGInfo - 발생모형 정보
	*/

	void SetTarget(KTarget* a_pTarget);

	void SetGenerationInfo(KGenerationInfo* a_pKGInfo);

	void SetParentSheet(CXTPPropertySheet* a_pParentSheet)
	{
		m_pParentSheet = a_pParentSheet;
	}

	void SetDefaultFreightInfo(KFPAGenerationparaInfo a_oParaInfo)
	{
		m_oParameterInfo = a_oParaInfo;
	}

	void SetDefaultPassengerData(KGenerationInfo* a_pDefaultInfo)
	{
		m_pDefaultGInfo = a_pDefaultInfo;
	}


protected:
	void SetFreightDefault();
	void SetPassengerDefault();

protected:

	 /**
    * @brief 리스트 정보 초기화
    */
	void initList(void);

	/**
    * @brief 입력 정보 유효성 검사
    */
	BOOL InvalidateInputInfo();

	CString ColumnCaptionToName(CString a_strCaption);

protected :

	KTarget*			m_pTarget;
	KGenerationInfo*	m_pKGInfo;
	KGenerationInfo*	m_pDefaultGInfo;
	KGUnitExpression*	m_pKExpression;
	double				m_dNumber;
	CString				m_strFieldName;
	CString				m_strFieldCaption;
	CListBox			m_lstField;
	ZNumberEdit			m_edtNumber;

	CXTPPropertySheet* m_pParentSheet;
	KGFormulaDefaultDlgPtr* m_spKGFormulaDefaultDlg;
	KFPAGenerationparaInfo m_oParameterInfo;

protected:
    /**
    * @brief 다이얼로그 초기화
    */
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkList1();
	virtual BOOL OnApply();
	afx_msg void OnBnClickedButtonDefault();
};
