// InterRegionChoiceParaDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterRegionChoiceParaDlg.h"
#include "afxdialogex.h"
#include "Target.h"

// KInterRegionChoiceParaDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterRegionChoiceParaDlg, KResizeDialogEx)

KInterRegionChoiceParaDlg::KInterRegionChoiceParaDlg(KTarget* a_pTarget, double a_dRegionanlMEU, std::map<int, KRegionalChoiceParameter> a_mapRegionalChoiceParameter,CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KInterRegionChoiceParaDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_dRegionalMEU(a_dRegionanlMEU)
	, m_mapRegionalChoiceParameter(a_mapRegionalChoiceParameter)
	, m_pCurrentShowClientDlg(nullptr)
{

}

KInterRegionChoiceParaDlg::~KInterRegionChoiceParaDlg()
{
	ClearSubPage();
}

void KInterRegionChoiceParaDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabCtrl);
}


BEGIN_MESSAGE_MAP(KInterRegionChoiceParaDlg, KResizeDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &KInterRegionChoiceParaDlg::OnTcnSelchangeTab1)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &KInterRegionChoiceParaDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KInterRegionChoiceParaDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &KInterRegionChoiceParaDlg::OnBnClickedButtonDefault)
	ON_BN_CLICKED(IDC_BUTTON_COPY, &KInterRegionChoiceParaDlg::OnBnClickedButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_PASTE, &KInterRegionChoiceParaDlg::OnBnClickedButtonPaste)
END_MESSAGE_MAP()


// KInterRegionChoiceParaDlg 메시지 처리기입니다.


BOOL KInterRegionChoiceParaDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	m_mapSubPage.clear();
	int nTabIndex(0);

	AutoType iter = m_mapRegionalChoiceParameter.begin();
	AutoType end  = m_mapRegionalChoiceParameter.end();
	while(iter != end)
	{
		KRegionalChoiceParameter oRegionalParameter = iter->second;

		CRect oTabRect;
		m_tabCtrl.GetClientRect(&oTabRect);
		KInterRegionalChoiceParaSubPage* pSubPage = new KInterRegionalChoiceParaSubPage;
		pSubPage->SetParameter(oRegionalParameter);
		pSubPage->SetPageID(iter->first);
		pSubPage->Create(IDD_6874_InterRegionChoiceParaSubPage, &m_tabCtrl);
		pSubPage->SetWindowPos(NULL, TAB_LEFT_MARGIN, TAB_TOP_MARGIN, oTabRect.Width()-TAB_WIDTH_MARGIN, oTabRect.Height()-TAB_HEIGHT_MARGIN, SWP_NOZORDER);
		m_tabCtrl.InsertItem(nTabIndex, oRegionalParameter.pPurpose->PurposeName());
		m_mapSubPage.insert(std::make_pair(nTabIndex, pSubPage));

		nTabIndex++;
		++iter;
	}

	DisplayTabControl(0);

	CString strValue(_T(""));
	strValue.Format(_T("%.2f"), m_dRegionalMEU);
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MEU);
	SetDlgItemText(IDC_EDIT_MEU, strValue);

	ResizeComponet();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KInterRegionChoiceParaDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	DisplayTabControl(m_tabCtrl.GetCurSel());
	*pResult = 0;
}


void KInterRegionChoiceParaDlg::OnSize(UINT nType, int cx, int cy)
{
	KResizeDialogEx::OnSize(nType, cx, cy);

	SubPageRedrawWindow();
}


