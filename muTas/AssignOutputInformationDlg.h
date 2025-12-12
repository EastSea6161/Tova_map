#pragma once

//^^ #include "IOColumns.h"
#include "DBaseAssignment.h"
#include "model_common.h"

class KTarget;
class KIOColumn;

class KAssignOutputInformationDlg : public CXTResizeDialog
{
	DECLARE_DYNAMIC(KAssignOutputInformationDlg)
private:
	//KIOColumns m_modeColumnCollection;
	KTarget*            m_pTarget;
public:
	void SetBusInitalVolume(bool a_bUseInitalVolume);
	void SetMode      (KTarget* pTarget, std::vector<CString> a_vecModeName);
	void SetModelType (int nModeInVDF,   KEMAssignmentModel  a_emMoel);
	void SetOutputInfo(bool a_bUseNew,   TAssignResultInfo   a_oTARsultInfo);
	void GetOutputInfo(bool& a_bUseNew,  TAssignResultInfo&  a_oTARsultInfo);

private:
	int  m_nRunSeq;
	int  m_nModeOutputType;
	int  m_nModeInVDF;
	bool m_bBusInitalVol;
	bool m_bUseOverWirte;

    KEMAssignmentModel m_emAModel;
	TAssignResultInfo  m_oAssign;		
	std::vector<TAssignResultInfo> m_vecAssignResultInfo;
	std::vector<CString> m_vecModeName;

public:
	KAssignOutputInformationDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KAssignOutputInformationDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_6791_AOutPutDesignDlg };

protected:
	void InitOutputInfo(void);
	void InitReportColumn(void);
	void InitReportRecord(void);
	void InitReportRecordModeType();
	void ChangeComboBox();
	void GetOutputDesignInfo();
	void SetDefualtDescription();

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
protected:
	int       m_nRadioOutputType;
	CString   m_strDescription;
	CComboBox m_cboOverwriteInfo;

private:
	CString   m_strLinkGroupName;
	CString   m_strAttriGroupName;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioNew();
	afx_msg void OnBnClickedRadioOverwrite();
	afx_msg void OnCbnSelchangeComboOverwriteInfo();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeCombo1();
};