#pragma once
//^#include "ResizeDialogEx.h"
//^^ #include "Observer.h"
#include "KEMMapMode.h"

class KTarget;
class KMapView;
class KIOTable;
class KIOColumn;
class KIOItem;
// KTocEditorTurn 대화 상자입니다.
#define KDialogExBase KDialogEx
class KTocEditorTurn : public KDialogExBase, public KTableObserver
{
	DECLARE_DYNAMIC(KTocEditorTurn)

public:
	KTocEditorTurn(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTocEditorTurn();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOC_EDITOR_TURN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
private:
	KTarget*  m_pTarget;
	KMapView* m_pMapView;
	KIOTable* m_pTurnTable;
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
	CString TurnCategoryName;
protected:
	CStatic m_stcDummy;
	CXTPPropertyGrid m_wndPropertyGrid;
private:
	void LoadTemplateComboBoxTurn();
	void LoadInitialBlankProperties();
	void LoadTurnPropertiesValue();
	void LoadTurnPropertiesValue(std::map<CString, CString>& ar_mapTemplateValueNode);
	
	void EditableCheck (CXTPPropertyGridItem* a_pPropertyGridItem, KIOColumn* a_pIOColumn);
private:
	void InitialControl();
private:
	CComboBox    m_cboModifyTask;
	CComboBox    m_cboTemplateTurn;
	int          m_nPreTurnTemplateID;

	afx_msg void OnBnClickedRadioAdd();
	        void AddTurnStart();
	afx_msg void OnBnClickedRadioModify();
	        void DeleteTurnStart();
	afx_msg void OnCbnSelchangeComboModifySelect();
	afx_msg void OnBnClickedButtonTemplateDeleteTurn();
	afx_msg void OnBnClickedButtonTemplateSaveTurn();
	afx_msg void OnCbnSelchangeComboTemplateTurn();
    afx_msg void OnBnClickedButtonStart();
    afx_msg void OnBnClickedButtonEnd();

	afx_msg LRESULT OnPropertyGridItemValueChange(WPARAM wParam, LPARAM lParam);
private:
	bool    TurnTemplateDataCheck();
	CString ConvertTemplateSaveValueTurn();
	void    GetCurrentTurnTemplateValue(std::map<CString, CString>& ar_mapTemplateValue);
private:
	int TurnTemplateType;
private:
    void ActionFeedback(KDialogFeedback::KEMActionFeedback emActionCode);
    void StopEditing();

private:
    __int64 m_nxTurnID;

private:
    void UpdateTurn(std::map<CString, CString>& mapKeyValue);
public:
    void NotifyTurnEdit(std::vector<__int64>& vecID);
};
