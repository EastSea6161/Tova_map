#pragma once

//^#include "ResizeDialogEx.h"
#include "TableAddFieldInfo.h"
//^^ #include "IOColumn.h"
#include "Purpose.h"

class KTarget;
class KProject;
class KIOTable;

// KTablePurposeAddFieldDlg 대화 상자입니다.

class KTablePurposeAddFieldDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KTablePurposeAddFieldDlg)

public:
	KTablePurposeAddFieldDlg(KTarget* a_pTarget, KProject* a_pProject, KIOTable* a_pIOTable, int a_nColumnNumber, int a_nNewDisplayOrder, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTablePurposeAddFieldDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4152_AddPurposeFieldDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	/**
    * @brief Group Code 선택 콤보 박스에 넣을 데이터 초기화
	* @param[out] mapGroupCode - Group Code 정보
    */
	void InitGroupCode(std::map<CString, int> &mapGroupCode);

	void InitialPurposeInfo();

	/**
    * @brief 체크박스 체크 여부에 따라. 보여줄 컨트롤 들을 변경 한다.
    */
	void SettingControlByMode();

public:
	void SetDisplayNames( std::vector<CString> &a_vecDisplayName )
	{
		m_vecDisplayName = a_vecDisplayName;
	}

protected :
	bool isExistsName( CString strName );

protected:
	std::map<CString, int> m_mapGroupCode;
	std::vector<CString>   m_vecDisplayName;
	KTarget*    m_pTarget;
	KProject*   m_pProject;
	KIOTable*   m_pIOTable;
	int         m_nNewColumnNumber;
	int         m_nNewDisplayOrder;

protected:
	virtual BOOL PreTranslateMessage(MSG* a_pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	bool CreateAddColumn();
	afx_msg void OnBnClickedColumnData();
	afx_msg void OnBnClickedColumnCode();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckDef();
	void ActiveDefineControl();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

protected:
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
	CComboBox           m_cboPurposeInfo;
public:
	KIOColumn           m_AddColumn;
	KPurpose*           m_pPurpose;
};
