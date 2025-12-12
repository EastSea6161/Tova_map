#pragma once

class KTarget;
class KProject;
class KIOTable;
class KMode;

#include "TableAddFieldInfo.h"
#include "DBaseAssignImport.h"
#include "afxwin.h"

// KTableVehicleConvAddFieldDlg 대화 상자입니다.

class KTableVehicleConvAddFieldDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KTableVehicleConvAddFieldDlg)

public:
	KTableVehicleConvAddFieldDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTableVehicleConvAddFieldDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4153_AddVehicleConvFieldDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget* m_pTarget;

protected:
	std::map<int, KFAModeInfo> m_mapFAModeInfo;
	
protected:
	CComboBox m_cboDataType;
	CComboBox m_cboCodeGroup;
	CComboBox m_cboMode;

protected:
	void InitGroupCode(void);
	void InitModeCode(void);
	void InitDataType(void);

protected:
	void CheckModeDefine(void);
	void CheckRadioButtonState(void);

protected:
	bool AddNewColumn(KIOTable* a_pTable);

public:
	virtual BOOL PreTranslateMessage(MSG* a_pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadio(UINT nID);


	afx_msg void OnBnClickedCheckDef();
	afx_msg void OnBnClickedButtonMode();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
