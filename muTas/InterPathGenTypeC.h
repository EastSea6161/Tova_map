#pragma once
//^#include "ResizeDialogEx.h"
#include "afxwin.h"
#include "DBaseDefineTransitCost.h"
#include "DBaseInterModal.h"
#include <fstream>
#include <iostream>
//#include "StdioFileEx.h"

class KTarget;
class KIOColumn;

// KInterPathGenTypeC 대화 상자입니다.

class KInterPathGenTypeC : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KInterPathGenTypeC)

public:
	KInterPathGenTypeC(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterPathGenTypeC();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6840_INTERMODALPATHGEN_C };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget*				m_pTarget;
	int						m_nTimeMethod;
	CXTPOfficeBorder<CXTPReportControl,false> m_wmReportControl;
	CComboBox				m_cboUsingLinkField;
	TAssignRule				m_oAssignInfo;
	KIOColumn*				m_pIOColumnUseLink;


public:
	void SetTarget(KTarget* a_pTarget);

protected:
	void InitComboLinkField();
	
	void InitAssingRuleValue();

	void InitControls(void);

	void InitReportHeader(void);

	void InitTerminalModeValue(std::map<int, TTerminalProcessTime>& a_mapTerminalTime);

	void UpdateTerminalTimeData(void);

public:
	void ArgumentRestriction(CStdioFileEx& of);
	void ArgumentTimeMethod(KIOColumn*& a_pColumn, CStdioFileEx& of);
	void ArgumentTime(CStdioFileEx& of);
	void ArgumnetVDF(CStdioFileEx& of);
	void ArgumentCondition(CStdioFileEx& of);
	void ArgumentProsessTime(CStdioFileEx& of);

	void SaveResultTime2DB();
	void SaveResultTerminalTime2DB();
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioTimeMethod(UINT nID);
	
};
