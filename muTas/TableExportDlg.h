/**
 * @file TableExportDlg.h
 * @brief KTableExportDlg 선언
 * @author 
 * @date 2010.08.16
 * @remark
 */


#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "kmz_common.h"
//#include "StdioFileEx.h"



/* Forward declarations */
class KIOColumns;
class KIOTable;
class KTarget;



/**
 * @brief Table Export 대화상자
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2010.08.16
 */
class KTableExportDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KTableExportDlg)

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_4170_ExportTableDlg };

	/**************************************
	* Constructors / Destructor           *
	**************************************/
public:
	KTableExportDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTableExportDlg();


	/**************************************
	* Accessors                           *
	**************************************/
public :
	inline void SetTable(KIOTable* pTable)
	{
		m_pTable = pTable;
	}
    	
	inline void SetTarget(KTarget* a_pTarget)
	{
		m_pTarget = a_pTarget;
	}

	void SetFilter(CString a_strFilter)
	{
		m_strFilter = a_strFilter;
	}

	/**************************************
	* Operations                          *
	**************************************/
public :
	bool ExportTable( void );

private:
	void ExportDB2Txt(CStdioFileEx& of);
	void ExportTransit2TxtTypeNode(CStdioFileEx& of);
	void InitComboBaseDataType(void);
	void UpdateExportColumnInfo(void);

public :
	virtual BOOL OnInitDialog();

private:
	static unsigned __stdcall TableExportThreadCaller(void* p);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


	/**************************************
	* Properties / Variables              *
	**************************************/
	KTarget*  m_pTarget;
	KIOTable* m_pTable;
	CString   m_strFilter;

	BOOL m_bExportColumn;
	int  m_nFieldSeparator;
	int  m_nEncoding;

	CXTBrowseEdit m_editFile;
	CListCtrl     m_lstColumnList;

	KIOColumns*    m_pExportColumns;
	KEMCSVEncoding m_emCSVEncoding;
	CString        m_strSeparator;
	bool           m_bSetDataType;

	/**************************************
	* Message handlers                    *
	**************************************/
protected :
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckSelectAll();
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelchangeComboBaseDataType();

private:
	CComboBox m_cboDataType;
};
