#pragma once

#include "kmz_common.h"

/** Data types */
enum KEMSurveyImportDataType
{
	KEMSurveyImportInteger	    = 1,	/** int		type */
	KEMSurveyImportDouble	    = 2,	/** double	type */
	KEMSurveyImportInt64		= 3		/** __int64 type */
};


// KTimeODSurveyImport2Dlg 대화 상자입니다.

class KTimeODSurveyImport2Dlg : public CXTResizeDialog
{
	DECLARE_DYNAMIC(KTimeODSurveyImport2Dlg)

public:
	KTimeODSurveyImport2Dlg(
		BOOL a_bContainHeader, KEMCSVEncoding a_emCSVEncoding, TCHAR a_cSeparator,
		CString a_strFileName, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTimeODSurveyImport2Dlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8032_TimeODSurveyDataImportStep2Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	bool	GoBack(void);

	void	InitDefineReport(void);
	void	UpdateDefineReport(void);

	bool	PreviewDataLoad(void);

	void	InitPreviewReport(void);
	void	UpdatePreviewReport(void);

	bool	GenerateHeaderColumn(void);
	void	ParseCSVLineString(CString& strLine, std::vector<CString>& row);

	void	WriteBinaryFile(QBicDonkeyProgress* pDonkey = NULL);
	void	LineData2Binary(std::vector<CString> a_vecLineData, std::ofstream& ofs);

	std::set<int>	PurposeSet(void);
	std::set<int>	ModeSet(void);

private:
	bool            m_bRunOK;
	bool			m_bGoBack;
	BOOL			m_bContainHeader;
	TCHAR			m_cSeparator;
	CString			m_strFileName;
	KEMCSVEncoding	m_emCSVEncoding;
	
	std::vector<CString> m_vecColumn;
	
	std::vector<std::vector<CString>> m_vecPreviewVecData;

	std::set<int> m_setPurpose;
	std::set<int> m_setMode;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBack();
	afx_msg void OnBnClickedOk();

private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndDefineReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndPreviewReport;
};
