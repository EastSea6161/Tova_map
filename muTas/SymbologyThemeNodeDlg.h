#pragma once
#include "ThemeNodeValueScale.h"
#include "ThemeNodeValueClassify.h"
//^#include "ResizeDialogEx.h"

class KTarget;
class KMapView;
// KSymbologyThemeNodeDlg 대화 상자입니다.

class KSymbologyThemeNodeDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KSymbologyThemeNodeDlg)

public:
	KSymbologyThemeNodeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KSymbologyThemeNodeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOC_NODE_SYMBOLOGY_THEME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();

public:
	void KSymbologyThemeNodeDlg::SetTarget(KTarget* a_pTarget)
	{
		m_pTarget = a_pTarget;
	}
	void KSymbologyThemeNodeDlg::SetMapView(KMapView* a_pMapView)
	{
		m_pMapView = a_pMapView;
	}

protected:
	KTarget*  m_pTarget;
	KMapView* m_pMapView;

	KThemeNodeValueScale    m_dlgThemeNodeValueScale;
	KThemeNodeValueClassify m_dlgThemeNodeValueClassify;

protected:
	int m_nRadio;

protected:
	void         RadioControl();
	
	afx_msg void OnBnClickedRadioScale();
	afx_msg void OnBnClickedRadioClassification();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonCancel();
		void Clear();
	afx_msg void OnBnClickedCheckLabel();
};

typedef std::shared_ptr<KSymbologyThemeNodeDlg> KSymbologyThemeNodeDlgPtr; 