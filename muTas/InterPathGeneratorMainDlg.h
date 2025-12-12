#pragma once

class KTarget;

#include "DBaseInterModal.h"
#include "DBaseDefineTransitCost.h"
#include <fstream>
#include <iostream>
#include "PurposeInfoCollection.h"

// KInterPathGeneratorMainDlg 대화 상자입니다.

class KInterPathGeneratorMainDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KInterPathGeneratorMainDlg)

public:
	KInterPathGeneratorMainDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterPathGeneratorMainDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6840_IntermodalPathGeneratorMainDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget* m_pTarget;

	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrlOD;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrlUrban;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrlRegional;

	CComboBox				m_cboUsingLinkField;
	KIOColumn*				m_pIOColumnUseLink;
	KPurposeInfoCollection	m_oPurpoeInfo;

	int m_nResultCode;
private:
	std::map<int, KIOColumn*> m_mapPurposeODMatrix;

private:
	std::map<int, TUrbanPathInfo>	m_mapUrbanModePath;
	std::map<int, TRegionPathInfo>	m_mapRegionModePath;

	int			m_nUseTurn;			//0:UnUes 1:Use
	int			m_nTimeMethod;		//1:Inition Time 2:Using LinkField
	KIOColumn*	m_pLinkTimeField;

private:
	//Urban Define
	TAssignRule					m_oUrbanParamter;
	std::vector<TInterAutoCost> m_vecUrbanAutoCost;
	std::vector<TInterTaxiCost> m_vecUrbanTaxiCost;
	std::vector<TTransitCost>	m_vecTransitCost;

private:
	//Regional Define
	TAssignRule							m_oRegionalParameter;
	std::vector<TInterAutoCost>			m_vecRegionalAutoCost;
	std::vector<TInterTaxiCost>			m_vecRegionalTaxiCost;
	std::map<int, TTerminalProcessTime>	m_mapRegionalProcessTime;

protected:
	void InitComboLinkField();

	void InitReportHeaderOD(void);
	void InitReportHeaderUrban(void);
	void InitReportHeaderRegional(void);

	void UpdateReportDataOD(void);
	void UpdateReportDataUrban(void);
	void UpdateReportDataRegion(void);

	void DefaultUrbanModeData(void);

	void InitUrbanModeData();
	void InitRegionModeData();

	void InitUrbanModeDefine(void);
	void InitRegionalModeDefine(void);

	void ResizeComponet(void);
	
	void SetCheckMatrixAll(void);
	void SetUnCheckMatrixAll(void);
	void CheckMatrixstate(void);

	void InitUrbanPathParameter(void);
	void InitUrbanConstraintCondition(void);
	void InitUrbanAutoCost(void);
	void InitUrbanTransitCost(void);

	void InitRegionalConstraintCondition(void);
	void InitRegionalAutoCost(void);
	void InitRegionalTerminalProcessTime(void);

	//Save
protected:
	bool SaveODMatrix(void);
	bool SaveUrbanMaxPath(void);
	bool SaveRegionalMaxPath(void);
	bool SaveHighwayOption(void);
	//Run
protected:
	static unsigned __stdcall InterGeneratorThreadCaller(void* p);
	static void AddStatusMessage(CString a_strMsg);
	void Run(void);


protected:
	void ClearFile(void);
	void CreateArgument(void);
	CString ConvertSavePath( CString a_strFileName );
	void DeleteAllFolderFile( CString a_strFolderName );

	void ArgumentWriteProjectName(CStdioFileEx& of);
	void ArgumentWriteModeOD(CStdioFileEx& of);
	void ArgumnetWriteUrbanMaxPath(CStdioFileEx& of);
	void ArgumnetWriteRegionMaxPath(CStdioFileEx& of);
	void ArgumentWriteTurnRestriction(CStdioFileEx& of);
	void ArgumnetWriteTimeMethod(CStdioFileEx& of);
	void ArgumentWriteUrbanAuto(CStdioFileEx& of);
	void ArgumentWriteUrbanTaxi(CStdioFileEx& of);
	void ArgumentWriteUrbanTransit(CStdioFileEx& of);
	void ArgumentWriteUrbanControlParameter(CStdioFileEx& of);
	void ArgumentWriteRegionalAuto(CStdioFileEx& of);
	void ArgumentWriteProcessingTime(CStdioFileEx& of);
	void ArgumentWirteRegionControlParameter(CStdioFileEx& of);
	void ArgumentWriteNoofClass(CStdioFileEx& of);
	void ArgumentWriteFolder(CStdioFileEx& of);

protected:
	void SaveResult2DB(void);
	void SaveResultPurposeOD(KDBaseConPtr spDBaseConnetion);
	void SaveResultUrbanMaxpath(KDBaseConPtr spDBaseConnetion);
	void SaveResultRegionMaxpath(KDBaseConPtr spDBaseConnetion);
	void SaveResultAutoCostInfo(KDBaseConPtr spDBaseConnetion);
	void SaveResultTaxiCostInfo(KDBaseConPtr spDBaseConnetion);
	void SaveTerminalTimeInfo(KDBaseConPtr spDBaseConnetion);
	void SaveParameterOption(KDBaseConPtr spDBaseConnetion);
	void SaveTransitCostInfo(KDBaseConPtr spDBaseConnection);
	void SaveLinkOption(KDBaseConPtr spDBaseConnection);

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonUrban();
	afx_msg void OnBnClickedButtonRegional();
	afx_msg void OnBnClickedRadioTimeMethod( UINT nID );
	afx_msg void OnReportCheckItemMode(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnBnClickedCheckAllMatrix();
public:
	afx_msg void OnBnClickedButtonDefault();
};
