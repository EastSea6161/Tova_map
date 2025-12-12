#pragma once

#include "AOBSingleVDFDlg.h"
#include "AOBMultiVDFDlg.h"
#include "AInputInfo.h"
#include <fstream>
#include <iostream>
//#include "StdioFileEx.h"
class KTarget;
// KAOBDefineVDFDlg 대화 상자입니다.

class KAOBDefineVDFDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KAOBDefineVDFDlg)

public:
	KAOBDefineVDFDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KAOBDefineVDFDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6720_AOBDefineVDFDlg };

private:
	void InitComboModeCntInVDF(void);

	bool WriteInputMultiArgument(CStdioFileEx& of);

	bool WriteInputSingleArgumet(CStdioFileEx& of);

public:
	void InputList(std::list<KAInputInfo*>& inputList);

	void SetTarget(KTarget* pTarget);

	void GetMultiVDFInfo(TAOBMultiVDF& a_TOBMultiVDF);

	int GetModeInVdf();

	bool WriteInputArgument(CStdioFileEx& of);

	bool ExportDB2LinkFile(KDBaseConPtr a_spDBaseConnection);

	bool ExportDB2SingeLinkFile(KDBaseConPtr a_spDBaseConnection);

	bool ExportDB2MulityLinkFile(KDBaseConPtr a_spDBaseConnection);

	bool ExportDB2LinkFileExcludeType(KDBaseConPtr a_spDBaseConnection, std::set<Integer> a_setIncludeLink, CString strLocation);

	bool ExportDB2SingeLinkFileExcludeType(KDBaseConPtr a_spDBaseConnection, std::set<Integer> a_setIncludeLink, CString strLocation);

	bool ExportDB2MulityLinkFileExcludType(KDBaseConPtr a_spDBaseConnection, std::set<Integer> a_setIncludeLink ,CString strLocation);

	void InitButtonControl(void);

	void UsedDefault(bool a_bUseDefault);

	bool MultiScenarioExportDB2LinkFileExcludeType(KDBaseConPtr a_spDBaseConnection, std::set<Integer> a_setIncludeLink, CString strLocation, KTarget* a_pRunTarget);

	bool MultiScenarioExportDB2SingeLinkFileExcludeType(KDBaseConPtr a_spDBaseConnection, std::set<Integer> a_setIncludeLink, CString strLocation, KTarget* a_pRunTarget);

	bool MultiScenarioExportDB2MulityLinkFileExcludType(KDBaseConPtr a_spDBaseConnection, std::set<Integer> a_setIncludeLink ,CString strLocation, KTarget* a_pRunTarget);

private:
	bool m_bUseDefault;
		
	int m_nCntSelInputMode;	/** 배정모형 Mode Input 선택개수 */
	int m_nModeInVDF;       /** Used Mode Number in VDF */
	KTarget* m_pTarget;

	std::list<KAInputInfo*> m_InputList;

private:
	CComboBox m_cboModeCntInVDF;

	KAOBSingleVDFDlg m_singleVDFDlg;
	KAOBMultiVDFDlg  m_multiVDFDlg;

	TAOBMultiVDF	m_oABMultiVDFInfo;
	TAOBSingleVDF	m_oABSingeVDFInfo;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonDefault();
};
