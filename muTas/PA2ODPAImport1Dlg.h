#pragma once

class KPA2ODPAImport1Dlg : public KDialogEx
{
	DECLARE_DYNAMIC(KPA2ODPAImport1Dlg)

public:
	KPA2ODPAImport1Dlg(
		BOOL a_bContainHeader, int a_nSeparatorType, int a_nEncodingType,
		CString a_strFileName, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KPA2ODPAImport1Dlg(void);

	enum { IDD = IDD_5820_PA2ODPAImport1Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP();

public:
	void 	ContainHeader(BOOL a_bContainHeader);
	BOOL 	ContainHeader(void);

	void 	SeparatorType(int a_nSepartorType);
	int 	SeparatorType(void);

	void 	EncodingType(int a_nEncodingType);
	int 	EncodingType(void);

	void 	FileName(CString a_strFileName);
	CString FileName(void);

private:
	BOOL 	m_bContainHeader;
	int 	m_nSeparatorType;
	int		m_nEncodingType;
	CString	m_strFileName;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

private:
	CXTBrowseEdit m_edtFile;
};
