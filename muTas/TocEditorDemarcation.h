#pragma once
//^#include "ResizeDialogEx.h"
//^^ #include "Observer.h"
#include "KEMMapMode.h"

class KTarget;
class KMapView;
class KPersonalGeoDatabase;
class KIOTable;
class KIOColumn;
class KIOItem;
// KTocEditorDemarcation 대화 상자입니다.
#define KDialogExBase KDialogEx
class KTocEditorDemarcation : public KDialogExBase, public KTableObserver
{
	DECLARE_DYNAMIC(KTocEditorDemarcation)

public:
	KTocEditorDemarcation(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTocEditorDemarcation();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOC_EDITOR_DEMARCATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
private:
	KTarget*  m_pTarget;
	KMapView* m_pMapView;
	KIOTable* m_pZoneTable;
public:
	void SetTargetMap(KTarget* a_pTarget, KMapView* a_pMapView);
private:
	virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);
protected:
	virtual BOOL OnInitDialog();
protected:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);	
	afx_msg void OnSize(UINT nType, int cx, int cy);
private:
	CString DemarcationCategoryName;
protected:
	CStatic m_stcDummy;
	CXTPPropertyGrid m_wndPropertyGrid;
private:
	void LoadTemplateComboBoxDemarcation();
	void LoadInitialBlankProperties();
	void LoadDemarcationPropertiesValue();
	
	void EditableCheck (CXTPPropertyGridItem* a_pPropertyGridItem, KIOColumn* a_pIOColumn);
private:
	void InitialControl();
private:
	CComboBox    m_cboModifyTask;
	CComboBox    m_cboTemplateDemarcation;
	int          m_nPreDemarcationTemplateID;

	afx_msg void OnBnClickedRadioAdd();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedRadioModify();
	afx_msg void OnBnClickedButtonModify();
	afx_msg void OnCbnSelchangeComboModifySelect();
	afx_msg void OnBnClickedButtonTemplateDeleteDemarcation();
	afx_msg void OnBnClickedButtonTemplateSaveDemarcation();
	afx_msg void OnCbnSelchangeComboTemplateDemarcation();
    afx_msg void OnBnClickedButton1();

	LRESULT OnPropertyGridItemValueChange(WPARAM wParam, LPARAM lParam);
private:
	bool    DemarcationTemplateDataCheck();
	CString ConvertTemplateSaveValueDemarcation();
private:
	int DemarcationTemplateType;
private:
    void ActionFeedback(KDialogFeedback::KEMActionFeedback emActionCode);
    void StopEditing();
public:    
    afx_msg void OnBnClickedButton2();
	
};
