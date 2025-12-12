#pragma once

#include "TThemeType.h"
//^#include "ResizeDialogEx.h"
//^^ #include "Observer.h"

#include "BulkDBaseDemarcation.h"

class KTarget;
class KMapView;
class KIOTable;
class KIOColumn;
class KExpressionChecker;
class KIDKeyDoubleRecords;
// KThemeZoneValueClassify 대화 상자입니다.

class KThemeZoneValueClassify : public KDialogEx, public KTableObserver
{
	DECLARE_DYNAMIC(KThemeZoneValueClassify)

	friend class KSymbologyThemeZoneDlg;

public:
	KThemeZoneValueClassify(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KThemeZoneValueClassify();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOC_ZONE_VALUE_CLASSIFY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	KTarget*         m_pTarget;
	KMapView*        m_pMapView;
	
	KIOTable*        m_pZoneTable;
    KIOTable*        m_pSEDTable;
	bool             m_bZoneTable;
public:
	void SetTargetMap(KTarget* a_pTarget, KMapView* a_pMapView, bool a_bZoneTable);
	//void SetZoneTable(KIOTable* a_pZoneTable);
	void ChangeTable(bool a_bZoneTable);

protected:
	virtual BOOL   OnInitDialog();	
protected:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void   OnSize(UINT nType, int cx, int cy);	
	afx_msg void   OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
private:
	CButton  m_btnLineSymbol;
	COLORREF m_clrSelectedLineColor;
protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReport;
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
	void ClearZoneThemes();
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
	void LoadReportCtrlData(bool a_bInit=true);

	void SetClassMinMax(double a_dMinDecrease, double a_dMaxIncrease, int a_nDisplayDecimalPoint,
		std::vector<TDevideDataInterval> &a_vecDevideInfo, size_t a_nxCount, size_t a_nIdx, CString &a_strValue, CString &a_strCaption);
	void LoadSavedStyle();
	void LoadUserStyle(bool a_bInit);
		void GetMinDecreaseMaxIncrease(int a_nDecimalPoint, double &a_dMinDecrease, double &a_dMaxIncrease);
		int  GetDisplayDecimalPoint   (int a_nDecimalPoint);

	void SetEditControlMinMax( double a_dMinVal, double a_dMaxVal );

private:
	virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);

private:
	afx_msg void OnBnClickedButtonFormula();
		bool CalculateResult();
		static unsigned __stdcall CalculateResultThreadCaller(void* p);
private:
	void GetDataByCalculate(std::vector<double> &a_vecData);
	void GetDrawDataByCalculate(std::vector<TZoneIDValue> &ar_vecData);

	CString                   m_strFunction;
	KExpressionChecker*       m_pExpressionChecker;
	std::map<Integer, double> m_mapCalculateResult;
	std::vector<KIOColumn*>   m_vecColumnCategory;
	KIDKeyDoubleRecords*      m_pCategoryFieldRecord;
	bool                      m_bCalcResult;
	int                       m_nRealDigit;
	
	CXTColorPicker            m_btnOtherSymbolColorPicker;

	double       m_dMinCalculateVal;
	double       m_dMaxCalculateVal;

	afx_msg void OnBnClickedRadioDrawData(UINT nID);
		void ControlRadioDrawData();
	afx_msg void OnBnClickedCheckAll();
		void ControlCheckAll();
	afx_msg void OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedCheckAllOtherValues();
		void ControlCheckOutRange();
		
};
