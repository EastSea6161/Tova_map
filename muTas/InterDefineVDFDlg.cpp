// InterDefineVDFDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterDefineVDFDlg.h"
#include "afxdialogex.h"

#include "InterBPRFunctionDlg.h"
#include "InterUserFunctionDlg.h"

#include "Target.h"
//#include "CodeGroup.h"
#include "InterFunctionBase.h"
#include "InterFunctionBPR.h"
#include "InterFunctionUser.h"


// KInterDefineVDFDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterDefineVDFDlg, KDialogEx)
KInterDefineVDFDlg::KInterDefineVDFDlg(KTarget* a_pTarget, KCodeGroup* a_pCodeData, CWnd* pParent /*=NULL*/)
	:	KDialogEx(KInterDefineVDFDlg::IDD, pParent),
		m_pTarget( a_pTarget),
		m_pCodeData( a_pCodeData )
{
	m_emFunctionType = KEMInterModalFunctionBPR;
	m_pFunctionBase = NULL;
}

KInterDefineVDFDlg::~KInterDefineVDFDlg()
{
	if(NULL != m_pFunctionBase)
	{
		delete m_pFunctionBase;
		m_pFunctionBase = NULL;
	}
}

void KInterDefineVDFDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KInterDefineVDFDlg, KDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, &KInterDefineVDFDlg::OnRadioClicked)
	ON_BN_CLICKED(IDC_BUTTON1, &KInterDefineVDFDlg::OnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &KInterDefineVDFDlg::OnClickedButton2)
END_MESSAGE_MAP()


// KInterDefineVDFDlg 메시지 처리기입니다.

KInterFunctionBase* KInterDefineVDFDlg::GetFunction(void) const
{
	return m_pFunctionBase;
}

void KInterDefineVDFDlg::SetFunction(KInterFunctionBase* pFunctionBase)
{
	if(NULL != pFunctionBase)
	{
		if(NULL != m_pFunctionBase)
		{
			delete m_pFunctionBase;
			m_pFunctionBase = NULL;
		}

		switch(pFunctionBase->FunctionType())
		{
		case KEMInterModalFunctionBPR : 
			m_pFunctionBase = new KInterFunctionBPR(*(KInterFunctionBPR*)pFunctionBase);
			break;
		case KEMInterModalFunctionUser :
			m_pFunctionBase = new KInterFunctionUser(*(KInterFunctionUser*)pFunctionBase);
			break;
		}
		m_emFunctionType = pFunctionBase->FunctionType();
	}
	else
	{
		m_pFunctionBase = new KInterFunctionBPR;
	}
}

BOOL KInterDefineVDFDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	UpdateControlState();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KInterDefineVDFDlg::UpdateControlState()
{
	if( KEMInterModalFunctionBPR == m_emFunctionType )
	{
		CButton* pRadio = (CButton*)(GetDlgItem(IDC_RADIO1));
		pRadio->SetCheck(TRUE);

		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	}
	else
	{
		CButton* pRadio = (CButton*)(GetDlgItem(IDC_RADIO2));
		pRadio->SetCheck(TRUE);

		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	}
}

void KInterDefineVDFDlg::OnRadioClicked(UINT nID)
{
	switch ( nID )
	{
	case IDC_RADIO1 : m_emFunctionType = KEMInterModalFunctionBPR;
		break;
	case IDC_RADIO2 : m_emFunctionType = KEMInterModalFunctionUser;
		break;
	}

	UpdateControlState();
}

void KInterDefineVDFDlg::OnClickedButton1()
{
	KInterBPRFunctionDlg dlg(m_pTarget, m_pCodeData);
	
	if((NULL != m_pFunctionBase) && (m_pFunctionBase->FunctionType() == KEMInterModalFunctionBPR))
	{
		dlg.SetFunction((KInterFunctionBPR*)m_pFunctionBase);
	}

	if(dlg.DoModal() == IDOK)
	{
		KInterFunctionBPR* pFunctionBPR = dlg.GetFunction();
		SetFunction(pFunctionBPR);
	}
}


void KInterDefineVDFDlg::OnClickedButton2()
{
	KInterUserFunctionDlg dlg(m_pTarget, m_pCodeData);
	dlg.DoModal();
}
