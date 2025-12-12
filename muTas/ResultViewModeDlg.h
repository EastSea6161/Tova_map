#pragma once
#include "afxwin.h"
#include "ResultViewTreeCtrl.h"

/*declare forward*/
class KViewResultInfo;
class KODResultViewVRecord;

// KResultViewModeDlg 대화 상자입니다.

class KResultViewModeDlg : public CXTResizeDialog
{
	DECLARE_DYNAMIC(KResultViewModeDlg)

public:
	KResultViewModeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KResultViewModeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5512_ResultViewModeDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	void	InitReportControl( void );
	void	UpdateReportControl( void );

private:
	std::vector<KViewResultInfo>	m_vecKViewResultInfo;
	KODResultViewVRecord*			m_pVirtualRecord;
	KResultViewTreeCtrl             m_treeState;
	CXTPTreeCtrl*                   m_pTreeFocus;
	CXTPReportControl	            m_wndReport;
	HICON                           m_hICon;

public:
	KODResultViewVRecord*  GetVirtualRecord()
	{
		return m_pVirtualRecord;
	}

protected:
	virtual BOOL OnInitDialog();	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();	
	afx_msg void OnBnClickedButtonSaveText();

private:
	CHOICE_MODEL m_emModel;
public:
	void SetTable(CHOICE_MODEL emModel)
	{
		m_emModel = emModel;
	}
	
private:
	CString m_strWriteTextFileName;

public:
	CString GetWriteTextFileName()
	{
		return m_strWriteTextFileName;
	}
private:
	static unsigned __stdcall WriteTextThreadProc(void* pParam);
	static void WriteTextFile(CString strFileName, std::map<KODKey, KSDoubleRecord*>& ar_mapResultRecord);
};
