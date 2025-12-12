#pragma once

class KTarget;

class KDlgTurnTypeEdit : public CDialogEx
{
	DECLARE_DYNAMIC(KDlgTurnTypeEdit)
public:
	KDlgTurnTypeEdit(KTarget* pTarget, CWnd* pParent = NULL);  
	virtual ~KDlgTurnTypeEdit();
	enum { IDD = IDD_201_01_04_IMPORT_TABLE_TURN_TYPE };

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
    void LoadTurnInfo();
private:
    afx_msg void OnBnClickedOk();
private:
    void UpdateTurnType();
public:
    afx_msg void OnBnClickedCancel();
};
