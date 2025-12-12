/**
 * @file GSimpleExpressionPage.h
 * @brief KGSimpleExpressionPage 선언 파일
 * @author 
 * @date 2011.04.29
 * @remark
 */


//@deprecated Class 


#pragma once


#include "resource.h"
#include "afxwin.h"


/* Forward declarations */
class KPurpose;


/**
 * @brief Zone group이 설정되지 않은 목적의 수식입력 Property page class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.04.29
 */
class KGSimpleExpressionPage : public CXTPPropertyPage
{
	DECLARE_DYNAMIC(KGSimpleExpressionPage)

public:
	KGSimpleExpressionPage();
	virtual ~KGSimpleExpressionPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_9900_KGSIMPLEEXPRESSIONPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public :
	/**
	 * @brief SimpleExpressionPage에서 수식을 입력 할 KPurpose를 설정한다.
	 * @param[in] pPurpose - Model Purpose 정보
	 */
	void SetPurpose(KPurpose* pPurpose);

protected :
	KPurpose* m_pPurpose;
	CEdit m_edtExpression;


public:
	afx_msg void OnBnClickedButton1();
};
