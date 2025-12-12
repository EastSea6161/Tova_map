#pragma once

//^#include "ResizeDialogEx.h"
//^^ #include "IOCommons.h"

class KTarget;
class CStdioFileEx;

// KTableImpEmmeMatrixDlg 대화 상자입니다.

class KTableImpEmmeMatrixDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KTableImpEmmeMatrixDlg)

public:
	KTableImpEmmeMatrixDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTableImpEmmeMatrixDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4176_TableImpEmmeMatrixDlg };

	typedef std::vector< CString > CSVRow;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	void InitMatrixComboCtrl(void);
	void OnCbnSelChangeMatrix(void);
	void InitColumnComboCtrl(void);

	static void               AddStatusMessage(CString a_strMsg);
	static unsigned __stdcall ImpEmmeMatrixThreadCaller(void* p);

	void Text2Binary(void);
	void ImportMatrixData(KDBaseConPtr spDBaseConnection);

	void Run(void);

	void ParseCSVLineString( CString& strLine, CSVRow& row );
	void Line2Binary(int a_nLine, CString a_strOriginID, CSVRow a_row, std::ofstream& ofs);
	void WriteOutputStream( CString a_strImportValue, KEMIODataType a_emDataType, std::ofstream& ofs );

	void RemoveDoubleSpace(CString& strLine);
	void RemoveSpaceColonOnLeft(CString& a_strLine);
	void RemoveSpaceColonOnRight(CString& a_strLine);

	void WriteErrorLog(CString a_strMsg);

	bool IsValidZoneID(CString a_strZoneID);

private:
	KTarget* m_pTarget;
	CString  m_strFileName;
	bool     m_bThreadOK;
	CString  m_strErrMsg;
	TCHAR    m_cSeparator;

	int      m_nDataCount;	//Import Data 전체개수

private:
	std::set<Integer> m_setZoneID;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

private:
	CXTPBrowseEdit m_editFile;
	CComboBox      m_cboMatrix;
	CComboBox      m_cboColumn;
};
