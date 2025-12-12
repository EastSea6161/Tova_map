#pragma once


// KDevPlanSelFacilityDlg 대화 상자입니다.

class KDevPlanSelFacilityDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KDevPlanSelFacilityDlg)

public:
	KDevPlanSelFacilityDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDevPlanSelFacilityDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5621_DevPlanSelFacilityDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL OnInitDialog();
};
