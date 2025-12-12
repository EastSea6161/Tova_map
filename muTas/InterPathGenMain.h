#pragma once
//^#include "ResizeDialogEx.h"
#include "InterPathGenTypeA.h"
#include "InterPathGenTypeB.h"
#include "InterPathGenTypeC.h"
#include <fstream>
#include <iostream>
//#include "StdioFileEx.h"

class KTarget;

class KInterPathGenMain : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KInterPathGenMain)

public:
	KInterPathGenMain(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterPathGenMain();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6840_INTERMODALPATHGEN_MAIN };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
protected:
    virtual BOOL OnInitDialog();

private:
    KTarget* m_pTarget;
	int m_nResultCode;
private:
    KInterPathGenTypeA m_dlgPathGenTypeA;
    KInterPathGenTypeB m_dlgPathGenTypeB;
    KInterPathGenTypeC m_dlgPathGenTypeC;


protected:
	static unsigned __stdcall InterGeneratorThreadCaller(void* p);
	//static void ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg);
	static void AddStatusMessage(CString a_strMsg);
	void Run(void);

	bool CreateArgument(void);
	void ClearFiles(void);
	CString ConvertSavePath(CString a_strFileName);
	void DeleteAllFolderFile(CString a_strFolderName);

	void ArgumentWriteProjectName( CStdioFileEx& of );
	void ArgumentWriteNoofClass(CStdioFileEx& of);
	void ArgumentWriteFolder(CStdioFileEx& of);

	void SaveResultValue(void);

private:
    int m_nStep;
private:
    void UIChange();
private:
    afx_msg void OnBnClickedButtonBack();
    afx_msg void OnBnClickedButtonNext();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();   
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
};
