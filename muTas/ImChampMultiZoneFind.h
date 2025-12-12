#pragma once
#include "afxwin.h"
//^#include "ResizeDialogEx.h"
#include "IDCaption.h"

class KTarget;
class KIOColumn;

// KImTasMulitiZoneFind 대화 상자입니다.

class KImChampMultiZoneFind : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KImChampMultiZoneFind)

public:
	KImChampMultiZoneFind(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KImChampMultiZoneFind();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_3013_FIND_MULTI_ZONE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()


private:

	bool m_bUseTransitInclude;
	bool m_bChekTransitInclude;

	std::map<int, KIOColumn*>			m_mapZoneColumn;
	std::map<Integer, TIDZoneCaption>	m_mapZoneInfo;

	std::set<Integer>	m_setSelectedZone;

	CString				m_strPreSearch;
	CString				m_strTitleName;

	KTarget*			m_pTarget;
	CImageList			m_imageListSelectIcon;

	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportUp;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportDown;
	CComboBox			m_cboFindColumn;

public:
	void SetSelectedID(std::set<Integer> a_setSelectedZone);

	void GetSelectedID(std::set<Integer>& a_setSeletedZone);

	void SetTitleName(CString a_strTitleName);

	void SetUseTransitInclude(bool a_bTransitUsed);

	bool GetTransitInclude();

protected:

	void InitialReportControl();

	void InitComboBox();

	void GetFindZoneColumn(void);

	void UpdateReportControlUp(CString a_strFilter);

	void UpdateReportControlDown();

protected:

	void GetZoneInfo(void);

protected:
	virtual BOOL OnInitDialog();
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnEnChangeEditSearch();
	afx_msg void OnBnClickedButtonFind();
	afx_msg void OnReportItemClickUp  (NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReportItemClickDown (NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedCheckSelectAll();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
