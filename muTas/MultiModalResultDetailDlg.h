#pragma once
//^#include "ResizeDialogEx.h"
#include "MultiModalResultInfo.h"
#include "TransitPathViewDlg.h"
class KTarget;
// KMultiModalResultDetailDlg 대화 상자입니다.

class KMultiModalResultDetailDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KMultiModalResultDetailDlg)

public:
	KMultiModalResultDetailDlg(KTarget* a_pTarget, bool a_bShowTrip, std::map<Integer, CString> &a_mapNodeName, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KMultiModalResultDetailDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6941_MultiModalResultDetailDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void         InitializeData(CString a_strPathIdex, KMBusAssignInfo* a_pTrasitAssignInfo, CRect rectParent);
	void         InitializeEditData(CString a_strPathIdex, KMTransitEditInfo* a_pTrasitEditInfo, CRect rectParent);
	void         ActivatedFrameWndNotifyProcess();
	void         DeActivatedFrameWndNotifyProcess();

private:
	void         InitReportAHeader();
	void         InitReportBHeader();

	void         UpdateReportAData    (KMBusAssignInfo* a_pTrasitAssignInfo);
	void         UpdateReportBData    (KMBusAssignInfo* a_pTrasitAssignInfo);
	void         UpdateEditReportAData(KMTransitEditInfo* a_pTrasitEditInfo);
	void         UpdateEditReportBData(KMTransitEditInfo* a_pTrasitEditInfo);

	void         GetPathInfoData      (KMBusAssignInfo* a_pTrasitAssignInfo, TPathSubData &a_oTPathSubData);
	void         GetEditPathInfoData  (KMTransitEditInfo* a_pTrasitEditInfo, TPathSubData &a_oTPathSubData);
	void         ResizeComponent();

private:
	KTarget*     m_pTarget;
	CWnd*        m_pParentWnd;
	bool         m_bShowTrip;

private:
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

typedef std::shared_ptr<KMultiModalResultDetailDlg> KMultiModalResultDetailDlgPtr;