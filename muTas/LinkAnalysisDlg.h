#pragma once
#include "LinkAnalysisInfo.h"
#include "LinkAnalysisMapDisplayDlg.h"
#include "LinkAnalysisTableODDlg.h"
#include "ImChampMapSelectionReceiver.h"
#include "afxwin.h"
#include "ShortestPathBase.h"
#include <fstream>
#include <iostream>
//#include "StdioFileEx.h"
#include "TDrawLinkInfo.h"
//^#include "ResizeDialogEx.h"
//^^ #include "Observer.h"


class KTarget;
class KMapView;
class KProject;
class KIOTable;
class KIOColumn;

#define MODE_COMBO_INIT_MSG		   _T("----Select----")
#define MODE_COMBO_INIT_MSG_K	   _T("-----선택-----")
#define DEFAULT_MODEID		 -1

// KLinkAnalysisDlg 대화 상자입니다.

class KLinkAnalysisDlg : public KDialogEx, public ImChampMapSelectionReceiver, public KShortestPathBase, public KTableObserver
{
	DECLARE_DYNAMIC(KLinkAnalysisDlg)

public:
	KLinkAnalysisDlg(KTarget* a_pTarget, KProject* a_pProject, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KLinkAnalysisDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5090_LinkAnalysis };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();

protected:
	KTarget*                   m_pTarget;	
	KProject*                  m_pProject;
	KMapView*                  m_pMapView;
	int                        m_nModeID;
	int                        m_nModeCount;
	int                        m_nRunOption;
	int                        m_nZoneThrough;
	std::vector<TLinkInfo>     m_vecLinkInfo;
	std::vector<TLinkVolume>   m_vecLinkVolume;
	std::vector<TLinkODVolume> m_vecLinkODVolume;
	std::vector<Integer>       m_vecSelectedLinkID;

	KLinkAnalysisMapDisplayDlg m_oMapDisplayDlg;
	KLinkAnalysisTableODDlg    m_oTableODDlg;
	CString                    m_strRunErrMsg;
	std::map<int, CString>     m_mapModeInfo;
	CString                    m_strPreSelectedModeName;
	KIOTable*                  m_pModeODTable;

	//void FeedbackFromMap(std::vector<Integer> a_vecLink);
private:    
    void MapSelectionFeedback(Integer a_nxID);
    void MapSelectionFeedback(std::vector<Integer> a_vecSelection);
    void MapSelectionNoneFeedback();
public:
	static unsigned __stdcall LinkVolumeThreadCaller(void* p);
	static void               ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg);
	static void               AddStatusMessage(CString a_strMsg);

	static bool m_bRunLinkVolumeSucceess;

protected:
	// Assignment dll
	HINSTANCE m_hAssignment;
	typedef int (*SELECTEDLINKANALYSIS)(void *pParam, void* pCallback);
	SELECTEDLINKANALYSIS AssignmentProcPtr;

	bool GetSearchPathVolume();

private:
	virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);

protected:
	void InitReportHeader();

	void InsertReportData();

	bool GetDBModeInfo();

	int  GetSelMode();

	bool GetDBRunOption();

	bool GetSearchLinkVolume();

	bool CreateArgument( KIOColumns &a_oUserColumnsLink );

	bool ClearFiles();
    
	bool WriteLinkVolumeArgument(CStdioFileEx& of);

	bool ReadOutPutFile();

	bool ReadOutputLink();

	bool ReadOutputOD();

	void MakeSelectedLink();

	CString ConvertSavePath( CString a_strFileName );

public:
	std::vector<TDrawLinkInfo> GetDrawLinkInfo()
	{
		return m_vecDrawLinkInfo;
	}

private:
	std::vector<TDrawLinkInfo> m_vecDrawLinkInfo;
private:
    void DrawSelectedLink();
protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
	CComboBox         m_cboMode;
	int               m_nRadio;
	CBrush            m_backBrush;

public:
	afx_msg void OnBnClickedOK();
	afx_msg void OnBnClickedButtonMap();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonSearch();
	void DoneSearch(bool a_bShowErrMsg);
	afx_msg void OnBnClickedButtonExport();
	afx_msg void OnBnClickedRadioVolumeMap();
	afx_msg void OnBnClickedRadioTimeOD();
	afx_msg void OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);

	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnCbnSelchangeComboMode();

protected:
	void RadioControl();
	void CleanMap();
	void ClearLinkViaPath();
public:
	afx_msg void OnBnClickedButtonDraw();
	void DoneDraw(bool a_bShowErrMsg);
	afx_msg void OnBnClickedCheckLabel();
	void DrawLinkVolume();
	KMapView*    GetMapView(KTarget* a_pTarget);
	void         CloseWindowProc();

private:
	int m_nResultCode;
};

typedef std::shared_ptr<KLinkAnalysisDlg> KLinkAnalysisDlgPtr;
