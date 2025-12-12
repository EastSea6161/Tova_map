#pragma once

#include "MarkupStatic.h"
//#include "StdioFileEx.h"

//forward declare
class KIOColumns;

// KSCAnalysisDlg 대화 상자입니다.
/*forward declare*/
class KTarget;
class KSCAnalysisInfo;

class KSCAnalysisDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KSCAnalysisDlg)

public:
	KSCAnalysisDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KSCAnalysisDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5700_SCAnalysisDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL OnInitDialog();

private:
	void InitDialogUI(void);
	void InitInputList(void);

	void ClearInputList(void);

	void RunSCAnalysisModel(void);

	void WriteArguments(void);

	bool WriteArgCommon(CStdioFileEx& of);
	bool WriteArgInput(CStdioFileEx& of);
	bool WriteArgSelect(CStdioFileEx& of);
	bool WriteArgCriteria(CStdioFileEx& of);

	void WriteBinFileODData( QBicDonkeyProgress* a_pDonkey = NULL );
	void WriteBinFileLineZoneData( QBicDonkeyProgress* a_pDonkey = NULL );
	void WriteBinFileLinkData( QBicDonkeyProgress* a_pDonkey = NULL );

	void RunnableCheck(void);

	void AddSCAnalysisResultColumn( KIOColumns& a_rODColumnCollection);
	void RollBackResultColumn(void);

	void KRedrawWindow( void );

private:
	//screen cordon analysis dll

	static void ReceiveStatusMessageFromDll(void* pParam, int nStatus, char* strMsg);

	HINSTANCE m_hSCAnalysis;
	typedef int (*FSCREENCORDON)(void* pParam, void* pCallback);
	FSCREENCORDON ScreenCordonProcPtr;

	int  m_nResultCode;

	//Validate Check
	bool m_bChkInput;
	bool m_bChkSelectSCLine;
	bool m_bChkCriteria;
	bool m_bChkOutput;

	int				m_nLineType;
	std::vector<int/*SCLineID*/> m_vecSelected;	
	/*Stopping Criteria Variable*/
	int				m_nIteration;
	double			m_dToleranceError;

	std::list<KSCAnalysisInfo*> m_lstSCAnlaysisInfo;
	
	KTarget*		m_pTarget;
	
	/*Argument Variable*/
	int	m_nTotalNodeCount;
	std::vector<__int64> m_vecZoneNode;

	std::map<__int64, std::map<__int64, int>> m_mapSCLineZoneNodeDirection;
	std::map<__int64, std::vector<__int64>> m_mapSCLineInterSectLink;

public:
	afx_msg void OnBnClickedBtnInput();
	afx_msg void OnBnClickedBtnSelect();
	afx_msg void OnBnClickedBtnCriteria();
	afx_msg void OnBnClickedBtnOutput();
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedBtnOutputtable();

protected:
	CMarkupStatic	m_uiBackGround;
	CMarkupStatic	m_uiLblModel;
	CXTPButton		m_uiBtnInput;
	CXTPButton		m_uiBtnSelect;
	CXTPButton		m_uiBtnCriteria;
	CXTPButton		m_uiBtnOutput;
	CXTPButton		m_uiBtnOutputTable;

	CButton         m_btnRun;

	ZArrowStatic	m_uiArrow1;
	ZArrowStatic	m_uiArrow2;
	ZArrowStatic	m_uiArrow3;
	ZArrowStatic	m_uiArrow4;
	
public:
	static unsigned __stdcall SCAnalysisThreadCaller(void* pParam);
	void RunModel();
	void AddStatusMessage( CString a_strMsg );
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
};
