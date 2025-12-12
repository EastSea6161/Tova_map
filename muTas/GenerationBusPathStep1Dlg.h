#pragma once

//^#include "ResizeDialogEx.h"
#include "afxwin.h"
#include "DBaseAssignment.h"

class KTarget;
class KIOColumn;
class KGenerationBusPathInfo;
class KMode;

#define CODE_GROUP_KEY_NODE   2
#define CODE_GROUP_KEY_LINK   3
#define CODE_NODE_ZONE        0
#define CODE_NODE_RAIL        9
#define CODE_LINK_ZONECONNECT 0
#define CODE_LINK_CONNECTLINE 8
#define CODE_LINK_RAIL        9



enum KEMTimeMethod
{
	KEMTimeMethodInitTime     = 1,
	KEMTimeMethodUsingField   = 2
};

// KGenerationBusPathStep1Dlg 대화 상자입니다.

class KGenerationBusPathStep1Dlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KGenerationBusPathStep1Dlg)

struct TNodeTypeInfo
{
	int        nNodeType;
	CString    strCodeDisplay;
};

struct TLinkTypeInfo
{
	int        nLinkType;
	CString    strCodeDisplay;
};

public:
	KGenerationBusPathStep1Dlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KGenerationBusPathStep1Dlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6915_GenerationBusPathStep1Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	void         LoadNodeType(std::vector<TNodeTypeInfo> &a_vecNodeTypeInfo);
	void         LoadLinkType(std::vector<TLinkTypeInfo> &a_vecLinkTypeInfo);
	void		 InitComboLinkField();
	void		 InitReportHeader();
	void		 InitReportData();
	bool		 CheckBusRailExclusive();
	void         FindeModeInfo( int a_nModeID, KMode*& a_pMode );

protected:
	void		 ResizeComponet(void);

protected:
	std::map<CString, int>			m_mapModeChoiceInfo;
	std::map<KMode*, KIOColumn*>	m_mapModeColumn;
	KTarget*						m_pTarget;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndModeReport;

public:
	int          m_nTimeMethod;
	KIOColumn*   m_pIOColumnUseLink;
	std::list<TGenerationPathInfo> m_lstGenerationInfo;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioTimeMethod(UINT nID);
	afx_msg void OnBnClickedNext();
	afx_msg void OnReportCheckItemMode(NMHDR* pNotifyStruct, LRESULT* pResult);

protected:
	CComboBox m_cboUsingLinkField;
};
