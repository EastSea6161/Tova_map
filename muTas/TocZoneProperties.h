#pragma once

//^#include "ResizeDialogEx.h"
class KTarget;
class KMapView;

// KTocZoneProperties 대화 상자입니다.

class KTocZoneProperties : public KDialogEx
{
	DECLARE_DYNAMIC(KTocZoneProperties)

public:
	KTocZoneProperties(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTocZoneProperties();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOC_ZONE_PROPERTIES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
private:
	KTarget*         m_pTarget;
	KMapView*        m_pMapView;
public:
	void SetTargetMap(KTarget* a_pTarget, KMapView* a_pMapView);
private:
	CXTColorPicker  m_btnFillColorPicker;
	CXTColorPicker  m_btnOutLineColorPicker;
protected:
	virtual BOOL   OnInitDialog();	
protected:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void   OnSize(UINT nType, int cx, int cy);	
protected:
protected:
	afx_msg void OnBnClickedRadio1();
			void ActionShowLayerAllScaleSelect();
	afx_msg void OnBnClickedRadio2();
			void ActionShowLayerZoomScaleSelect();
	afx_msg void OnBnClickedButtonMinScale();
	afx_msg void OnBnClickedButtonMaxScale();
	afx_msg void OnBnClickedReset();
			void Reset();
	afx_msg void OnBnClickedButtonApply();
			void Apply();	
};
