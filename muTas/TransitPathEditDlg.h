#pragma once
//^#include "ResizeDialogEx.h"
#include "TTransitPath.h"
#include "BulkDBaseLink.h"
#include "ITransitPathEditOpener.h"
#include "afxwin.h"
class KTarget;

// KTransitEditDlg 대화 상자입니다.

class KTransitPathEditDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KTransitPathEditDlg)

public:
	KTransitPathEditDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTransitPathEditDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4001_TRANSIT_MODIFY };

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
    CComboBox m_cboReverseSeq;
private:
    CComboBox m_cboDeleteSeq;
private:
    afx_msg void OnBnClickedRadioEditMethod(UINT nID);
private:
    void UpdatePathInfo();

private:
    afx_msg void OnBnClickedOk();
            void TransitSave();
    afx_msg void OnBnClickedButtonDelete();
    afx_msg void OnBnClickedButtonReverse();

    void ReverseTransitPath();
    int  FirstRecordSelectIndex();
private:
    KTarget* m_pTarget;
    Integer  m_nxTransitID;
    std::map<Integer, TLinkFTNodeID> m_mapLinkFTNode;

public:
    void SetEditTransit(KTarget* a_pTaget, Integer a_nxTransitID);
private:
    std::vector<TTransitPath> m_vecTransitPath;
public:
    std::vector<TTransitPath>& GetTransitPath()
    {
        return m_vecTransitPath;
    }
    void SetTransitPath(std::vector<TTransitPath> a_vecPath)
    {
        m_vecTransitPath = a_vecPath;
    }
private:
    ITransitPathEditOpener* m_pOpener;
public:
    void SetOpener(ITransitPathEditOpener* a_pOpener)
    {
        m_pOpener = a_pOpener;
    }
public:
    void TransitPathPopulate(Integer a_nxDeSelectStationID=0);
private:
    CComboBox m_cboTask;
    afx_msg void OnCbnSelchangeCombo1();
    afx_msg void OnBnClickedStop();

private:
    int m_nCommand;
public:
    int GetCommand()
    {
        return m_nCommand;
    }   

private:
    void OnCancel();
    afx_msg void OnBnClickedCancel();
private:
    void GenerateOppositeStation(int nIndex);   
private:
    void OppositeStationStatusChange(Integer a_nxStationID, int a_nStationCode);
    void RedrawReportTransitPath();
};

typedef std::shared_ptr<KTransitPathEditDlg> KTransitPathEditDlgPtr;