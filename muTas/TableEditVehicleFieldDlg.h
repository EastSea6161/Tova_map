#pragma once
#include "afxwin.h"
#include "DBaseAssignImport.h"

class KTarget;
// KEditVehicleFieldDlg 대화 상자입니다.

class KTableEditVehicleFieldDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KTableEditVehicleFieldDlg)

public:
	KTableEditVehicleFieldDlg(KTarget* a_pTarget, KIOColumn* a_pColumn, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTableEditVehicleFieldDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4159_EditVehicleFieldDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget*	m_pTarget;
	KIOColumn*	m_pColumn;

	int			m_nModeCode;
	std::map<int, KFAModeInfo> m_mapFAModeInfo;

protected:
	void InitModeCode( void );

	bool InvalidateInput();

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* a_pMsg);

	CComboBox m_cboMode;
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	

};
