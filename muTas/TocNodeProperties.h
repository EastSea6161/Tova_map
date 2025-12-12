#pragma once

class KTarget;
class KMapView;
class KIOTable;
// KTocNodeProperties 대화 상자입니다.

class KTocNodeProperties : public KDialogEx, public KTableObserver
{
	DECLARE_DYNAMIC(KTocNodeProperties)

public:
	KTocNodeProperties(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTocNodeProperties();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOC_NODE_PROPERTIES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	int   m_nMarker; 
private:
	KTarget*   m_pTarget;
	KMapView*  m_pMapView;
    KIOTable*  m_pTable;
private:
	ITxFeatureLayerPtr m_spFLayer;
public:
	void SetTargetMap(KTarget* a_pTarget, KMapView* a_pMapView);
private:
    virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);
private:
	CFont           m_fntMarker;
	CButton         m_btnMarker;
	CXTColorPicker  m_btnSymbolColorPicker;

protected:
	virtual BOOL   OnInitDialog();	
protected:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void   OnSize(UINT nType, int cx, int cy);	
	
protected:	
	afx_msg void OnBnClickedButtonMarker();
	
protected:
	afx_msg void OnBnClickedRadio1();
	        void ActionShowLayerAllScaleSelect();
	afx_msg void OnBnClickedRadio2();
	        void ActionShowLayerZoomScaleSelect();
	    afx_msg void OnBnClickedButtonMinScale();
	    afx_msg void OnBnClickedButtonMaxScale();
	afx_msg void OnBnClickedReset();
		void Reset();
	afx_msg void OnBnClickedButtonApply();
		void Apply();
	afx_msg void OnBnClickedCheckLabel();
	    void LabelCheckEvent();
	afx_msg void OnBnClickedButtonLabel();
	    CString m_strLabelFields;
		void LabelFieldNameDisplay(CString a_strColumnNames);
        void LoadDefaultLabel();
	afx_msg void OnBnClickedCheckLabelScale();        
	afx_msg void OnBnClickedButtonLabelScale();
	
private:
	void LoadInitTemplate(void);
	void LoadRptCtrlData(void);
	void InitRptHeader(void);
	void UpdateRptData(void);
	void UpdateRptDataPart(int a_nRedrawMode);

	CComboBox         m_cboTemplate;
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReport;
	bool              m_bUseEditBox;

private:
	afx_msg void OnBnSelChangeTemplate(void);
	afx_msg void OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
    afx_msg void OnBnClickedButtonMarkerColor();
    afx_msg void OnEnChangeEditNodeSize();
    afx_msg void OnBnClickedCheckTemplate();
    afx_msg void OnBnClickedRadioType( UINT nID );
	
	afx_msg void OnReportColumnRButtonClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectAll();
	afx_msg void OnDeSelectAll();
			void ReportSelectControl(BOOL a_bSelect);
	afx_msg void OnEnSetfocusEditNodeSize();
	afx_msg void OnEnKillfocusEditNodeSize();

private:
    CString GetCheckedTypeToWhere();
};
