#pragma once
//^#include "ResizeDialogEx.h"
#include "IntersectionTopologyPage.h"
#include "IntersectionBasicPage.h"
#include "IntersectionThreeBasicPage.h"
#include "IntersectionExtensionPage.h"

class KTarget;
class KMapView;
class KIOTable;
// KIntersectionEditMainDlg 대화 상자입니다.

class KIntersectionEditMainDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KIntersectionEditMainDlg)

public:
	KIntersectionEditMainDlg(KTarget* a_pTarget, KMapView* a_pMapView, TIntersectionBase a_oTSelectIntersection, std::map<Integer, TBoundInfo> a_mapBoundInfo, KIOTable* a_pTable, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KIntersectionEditMainDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5900_IntersectionDataEditDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	void         RadioControl();

protected:
	KTarget*     m_pTarget;
	KMapView*    m_pMapView;
	KIOTable*    m_pTable; // Intersection table
	TIntersectionBase             m_oTSelectIntersection;
	std::map<Integer, TBoundInfo> m_mapBoundInfo;
	KIntersectionTopologyPage     m_oTopologyDlg;
	KIntersectionBasicPage        m_oBasicFourDlg;
	KIntersectionThreeBasicPage   m_oBasicThreeDlg;
	KIntersectionExtensionPage    m_oExtensionDlg;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioOption(UINT nID);
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedCancel();
};

typedef std::shared_ptr<KIntersectionEditMainDlg> KIntersectionEditMainDlgPtr;