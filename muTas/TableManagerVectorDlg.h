#pragma once

//^#include "ResizeDialogEx.h"
#include "afxwin.h"
#include "TableManager.h"

#include "VectorOperVRecord.h"

class KProject;
class KIOTable;
class KIOColumns;
class KTarget;

// KTableManagerVectorDlg 대화 상자입니다.

class KTableManagerVectorDlg : public KResizeDialogEx, public KTableObserver
{
	DECLARE_DYNAMIC(KTableManagerVectorDlg)

public:
	KTableManagerVectorDlg(KProject* a_pProject, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTableManagerVectorDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8100_TableManagerVectorDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	void DeallocateMap(void);

	void InitComboDataType();
	void InitUserColumn();

	void ResizeComponent();
	void InitReportAHeader();
	void UpdateReportAData();
	void AddTreeDepth3(CXTPReportRecords* a_pThParRecords, KIOTable* a_pIOTable, BOOL a_bCheck);

	void InitReportBHeader();
	void UpdateReportBData();

	void Run(void);
	void SrchOperData(void);

	CString GetExportHeaderLine(void);

	static unsigned __stdcall SrchOperDataThreadProc(void* pParam);
	static unsigned __stdcall WriteTextThreadProc(void* pParam);

	ThreadResultInfo RThreadInfo;
	static unsigned __stdcall CopyUserColumnThreadProc(void* pParam);
	static void AddStatusMessage(CString a_strMsg);

	static void WriteTextFile(
		CString a_strFileName, KIDKeyDoubleRecords* a_pFieldRecord, CString a_strHeaderLine, CString &a_strMsgExportResult);

private:
	virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);

private:
	bool  m_bOperDataThreadOK;

	KProject*            m_pProject;
	KIOColumns* m_pUserColumns;
	vector<TTargetTable> m_vecTargetTable;

private:
	map<Integer, KSDoubleRecord*> m_mapResultRecord;
	KVectorOperVRecord*           m_pVirtualRecord;

	//KSDoubleRecordArray*          m_pRecordArray;
	KIDKeyDoubleRecords*          m_pFieldRecord;

	std::vector<Integer>          m_vecFieldRecordKey;
	CString                       m_strMsgExportResult;

private:
	int  m_nSelectedMethod;			//Column 선택시 - 1;
	int  m_nSelectedColumnIndex;	//Column 선택시 Column의 Index

private:
	CString m_strExpFileName;

private:
	std::vector<TUserColumnInfo> m_vecUserColumnInfo;
	KTarget*  m_pSelectedTarget;
	KIOTable* m_pSelectedIOTable;

	CString   m_strRunTableName;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonRun();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeComboDataType();
			void ResetResultData();
	afx_msg void OnReportAItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonCalc();
	afx_msg void OnBnClickedBtnExport();
	afx_msg void OnMenuStatisticsIncludeZero();
	afx_msg void OnReportBColumnRButtonClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButtonCopy();
		void	 RunCopyPart1(ThreadPara* pPara);
		void     RunCopyPart2(ThreadPara* pPara);
		void     RunCopyPart3(ThreadPara* pPara);
		void     GetCurUserColumnInfo(std::vector<TUserColumnInfo> &a_vecUserColumnInfo);
		bool     CopyUserColumn(KIOTable* a_pIOTable, std::vector<TUserColumnInfo> &a_vecUserColumnInfo);
		bool     CopyUserColumnData(KTarget* a_pTarget, KIOTable* a_pIOTable, std::vector<TUserColumnInfo> &a_vecUserColumnInfo);
		bool	 DeleteCopiedUserColumn(KIOTable* a_pIOTable, std::vector<TUserColumnInfo> &a_vecAddUserColumnInfo);

private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportACtrl;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportBCtrl;
	CComboBox         m_cboDataType;
};
