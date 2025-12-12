#pragma once
#include "afxwin.h"

#include "TThemeType.h"
#include "ImTasDBase.h"
#include "ThemeSetFilterDlg.h"

#include "TThemeType.h"
#include "BulkDBaseNode.h"
#include "TDrawNodeInfo.h"

class KTarget;
class KMapView;
class KIOTable;
class KIOColumn;
class KExpressionChecker;
class KIDKeyDoubleRecords;
// KThemeNodeValueClassify 대화 상자입니다.

class KThemeNodeValueClassify : public KDialogEx, public KTableObserver
{
	DECLARE_DYNAMIC(KThemeNodeValueClassify)

	friend class KSymbologyThemeNodeDlg;

public:
	KThemeNodeValueClassify(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KThemeNodeValueClassify();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOC_NODE_VALUE_CLASSIFY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	KTarget*         m_pTarget;
	KMapView*        m_pMapView;
	KIOTable*        m_pNodeTable;
public:
	void SetTargetMap(KTarget* a_pTarget, KMapView* a_pMapView);
	void SetNodeTable(KIOTable* a_pNodeTable);

protected:
	virtual BOOL   OnInitDialog();	
protected:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void   OnSize(UINT nType, int cx, int cy);	
	afx_msg void   OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
private:
	CFont    m_oQbicDefaultMarkerFont;
	CButton  m_btnMarker;
	int      m_nSelectedMarkerIndex; 
	COLORREF m_clrSelectedMarkerColor;

	CButton  m_btnMarkerOther;
	int      m_nOtherMarkerIndex; 
	COLORREF m_clrOtherMarkerColor;
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
	
	void ClearNodeThemes();
	void Clear();
	void Draw(bool a_bLabel=false);
	bool InvalidateInput();

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
	void LoadReportCtrlData(bool a_bStlyeReset = true);
	void LoadSavedStyle    (bool a_bStlyeReset = true);
	void ResetSavedStlye       (std::vector<TNodeTemplateValue> &a_vecTemplateValue);
	void RefreshOnlySavedRange (std::vector<TNodeTemplateValue> &a_vecTemplateValue);
	void LoadUserStyle     (bool a_bStlyeReset = true);
	void ResetUserStyle        (std::vector<TDevideDataInterval> &a_ovecDevideInfo);
	void RefreshOnlyUserRange  (std::vector<TDevideDataInterval> &a_ovecDevideInfo);
		void GetMinDecreaseMaxIncrease(int a_nDecimalPoint, double &a_dMinDecrease, double &a_dMaxIncrease);
		int  GetDisplayDecimalPoint   (int a_nDecimalPoint);

	void SetEditControlMinMax( double a_dMinVal, double a_dMaxVal );

private:
	virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);

private:
	void         InitFitlerData(void);
	afx_msg void OnBnClickedBtnFilter();
	afx_msg void OnBnClickedFilter();
	CString      GenQueryFilter(void);
	CString      GenNotQueryFilter( void );
	void         GenFilter(std::set<int> &a_setCheckType);
	void         UpdateTxtFilter(void);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	BOOL m_bCheckedFilter;
	vector<TFilterInfo*> m_vecFilterInfo;

private:
	afx_msg void OnBnClickedButtonFormula();
		bool CalculateResult();
		static unsigned __stdcall CalculateResultThreadCaller(void* p);
private:
	void GetDataByCalculate    (std::vector<double> &a_vecData);
	void GetDrawDataByCalculate(std::vector<TNodeIDValue> &ar_vecData, std::vector<TNodeIDValue> &ar_vecOutFilterData);
	static bool Descending(TDrawNodeInfo left, TDrawNodeInfo right);

	CString                   m_strFunction;
	KExpressionChecker*       m_pExpressionChecker;
	std::map<Integer, double> m_mapCalculateResult;
	std::vector<KIOColumn*>   m_vecColumnCategory;
	KIDKeyDoubleRecords*      m_pCategoryFieldRecord;
	std::map<Integer, int>    m_mapNodeIdType;
	bool                      m_bCalcResult;
	int                       m_nRealDigit;

	double                    m_dMinCalculateVal;
	double                    m_dMaxCalculateVal;

	afx_msg void OnBnClickedRadioDrawData(UINT nID);
		void ControlRadioDrawData();
	afx_msg void OnBnClickedCheckAll();
		void ControlCheckAll();
	afx_msg void OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);

	afx_msg void OnBnClickedCheckAllOtherValues();
		void ControlCheckOutRange();
	afx_msg	void OnBnClickedButtonMarkerOther();
};
