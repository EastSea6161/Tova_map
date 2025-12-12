#pragma once
//^#include "ResizeDialogEx.h"
//^^ #include "Observer.h"
#include "KEMMapMode.h"

class KTarget;
class KMapView;
class KIOTable;
class KIOColumn;
class KIOItem;
// KTocEditorNode 대화 상자입니다.
#define KDialogExBase KDialogEx
class KTocEditorNode : public KDialogExBase, public KTableObserver
{
	DECLARE_DYNAMIC(KTocEditorNode)

public:
	KTocEditorNode(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTocEditorNode();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOC_EDITOR_NODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
private:
	KTarget*  m_pTarget;
	KMapView* m_pMapView;
	KIOTable* m_pNodeTable;
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
	CString NodeCategoryName;
protected:
	CStatic m_stcDummy;
	CXTPPropertyGrid m_wndPropertyGrid;
private:
	void LoadTemplateComboBoxNode();
	void LoadInitialBlankProperties();
	void LoadNodePropertiesValue();
	void LoadNodePropertiesValue(std::map<CString, CString>& ar_mapTemplateValueNode);
	void EditableCheck (CXTPPropertyGridItem* a_pPropertyGridItem, KIOColumn* a_pIOColumn);
private:
	void InitialControl();
private:
	CComboBox    m_cboTemplateNode;
	int          m_nPreNodeTemplateID;

    afx_msg void OnBnClickedButtonEditStart();
	afx_msg void OnBnClickedButtonEditEnd();
    
	afx_msg void OnBnClickedButtonTemplateDeleteNode();
	afx_msg void OnBnClickedButtonTemplateSaveNode();
	afx_msg void OnCbnSelchangeComboTemplateNode();

	afx_msg LRESULT OnPropertyGridItemValueChange(WPARAM wParam, LPARAM lParam);

private:
    __int64 m_nxAddedID;
public:
    void NotifyNodeAdded(__int64 nxID);
private:
    void Update(std::map<CString, CString>& mapKeyValue);
private:
	bool    NodeTemplateDataCheck();
	CString ConvertTemplateSaveValueNode();
	void    GetCurrentNodeTemplateValue(std::map<CString, CString>& ar_mapTemplateValue);

    void    GetNodeStartIDValue(Integer& ar_nxNodeID);
    void    SetNodeStartIDValue();
private:
	int NodeTemplateType;
private:
    void ActionFeedback(KDialogFeedback::KEMActionFeedback emActionCode);
    void StopEditing();
public:
    afx_msg void OnBnClickedRadioAdd();
    afx_msg void OnBnClickedRadioDelete();
    afx_msg void OnBnClickedRadioModify();
    afx_msg void OnBnClickedRadioMove();
};
