#pragma once

//^#include "ResizeDialogEx.h"
#include "KDBaseAccessibilityModify.h"

#include "afxwin.h"
#include <fstream>
#include <iostream>
//#include "StdioFileEx.h"
#include "DBaseDefaultParaAccessSetting.h"

class KTarget;
class KProject;
class KODKey;

#define CODE_GROUP_KEY_NODE 2
#define CODE_NODE_ZONE      0
#define CODE_NODE_RAIL      9

enum KEMProcessMethod
{
	KEMProcessMethodCode     = 1,
	KEMProcessMethodDistance = 2
};

class KDefaultAccessClassinfo
{
public:
	 KDefaultAccessClassinfo:: KDefaultAccessClassinfo():strDefaultClassName(_T("")),nDefaultTime(0)
	{
	}
	 KDefaultAccessClassinfo::~ KDefaultAccessClassinfo()
	{
	}

public:
	CString    strDefaultClassName;
	int		   nDefaultTime;
};

// KSetAccessibility 대화 상자입니다.

class KSetAccessibility : public KDialogEx
{
	DECLARE_DYNAMIC(KSetAccessibility)

public:
	KSetAccessibility(KProject* a_pProject, KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KSetAccessibility();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6901_SetOfAccessibility };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	void         LoadNodeType(std::vector<KNodeTypeInfo> &a_vecNodeTypeInfo);
	void         InitComboClass(int a_nInitGrade);
	void		 InitSetting();
	void		 InitDefaultSetting();
	void         InitReportHeader(void);
	void		 InitDefaultReportRecord();
	void         InitReportRecord();
	void         UpdateReportRecord(void);
	void         ResortReportRecord(std::map<int, int> &a_mapTime);
	bool         InvalidateInputInfo();
	void         Run();
	void         CreateArgument();
	void	     ClearFiles();
	void         ArgumentWriteProjectName  (CStdioFileEx& of);
	void         ArgumentWriteProcessMethod(CStdioFileEx& of);
	void         ArgumentWriteClass        (CStdioFileEx& of);
	void         ArgumentWriteFolder       (CStdioFileEx& of);
	bool	     ReadOutPutFile(std::map<Integer, CString> a_mapZoneName);
	void         LoadZoneName(std::map<Integer, CString> &a_mapZoneName);
	void         LoadTransitName(std::map<Integer, CString> &a_mapZoneName);
	void         InsertAccessLineStation();
	CString      ConvertSavePath(CString a_strFileName);
	bool		 UsingCheckZoneID();

public:
	static unsigned __stdcall AccessThreadCaller(void* p);
	static void ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg);
	static void AddStatusMessage(CString a_strMsg);

protected:
	KAccessStationSetting  m_oSetting;

private:
	int					   m_nResultCode;
	KProject*              m_pProject;
	KTarget*			   m_pTarget;
	bool                   m_bRunResult;
	CString                m_strOutputFolerPath;
	CString                m_strRunErrMsg;
	CString				   m_strBoundaryDistance;
	std::vector<TAccessStopNodeInfo> m_vecResultInfo;

	std::map<int, CString>		  m_mapAccessibility;
	std::vector<TAccessClassInfo> m_vecAccessClassInfo;
	std::map<int, std::vector<TAccessClassInfo>> m_mapDefaultAccessClassInfo;
	std::vector<KAccessStationSetting> m_vecAccessSetting;
	std::map<int, std::vector<KAccessStationSetting>> m_mapDefaultAccessSetting;

	//std::map<KODKey, TAccBusSubwayStopAtZoneInfo> m_mapAccessLineStation;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboClass();
	afx_msg void OnBnClickedButtonRun();
	afx_msg void OnReportItemValueChanged(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnBnClickedDefault();

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
	CComboBox           m_cboClass;
};