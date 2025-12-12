#pragma once
//^#include "ResizeDialogEx.h"
#include "KEMLayer.h"
class KTarget;
// KTocConfigLabelDlg 대화 상자입니다.



class KTocConfigLabelDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KTocConfigLabelDlg)

public:
	KTocConfigLabelDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTocConfigLabelDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOC_ConfigLabelDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	KTarget* m_pTarget;
	KEMLayer m_emLayerType;
	CString  m_strSelectColumns;
public:
	void SetTarget(KTarget* a_pTarget, KEMLayer a_emLayerType, CString a_strSelectColumns)
	{
		m_pTarget          = a_pTarget;
		m_emLayerType      = a_emLayerType;
		m_strSelectColumns = a_strSelectColumns;
	}
public:
	CString GetSelectColumns()
	{
		return m_strSelectColumns;
	}

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
protected:
	virtual BOOL OnInitDialog();

private:
	void InitReportHeader();
	void InitReportData();
private:
	afx_msg void OnBnClickedOk();

private: //★ 복수 라벨 선택할 수 없도록 처리
    afx_msg void OnReportItemClick  (NMHDR* pNMHDR, LRESULT* pResult);
};
