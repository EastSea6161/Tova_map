#pragma once
//^#include "ResizeDialogEx.h"
#include "ImChampMapDefineInfo.h"
#include "afxwin.h"

// KPreferenceSymbol 대화 상자입니다.

class KPreferenceSymbol : public KDialogEx
{
	DECLARE_DYNAMIC(KPreferenceSymbol)

public:
	KPreferenceSymbol(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KPreferenceSymbol();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_0100_Preference_Symbol };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSelEndOKColor();
	afx_msg void OnBnClickedButtonMarkerNode();
	afx_msg void OnBnClickedButtonMarkerZone();

private:
	CXTColorPicker  m_cpNodeColor;
	CXTColorPicker  m_cpZoneColor;
	COLORREF     m_clrNode;
	COLORREF     m_clrZone;
	TPreferenceSymbol m_oSymbolNode;
	TPreferenceSymbol m_oSymbolZone;

	CFont        m_oQbicDefaultMarkerFont;
	CButton      m_btnMarkerNode;
	CButton      m_btnMarkerZone;

	int          m_nMarkerNodeIndex;
	int          m_nMarkerZoneIndex; 

public:
	virtual void UserCommand(int a_nCommand=0);
private:
	bool         InvalidateInputInfo();
};
