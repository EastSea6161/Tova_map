#pragma once

#include "TableAddFieldInfo.h"

class KTarget;
class KProject;
class KIOTable;
// KFTableModeAddFieldDlg 대화 상자입니다.

enum KEMFreightOD
{
	KEMFreightODModeOD = 0,
	KEMFreightODChainOD
};

class KFTableModeAddFieldDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KFTableModeAddFieldDlg)

public:
	KFTableModeAddFieldDlg(KTarget* a_pTarget, KProject* a_pProject, KIOTable* a_pIOTable, int a_nColumnNumber, int a_nNewDisplayOrder, 
		KEMFreightOD a_emFreightOD, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KFTableModeAddFieldDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4153_FAddModeFieldDlg };

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
	bool CreateAddColumn();
	bool CreateAddColumns();
	void InitArrays(void);
	int  GetNextRunSeq(void);

	void UpdateStaticName(void);
	void UpdateNextRunSeq(void);

protected:
	std::map<CString, int> m_mapGroupCode;
	std::vector<CString>   m_vecDisplayName;
	KTarget*    m_pTarget;
	KProject*   m_pProject;
	KIOTable*   m_pIOTable;
	int         m_nNewColumnNumber;
	int         m_nNewDisplayOrder;

	CStringArray m_strArray;
	KEMFreightOD m_emFreightOD;

protected:
	virtual BOOL PreTranslateMessage(MSG* a_pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedColumnData();
	afx_msg void OnBnClickedColumnCode();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckDef();
	        void ActiveDefineControl();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnCbnSelChangeCombo1(void);
			
protected:
	CComboBox			m_comboDataType;
	KExistNameChecker*	m_pNameChecker;
	CComboBox			m_comboCodeGroup;
	CString				m_strModeName;
	CEdit				m_editColName;
	int					m_nColumnMode;
	CComboBox           m_cboPurposeInfo;

public:
	KIOColumn           m_AddColumn;
	KIOColumns          m_oColumns;
	KPurpose*           m_pPurpose;
	CString				m_strColumnName;
	CString				m_strColumnInfo;
	KEMIODataType		m_emDataType;
	bool				m_bCode;
	int					m_nCodeGroup;
};
