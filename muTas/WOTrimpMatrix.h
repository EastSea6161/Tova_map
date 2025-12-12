#pragma once
#include "afxwin.h"
//^#include "ResizeDialogEx.h"

class KTarget;
class KIOColumn;
class KMultiModalInfo;

// KWOTrimpMatrix 대화 상자입니다.

class KWOTrimpMatrix : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KWOTrimpMatrix)

public:
	KWOTrimpMatrix(KTarget* a_pTarget, std::list<KMultiModalInfo>& a_lstMaInfo,int& a_nUsingTime,KIOColumn*& a_pLinkColumn, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KWOTrimpMatrix();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6932_WOTrimpMatrix };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	KTarget*						m_pTarget;
	std::list<KMultiModalInfo>&	m_lstMAInfo;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndModeReport;
	std::set<CString>				m_setLinkTimeField;
	KIOColumn*&						m_pLinkTimeColumn;
	CComboBox						m_cboLinkTime;
	int&							m_nUsingLinkTravelTime;

protected:

	virtual BOOL OnInitDialog();

	void InitModeReportControl(void);

	void UpdateModeReportRecord(void);

	void SetLinkTaravelTimeField(void);

	bool ApplyInputData(void);

	void ResizeComponent();

protected:

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnRadioClicked(UINT nID);

};
