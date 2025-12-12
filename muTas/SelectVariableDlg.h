#pragma once

class KTarget;


struct TUseColumnInfo
{
	CString					strPurposeName;
	CString					strModeNodeName;
	std::vector<CString>	vecUsedColumnName;
};


// KSelectVariableDlg 대화 상자입니다.

class KSelectVariableDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KSelectVariableDlg)

public:
	KSelectVariableDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KSelectVariableDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6410_SelectVariableDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:

	KTarget*						m_pTarget;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
	std::map<CString, CString>		m_mapUseColumnInfo;

public:

	void SetTarget(KTarget* a_pTarget)
	{
		m_pTarget = a_pTarget;
	}

	void ParameterInfo(std::vector<TUseColumnInfo> vecUseColumnInfo);

	void GetChangedFunction(std::map<CString, CString>& a_mapFunction);

protected:

	virtual BOOL OnInitDialog();

	void InitReport();

	void UpdateReport();

	CString FindColumnCaption(CString a_strColumnName);

	CString FindColumnName(CString a_strColumnCaption);
	
public:
	afx_msg void OnBnClickedOk();
};