void KInterRegionChoiceParaDlg::SubPageRedrawWindow()
{
	try
	{
		if (nullptr != m_pCurrentShowClientDlg)
		{        
			CRect oTabRect;
			m_tabCtrl.GetClientRect(&oTabRect);	

			m_pCurrentShowClientDlg->SetWindowPos(NULL, TAB_LEFT_MARGIN, TAB_TOP_MARGIN, oTabRect.Width()-TAB_WIDTH_MARGIN, oTabRect.Height()-TAB_HEIGHT_MARGIN, SWP_NOZORDER);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterRegionChoiceParaDlg::OnBnClickedOk()
{
	try
	{
		SaveRegionChoicePara();
		if (SaveRegionMEU() == false)
		{
			QBicMessageBox::MessageBox(_T("MEU의 값이 잘못되었습니다."), _T("Information"));
			return;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		QBicMessageBox::MessageBox(_T("저장중 오류가 생겼습니다."), _T("Information"));
		KResizeDialogEx::OnCancel();
	}
	catch(...)
	{
		TxLogDebugException();
		QBicMessageBox::MessageBox(_T("저장중 오류가 생겼습니다."), _T("Information"));
		KResizeDialogEx::OnCancel();
	}

	KResizeDialogEx::OnOK();
}


void KInterRegionChoiceParaDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

void KInterRegionChoiceParaDlg::DisplayTabControl( int a_nTabIndex )
{
	int nIndex = a_nTabIndex;

	if (m_pCurrentShowClientDlg != nullptr)
	{
		m_pCurrentShowClientDlg->ShowWindow(SW_HIDE);
		m_pCurrentShowClientDlg = nullptr;
	}

	AutoType find = m_mapSubPage.find(nIndex);
	AutoType end  = m_mapSubPage.end();
	if (find != end)
	{
		m_pCurrentShowClientDlg = find->second;
		m_pCurrentShowClientDlg->ShowWindow(SW_SHOW);
		SubPageRedrawWindow();
	}
}

void KInterRegionChoiceParaDlg::ResizeComponet()
{
	SetResize(IDC_STATIC_MEU,		SZ_TOP_LEFT,       SZ_TOP_LEFT);
	SetResize(IDC_EDIT_MEU,			SZ_TOP_LEFT,      SZ_TOP_LEFT);

	SetResize(IDC_TAB1,				SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	
	SetResize(IDC_BUTTON1,			SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);
	SetResize(IDOK,					SZ_BOTTOM_RIGHT,   SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,				SZ_BOTTOM_RIGHT,   SZ_BOTTOM_RIGHT);

	SetResize(IDC_STATIC_SHEET, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_COPY,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_PASTE, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

}

void KInterRegionChoiceParaDlg::SaveRegionChoicePara()
{
	m_mapRegionalChoiceParameter.clear();
	try
	{
		AutoType iter = m_mapSubPage.begin();
		AutoType end  = m_mapSubPage.end();
		while(iter != end)
		{
			KInterRegionalChoiceParaSubPage* pSubPage = iter->second;
			KRegionalChoiceParameter oRegionalParameter;
			pSubPage->GetParameter(oRegionalParameter);
			m_mapRegionalChoiceParameter.insert(std::make_pair(oRegionalParameter.pPurpose->PurposeID(), oRegionalParameter));
			++iter;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

bool KInterRegionChoiceParaDlg::SaveRegionMEU()
{
	try
	{
		CString strValue(_T(""));
		GetDlgItemText(IDC_EDIT_MEU, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			GetDlgItem(IDC_EDIT_MEU)->SetFocus();
			return false;
		}
		m_dRegionalMEU = _ttof(strValue);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}

void KInterRegionChoiceParaDlg::GetRegionParameter( double& ar_dRegionMEU, std::map<int, KRegionalChoiceParameter>& ar_mapRegionChoicePara )
{
	try
	{
		ar_dRegionMEU			= m_dRegionalMEU;
		ar_mapRegionChoicePara	= m_mapRegionalChoiceParameter;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KInterRegionChoiceParaDlg::ClearSubPage()
{
	try
	{
		AutoType iter = m_mapSubPage.begin();
		AutoType end  = m_mapSubPage.end();
		while(iter != end)
		{
			QBicDelete(iter->second);
			++iter;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KInterRegionChoiceParaDlg::SetDefaultContent( std::map<int, KRegionalChoiceParameter> a_mapRegionalChoiceParameter_Default, double a_dRegionalChoiceMEU_Default )
{
	m_mapRegionalChoiceParameter_Default = a_mapRegionalChoiceParameter_Default;
	m_dRegionalChoiceMEU_Default = a_dRegionalChoiceMEU_Default;
}


void KInterRegionChoiceParaDlg::SetDefault()
{
	try
	{
		//MEU Initialize
		CString strValue;
		strValue.Format(_T("%.2f"), m_dRegionalChoiceMEU_Default);
		SetDlgItemText(IDC_EDIT_MEU, strValue);

		//Tab Initialize
		AutoType bIter = m_mapSubPage.begin();
		AutoType eIter = m_mapSubPage.end();

		while (bIter != eIter)
		{
			KInterRegionalChoiceParaSubPage* pPage = bIter->second;
			int nPageID = pPage->GetPageID();

			AutoType itFind = m_mapRegionalChoiceParameter_Default.find(nPageID);
			if (itFind != m_mapRegionalChoiceParameter_Default.end())
			{
				KRegionalChoiceParameter &oRegionalChoiceParameter = itFind->second;
				pPage->UpdateReportData(oRegionalChoiceParameter.mapModeRegionalChoiceValue);
			}

			++bIter;
		}

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		QBicMessageBox::MessageBox(_T("초기화중 오류가 생겼습니다."), _T("Information"));
		KResizeDialogEx::OnCancel();
	}
	catch (...)
	{
		TxLogDebugException();
		QBicMessageBox::MessageBox(_T("초기화중 오류가 생겼습니다."), _T("Information"));
		KResizeDialogEx::OnCancel();
	}
}


void KInterRegionChoiceParaDlg::OnBnClickedButtonDefault()
{
	SetDefault();
}


void KInterRegionChoiceParaDlg::OnBnClickedButtonCopy()
{
	m_tmpMapModeRegionalChoiceValue.clear();

	try
	{
		KRegionalChoiceParameter oRegionalChoiceParameter;

		int nTabIndex = m_tabCtrl.GetCurSel();

		std::map<int, KInterRegionalChoiceParaSubPage*>::iterator fIter = m_mapSubPage.find(nTabIndex);

		if (fIter != m_mapSubPage.end())
		{
			fIter->second->GetParameter(oRegionalChoiceParameter);
			m_tmpMapModeRegionalChoiceValue = oRegionalChoiceParameter.mapModeRegionalChoiceValue;
		}
		else
		{
			throw 1;
		}

		GetDlgItem(IDC_BUTTON_PASTE)->EnableWindow(TRUE);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}


void KInterRegionChoiceParaDlg::OnBnClickedButtonPaste()
{
	try
	{
		int nTabIndex = m_tabCtrl.GetCurSel();

		std::map<int, KInterRegionalChoiceParaSubPage*>::iterator fIter = m_mapSubPage.find(nTabIndex);

		if (fIter != m_mapSubPage.end())
		{
			fIter->second->UpdateReportData(m_tmpMapModeRegionalChoiceValue);
		}
		else
		{
			throw 1;
		}
		
		AfxMessageBox(_T("복사되었습니다."));
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}
