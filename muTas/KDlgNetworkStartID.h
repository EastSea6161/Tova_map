#pragma once

class KMapView;

class KDlgNetworkStartID : public CDialogEx
{
	DECLARE_DYNAMIC(KDlgNetworkStartID)

public:
	KDlgNetworkStartID(KMapView* pMapView, CWnd* pParent = NULL); 
	virtual ~KDlgNetworkStartID();
	enum { IDD = IDD_8000_ID_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	DECLARE_MESSAGE_MAP()
protected:	
    virtual BOOL OnInitDialog();
private:
    KMapView* m_pMapView;
protected:
    afx_msg void OnBnClickedOk();
};
