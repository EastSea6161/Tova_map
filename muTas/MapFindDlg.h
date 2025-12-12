#pragma once

//^#include "ResizeDialogEx.h"

class KTarget;
class KMapView;

struct TFindResult
{
	Integer TnxSeq;
	Integer TnxObjectID;
	CString TstrObjectName;
};

// KMapFindDlg 대화 상자입니다.

class KMapFindDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KMapFindDlg)

public:
	KMapFindDlg(KMapView* a_pMapView, KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KMapFindDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_3010_MapFindDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	KTarget*  m_pTarget;
	KMapView* m_pMapView;
	int       m_nType;
	Integer   m_nxID;

	std::vector<TFindResult> m_vecResult;

public:
	void GetUserSelectedInfo(int& ar_nType, Integer& ar_nxID);

	void Initialize(void);

private:
	void InitReportHeader(void);
	void InitReportData(void);
	void LoadRecentHistory(void);
	void LoadRecentNodeHist(void);
	void LoadRecentLinkHist(void);
	void InsertRecentObject(void);
	void DeleteSelObject(std::vector<Integer> a_vecObject);

private:
	afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRadio(UINT nID);
	afx_msg void OnReportRowDblClick(NMHDR* a_pNMHDR, LRESULT* a_pResult);

private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
public:
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedCheckAll();
};

typedef std::shared_ptr<KMapFindDlg> KMapFindDlgPtr;