#pragma once
//^#include "ResizeDialogEx.h"
//^^ #include "Observer.h"

class KTarget;
class KMapView;
class KIOTable;
class KIOColumn;
// KTocEditorTransit 대화 상자입니다.

class KTocEditorTransit : public KDialogEx, public KTableObserver
{
	DECLARE_DYNAMIC(KTocEditorTransit)

public:
	KTocEditorTransit(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTocEditorTransit();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOC_EDITOR_TRANSIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
private:
	KTarget*         m_pTarget;
	KMapView*        m_pMapView;
	KIOTable*        m_pTransitTable;
public:
	void SetTargetMap(KTarget* a_pTarget, KMapView* a_pMapView);
private:
	virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);
protected:
	CComboBox        m_cboTransitID;
	CStatic          m_stcDummy;
	CXTPPropertyGrid m_wndPropertyGrid;
private:
	CString TransitCategoryName;
protected:
	virtual BOOL OnInitDialog();
protected:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
    afx_msg void OnBnClickedRadioAdd();
    afx_msg void OnBnClickedRadioModify();

    afx_msg void OnCbnSelchangeComboTransitId();
    afx_msg void OnBnClickedButtonStart();
                 void AddTransitStart();
                 void ModifyTransitStart();
    afx_msg void OnBnClickedButtonEnd();

	LRESULT OnPropertyGridItemValueChange(WPARAM wParam, LPARAM lParam);

private:
	void InitialControl();
	void LoadInitialBlankProperties();
    void EditableCheck (CXTPPropertyGridItem* a_pPropertyGridItem, KIOColumn* a_pIOColumn);
	void LoadComboTransitID();
    void LoadProperties();

    bool TransitTemplateDataCheck();
    void GetCurrentTransitTemplateValue(std::map<CString, CString>& ar_mapTemplateValue);
private:
    void ActionFeedback(KDialogFeedback::KEMActionFeedback emActionCode);
    void StopEditing();
	
private:
	__int64 m_nxAddedID;
public:
    afx_msg void OnCbnEditchangeComboTransitId();

public:
    void NotifyRouteAdd(__int64 nxID);

private:
    void UpdateTransit(std::map<CString, CString>& mapKeyValue);
};
