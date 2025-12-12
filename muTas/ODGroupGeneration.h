#pragma once
#include "afxwin.h"
//^#include "ResizeDialogEx.h"
#include "GroupGenerationInfo.h"

class KTarget;
class TFormulaField;
class TZoneClassRange;
class KIOColumn;
class KIntKey;

class KODGroupGeneration : public KResizeDialogEx
{
    DECLARE_DYNAMIC(KODGroupGeneration)
public:
    KODGroupGeneration(KTarget* a_pTarget, CWnd* pParent = NULL);   
    virtual ~KODGroupGeneration();
    enum { IDD = IDD_2198_ODGROUP_GEN };
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    
    DECLARE_MESSAGE_MAP()
private:
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
private:
    afx_msg void OnCbnSelchangeCboGroupName();
    void LoadCodeGroup();
    afx_msg void OnBnClickedButtonModify();            
    afx_msg void OnBnClickedButtonExpression();

private:
    KTarget*  m_pTarget;
    CComboBox m_cboGroupName;
private:
    CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;    
    CXTPOfficeBorder<CXTPReportControl,false> m_wndReport2;   
    void InitReportCtrl();
    void LoadReportData();
private:
    CString m_strPreValue;
    CString m_strPreCaption;
    afx_msg void OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
    afx_msg void OnReportItemValueChanged(NMHDR* pNotifyStruct, LRESULT* pResult);
    void RecalRange();
private:
    std::vector<TFormulaField> m_vecFomulaField;
private:
    CComboBox m_cboOutColumnName;
    void LoadOutColumnName();
    afx_msg void OnBnClickedCheckNew();
private:
    afx_msg void OnBnClickedOk();
    bool InputCheck(bool bCheckOutColumn=true);
    static unsigned __stdcall ThreadCaller(void* p); 
    void Run();
    void GetSedValue(std::vector<CString>& a_vecField, KIDKeyDoubleRecords& ar_valuesRecord);
private:
    std::vector<TZoneClassRange> m_vecZoneClass;
private:
    void ZoneClass();
    int  FindZoneClass(double a_dValue);
    void UpdateParaODTable(KIOColumn* a_pColumn, std::map<Integer, int>& ar_mapValue);
    int  AddCodeGroup();
    KIOColumn* AddParaODCodeColumn(int a_nCodeGroup, CString a_strColumnName);

private:
    int  m_nRunResult;
    bool m_bCalcData;
    std::map<Integer, int> m_mapResult;
    std::vector<double>    m_vecCalcData;
private:
    afx_msg void OnBnClickedButtonCalc();

private:
    std::map<KIntKey, TCodeName> m_mapKeyCodeName;
    void KeyCodeName();
};
