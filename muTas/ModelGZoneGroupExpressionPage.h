/**
 * @file : ModelGZoneGroupExpressionPage.h
 * @brief : KModelGZoneGroupExpressionPage 선언 파일
 * @author : teegee@zaolsoft.com
 * @date : 2011.04.22
 * @remark :
 */



#pragma once



/**
 * @class : KModelGZoneGroupExpressionPage
 * @brief : 발생 모형의 Input 설정 시 Zone Group이 설정된 목적에 대한
 *          복합 수식 입력에 사용되는 Property page class
 * @remark :
 * @warning :
 * @version : 1.0
 * @author : teegee@zaolsoft.com
 * @date : 2011.04.22
 */
class KModelGZoneGroupExpressionPage : public CPropertyPage
{
	DECLARE_DYNAMIC(KModelGZoneGroupExpressionPage)

public:
	KModelGZoneGroupExpressionPage();
	virtual ~KModelGZoneGroupExpressionPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_KMODELGZONEGROUPEXPRESSIONPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public :	// Getter/Setter and Operations


protected :

};
