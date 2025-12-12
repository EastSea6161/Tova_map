#pragma once
#include "ThemeZoneValueClassify.h"
//^#include "ResizeDialogEx.h"

class KTarget;
class KMapView;
// KSymbologyThemeZoneDlg 대화 상자입니다.

class KSymbologyThemeZoneDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KSymbologyThemeZoneDlg)

public:
	KSymbologyThemeZoneDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KSymbologyThemeZoneDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOC_ZONE_SYMBOLOGY_THEME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();

public:
	void KSymbologyThemeZoneDlg::SetTarget(KTarget* a_pTarget)
	{
		m_pTarget = a_pTarget;
	}
	void KSymbologyThemeZoneDlg::SetMapView(KMapView* a_pMapView)
	{
		m_pMapView = a_pMapView;
	}

protected:
	KTarget*  m_pTarget;
	KMapView* m_pMapView;
	KThemeZoneValueClassify m_dlgThemeZoneValueClassify;

protected:
	bool m_bZoneTable;

protected:
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedRadio(UINT nID);
		void ControlRadio();
		void Clear();
	afx_msg void OnBnClickedCheckLabel();
};

typedef std::shared_ptr<KSymbologyThemeZoneDlg> KSymbologyThemeZoneDlgPtr;