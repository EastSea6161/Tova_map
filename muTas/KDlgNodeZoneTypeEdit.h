#pragma once

class KTarget;

class KDlgNodeZoneTypeEdit : public CDialogEx
{
	DECLARE_DYNAMIC(KDlgNodeZoneTypeEdit)

public:
	KDlgNodeZoneTypeEdit(KTarget* pTarget, CWnd* pParent = NULL);  
	virtual ~KDlgNodeZoneTypeEdit();
	enum { IDD = IDD_201_01_01_IMPORT_TABLE_NODE_TYPE };

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
    bool UpdateType();
public:
    afx_msg void OnBnClickedCancel();

private:
    ITxMapServerPtr   m_spMapServer;    
    TxFeatureLayerPtr m_spLayerNode; 
    TxFeatureLayerPtr m_spLayerZone;    

public:
    void Init(ITxMapServerPtr spMapServer, TxFeatureLayerPtr spLayerNode, TxFeatureLayerPtr spLayerZone) {
        m_spMapServer = spMapServer;
        m_spLayerNode = spLayerNode;
        m_spLayerZone = spLayerZone;
    }
private:
    std::map<__int64, ITxGeometryPtr> m_mapGeoNode;
    
private:
    ThreadResultInfo RThreadInfo;
    static unsigned __stdcall ThreadRun(void* p); 
    void Execute();
private:
    std::set<__int64> LoadNode(KDBaseConPtr spDBase, std::vector<int> vecNodeType);
private:
    void ZoneToNewNodeType(KDBaseConPtr spDBase, int nNewType);
    void NodeToZoneType(KDBaseConPtr spDBase, std::vector<int>& vecNodeType, std::set<__int64>& setID );
};
