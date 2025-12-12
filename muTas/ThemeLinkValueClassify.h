#pragma once
#include "SymbologyThemeLink.h"

class KTarget;
class KMapView;
class KIOTable;
class KIOColumn;
class KExpressionChecker;
class KIDKeyDoubleRecords;


class KThemeLinkValueClassify : public KDialogEx, public KTableObserver, public ISymbologyThemeLink
{
	DECLARE_DYNAMIC(KThemeLinkValueClassify)

	friend class KSymbologyThemeLinkDlg;

public:
	KThemeLinkValueClassify(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KThemeLinkValueClassify();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOC_LINK_VALUE_CLASSIFY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	KIOTable*        m_pLinkTable;
public:
	void SetTargetMap(KTarget* a_pTarget, KMapView* a_pMapView);
	void SetLinkTable(KIOTable* a_pLinkTable);

protected:
	virtual BOOL   OnInitDialog();	
protected:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void   OnSize(UINT nType, int cx, int cy);	
	afx_msg void   OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
private:
	CButton  m_btnLineSymbol;
	COLORREF m_clrSelectedLineColor;
private:
    CXTColorPicker  m_btnOtherSymbolColorPicker;
protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
	CString m_strPreValue;
	CString m_strPreCaption;
	afx_msg void OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportItemValueChanged(NMHDR* pNotifyStruct, LRESULT* pResult);
	void RecalRange();
protected:
	CComboBox m_cboCategory;
	CComboBox m_cboStyle;
	CComboBox m_cboClass;
	CComboBox m_cboDigit;

private:
	CFont   m_oQbicDefaultMarkerFont;
	int     m_MarkerIntex;

protected:
	afx_msg void OnCbnSelchangeComboField();
	void FieldSelectChangeAction();
	afx_msg void OnBnClickedRadio();
	void StyleRadioSelectChangeAction();
	afx_msg void OnCbnSelchangeComboStyle();
	void StyleSelectChangeAction();
	afx_msg void OnCbnSelchangeComboClass();

	afx_msg void OnBnClickedButtonStyleSave();
	afx_msg void OnBnClickedButtonStyleEdit();

	afx_msg void OnBnClickedButtonMarker();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnCbnSelchangeComboDigit();

	void Draw(bool a_bLabel=false);
	bool InvalidateInput();
	void MakeDrawData(std::vector<TDrawLinkInfo> &a_vecOutDrawLinkInfo);
	
	afx_msg void OnBnClickedBtnFilter();
	afx_msg void OnBnClickedFilter();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	CString m_strPreSelectedColumnName;
	int     m_nPreSelectedStyleID;
	int     m_nPreSelectedClass;
private:
	void LoadInitialData();

	void GetCategoryField       (std::vector<KIOColumn*> &a_vecColumnCategory);
	void LoadCategoryFieldRecord(std::vector<KIOColumn*> &a_vecColumnCategory);
	void LoadInitialCategory    (std::vector<KIOColumn*> &a_vecColumnCategory);
	void InitExpressionChecker  (std::vector<KIOColumn*> &a_vecColumnCategory);

	void LoadInitialStyle();
	void LoadInitialClass();
	void LoadInitialDigit();
	void LoadReportCtrlData    (bool a_bStlyeReset = true);
	void LoadSavedStyle        (bool a_bStlyeReset = true);
	void ResetSavedStlye        (std::vector<TLinkTemplateValue> &a_vecTemplateValue);
	void RefreshOnlySavedRange (std::vector<TLinkTemplateValue> &a_vecTemplateValue);
	void LoadUserStyle         (bool a_bStlyeReset = true);
	void ResetUserStyle        (std::vector<TDevideDataInterval> &a_ovecDevideInfo);
	void RefreshOnlyUserRange  (std::vector<TDevideDataInterval> &a_ovecDevideInfo);
		void GetMinDecreaseMaxIncrease(int a_nDecimalPoint, double &a_dMinDecrease, double &a_dMaxIncrease);
		int  GetDisplayDecimalPoint   (int a_nDecimalPoint);
	static bool Descending(TDrawLinkInfo left, TDrawLinkInfo right);

	void SetEditControlMinMax( double a_dMinVal, double a_dMaxVal );

private:
	virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);

private:
	BOOL m_bCheckedFilter;
	vector<TFilterInfo*> m_vecFilterInfo;
	
	void UpdateTxtFilter(void);
	void InitFilterData(void);
	CString GenQueryFilter(void);
    CString GenNotQueryFilter(void);
	void GenFilter(std::set<int> &a_setCheckType);

private:
	afx_msg void OnBnClickedButtonFormula();
		bool CalculateResult();
		static unsigned __stdcall CalculateResultThreadCaller(void* p);
private:	
	void GetDataByCalculate(std::vector<double> &a_vecData);
	void GetDrawDataByCalculate(std::vector<TLinkIDValue> &ar_vecData, std::vector<TLinkIDValue> &ar_vecOutFilterData);

	CString                   m_strFunction;
	KExpressionChecker*       m_pExpressionChecker;
	std::map<Integer, double> m_mapCalculateResult;
	std::vector<KIOColumn*>   m_vecColumnCategory;
	KIDKeyDoubleRecords*      m_pCategoryFieldRecord;
	std::map<Integer, int>    m_mapLinkIdType;
	bool                      m_bCalcResult;
	int                       m_nRealDigit;

	double                    m_dMinCalculateVal;
	double                    m_dMaxCalculateVal;

	afx_msg void OnBnClickedRadioDrawData(UINT nID);
		void ControlRadioDrawData();
	afx_msg void OnBnClickedCheckAllOtherValues();
		void ControlCheckOutRange();
	afx_msg void OnBnClickedCheckAll();
		void ControlCheckAll();
	afx_msg void OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
public:
    void NodeLayerSettingChanged();
};
