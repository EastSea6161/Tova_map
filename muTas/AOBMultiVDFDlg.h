#pragma once


// KAOBMultiVDFDlg 대화 상자입니다.
#include "AInputInfo.h"
#include "afxwin.h"
#include "AOBVDFInfo.h"

class KTarget;

class KAOBMultiVDFDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KAOBMultiVDFDlg)

public:
	KAOBMultiVDFDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KAOBMultiVDFDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6722_AOBMultiVDFDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:

	void SetModeNumInVDF(int a_nModeNumInVDF);

	bool GetAOBMultiVDFInfo(TAOBMultiVDF& a_oAOBMultiVDF );

	void SetInitData();

	void SetInitEachLinkData();

	void SetComboBoxInfo( TAOBMultiVDF& a_TAOBMultiVDF);

private:

	void InitVOTReport(void);

	void UpdateVOTReport(void);

	void InitMatchReport(void);

	void UpdateMatchReport(void);

	void SetFunctionColumnName(void);
	
	void InitFunctionTypeReport();

	void UpdateFunctionTypeReport();

	void InitEachLinkReport();

	void UpdateEachLinkReport(void);

	void ChangedEachLinkparameter(void);

	void CheckRadioButton();  

	void InitComboboxEachLink(void);

	bool GetMatchingCtrl(std::map<int, int>& a_mapVDFMatching);

	bool GetValueofTimeInfo(int& a_nStandardVDF, std::map<int, double>& a_mapVDFValue);

	bool GetChangeParmeter(bool& a_bAlBeta, bool& a_bABToll, bool& a_bInitionTime);

	bool GetFunctionType(KEMAssignmentBPRFunctionType& a_emBPRFunction);

	bool GetFunctionTypeValue(std::vector<std::vector<CString>>& a_vecFunctionType);

	bool GetEachLInkValue(std::vector<CString>& a_vecLinkColumn);

	CString GetColumnName(CString a_strColumCaption);

	CString GetColumnCaption( CString a_strColumnName );

	bool ReadEachLinkReport(std::vector<CString>& a_vecLinkReportName);

public:

	void SetInputList(std::list<KAInputInfo*>& inputList);

	bool GetMultiVDFInfo(TAOBMultiVDF& TMultiVDF);

private:

	KTarget* m_pTarget;

	int m_nModeNumInVDF;

	std::list<KAInputInfo*> m_InputList;

public:
	virtual BOOL OnInitDialog();
	
	
	CXTPOfficeBorder<CXTPReportControl,false> m_wndMatch;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndVOT;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndFunction;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndEachLink;

protected:

	CComboBox m_comboLinkAlpha;
	CComboBox m_comboLinkBeta;
	CComboBox m_comboLinkA;
	CComboBox m_comboLinkB;
	//CComboBox m_comboLinkT;

	int m_RadioFunction;

	BOOL m_ChkAlphaBeta;
	BOOL m_ChkabToll;
	BOOL m_ChkInitialTime;

	int			m_nComboLinkAllAlphaCursor;
	int			m_nComboLinkAllBetaCursor;
	int			m_nComboLinkAllACursor;
	int			m_nComboLinkAllBCursor;
	int			m_nComboLinkAllTCursor;

	std::vector<CString> m_vecInitFunctionColumn;
	std::vector<CString> m_vecInitEachLinkColumn;
	std::vector<std::vector<CString>> m_vecFunctionRows;

protected:

	afx_msg void OnMatchDataClick(NMHDR * a_pNotifyStruct, LRESULT * a_result);
	afx_msg void OnVOTDataClick(NMHDR * a_pNotifyStruct, LRESULT * a_result);
	afx_msg void OnBnClickedAlphaBeta();
	afx_msg void OnBnClickedAB();
	afx_msg void OnBnClickedInitialTime();
	afx_msg void OnBnClickedRadio(UINT nID);
	afx_msg void OnBnClickedButtonSetbprfn();
	


};
