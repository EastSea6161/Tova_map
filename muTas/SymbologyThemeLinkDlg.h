#pragma once
#include "ThemeLinkValueScale.h"
#include "ThemeLinkValueClassify.h"
//^#include "ResizeDialogEx.h"

class KTarget;
class KMapView;
// KSymbologyThemeLinkDlg 대화 상자입니다.

class KSymbologyThemeLinkDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KSymbologyThemeLinkDlg)

public:
	KSymbologyThemeLinkDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KSymbologyThemeLinkDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOC_LINK_SYMBOLOGY_THEME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();

public:
	void KSymbologyThemeLinkDlg::SetTarget(KTarget* a_pTarget)
	{
		m_pTarget = a_pTarget;
	}
	void KSymbologyThemeLinkDlg::SetMapView(KMapView* a_pMapView)
	{
		m_pMapView = a_pMapView;
	}

protected:
	KTarget*  m_pTarget;
	KMapView* m_pMapView;

	KThemeLinkValueScale    m_dlgThemeLinkValueScale;
	KThemeLinkValueClassify m_dlgThemeLinkValueClassify;

protected:
	int m_nRadio;

protected:
	void         RadioControl();

	afx_msg void OnBnClickedRadioScale();
	afx_msg void OnBnClickedRadioClassification();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonCancel();
    afx_msg void OnBnClickedCheckLabel();
private:
    void Clear();
public:
    void ModelessInitialize();
public:
    void NodeLayerSettingChanged();
};

typedef std::shared_ptr<KSymbologyThemeLinkDlg> KSymbologyThemeLinkDlgPtr;