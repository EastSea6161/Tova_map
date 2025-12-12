#pragma once
//^#include "ResizeDialogEx.h"
#include "BulkDBaseLink.h"
#include "BulkDBaseTransit.h"
#include <fstream>
#include <iostream>
//#include "StdioFileEx.h"

class KProject;
class KTarget;

// KNetworkInspectionDlg 대화 상자입니다.

class KNetworkInspectionDlg : public KResizeDialogEx, public KTableObserver
{
	DECLARE_DYNAMIC(KNetworkInspectionDlg)

public:
	KNetworkInspectionDlg(KProject* a_pProject, KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KNetworkInspectionDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_5170_NetworkInspectionDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

protected:
    virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), __int64 a_nxObjectID=0);
private:
    KIOTable*    m_pLinkTable;
    KIOTable*    m_pTransitTable;
    KIOTable*    m_pTransitLinkTable;

public:
	void		 Initialize();
	BOOL         CheckEmptyOutFile(CString a_strFile);
	void		 ResultFileOpen(CString a_strFile);
	int			 ExistErrorFile();

protected:
	void         InitReportAHeader();
	void         InitReportBHeader();
	void         ResizeComponent();
	bool         IsExsitTransitData();
	bool         IsExsitTransitLineData();
	void         MsgNoExistTransitData(bool a_bExistTransitData, bool a_bExistTransitLineData);
	void         LoadUsedLinkType (std::vector<TLinkTypeName>    &a_vecTypeName);
	void         LoadUsedBusType  (std::vector<TTransitTypeName> &a_vecBusType);
	void         UpdateReportAData(std::vector<TLinkTypeName>    &a_vecTypeName);
	void         UpdateReportBData(std::vector<TTransitTypeName> &a_vecBusType);

public:
	static unsigned __stdcall InspectionThreadCaller(void* p);
	static void ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg);
	static void AddStatusMessage(CString a_strMsg);

protected:
	void         AfterRunCheckEmptyFile();
	bool         InvalidateInputInfo();
	void         MakeCheckedReportAData(std::map<int, int> &a_mapDefineLinkType);
	void         MakeCheckedReportBData(std::map<int, int> &a_mapDefineDetailBusType);

	void         Run();   
	bool         CreateArgument();
	bool         ClearFiles();
	bool         WriteDataInspectionArgument(CStdioFileEx& of);
	CString      ConvertSavePath(CString a_strFileName);

private:
	int			 m_nExistError;
	KTarget*     m_pTarget;
	KProject*    m_pProject;
	int          m_nInspectionMode;
	bool         m_bUseHighwayInspection;
	bool         m_bUseTransitInspection;
	std::map<int, int> m_mapDefineLinkType;
	std::map<int, int> m_mapDefineDetailBusType;

    bool         m_bApplyTurn;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckTransit();
	afx_msg void OnBnClickedButtonRun();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnReportACheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	        bool IsZeroCheckReportA();
	afx_msg void OnReportBCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	        bool IsZeroCheckReportB();	
	afx_msg void OnBnClickedButtonHighwayError();
	afx_msg void OnBnClickedButtonHighwayWarning();
	afx_msg void OnBnClickedButtonTransitWarning();
	afx_msg void OnBnClickedButtonTransitError();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	
private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportACtrl;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportBCtrl;
	int                 m_nResultCode;
	CString             m_strErrMsg;

private:
    void TransitCount(int& nBusCount, int& nRailCount);
public:
    afx_msg void OnBnClickedCheckHighway();
    void CheckControlHighway();
};

typedef std::shared_ptr<KNetworkInspectionDlg> KNetworkInspectionDlgPtr;