//^#include "ResizeDialogEx.h"

#include "afxwin.h"
#include <fstream>
#include <iostream>
//#include "StdioFileEx.h"
#include "DBaseInterModal.h"
#include "DBaseDefaultParaAccessTerminalSetting.h"

class KTarget;
class KIOColumns;


#pragma once

// KAccessTerminalGeneratorDlg 대화 상자입니다.

class KAccessTerminalGeneratorDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KAccessTerminalGeneratorDlg)

public:
	KAccessTerminalGeneratorDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KAccessTerminalGeneratorDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6820_AccessTerminalGeneratorDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrlTerminalType;

	KTarget*				 m_pTarget;
	std::map<int, double>    m_mapTerminalType;
	//std::map<CString, int>   m_mapPurposeOD;

	std::map<CString, int>   m_mapSelectPurposeOD;
	std::map<int, double>    m_mapSaveTerminalType;
	std::map<int, std::vector<KAccessTerminal>> m_mapDefaultAccessTerminal;
	std::map<int, double> m_mapTerminalNode;
	std::set<int> m_setAccessTerminalTableCode;
	std::set<int> m_setNodeSystemUserCodeMatching;
	std::map<int, KAccessTerminal> m_mapGetAccessTerminalTable;

	int m_nResultCode;

protected:

	void InitReportHeaderTType(void);

	void UpdateReportControlTType(void);
	void DefaultUpdateReportControlTType(int a_nMasterCode);

	void GetTerminalInfo(void);

	void Run(void);

	void ClearFiles(void);

	CString  ConvertSavePath(CString a_strFileName);

	void CreateArgument(void);

	void ArumentWriteProjectName(CStdioFileEx& of);

	void ArumentWriteTypeDist(CStdioFileEx& of);

	void ArumentWriteFolder(CStdioFileEx& of);

	bool GetSelectODColumnCollection(KIOColumns& a_ColumnCollection);

	bool ReadOutputFile(std::vector<TAccessTerminalNodeInfo>& a_vecTerminalNodeInfo);

	void SaveInfo2DB();

public:
	static unsigned __stdcall AccessThreadCaller(void* p);
	static void ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg);
	static void AddStatusMessage(CString a_strMsg);

public:
	
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	void ResizeComponent();
	afx_msg void OnBnClickedDefault();
};
