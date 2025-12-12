#pragma once
class KMapView;
// KNombaraTester 대화 상자입니다.

class KNombaraTester : public KDialogEx
{
	DECLARE_DYNAMIC(KNombaraTester)

public:
	KNombaraTester(KMapView* pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KNombaraTester();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_NOM_TESTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

    virtual BOOL OnInitDialog();
protected:
    afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
    void AddStatusMessage(CString a_strMsg);
    static unsigned __stdcall ReceivedThreadCaller(void* p);
    void Run();
private:
    KMapView* m_pMapView;
private:
    int m_nResultCode;
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedButton3();
    afx_msg void OnBnClickedButton4();
    afx_msg void OnBnClickedButton5();
    afx_msg void OnBnClickedCancel();
};
