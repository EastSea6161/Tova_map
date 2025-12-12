#pragma once
//^#include "ResizeDialogEx.h"
#include "Target.h"
//^^ #include "Observer.h"
#include "TTransitPath.h"
#include "BulkDBaseLink.h"
#include "ITransitPathEditOpener.h"
#include "afxwin.h"
class KIOTable;


// KTransitPathEditDlg 대화 상자입니다.
class KTransitPathInsertDlg : public KResizeDialogEx, public KTableObserver
{
	DECLARE_DYNAMIC(KTransitPathInsertDlg)

public:
	KTransitPathInsertDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTransitPathInsertDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4001_TRANSIT_INSERT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	void InitialControl();
private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
private:
	afx_msg void OnReportRowDblClick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReportItemClick  (NMHDR* pNMHDR, LRESULT* pResult);
private:
    CComboBox m_cboDeleteOption;
private:
    afx_msg void OnCbnSelchangeCombo1();
private:
    CComboBox m_cboReverseSeq;    
private:
    CComboBox m_cboDeleteSeq;
private:
    void UpdatePathInfo();
private:
    afx_msg void OnBnClickedRadio1();
    afx_msg void OnBnClickedRadio2();
private:
    void UnCheckedAllSelect();
private:
	KTarget* m_pTarget;
	Integer  m_nxTransitID;
public:
	void SetEditTransit(KTarget* a_pTaget, Integer a_nxTransitID);
	
private:
    std::vector<TTransitPath> m_vecTransitPath;
public:
    std::vector<TTransitPath>& GetTransitPath()
    {
        return m_vecTransitPath;
    }
public:
    void TransitPathPopulate(Integer a_nxDeSelectStationID=0);
private:
    virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);
    KIOTable* m_pLinkTable;
    std::map<Integer, TLinkFTNodeID> m_mapLinkFTNode;
private:
    ITransitPathEditOpener* m_pOpener;
public:
    void SetOpener(ITransitPathEditOpener* a_pOpener)
    {
        m_pOpener = a_pOpener;
    }

private:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedButtonDelete();
    afx_msg void OnBnClickedButtonReverse();
    afx_msg void OnBnClickedCancel();
private:
    std::map<CString, CString> m_mapTransitKeyValue;
public:
    void SetDefaultValue(std::map<CString, CString>& ar_mapTransitKeyValue)
    {
        m_mapTransitKeyValue.clear();
        m_mapTransitKeyValue.insert(ar_mapTransitKeyValue.begin(), ar_mapTransitKeyValue.end());
    }
private:
    void TransitSave();
    void ReverseTransitPath();
    int  FirstRecordSelectIndex();       
private:
    void GenerateOppositeStation(int nIndex);    

private:
    void OppositeStationStatusChange(Integer a_nxStationID, int a_nStationCode);
    void RedrawReportTransitPath();
};


typedef std::shared_ptr<KTransitPathInsertDlg> KTransitPathInsertDlgPtr;