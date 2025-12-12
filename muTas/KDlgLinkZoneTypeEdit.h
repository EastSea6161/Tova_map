#pragma once

class KTarget;

class KDlgLinkZoneTypeEdit : public CDialogEx
{
	DECLARE_DYNAMIC(KDlgLinkZoneTypeEdit)
public:
	KDlgLinkZoneTypeEdit(KTarget* pTarget, CWnd* pParent = NULL);  
	virtual ~KDlgLinkZoneTypeEdit();
	enum { IDD = IDD_201_01_02_IMPORT_TABLE_LINK_TYPE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	DECLARE_MESSAGE_MAP()
protected:
    virtual BOOL OnInitDialog();
private:
    CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;

private:
    KTarget* m_pTarget;
private:
    void LoadReportData();
private:
    afx_msg void OnBnClickedOk();
private:
    bool UpdateLinkType();
public:
    afx_msg void OnBnClickedCancel();
};
