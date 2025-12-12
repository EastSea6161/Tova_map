/**
 * @file AttributeSetDlg.h
 * @brief KAttributeSetDlg 선언
 * @author 
 * @date 2010.08.26
 * @remark
 */


#pragma once
#include "afxcmn.h"
#include "Resource.h"


/* Forward Declarations */
class KMapView;
class IEditBase;
class KIOColumns;
class KIORecordset;
class KIORow;
class KTarget;



/**
 * @brief Map view에서 새로운 feature를 생성시 Table의 속성을 입력하거나 
 *          feature의 속성을 편집 하는 다이얼로그
 * @remark Modaless 다이얼로그로 표출된다.
 * @warning
 * @version 1.0
 * @author 
 * @date 2010.08.26
 */
class KAttributeSetDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(KAttributeSetDlg)

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_3610_CreateShapeAttributeDlg };

	/**************************************
	* Constructors / Destructor           *
	**************************************/
public:
	KAttributeSetDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KAttributeSetDlg();


	/**************************************
	* Accessors                           *
	**************************************/
public :

	void SetTarget(KTarget* pTarget);

	/**
	* @brief Edit base를 설정한다.
	* @param[in] pEditBase - Edit base
	*/
	void SetEditBase(IEditBase* pEditBase);

	/**
	* @brief Column Collection을 설정한다.
	* @param[in] pColumns - Column Collection
	*/
	void SetColumns(const KIOColumns* pColumns);

	/**
	* @brief Row를 설정한다.
	* @param[in] pRow - Row
	*/
	void SetValues(const KIORow* pRow);

	/**
	* @brief Row를 반환한다.
	* @return KIORow* - Row 정보
	*/
	KIORow* GetValues(void) const;


	/**************************************
	* Operations                          *
	**************************************/
public :

	/**
	* @brief 속성을 표출한다.
	*/
	void displayAttribute( void );


	/**************************************
	* Overrides                           *
	**************************************/
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	static const CString SYSTEMFIELDS;
    static const CString USERFIELDS;

	/**************************************
	* Properties / Variables              *
	**************************************/
	
// public:	
// 	bool	m_bInserted;
// 	BOOL	m_bCopyPrevValue;
// 	ZListCtrlEx m_lstAttribute;

protected :
	KMapView*	m_pMapView;
	const KIOColumns* m_pColumns;;
	KIORow* m_pRow;

	static IEditBase*	m_pEditBase;
	CStatic m_stcDummy;

	CXTPPropertyGrid m_wndPropertyGrid;
	
	KTarget* m_pTarget;

	/**************************************
	* Message handlers                    *
	**************************************/
protected :
	DECLARE_MESSAGE_MAP()
private:
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	void	InitPropertyGrid(void);	
	CXTPPropertyGridItem* AddMultiCodeItem( CXTPPropertyGridItem* a_pParentGridItem, KIOColumn* a_pIOColumn, KIOItem* a_pIOItem );

private:
    void EditableCheck();
private:
    CString m_strTableName;
public:
    void SetTableName(LPCTSTR a_strTableName)
    {
        m_strTableName = a_strTableName;
    }
private:
    bool m_bApplyOppositeLink;
public:
    bool ApplyOppositeLink();
private:
    afx_msg void OnBnClickedCheck1();
	afx_msg LRESULT OnGridNotify( WPARAM wParam, LPARAM lParam );

};