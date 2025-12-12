#pragma once

//^#include "ResizeDialogEx.h"
#include "DBaseDefineTransitCost.h"
#include "DBaseAssignment.h"

class KTarget;
class KIOTable;
class KIOColumn;
class KIOColumns;

// KCalculatParameterDlg 대화 상자입니다.

class KCalculatParameterDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KCalculatParameterDlg)

public:
	KCalculatParameterDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KCalculatParameterDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4600_CalculateODParameterDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:

	int m_nResultCode;

	bool m_bUseTransit;

	KTarget*						m_pTarget;
	CXTPOfficeBorder<CXTPReportControl,false> m_wmReportControl;

	CString		 m_strOutputFolder;
	int          m_nTimeMethod;
	KIOColumn*   m_pIOColumnUseLink;
	TAssignRule	 m_oAssignInfo;
    bool         m_bApplyTurn;

	std::vector<TParaOutputInfo>	m_vecOutputInfo;
	std::vector<TAutoFuel>			m_vecAutoCost;
	std::vector<TTaxiFuel>			m_vecTaxiCost;
	std::vector<TTruckFuel>			m_vecTruckCost;
	std::vector<TTransitCost>		m_vecTransitCost;

	CString				m_argFilePath;
public:


public:
	static unsigned __stdcall AccessThreadCaller(void* p);
	static void ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg);
	static void AddStatusMessage(CString a_strMsg);

protected:

	void InitBaseData();

	void ResizeComponent();

	void InitReportHeader();

	void InitAutoCost();

	void InitTaxiCost();

	void InitTruckCost();

	void InitTransitCost();

	void UpdateReportControl();
	
	void InitOutputInfo(TParaOutputInfo& a_oParaOutputInfo, CString strHeader);

	void CheckButtonState();

	void SaveOutputInfo();

	void GetColumnName(TParaOutputInfo& a_oParaOutputInfo);

	void RunModel();

	void CreateArgument(TParaSelectedMode oSelectedMode);

	void ClearFiles();

	CString ConvertSavePath(CString a_strFileName);

	void ArgumentWriteProjectName(CStdioFileEx& of);

	void ArgumentWriteUseMode(CStdioFileEx& of);

	void ArgumentWriteAutoCost(CStdioFileEx& of);

	void ArgumentWriteTaxiCost(CStdioFileEx& of);

	void ArgumentWriteTruckCost(CStdioFileEx& of);

	void ArgumentWriteTransitCost(CStdioFileEx& of);

	void ArgumentWriteTimeMethod(CStdioFileEx& of);

	void ArgumentWriteAssignRule(CStdioFileEx& of);

	void ArgumentWriteNoClass(CStdioFileEx& of);

    void ArgumentWriteTurnRestriction(CStdioFileEx& of);

    void ArgumentWirteFileFolder(CStdioFileEx& of);

	void GetOutputColumnCollection(KIOColumns& rParameterColumnCollection);

	void GetResultColumn(KIOColumn*& a_pColumn, KIOTable* a_pTable ,CString a_strColumnName);

	void UpadateParaODFile2DB(KDBaseConPtr spDBaseConnection, KIOColumns& rParaODColumnCollection,TParaSelectedMode oSelectedMode );

	void SelectedMode(TParaSelectedMode& oSelectedMode);

protected:

	virtual BOOL OnInitDialog();
	afx_msg void OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedButtonCost();
	afx_msg void OnBnClickedButtonTime();
	afx_msg void OnBnClickedButtonRun();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnBnClickedButtonView();
};
