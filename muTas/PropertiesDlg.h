#pragma once
#include "afxwin.h"

class KTarget;
class KIOTable;
class KIOColumn;
class KIOItem;

// KPropertiesDlg 대화 상자입니다.

class KPropertiesDlg : public KResizeDialogEx, public KTableObserver
{
	DECLARE_DYNAMIC(KPropertiesDlg)

public:
	KPropertiesDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KPropertiesDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_2200_PROPERTIES };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
private:
	KTarget*  m_pTarget;
	KIOTable* m_pTable;
	CString   m_strTableName;
	CString   m_strWhere;
private:
	CString CategoryName;
private:
	BOOL m_bEditCheck;
protected:
	CStatic m_stcDummy;
	CXTPPropertyGrid m_wndPropertyGrid;
private:
	void LoadUpdateInitialControl();
	
	CXTPPropertyGridItem* AddCodeItem     ( CXTPPropertyGridItem* a_pParentGridItem, KIOColumn* a_pIOColumn, KIOItem* a_pIOItem );    
	CXTPPropertyGridItem* AddMultiCodeItem( CXTPPropertyGridItem* a_pParentGridItem, KIOColumn* a_pIOColumn, KIOItem* a_pIOItem );

	void UpdateEditableCheck (CXTPPropertyGridItem* a_pPropertyGridItem, KIOColumn* a_pIOColumn);
protected:
	virtual BOOL OnInitDialog();
    void InitialButtonStatus();
	afx_msg void OnBnClickedOk();
    void UpdateSave();
	afx_msg void OnStnClickedPicZoom();
    afx_msg void OnBnClickedCheck1();
public:
	void ShowInformation(KTarget* a_pTarget, CString a_strTableName, CString a_strWhere, BOOL a_bInitEdit=FALSE);	
private:
    Integer m_nxLinkID;
public:
    void SetLinkID(Integer a_nxLinkID)
    {
        m_nxLinkID = a_nxLinkID;
    }

private:
    afx_msg void OnBnClickedButtonCopy();
    afx_msg void OnBnClickedButtonPaste();
	afx_msg LRESULT OnGridNotify(WPARAM, LPARAM);

    bool UpdateDataCheck();
    void NodeCopy();
    void NodePaste();
    void LinkCopy();
    void LinkPaste();
private:
    virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);
private:
    int m_nEditFlag; // 0:편집, 1:추가
private:
    KIORow m_oInsertRow;
public:
    void ShowAddRowData(KTarget* a_pTarget, CString a_strTableName);	
private:
    void LoadInsertInitialControl();
    void InsertSave();
    bool IsUsablePrimaryKey( KTarget* a_pTarget, CString a_strTableName, KIORow* a_pRow );
};

typedef std::shared_ptr<KPropertiesDlg> KPropertiesDlgPtr;
