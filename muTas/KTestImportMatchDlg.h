#pragma once


class CXTPReportControlCustom : public CXTPReportControl
{
public :
	CXTPReportControlCustom()
	{

	}

	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point, int nState)
	{
		DROPEFFECT dropeffect = CXTPReportControl::OnDragOver(pDataObject,dwKeyState,point,nState);

		point.y += 9;// CXTPReportControl::OnDraw 에서 DrawDropMarker offset이 -4 ~ +4 인 관계로 실제 마우스 over 위치를 이에 맞게 조절 함: jyk 
		CXTPReportRow *pRow = HitTest(point);
		SetFocusedRow(pRow);

		return dropeffect;
	}
};

// KTestImportMatchDlg 대화 상자입니다.

class KTestImportMatchDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KTestImportMatchDlg)

public:
	KTestImportMatchDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTestImportMatchDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_9999_TestImportMatchDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void Input();
	CXTPOfficeBorder<CXTPReportControlCustom,false> m_ctrlReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReport2;

	std::vector<CString> m_vecAlpha;
	afx_msg void OnReportDrop2(NMHDR *pNotifyStruct, LRESULT *pResult);
	afx_msg void OnReportBeginDrag2(NMHDR *pNotifyStruct, LRESULT *pResult);
	afx_msg void OnReportDrop1(NMHDR *pNotifyStruct, LRESULT *pResult);
	afx_msg void OnReportRecordsDropped1(NMHDR *pNotifyStruct, LRESULT *pResult);
};
