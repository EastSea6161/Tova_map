#pragma once
//^#include "ResizeDialogEx.h"
//^^ #include "Observer.h"

#include "ThemeSetFilterDlg.h"
#include "TThemeType.h"
#include "TDrawNodeInfo.h"

class KTarget;
class KMapView;
class KIOTable;
class KIOColumn;
class KExpressionChecker;
class KIDKeyDoubleRecords;
// KThemeNodeValueScale 대화 상자입니다.

class KThemeNodeValueScale : public KDialogEx, public KTableObserver
{
	DECLARE_DYNAMIC(KThemeNodeValueScale)

	friend class KSymbologyThemeNodeDlg;

public:
	KThemeNodeValueScale(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KThemeNodeValueScale();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOC_NODE_VALUE_SCALE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	int      m_nSelectedMarkerIndex; 
	COLORREF m_clrSelectedMarkerColor;
	CString  m_strPreSelectedColumnName;

	int      m_nOtherMarkerIndex; 
	COLORREF m_clrOtherMarkerColor;
private:
	KTarget*         m_pTarget;
	KMapView*        m_pMapView;
	KIOTable*        m_pNodeTable;
public:
	void SetTargetMap(KTarget* a_pTarget, KMapView* a_pMapView);
	void SetNodeTable(KIOTable* a_pNodeTable);

private:
	void InitFitlerData(void);

private:
	vector<TFilterInfo*> m_vecFilterInfo;

private:
	CFont           m_oQbicDefaultMarkerFont;
	CButton         m_btnMarker;
	CButton         m_btnMarkerOther;
protected:
	CComboBox m_cboCategory;
protected:
	void GetCategoryField       (std::vector<KIOColumn*> &a_vecColumnCategory);
	void LoadCategoryFieldRecord(std::vector<KIOColumn*> &a_vecColumnCategory);
	void LoadInitialCategory    (std::vector<KIOColumn*> &a_vecColumnCategory);
	void InitExpressionChecker  (std::vector<KIOColumn*> &a_vecColumnCategory);

protected:
	virtual BOOL   OnInitDialog();	
protected:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void   OnSize(UINT nType, int cx, int cy);	
	afx_msg void   OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:	
	afx_msg void OnBnClickedButtonMarker();
	
	afx_msg void OnCbnSelchangeComboField();
		void FieldSelectChangeAction();

		void ClearNodeThemes();
		void Clear();

		void Draw(bool a_bLabel=false);
		bool InvalidateInput();
	afx_msg void OnBnClickedCheckAllOtherValues();
		void ControlCheckOutRange();
	afx_msg void OnBnClickedButtonMarkerOther();

private:
	virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);

private:
	afx_msg void OnBnClickedBtnFilter();
	afx_msg void OnBnClickedFilter();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	BOOL m_bCheckedFilter;

	CString GenQueryFilter(void);
	CString GenNotQueryFilter( void );
	void    UpdateTxtFilter(void);
	void    GenFilter(std::set<int> &a_setCheckType);

private:
	afx_msg void OnBnClickedButtonFormula();
		bool CalculateResult();
		static unsigned __stdcall CalculateResultThreadCaller(void* p);

private:
	void  GetThemeResultSettingInfo(TThemeResultSettingInfo &a_oTThemeResultSettingInfo);
	void  GetDrawDataByNormal      (TThemeResultSettingInfo &a_oTThemeResultSettingInfo, std::vector<TDrawNodeInfo> &a_vecOutDrawInfo);
	void  GetDrawDataByCalculate   (TThemeResultSettingInfo &a_oTThemeResultSettingInfo, std::vector<TDrawNodeInfo> &a_vecOutDrawInfo);
		bool  Calculate2Width      (BOOL a_bChecked, BOOL a_bIncludeZero, TThemeResultSettingInfo &a_oTThemeResultSettingInfo, double a_dValue, bool &a_bOutRange, double &a_dSize, CString &a_strLabel);

	static  bool Descending(TDrawNodeInfo left, TDrawNodeInfo right);

private:
	void  SetEditControlMinMax     (double a_dMinVal, double a_dMaxVal);

private:
	CString                   m_strFunction;
	KExpressionChecker*       m_pExpressionChecker;
	std::map<Integer, double> m_mapCalculateResult;
	std::vector<KIOColumn*>   m_vecColumnCategory;
	KIDKeyDoubleRecords*      m_pCategoryFieldRecord;
	std::map<Integer, int>    m_mapNodeIdType;
	bool                      m_bCalcResult;

	double       m_dMinCalculateVal;
	double       m_dMaxCalculateVal;

private:
	afx_msg void OnBnClickedRadioDrawData(UINT nID);
		void ControlRadioDrawData();
};
