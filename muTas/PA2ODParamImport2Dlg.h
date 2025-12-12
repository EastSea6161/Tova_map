#pragma once

#include "kmz_common.h"

//forward declare
class KPA2ODInfo;

class KPA2ODParamImport2Dlg : public KDialogEx
{
	DECLARE_DYNAMIC(KPA2ODParamImport2Dlg)

public:
	KPA2ODParamImport2Dlg(
		BOOL a_bContainHeader, KEMCSVEncoding a_emCSVEncoding, TCHAR a_cSeparator,
		CString a_strFileName, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KPA2ODParamImport2Dlg(void);

	enum { IDD = IDD_5835_PA2ODParamImport2Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP();

public:
	bool GoBack(void);
	void HomeBasedInfo(std::list<KPA2ODInfo*> a_lstHomeBasedPA2ODInfo);
	void NotHBasedInfo(std::list<KPA2ODInfo*> a_lstNotHBasedPA2ODInfo);

private:
	void GenerateHeaderColumn(void);
	void ParseCSVLineString(CString& strLine, std::vector<CString>& row);

	void PreviewDataLoad(void);

	void InitZoneReport(void);
	void UpdateZoneReport(void);

	void InitHBReport(void);
	void UpdateHBReport(void);

	void InitNHBReport(void);
	void UpdateNHBReport(void);

	void InitPreviewReport(void);
	void UpdatePreviewReport(void);

	void WriteBinaryFile(void);
	void LineData2Binary(std::vector<CString> a_vecLineData, std::ofstream& ofs);

private:
	std::vector<CString>   			  m_vecColumnHeader;
	std::vector<std::vector<CString>> m_vecPreviewVecData;

	int m_nCurrentHeader;

	bool            	   			  m_bRunOK;
	bool				   			  m_bGoBack;
	BOOL				   			  m_bContainHeader;
	TCHAR				   			  m_cSeparator;
	CString				   			  m_strFileName;
	KEMCSVEncoding		   			  m_emCSVEncoding;

	std::list<KPA2ODInfo*> 			  m_lstHomeBasedPA2ODInfo;
	std::list<KPA2ODInfo*> 			  m_lstNotHBasedPA2ODInfo;

private:
	virtual BOOL OnInitDialog();

private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndZoneReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndHBReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndNHBReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndPreviewReport;

public:
	afx_msg void OnBnClickedBack();
	afx_msg void OnBnClickedOk();
};
