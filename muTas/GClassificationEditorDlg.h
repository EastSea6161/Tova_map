#pragma once

#include "DBaseGeneration.h"
#include "afxwin.h"

class KTarget;


// KGClassificationEditorDlg 대화 상자입니다.

class KGClassificationEditorDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KGClassificationEditorDlg)

public:
	KGClassificationEditorDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KGClassificationEditorDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6131_GClassificationEdltorDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	KTarget*			m_pTarget;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;

private:
	CComboBox m_cboName;
	CComboBox m_cboAge;

protected:
	std::map<int, TCategoryClassInto> m_mapCategoryClassInfo;

protected:
	CString m_strPreValue;
	CString m_strPreCaption;

	int m_nClassID;

public:
	int GetClassID();

protected:
	void InitReportHeader(void);
	void UpdateReportData(void);
		void UpdateReportDataNew(void);
		void UpdateReportDataExist(void);
	void ChangeReportData(void);

	void CalculateClassGroup(int a_AgeCategory, std::map<int, KODKey>& ar_mapAgeMinMaxValue);
	void CalculateReportAge( void );

protected:
	void InitComboAge(void);
	void InitComboName(void);

	void ChangeControl( TCategoryClassInto a_oCategoryInfo);
	void ClearControl(void);

protected:
	void SaveExistCategory();
	void SaveNewCategory();

protected:
	void SaveCategoryClass(int a_nMaxID);
	void SaveCategoryAgeGroup(int a_nMaxID);
	void SaveCategoryClassValue(int a_nMaxID);
	void SaveCategoryClassValueUseSex(std::map<KODKey, double>& ar_mapCategroyValue);
	void SaveCategoryClassValueNotUseSex(std::map<KODKey, double>& ar_mapCategroyValue);

	void ResizeComponet(void);
protected:

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedRadio( UINT nID );
	afx_msg void OnBnClickedCheckSex();
	afx_msg void OnBnClickedCheckAge();
	afx_msg void OnCbnSelchangeComboAge();
	afx_msg void OnCbnSelchangeComboName();

	
	afx_msg void OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportItemValueChanged(NMHDR* pNotifyStruct, LRESULT* pResult);
	
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	
};
