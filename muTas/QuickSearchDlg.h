#pragma once

class KIOTableRecord;

// KQuickSearchDlg 대화 상자입니다.

class KQuickSearchDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KQuickSearchDlg)

public:
	KQuickSearchDlg(KIOTable* a_pTable, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KQuickSearchDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4125_QuickSearchDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	inline CString GetFilter(void)
	{
		return m_strFilter;
	}

public:
    void GetSelectedID(std::set<Integer> &a_setSelectedID)
    {
        a_setSelectedID = m_setSelection;
    }
    void SetSelectedID(std::set<Integer> &a_setSelectedID)
    {
        m_setSelection = a_setSelectedID;
    }

private:
	void InitColumns(void);
	void InitResizeCtrl(void);
	void InitComboCtrl(void);
	void InitReportHeader(void);
	void DrawReport(CString a_strFilter=_T(""));
	void DrawReport2(void);

	void UpdateReportHeader(void);
	void ConvertCodeData();

private:
	KIOTable*   m_pTable;
	KIOColumns* m_pColumns;
	KIOColumn*  m_pKeyColumn;
	CString     m_strFilter;

private:
	std::set<Integer> m_setSelection;	//선택된 Record의 DB상의 RowID
	std::map<Integer, std::vector<CString>> m_mapData;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnChangeEditSearch();
	afx_msg void OnBnClickedButtonFind();
	afx_msg void OnReportItemClick(NMHDR* a_pNMHDR, LRESULT* a_pResult);
	afx_msg void OnReport1ItemClick(NMHDR* a_pNMHDR, LRESULT* a_pResult);
	afx_msg void OnBnClickedCheckSelectAll(void);

private:
	CComboBox         m_cboFindColumn;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndSelReport;
	
public:
	afx_msg void OnSelchangeCombo1();
};
