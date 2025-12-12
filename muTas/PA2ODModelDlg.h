#pragma once

#include "MarkupStatic.h"
#include "PA2ODDefinePADlg.h"

/*forward delcare*/
class KTarget;
class KPA2ODInfo;
class KIOColumns;

// KPA2ODModelDlg 대화 상자입니다.

class KPA2ODModelDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KPA2ODModelDlg)

public:
	KPA2ODModelDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KPA2ODModelDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5800_PA2ODModelDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	void ClearInputInfoList(void);
	void InitInputInfoList(void);
	void InitDialogUI(void);
	void RunnableCheck(void);

	void CreateArguments(void);

	void AddPA2ODResultColumn(KIOColumns& a_rODColumnCollection);

	void ApplyResult(KDBaseConPtr spDBaseConnection, 
		CString &a_strStartTime, CString &a_strEndTime);

	static void ReceiveStatusMessageFromDll(void* pParam, int nStatus, char* strMsg);

	static unsigned __stdcall PA2ODModelThreadCaller(void* p);
	void RunModel(void);

private:
	HINSTANCE m_hPAtoOD;  
	typedef int (*FPATOOD)(void* pParam, void* pCallback);
	FPATOOD PAtoODProcPtr;

	KEMPA2ODReturnType     m_emPA2ODReturnType;
	KTarget*               m_pTarget;
	std::list<KPA2ODInfo*> m_lstHomeBasedPA2ODInfo;
	std::list<KPA2ODInfo*> m_lstNotHBasedPA2ODInfo;

	std::list<KPA2ODInfo*> m_lstOutput;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnDefine();
	afx_msg void OnBnClickedBtnImportPA();
	afx_msg void OnBnClickedBtnImportParam();
	afx_msg void OnBnClickedBtnOutput();

private:
	CMarkupStatic m_uiBackGround;
	CMarkupStatic m_uiLblModel;
	CXTPButton 	  m_uiBtnSelect;
	CXTPButton 	  m_uiBtnPA;
	CXTPButton 	  m_uiBtnParam;
	CXTPButton 	  m_uiBtnOutputDesign;

	ZArrowStatic  m_uiArrow1;
	ZArrowStatic  m_uiArrow2;
	ZArrowStatic  m_uiArrow3;
	ZArrowStatic  m_uiArrow4;
public:
	afx_msg void OnBnClickedOk();

private:
	int  m_nResultCode;
public:
	static void AddStatusMessage(CString a_strMsg);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
};
