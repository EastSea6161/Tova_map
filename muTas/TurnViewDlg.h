#pragma once
#include "GraphicTurnVolumeDisp.h"
#include "afxwin.h"

struct TTurnVolumeSettingInfo
{
	double dMaxVolume;
	double dMinVolume;
	double dMinSize;
	double dMaxSize;
};

class KMapView;

class KTurnViewDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KTurnViewDlg)

public:
	KTurnViewDlg(std::map<int, CString> &a_mapModeIndexName, std::map<KODKey, std::vector<double>> &a_mapFTNodeVolume, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTurnViewDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_0831_TURNVIEW };
protected:
	DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.	
    HICON m_hIcon;
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
private:
private:
    KGraphicTurnVolumeDisp m_oGraphicTurnVolumeDisp;
private:
    bool  m_bDrawRoad;
    CRect m_oTurnViewRect;
private:
    void Refresh();
private:
    void SetData(); //Test..

public:
	void SetPrepareData(Integer &a_nxAtNodeID, CString &a_strAtNodeName, TDrawNodeXY &a_oCenterNodeXY, std::vector<TDrawNodeXY> &a_vecNodes);
private:
	Integer                      m_nxAtNodeID; 
	CString                      m_strAtNodeName;
	std::vector<TDrawNodeXY>     m_vecNodes;  
	TDrawNodeXY                  m_oCenterNodeXY;

	std::map<int, CString>                &m_mapModeIndexName;
	std::map<KODKey, std::vector<double>> &m_mapFTNodeVolume;

	KMapView*    m_pMapView;

	double       m_dMaxVolume;
	double       m_dMinVolume;

private:
	void         InitComboMode();
	void         InitReportHeader();
	void         InitReportData();
	void         UpdateReportData();
	void         ResizeComponent();

	void         DrawTurnVolume();
	void         CalculateValue2Width(double a_dVolume, TTurnVolumeSettingInfo &a_oSettingInfo, double& dWidth );

private:         
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReport;
	CComboBox    m_cboMode;
	CWnd*        m_pParentWnd;

private:
	afx_msg void OnCbnSelchangeComboMode();
	afx_msg void OnBnClickedCheckAll();
	void         ControlCheckAll();
	afx_msg void OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedCheckLabel();
	void         ControlCheckLabel();
	afx_msg void OnBnClickedCheckLine();
	void         ControlCheckLine();
	afx_msg void OnBnClickedCancel();
	
};
