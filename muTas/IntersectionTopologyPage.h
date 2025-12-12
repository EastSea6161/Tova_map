/**
 * @file IntersectionTopologyPage.h
 * @brief KIntersectionTopologyPage 선언 파일
 * @author jyk@qbicware.com
 * @date 2012.06.07
 * @remark
 */
#pragma once
//^#include "ResizeDialogEx.h"
#include "intersection_common.h"
#include "afxwin.h"
#include "afxext.h"

// KIntersectionTopologyPage 대화 상자입니다.

/**
 * @brief Intersection Topology 설정 page class
 * @remark
 * @warning
 * @version 1.0
 * @author jyk@qbicware.com
 * @date 2012.06.07
 */
class KIntersectionTopologyPage : public KDialogEx
{
	DECLARE_DYNAMIC(KIntersectionTopologyPage)

public:
	KIntersectionTopologyPage(KDBaseConPtr spDBaseConnection, TIntersectionBase &a_oTSelectIntersection, std::map<Integer, TBoundInfo> &a_mapBoundInfo, CWnd* pParent = NULL);
	virtual ~KIntersectionTopologyPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5910_IntersectionTopologyPage };

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


	void SetLaneBoundInfo(std::map<Integer, TBoundInfo> &a_mapBoundInfo)
	{
		m_mapBoundInfo = a_mapBoundInfo;
	}

protected:
	void    InitIntersectionFourDir(void);
	void    InitIntersectionThreeDir();
	void    InitComboBox( CComboBox &a_cboBound, int a_initIndex );
	void    InitComboBoxThree( CComboBox &a_cboBound, int a_initIndex );
	void    SetDisplayBoundInfo( CComboBox &a_cboBound, int nIDC );
	bool    UpdateIntersectionThreeDir();
	bool    UpdateIntersectionFourDir();
	bool    EditOnlyNode( int &a_nOriginCrossType);
	bool    EditNodeAndCross( int &a_nEditedCrossType );
	CString ConvertFiledName(int a_nIndex);
	Integer GetCurBoundNode(CComboBox& a_cboBound);
	bool    InputCheckThreeDir();
	bool    InputCheckFourDir();
	void    EnableControls(int a_nIndex);

public:
	BOOL    SaveTopology();

protected :
	KDBaseConPtr m_spDBaseConnection;

	CXTPPropertySheet*            m_pParentSheet;
	TIntersectionBase             m_oTSelectIntersection;
	std::map<Integer, TBoundInfo> m_mapBoundInfo;
	KEMIntersectionType           m_emIntersectionType;
	std::vector<int>              m_vecMainRoadNum;
	std::vector<int>              m_vecSubRoadNum;

protected:
	/**
	* @brief 다이얼로그 초기화
	*/
	virtual BOOL OnInitDialog();
	/*virtual BOOL OnApply();*/
	afx_msg void OnCbnSelchangeComboLeft();
	afx_msg void OnCbnSelchangeComboTop();
	afx_msg void OnCbnSelchangeComboRight();
	afx_msg void OnCbnSelchangeComboBottom();

protected:

	CComboBox m_cboLeft;
	CComboBox m_cboTop;
	CComboBox m_cboRight;
	CComboBox m_cboBottom;
	int       m_nRadioCtrlNum;

public:
	afx_msg void OnBnClickedRadioLeft();
	afx_msg void OnBnClickedRadioTop();
	afx_msg void OnBnClickedRadioRight();
	afx_msg void OnBnClickedRadioBottom();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
};
