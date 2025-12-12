#pragma once

#include "resource.h"
#include "afxwin.h"

// KAStoppingCriteriaOriginBaseDlg 대화 상자입니다.

class KAStoppingOriginBaseUeDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KAStoppingOriginBaseUeDlg)

public:
	KAStoppingOriginBaseUeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KAStoppingOriginBaseUeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6785_AStoppingOriginBaseUeDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:

	int    NumberOfIteration(void);	
	void   NumberOfIteration(int nIteration);

	double RelativeGap(void);	
	void   RelativeGap(double dGap);

    int RelativeGapExp()
    {
        return m_nHighwayRelativeGapExp;
    }

    void RelativeGapExp(int a_nRelativeGapExp)
    {
        m_nHighwayRelativeGapExp = a_nRelativeGapExp;
    }

	int    ZoneCentroidThrought(void);	
	void   ZoneCentroidThrought(int nIteration);

	double ScaleFactor(void);	
	void   ScaleFactor(double dGap);

	int	   ScaleFactorCursor(void);
	void   ScaleFactorCursor(int a_nCursorIndex);

protected :
	ZNumberEdit m_edtHighwayRelativeGap;
	ZNumberEdit m_edtUEOBaseScaleFactor;
	int         m_nHighwayIteration;
	double      m_dHighwayRelativeGap;
	int         m_nUEOBaseZoneCentroidThrought;
	double      m_dUEOBaseScaleFactor;

    int         m_nHighwayRelativeGapExp;

	int			m_nScaleFactorCursor;

protected:
    /**
    * @brief 다이얼로그 초기화
    */
	virtual BOOL OnInitDialog();
    afx_msg void OnCbnSelchangeComboAdjustFactor();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedOk();

protected:
    CComboBox m_cboAdjustFactor;
    CComboBox m_cboRelativeGapExp;
	CComboBox m_cboScaleFactor;

public:
    afx_msg void OnCbnSelchangeComboRelativeGap();
	afx_msg void OnSelchangeComboScaleFactor();
};
