#pragma once

#include "LinkAnalysisInfo.h"
#include "TDrawLinkInfo.h"
#include "afxwin.h"
//^#include "ResizeDialogEx.h"
#include "PathVolumeInfo.h"

class KIOTable;
class KIOColumns;
class KIOColumn;

// KLinkAnalysisMapDisplayDlg 대화 상자입니다.

class KLinkAnalysisMapDisplayDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KLinkAnalysisMapDisplayDlg)

public:
	KLinkAnalysisMapDisplayDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KLinkAnalysisMapDisplayDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5090_LinkAnalysisMapDisplayDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	virtual BOOL OnInitDialog();

private:
    /**
    * @brief Report Control Header 초기화
    */
    void InitReportHeader();

    /**
    * @brief widht value setting Combo Boxes 초기화
    */
	void InitWidthValCombos();

    /**
    * @brief Combo Box 초기화
    */
    void InitCombo();

    /**
    * @brief 사용자가 선택한 색상 정의
    * @param[in] nSelectIndex - 색상 정보 인덱스
    */
    void SetColorRampInfo(int nSelectIndex);


	void SetDefaultWidthInfo();
	
	/**
    * @brief Color Ramp Combo Box 초기화
    */
    void InitialColorRampCtrl();
	
	void UpdateBushMinMax();

	void InitBushVolMinMax();

	void SetMinBushVol(double &a_dBushVol);

	void SetMaxBushVol(double &a_dBushVol);

public:
	
	bool OldGetLinkInfo(std::vector<TDrawLinkInfo> &a_vecDrawLinkInfo);

	bool NewGetLinkInfo(std::vector<TDrawLinkInfo> &a_vecDrawLinkInfo, bool a_bShowErrMsg);
	
	void SetLinkVolume(std::vector<TLinkVolume> a_vecLinkVolume);

	BOOL GetChkDisplayLabel()
	{
		UpdateData(TRUE);
		return m_bChkDisplayLabel;
	}

    /**
    * @brief Report Control Data 초기화
    */
    void InitReportData();
	
	void ClearReportData();

	void InitSettingInfo();

private:

	CImageList       m_colorRampImageList;

	COLORREF         m_clrStart;
	COLORREF         m_clrEnd;

	std::vector<TLinkVolume>       m_vecLinkVolume;
	std::vector<TMinMaxInfo>       m_vecMinMaxInfo;

	std::vector<double>            m_vecWidth;
	std::vector<double>            m_vecIncrement;

	double                     m_dMaxBushVol;
	double                     m_dMinBushVol;

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
	CComboBox         m_cboClasses;
	CComboBoxEx       m_cboColorRamp;    
	CComboBox         m_cmbWidth;
	CComboBox         m_cmbIncrement;
	BOOL              m_bChkOptimizeDevide;
	BOOL              m_bChkDisplayLabel;

	CXTPColorPicker   m_cpBackColor;
	COLORREF          m_clrBack;

protected:
	afx_msg void OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnCbnSelchangeCboClassescount();
	afx_msg void OnCbnSelchangeCboColorramp();
	afx_msg void OnCbnSelchangeCboWidth();
	afx_msg void OnCbnSelchangeCboIncretment();
	afx_msg void OnBnClickedCheckOptimizedevide();
	afx_msg void OnBnClickedCheckDisplayLabel();
	afx_msg void OnSelEndOkBackClr();
	bool UserBushInputCheck(bool a_bShowErrMsg);
	void CalculateValue2Width(double a_dTrip, TPathVolumeSettingInfo &a_oSettingInfo, double& dWidth );
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
