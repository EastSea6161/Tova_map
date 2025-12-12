#pragma once
//^#include "ResizeDialogEx.h"
#include "InterModalPathInfo.h"
#include "TransitPathViewDlg.h"
class KTarget;


// KInterResultDetailDlg 대화 상자입니다.

class KInterResultDetailDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KInterResultDetailDlg)

public:
	KInterResultDetailDlg(KTarget* a_pTarget, bool a_bShowTrip, std::map<Integer, CString> &a_mapNodeName, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterResultDetailDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6861_InterAssignResultDetailDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void		 ActivatedFrameWndNotifyProcess();
	void         DeActivatedFrameWndNotifyProcess();

public:
	void         InitializeEditData(KEMInterModalPathMode a_emInterModalPathMode, CString a_strPathIdex, KInterPathEditInfo* a_pTrasitEditInfo, CRect rectParent);
private:
	void         GetActiveDetailInfoData(KInterPathEditInfo* a_pTrasitEditInfo, std::vector<TInResultDetailActiveInfo> &a_vecActiveDetailInfo);
	void         RegisterDetailInfo(CString a_strContentName, double a_dValue, std::vector<TInResultDetailActiveInfo> &a_vecActiveDetailInfo);

	void         InitReportAHeader(std::vector<TInResultDetailActiveInfo> &a_vecActiveDetailInfo);
	void         InitReportBHeader();

	void         UpdateEditReportAData(std::vector<TInResultDetailActiveInfo> &a_vecActiveDetailInfo);
	void         UpdateEditReportBData(KInterPathEditInfo* a_pTrasitEditInfo);

	void         ResizeComponent();

private:
	KTarget*     m_pTarget;
	CWnd*        m_pParentWnd;
	bool         m_bShowTrip;
	KEMInterModalPathMode m_emInterModalPathMode;

	std::map<Integer, CString> m_mapNodeName;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnReportBItemDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReportBItemClick (NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnItemButtonClick (NMHDR* pNotifyStruct, LRESULT* pResult);
	void         GetTransitName(KTarget* a_pTarget, Integer &a_nxTransitID, CString &a_strTransitName);
	afx_msg void OnBnClickedCancel();
	

private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportACtrl;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportBCtrl;
private:
	KTransitPathViewDlgPtr m_spTransitPathViewDlg;
};

typedef std::shared_ptr<KInterResultDetailDlg> KInterResultDetailDlgPtr;