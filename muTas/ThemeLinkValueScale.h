#pragma once
#include "SymbologyThemeLink.h"

class KTarget;
class KMapView;
class KIOTable;
class KIOColumn;
class KExpressionChecker;
class KIDKeyDoubleRecords;

// KThemeLinkValueScale 대화 상자입니다.

class KThemeLinkValueScale : public KDialogEx,  public KTableObserver, public ISymbologyThemeLink
{
	DECLARE_DYNAMIC(KThemeLinkValueScale)
	
	friend class KSymbologyThemeLinkDlg;

public:
	KThemeLinkValueScale(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KThemeLinkValueScale();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOC_LINK_VALUE_SCALE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	CString  m_strPreSelectedColumnName;

private:
	KIOTable*        m_pLinkTable;

public:
	void SetTargetMap(KTarget* a_pTarget, KMapView* a_pMapView);
	void SetLinkTable(KIOTable* a_pLinkTable);

private:
	void InitFilterData(void);
	void UpdateTxtFilter(void);

	CString GenQueryFilter(void);
    CString GenNotQueryFilter(void);
	void    GenFilter(std::set<int> &a_setCheckType);

private:
	BOOL m_bCheckedFilter;
	vector<TFilterInfo*> m_vecFilterInfo;

protected:
	CComboBox       m_cboCategory;
	CXTColorPicker  m_btnDefaultSymbolColorPicker;
	CXTColorPicker  m_btnOtherSymbolColorPicker;

protected:
	void GetCategoryField       (std::vector<KIOColumn*> &a_vecColumnCategory);
	void LoadInitialCategory    (std::vector<KIOColumn*> &a_vecColumnCategory);
	void InitExpressionChecker  (std::vector<KIOColumn*> &a_vecColumnCategory);
	void LoadCategoryFieldRecord(std::vector<KIOColumn*> &a_vecColumnCategory);
	
	void FieldSelectChangeAction();
	void ClearLinkThemes();
	
	void Draw(bool a_bLabel=false);
	bool InvalidateInput();

protected:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void   OnSize(UINT nType, int cx, int cy);	

protected:	
	virtual BOOL OnInitDialog();	
	afx_msg void OnCbnSelchangeComboField();
	afx_msg void OnBnClickedCheckAllOtherValues();
		void ControlCheckOutRange();
	afx_msg void OnBnClickedFilter();
	afx_msg void OnBnClickedBtnFilter();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	static  bool Descending(TDrawLinkInfo left, TDrawLinkInfo right);

private:
	virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);

protected:
	afx_msg void OnBnClickedButtonFormula();
		bool CalculateResult();
		static unsigned __stdcall CalculateResultThreadCaller(void* p);
private:
	  void  GetThemeResultSettingInfo(TThemeResultSettingInfo &a_oTThemeResultSettingInfo);
	  void  GetDrawDataByNormal    (TThemeResultSettingInfo &a_oTThemeResultSettingInfo, std::vector<TDrawLinkInfo> &a_vecOutDrawLinkInfo);
	  void  GetDrawDataByCalculate (TThemeResultSettingInfo &a_oTThemeResultSettingInfo, std::vector<TDrawLinkInfo> &a_vecOutDrawLinkInfo);
		bool  Calculate2Width        (BOOL a_bChecked, BOOL a_bIncludeZero, TThemeResultSettingInfo &a_oTThemeResultSettingInfo, double a_dValue, bool &a_bOutRange, double &a_dWidth, CString &a_strLabel);
private:
	void   SetEditControlMinMax   (double a_dMinVal, double a_dMaxVal);
	
	CString                   m_strFunction;
	KExpressionChecker*       m_pExpressionChecker;
	std::map<Integer, double> m_mapCalculateResult;
	std::vector<KIOColumn*>   m_vecColumnCategory;
	KIDKeyDoubleRecords*      m_pCategoryFieldRecord;
	std::map<Integer, int>    m_mapLinkIdType;
	bool                      m_bCalcResult;

	double       m_dMinCalculateVal;
	double       m_dMaxCalculateVal;

	afx_msg void OnBnClickedRadioDrawData(UINT nID);
		void ControlRadioDrawData();

public:
    void NodeLayerSettingChanged();
};
