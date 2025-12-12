#pragma once
#include "afxwin.h"
#include "PropertiesDlg.h"
#include "TSystemField.h"

class KProject;
class KTarget;
class KMapView;

/*
                        Geometry     Attribute
동일한 ID 데이터가 존재   (1) A == B    (2) A != B
A에만 존재하는 경우       (3) A(o)-B(x) (3) A(o)-B(x)  : 존노드 처리 필요
B에만 존재하는 경우       (4) A(x)-B(o) (4) A(x)-B(o)  : 존노드 처리 필요
*/

class KNetworkCompareDlg : public KResizeDialogEx, public KTableObserver
{
	DECLARE_DYNAMIC(KNetworkCompareDlg)
public:
	KNetworkCompareDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KNetworkCompareDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_3900_NETWORKCOMPARE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
    virtual BOOL OnInitDialog();
private:
    virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);
private:
    KProject* m_pProject;
private:
    CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
    afx_msg void OnReportItemClick (NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnItemButtonClick (NMHDR* pNotifyStruct, LRESULT* pResult);
private:
    void InitialControl();
    void LoadScenarioCombo();
    void LoadTargetComboA();
    void LoadTargetComboB();
    void SetReportToolTip(KTarget* a_pTargetA, KTarget* a_pTargetB);

private:
	void CheckBoxState();

private:
    CComboBox m_cboScenarioA;
    CComboBox m_cboTargetA;
    CComboBox m_cboScenarioB;
    CComboBox m_cboTargetB;

private:
    KIOTable*  m_pTableA;
    KIOTable*  m_pTableB;

private:
    KPropertiesDlgPtr  m_spPropertiesDlgA;
    KPropertiesDlgPtr  m_spPropertiesDlgB;

private:
    std::map<Integer, TCompareNodeField> m_mapNodesA;
    std::map<Integer, TCompareNodeField> m_mapNodesB;

    std::map<Integer, TCompareLinkField> m_mapLinksA;
    std::map<Integer, TCompareLinkField> m_mapLinksB;

	std::map<Integer, TFCompareNodeField> m_mapFNodesA;
	std::map<Integer, TFCompareNodeField> m_mapFNodesB;

	std::map<Integer, TFCompareLinkField> m_mapFLinksA;
	std::map<Integer, TFCompareLinkField> m_mapFLinksB;

private:
    std::set<int> m_setTypeA;
    std::set<int> m_setTypeB;

private:
    bool m_bCompared;
private:
    void Compare();
    void CompareNode(KTarget* a_pTargetA, KTarget* a_pTargetB);
    void CompareLink(KTarget* a_pTargetA, KTarget* a_pTargetB);
private:
    void Clear();
private:
    afx_msg void OnCbnSelchangeComboScenarioA();
    afx_msg void OnCbnSelchangeComboScenarioB();
    afx_msg void OnCbnSelchangeComboTargetA();
    afx_msg void OnCbnSelchangeComboTargetB();
    afx_msg void OnBnClickedRadioNode();
    afx_msg void OnBnClickedRadioLink();
    afx_msg void OnBnClickedRadioTurn();
    afx_msg void OnBnClickedButtonCompare();
    afx_msg void OnBnClickedCancel();    
    afx_msg void OnBnClickedButtonSaveA();
    afx_msg void OnBnClickedButtonSaveB();
    afx_msg void OnBnClickedCheck1();
    afx_msg void OnBnClickedCheck2();
private:
    void SavaNodeB2A();
    void SavaNodeA2B();

    void SavaLinkA();
    void SavaLinkB();
private:
    class TTempZoneInfo
    {
    public:
        TTempZoneInfo() : m_nxZoneID(0), m_strName(_T("")), m_strFullName(_T("")), m_nMainCategory(0), m_nSubCategory(0)
        {
        }
        ~TTempZoneInfo() {}
    public:
        Integer m_nxZoneID;
        CString m_strName;
        CString m_strFullName;

        int     m_nMainCategory;
        int     m_nSubCategory;
    };
private:
    void FindZoneInfo  (KTarget* pTarget, Integer nxID, TTempZoneInfo& oZoneInfo);
    ITxGeometryPtr FindZoneGeometry(KMapView* pMapView, Integer nxID);
private:
    void InsertZone(KMapView* pMapView, ITxGeometryPtr spGeometry, TTempZoneInfo& oZoneInfo);
    void DeleteZone(KMapView* pMapView, Integer nxID);

private:
    void InsertNode(KMapView* pMapView, __int64 nxID, ITxGeometryPtr spGeometry, TxRecord& oRecord);
    void InsertLink(KMapView* pMapView, __int64 nxID, ITxGeometryPtr spGeometry, TxRecord& oRecord);

private:
    ITxGeometryPtr FineLinkGeometry(KMapView* pMapView, Integer nxID);
private:
    afx_msg void OnBnClickedChkFilter();
    void UpdateFilter();
public:
    std::map<int, std::map<int,int>> m_mapCboType;
    CComboBox m_cboType;
    afx_msg void OnCbnSelchangeCboType();
    void InitCboType();

private:
    afx_msg void SetRadioStatus(UINT nID);
};

typedef std::shared_ptr<KNetworkCompareDlg> KNetworkCompareDlgPtr;