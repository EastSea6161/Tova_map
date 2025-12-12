/**
 * @file IntersectionExtensionPage.h
 * @brief KIntersectionExtensionPage 선언 파일
 * @author jyk@qbicware.com
 * @date 2012.06.07
 * @remark
 */
#pragma once
//^#include "ResizeDialogEx.h"
#include "intersection_common.h"
/* Forward declarations */

// KIntersectionExtensionPage 대화 상자입니다.

/**
 * @brief Intersection Extension page class
 * @remark
 * @warning
 * @version 1.0
 * @author jyk@qbicware.com
 * @date 2012.06.07
 */

class KIntersectionExtensionPage : public KDialogEx
{
	DECLARE_DYNAMIC(KIntersectionExtensionPage)

public:
	KIntersectionExtensionPage(KDBaseConPtr spDBaseConnection, TIntersectionBase &a_oTSelectIntersection, CWnd* pParent = NULL);
	virtual ~KIntersectionExtensionPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5930_IntersectionExtensionPage };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void SetParentSheet(CXTPPropertySheet* a_pParentSheet)
	{
		m_pParentSheet = a_pParentSheet;
	}

	void SetIntersection(TIntersectionBase &a_oTSelectIntersection)
	{
		m_oTSelectIntersection = a_oTSelectIntersection;
	}

protected:
	/**
	* @brief Report Control Header 초기화
	*/
	void InitReportHeader();

	void InitReportControl();

	void GetIntersectionExtensionInfo();

	double GetValue( KEMBoundDirectionCode emDirectionCode, size_t a_nVariable );

	void SetValue( std::map<int, TExtension> &a_mapExtensionInfo, KEMBoundDirectionCode emDirectionCode, int a_nRowIndex, int a_nValue );

	bool UpsertExtensionData(std::map<int, TExtension> &a_mapExtensionInfo);

public:
	BOOL SaveExtension();

protected :
	KDBaseConPtr m_spDBaseConnection;

	CXTPPropertySheet*            m_pParentSheet;
	TIntersectionBase             m_oTSelectIntersection;
	std::map<int, TExtension>     m_mapExtensionInfo;
	CXTPReportControl             m_wndReportCtrl;

protected:
	/**
	* @brief 다이얼로그 초기화
	*/
	virtual BOOL OnInitDialog();
	
	/*virtual BOOL OnApply();*/
	
};
