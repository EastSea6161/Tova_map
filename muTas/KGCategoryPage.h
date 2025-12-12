/**
 * @file KGCategoryPage.h
 * @brief Categrory 수식 설정 
 * @author nomplus2@qbicware.com
 * @date 2011.08.26
 * @remark 
 */


#pragma once
#include "afxwin.h"
#include "KGCategoryExpression.h"
/* Forward declarations */


/**
* @brief Categrory 수식 설정 
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
class KGenerationInfo;
class KTarget;
// KGCategoryPage 대화 상자입니다.

class KGCategoryPage : public CXTPPropertyPage
{
	DECLARE_DYNAMIC(KGCategoryPage)

public:
	KGCategoryPage();
	virtual ~KGCategoryPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6130_GSimpleClassificationPage };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public :
    /**
    * @brief 발생 입력 정보 설정
    * @param[in] pGInfo - 발생모형 정보
    */

	void SetTarget(KTarget* a_pTarget);

    void SetGenerationInfo(KGenerationInfo* pGInfo);

	void SetParentSheet(CXTPPropertySheet* a_pParentSheet)
	{
		m_pParentSheet = a_pParentSheet;
	}

	void SetDefaultPassengerInfo(KGenerationInfo* a_pGInfo)
	{
		m_pDefualtGInfo = a_pGInfo;
	}

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
    * @brief 연령그룹 변경 정보 로드
    */
    void SelchangeComboAgeGroup();

	/**
    * @brief 입력 정보 유효성 검사
    */
	BOOL InvalidateInputInfo();
    
protected :
	KTarget*					m_pTarget;
    KGenerationInfo*			m_pGInfo;
	KGenerationInfo*			m_pDefualtGInfo;
    std::vector<TCategoryClass> m_vecCategoryClass;
    int GetCategoryIndex(int category_id);
    std::vector<TAgeRange>	    m_vecAgeRange;

    KGCategoryExpression* m_pGCategoryExpression;
    TCategoryClass*       m_pCategoryClass;

	CXTPPropertySheet* m_pParentSheet;

protected:
    CButton m_chkSex;
    CButton m_chkAge;
    CComboBox m_cboCategoryClass;
    CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;

protected:
    virtual BOOL OnInitDialog();
    virtual BOOL DestroyWindow();
    /*virtual void OnOK();*/
	virtual BOOL OnApply();
    
    afx_msg void OnBnClickedCheckSex();
    afx_msg void OnBnClickedCheckAge();
    afx_msg void OnCbnSelchangeComboAgeGroup();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnBnClickedButtonDefault();
};
