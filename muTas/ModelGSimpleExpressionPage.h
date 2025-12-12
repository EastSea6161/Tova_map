/**
 * @file : ModelGSimpleExpressionPage.h
 * @brief : KModelGSimpleExpressionPage 선언 파일
 * @author : teegee@zaolsoft.com
 * @date : 2011.04.22
 * @remark :
 */


#pragma once


/* Forward declarations */
class KModelPurpose;


/**
 * @class : KModelGSimpleExpressionPage
 * @brief : 발생 모형의 Input 설정 시 Zone Group이 설정되지 않은 목적에 대한
 *          단순한 수식 입력에 사용되는 Property page class
 * @remark :
 * @warning :
 * @version : 1.0
 * @author : teegee@zaolsoft.com
 * @date : 2011.04.22
 */
class KModelGSimpleExpressionPage : public CXTPPropertyPage
{
	DECLARE_DYNAMIC(KModelGSimpleExpressionPage)

public:
	KModelGSimpleExpressionPage();
	virtual ~KModelGSimpleExpressionPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_KMODELGSIMPLEEXPRESSIONPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public :
	/**
	* @name : SetPurpose
	* @brief : 발생모형의 목적을 설정한다.
	* @param[in] : pPurpose - 발생모형의 목적
	*/
	void SetPurpose(KModelPurpose* pPurpose);


protected :
	KModelPurpose* m_pPurpose;

public:
	afx_msg void OnBnClickedButton1();
};
