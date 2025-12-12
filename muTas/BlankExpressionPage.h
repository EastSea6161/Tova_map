/**
 * @file BlankExpressionPage.h
 * @brief KBlankExpressionPage 선언 파일
 * @author 
 * @date 2011.05.18
 * @remark
 */


#pragma once




/**
 * @brief Input Property page 표출 시 빈 다이얼로그 표출 class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.18
 */
class KBlankExpressionPage : public CXTPPropertyPage
{
	DECLARE_DYNAMIC(KBlankExpressionPage)

public:
	KBlankExpressionPage();
	virtual ~KBlankExpressionPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_9900_KBLANKEXPRESSIONPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
