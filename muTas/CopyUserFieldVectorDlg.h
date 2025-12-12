#pragma once
#include "afxwin.h"
#include "TableManager.h"

class KProject;
class KTarget;
// KCopyUserFieldVectorDlg 대화 상자입니다.

class KCopyUserFieldVectorDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KCopyUserFieldVectorDlg)

public:
	KCopyUserFieldVectorDlg(KProject* a_pProject, CString a_strTableName, std::vector<TUserColumnInfo> &a_vecUserColumnInfo, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KCopyUserFieldVectorDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8101_CopyUserFiledDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	KTarget*     GetSelectedTarget()
	{
		return m_pSelTarget;
	}

private:
	void         ResizeComponent();

	void         InitComboScenario();
	void         InitComboTarget();

	void         InitReportHeader();
	void         UpdateReportData();

private:
	KProject*    m_pProject;
	CString      m_strTableName;
	std::vector<TUserColumnInfo> &m_vecUserColumnInfo;
	std::vector<KIOColumn*>       m_vecUpdateColumn;

	KTarget*     m_pSelTarget;

protected:
	virtual BOOL OnInitDialog();

	afx_msg void OnCbnSelchangeComboScenario();
		void     ControlComboScenario();
	
	afx_msg void OnCbnSelchangeComboTarget();
		void	 ControlComboTarget();
			void InitTableDataField();
			void RedrawComboCopyField();
			void ChangeItemToNormal  (CXTPReportRecord* a_pRecord, CString a_strDefaultCaption);
			void ChangeItemToComboBox(CXTPReportRecord* a_pRecord, std::vector<KIOColumn*> &a_vecUpdateColumn);
	
	afx_msg void OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);

	afx_msg void OnBnClickedOk();
		bool     InvalidateInputInfo();
		bool     InvalidateAddColumn(KTarget* a_pTarget, std::set<CString> &a_setAddColumnName, std::vector<CString> &a_vecWrongColumnName);
	
	afx_msg void OnBnClickedCancel();
private:
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReport;
	CComboBox    m_cboScenario;
	CComboBox    m_cboTarget;
};
