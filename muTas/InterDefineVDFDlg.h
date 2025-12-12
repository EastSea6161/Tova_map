#pragma once

#include "resource.h"
#include "model_common.h"

// KInterDefineVDFDlg 대화 상자입니다.

/*forward declarations*/
class KTarget;
class KCodeGroup;
class KInterFunctionBase;

class KInterDefineVDFDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KInterDefineVDFDlg)

public:
	
	KInterDefineVDFDlg(KTarget* a_pTarget, KCodeGroup* a_pCodeData, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterDefineVDFDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6920_InterDefineVDFDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	
	void UpdateControlState(void);

public:

	KInterFunctionBase* GetFunction(void) const;

	void SetFunction(KInterFunctionBase* a_pFunction);
	
protected:
	
	KEMInterModalFunction	m_emFunctionType;
	KTarget*				m_pTarget;
	KCodeGroup*				m_pCodeData;
	KInterFunctionBase*		m_pFunctionBase;

protected:

	virtual BOOL OnInitDialog();
	afx_msg void OnRadioClicked(UINT nID);
	afx_msg void OnClickedButton1();
	afx_msg void OnClickedButton2();

};
