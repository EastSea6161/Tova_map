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
// KTocEditorLink 대화 상자입니다.
#define KDialogExBase KDialogEx
class KTocEditorLink : public KDialogExBase, public KTableObserver
{
	DECLARE_DYNAMIC(KTocEditorLink)

public:
	KTocEditorLink(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTocEditorLink();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOC_EDITOR_LINK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
private:
	KTarget*  m_pTarget;
	KMapView* m_pMapView;
	KIOTable* m_pNodeTable;
	KIOTable* m_pLinkTable;
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
	CString LinkCategoryName;
protected:
	CStatic m_stcDummy;
	CXTPPropertyGrid m_wndPropertyGrid;
private:
	void LoadTemplateComboBoxNode();
	void LoadTemplateComboBoxLink();
	void LoadInitialBlankProperties();
	void LoadNodePropertiesValue();
	void LoadNodePropertiesValue(std::map<CString, CString>& ar_mapTemplateValueNode);
	void LoadLinkPropertiesValue();
	void LoadLinkPropertiesValue(std::map<CString, CString>& ar_mapTemplateValueLink);
	
	void EditableCheck (CXTPPropertyGridItem* a_pPropertyGridItem, KIOColumn* a_pIOColumn);
private:
	void InitialControl();
private:
	//CComboBox    m_cboAddLinkMethod;
	CComboBox    m_cboTemplateNode;
	CComboBox    m_cboTemplateLink;
	int          m_nPreLinkTemplateID;
	int          m_nPreNodeTemplateID;

	afx_msg void OnBnClickedTwoWayLinkAdd();
	        void AddLinkStart();
    afx_msg void OnBnClickedOneWayLinkAdd();
    	
    afx_msg void OnBnClickedLinkMultiDelete();
            void DeleteLinkStart();
    afx_msg void OnBnClickedLinkSingleDelete();
            void DeleteSingleLinkStart();

    afx_msg void OnBnClickedEditOneWayVertex();
            void LinkOneWayVertexEditStart();
    afx_msg void OnBnClickedEditTwoWayVertex();
            void LinkTwoWayVertexEditStart();
    afx_msg void OnBnClickedEditAtt();
            void LinkEditPropertiesStart();

    afx_msg void OnBnClickedButtonStart();
    afx_msg void OnBnClickedButtonEnd();
    
	afx_msg void OnBnClickedButtonTemplateDeleteNode();
	afx_msg void OnBnClickedButtonTemplateSaveNode();
	afx_msg void OnCbnSelchangeComboTemplateNode();

	afx_msg void OnBnClickedButtonTemplateDeleteLink();
	afx_msg void OnBnClickedButtonTemplateSaveLink();
	afx_msg void OnCbnSelchangeComboTemplateLink();

	afx_msg LRESULT OnPropertyGridItemValueChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCbnSelchangeComboWay();

private:
	bool    LinkTemplateDataCheck();
	bool    NodeTemplateDataCheck();
	CString ConvertTemplateSaveValueLink();
	CString ConvertTemplateSaveValueNode();
	void    GetCurrentNodeTemplateValue(std::map<CString, CString>& ar_mapTemplateValue);
	void    GetCurrentLinkTemplateValue(std::map<CString, CString>& ar_mapTemplateValue);

    void    GetCurrentNodeValue(std::map<CString, CString>& ar_mapTemplateValue);
    void    GetCurrentLinkValue(std::map<CString, CString>& ar_mapTemplateValue);
private:
	int LinkTemplateType;
	int NodeTemplateType;
private:
    void ActionFeedback(KDialogFeedback::KEMActionFeedback emActionCode);
    void StopEditing();

private:
    void GetNodeStartIDValue(Integer& ar_nxNodeID);
    void GetLinkStartIDValue(Integer& ar_nxLinkID);

    void SetNodeStartIDValue();
    void SetLinkStartIDValue();
private:
    
private:
    __int64 m_nxNodeAddedID;
    std::vector<__int64> m_vecLinkAddedID;

public:
    void NotifyNodeEdit(int nType, __int64 nxID);
    void NotifyLinkEdit(std::vector<__int64>& vecID);

private:
    void LoadNode(__int64 nxID, std::map<CString, CString>& mapKeyValue);
    void UpdateNode(std::map<CString, CString>& mapKeyValue);
    void UpdateLink(std::map<CString, CString>& mapKeyValue);

private:
    int GetNodeDefaultType();
    void NodeTypeEnable();
};
