/**
 * @file TableAddFieldDlg.h
 * @brief KTableAddFieldDlg 선언
 * @author 
 * @date 2010.08.23
 * @remark
 */


#pragma once
#include "afxwin.h"
#include "Resource.h"
#include "TableAddFieldInfo.h"

class KTarget;

/**
 * @brief Table에 Field를 추가하기위해 field 정보를 입력받는 Dialog.
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2010.08.23
 */
class KTableAddFieldDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KTableAddFieldDlg)
	enum { IDD = IDD_4150_AddFieldDlg };
public:
	KTableAddFieldDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   
	virtual ~KTableAddFieldDlg();
private:
	void InitGroupCode(std::map<CString, int> &mapGroupCode);
	void SettingControlByMode();

public:
	void SetDisplayNames( std::vector<CString> &a_vecDisplayName )
	{
		m_vecDisplayName = a_vecDisplayName;
	}

protected :
	bool isExistsName( CString strName );

protected:
	virtual BOOL PreTranslateMessage(MSG* a_pMsg);
	virtual BOOL OnInitDialog();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
    DECLARE_MESSAGE_MAP()

protected:
	std::map<CString, int> m_mapGroupCode;
	std::vector<CString>   m_vecDisplayName;

private:
    KTarget*            m_pTarget;
public:
	bool				m_bCode;
	CString				m_strColumnName;
	CComboBox			m_comboDataType;
	CString				m_strColumnInfo;
	KExistNameChecker*	m_pNameChecker;
	KEMIODataType		m_emDataType;
	int					m_nCodeGroup;
	CComboBox			m_comboCodeGroup;
	CString				m_strModeName;
	CEdit				m_editColName;
	int					m_nColumnMode;

protected :
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedColumnData();
	afx_msg void OnBnClickedColumnCode();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
