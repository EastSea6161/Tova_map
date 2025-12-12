#pragma once

#include "DBaseDefaultParaHighwayAssign.h"
//^^ #include "IOColumns.h"
#include "AOBVDFInfo.h"

class KTarget;
class KProject;

// KDefaultHighwayAssignment 대화 상자입니다.

class KDefaultHighwayAssignment : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KDefaultHighwayAssignment)

public:
	KDefaultHighwayAssignment(KTarget* a_pTarget, KProject* a_pProject, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDefaultHighwayAssignment();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8507_Default_highwayAssignment };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget*					 m_pTarget;
	sqlite3*					 m_pDB;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndModeReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndVDFReport;
	CComboBox					 m_cboClass;
	CComboBox					 m_cboAlpha;
	CComboBox					 m_cboBeta;
	CComboBox					 m_cboA;
	CComboBox					 m_cboB;
	KProject*                    m_pProject;
	//KEMAssignmentBPRFunctionType m_emBPRFnType;

protected:
	std::map<int, CString>							m_mapGetModeinfo;
	std::map<int, KODTime>							m_mapDefaultODTime;
	std::map<int, KVDFPara>							m_mapDefaultVDFPara;
	std::map<int, std::vector<KVDFFunction>>		m_mapDefaultVDFFuntion;
	std::map<int, std::vector<KVDFFunction>>		m_mapDefaultComboVDFFuntion;
	std::vector<CString>							m_vecLinkFunction;
	std::map<CString, int>							m_mapDefaultVDFFuntionGroup;
	std::map<int, std::vector<KOccupancyPCEMode>>	m_mapDefaultOccupancyPCEMode;
	std::vector<std::vector<CString>>				m_vecReportData;
	std::map<int, CString>							m_mapGetDefaultModeID;

protected:
	int		    m_nComboFnCursor;
	int		    m_nFnGroupID;
	int			m_nComboLinkAllAlphaCursor;
	int			m_nComboLinkAllBetaCursor;
	int			m_nComboLinkAllACursor;
	int			m_nComboLinkAllBCursor;
	int			m_nRadiobtnFunction;
	CString     m_strUseFunctionGroup;
	ZNumberEdit m_edtTravelTime;
	ZNumberEdit m_edtTime;
	ZNumberEdit m_edtAlpha;
	ZNumberEdit m_edtBeta;


public:
	virtual BOOL OnInitDialog();

protected:
	void InitModeReportHeader();
	void InitVDFReportHeader();

	void InitODTimeSetting(int a_nMasterCode);
	void InitVDFParaSetting(int a_nMasterCode);
	void GetDefaultModeInfo(int a_nMasterCode, std::map<int, CString>& a_mapGetDefaultModeID);
	void InitModeReportRecord(int a_nMasterCode);
	void InitVDFReportRecord(int a_nMasterCode);
	void InitComboBoxEachFn(int a_nMasterCode);
	void InitComboBoxEachLink(int a_nMasterCode);

	void SetModeReportRecord();
	void SetComboBoxEachLink();
	void SetComboBoxEachFn();
	void SetODTimeSetting();
	void SetVDFParaSetting();

	void SaveOccupancyPCEMode(int a_nMasterCode);
	void SaveODTimeSetting(int a_nMasterCode);
	void SaveVDFParaSetting(int a_nMasterCode);

	void InitComboClass();
	void UpdateInitVDFReportCtrl();
	void UpdateComBoxEachFn();

public:
	afx_msg void OnCbnSelchangeComboFunction();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedDefault();
	afx_msg void OnBnClickedButtonSetbprfn();
};
