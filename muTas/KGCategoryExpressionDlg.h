/**
 * @file KGCategoryExpressionDlg.h
 * @brief Categrory 수식 설정 
 * @author nomplus2@qbicware.com
 * @date 2011.08.26
 * @remark 
 */


#pragma once
#include "KGCategoryExpression.h"

class KTarget;
// KGCategoryExpressionDlg 대화 상자입니다.

/**
* @brief Categrory 수식 설정 
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
class KGCategoryExpressionDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(KGCategoryExpressionDlg)

public:
	KGCategoryExpressionDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KGCategoryExpressionDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6160_GFunctionClassificationDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public :

	void SetTarget(KTarget* a_pTarget);

    /**
    * @brief Category 수식 정보 설정
    * @param[in] pExpression - Category 수식 정보
    */
    void Expression(KGCategoryExpression* pExpression);
protected:
    /**
    * @brief Category 클래스 기본 정보 로드
    */
    void loadCategoryClass();

    /**
    * @brief 연령 그룹 정보 조회
    * @param[in] pCategory - Category 클래스 정보
    */
    void loadCategoryAgeGroup(TCategoryClass* pCategory);

    /**
    * @brief 카테고리 원단위 값 설정
    */
    void loadCategoryValue();

    /**
    * @brief 카테고리 인덱스 조회
    * @param[in] category_id - Category ID
    */
    int GetCategoryIndex(int category_id);

    /**
    * @brief 연령그룹 변경 정보 로드
    */
    void SelchangeComboAgeGroup();      

	BOOL InvalidateInputInfo();

protected :

	KTarget*			m_pTarget;
    CButton				m_chkSex;
    CButton				m_chkAge;
    CComboBox			m_cboCategoryClass;
    CXTPOfficeBorder<CXTPReportControl,false>	m_wndReportCtrl;

    KGCategoryExpression* m_pOriginCategoryExpression;
    KGCategoryExpression* m_pGCategoryExpression;
    std::vector<TCategoryClass> m_vecCategoryClass;
    
    std::vector<TAgeRange> m_vecAgeRange;    
    TCategoryClass*       m_pCategoryClass;
protected:
    /**
    * @brief 다이얼로그 초기화
    */
    virtual BOOL OnInitDialog();    
    afx_msg void OnBnClickedCheckSex();
    afx_msg void OnBnClickedCheckAge();
    afx_msg void OnCbnSelchangeComboAgeGroup();      
    afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedButtonEdit();
	
};
