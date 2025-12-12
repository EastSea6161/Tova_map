// InterModeDefineDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterModeDefineDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "DBaseDefaultInterRunOption.h"

// KInterModeDefineDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterModeDefineDlg, KResizeDialogEx)

KInterModeDefineDlg::KInterModeDefineDlg(KTarget* a_pTarget, double a_dAnalyTime, std::vector<TInterModeDefine> a_vecUrban,std::vector<TInterModeDefine> a_vecRegion, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KInterModeDefineDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_dAnalysisTime(a_dAnalyTime)
	, m_vecModeDefineUrban(a_vecUrban)
	, m_vecModeDefineRegion(a_vecRegion)
{

}

KInterModeDefineDlg::~KInterModeDefineDlg()
{
}

void KInterModeDefineDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_URBAN, m_wndReportCtrlUrban);
	DDX_Control(pDX, IDC_REPORT_REGION, m_wndReportCtrlRegion);
}


BEGIN_MESSAGE_MAP(KInterModeDefineDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KInterModeDefineDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KInterModeDefineDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &KInterModeDefineDlg::OnBnClickedButtonDef)
END_MESSAGE_MAP()


// KInterModeDefineDlg 메시지 처리기입니다.


BOOL KInterModeDefineDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndReportCtrlUrban, TRUE);
	KReportCtrlSetting::Default(m_wndReportCtrlRegion, TRUE);

	InitReportHeaderUrban();
	InitReportHeaderRegion();

	UpdateReportDataUrban();
	UpdateREportDataRegion();

	CString strValue(_T(""));
	strValue.Format(_T("%.2f"), m_dAnalysisTime);
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_ATIME);
	SetDlgItemText(IDC_EDIT_ATIME, strValue);

	ResizeComponent();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KInterModeDefineDlg::ResizeComponent()
{
	try
	{
		SetResize(IDC_STATIC11,			      SZ_TOP_LEFT,      SZ_TOP_RIGHT);
		SetResize(IDC_REPORT_URBAN,			  SZ_TOP_LEFT,      SZ_MIDDLE_RIGHT);
		SetResize(IDC_STATIC10,	     		  SZ_MIDDLE_LEFT,   SZ_MIDDLE_RIGHT);

		SetResize(IDC_STATIC,			      SZ_MIDDLE_LEFT,   SZ_MIDDLE_LEFT);		
		SetResize(IDC_REPORT_REGION,	      SZ_MIDDLE_LEFT,   SZ_BOTTOM_RIGHT);
		SetResize(IDC_STATIC4,			      SZ_BOTTOM_LEFT,   SZ_BOTTOM_RIGHT);

		SetResize(IDC_BUTTON1,			      SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);

		SetResize(IDOK,					      SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
		SetResize(IDCANCEL,					  SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterModeDefineDlg::InitReportHeaderUrban( void )
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndReportCtrlUrban.AddColumn(new CXTPReportColumn(0, _T("Mode"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);

    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("통행수단"));
    }

	pColumn = m_wndReportCtrlUrban.AddColumn(new CXTPReportColumn(1, _T("Occupancy"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("평균 재차인원"));
    }

	pColumn = m_wndReportCtrlUrban.AddColumn(new CXTPReportColumn(2, _T("PCE"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("승용차 환산계수"));
    }
}

void KInterModeDefineDlg::InitReportHeaderRegion( void )
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndReportCtrlRegion.AddColumn(new CXTPReportColumn(0, _T("Mode"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("통행수단"));
    }

	pColumn = m_wndReportCtrlRegion.AddColumn(new CXTPReportColumn(1, _T("Occupancy"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("평균 재차인원"));
    }

	pColumn = m_wndReportCtrlRegion.AddColumn(new CXTPReportColumn(2, _T("PCE"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("승용차 환산계수"));
    }
}

void KInterModeDefineDlg::UpdateReportDataUrban( void )
{
	m_wndReportCtrlUrban.ResetContent(TRUE);

	try
	{
		size_t nCount = m_vecModeDefineUrban.size();
		for (size_t i = 0; i < nCount; i++)
		{
			TInterModeDefine oInfo = m_vecModeDefineUrban[i];
			CXTPReportRecord*		pRecord = m_wndReportCtrlUrban.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem*	pItem	= nullptr;

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(oInfo.strModeName));
			pItem->SetEditable(FALSE);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetAlignment(DT_CENTER);
			pItem->SetItemData(oInfo.nModeID);

			CString strOccupancy(_T(""));
			strOccupancy.Format(_T("%.4f"), oInfo.dOccupancy);
			if (oInfo.nModeID != -3)
			{
				pItem = pRecord->AddItem((new CXTPReportRecordItemText(strOccupancy)));
				pItem->SetEditable(TRUE);
				pItem->SetAlignment(DT_CENTER);
			}
			else
			{
				pItem = pRecord->AddItem((new CXTPReportRecordItemText(_T("-"))));
				pItem->SetEditable(FALSE);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetAlignment(DT_CENTER);
			}
			
			CString strPCE(_T(""));
			strPCE.Format(_T("%.3f"), oInfo.dPCE);
			pItem = pRecord->AddItem((new CXTPReportRecordItemText(strPCE)));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);
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
	

	m_wndReportCtrlUrban.Populate();
}

void KInterModeDefineDlg::UpdateREportDataRegion( void )
{
	m_wndReportCtrlRegion.ResetContent(TRUE);

	try
	{
		size_t nCount = m_vecModeDefineRegion.size();
		for (size_t i = 0; i < nCount; i++)
		{
			TInterModeDefine oInfo = m_vecModeDefineRegion[i];
			CXTPReportRecord*		pRecord = m_wndReportCtrlRegion.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem*	pItem	= nullptr;

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(oInfo.strModeName));
			pItem->SetEditable(FALSE);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetAlignment(DT_CENTER);
			pItem->SetItemData(oInfo.nModeID);

			CString strOccupancy(_T(""));
			strOccupancy.Format(_T("%.4f"), oInfo.dOccupancy);
			if (oInfo.nModeID == -1)
			{
				pItem = pRecord->AddItem((new CXTPReportRecordItemText(strOccupancy)));
				pItem->SetEditable(TRUE);
				pItem->SetAlignment(DT_CENTER);
			}
			else
			{
				pItem = pRecord->AddItem((new CXTPReportRecordItemText(_T("-"))));
				pItem->SetEditable(FALSE);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetAlignment(DT_CENTER);
			}

			CString strPCE(_T(""));
			strPCE.Format(_T("%.3f"), oInfo.dPCE);
			pItem = pRecord->AddItem((new CXTPReportRecordItemText(strPCE)));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);
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

	m_wndReportCtrlRegion.Populate();
}


void KInterModeDefineDlg::OnBnClickedOk()
{
	m_vecModeDefineRegion.clear();
	m_vecModeDefineUrban.clear();

	try
	{

		CString strATime;
		GetDlgItemText(IDC_EDIT_ATIME, strATime);
		m_dAnalysisTime = _ttof(strATime);

		CXTPReportRecords*				pUrbanRecords  = m_wndReportCtrlUrban.GetRecords();
		CXTPReportRecords*				pRegionRecords = m_wndReportCtrlRegion.GetRecords();
		CXTPReportRecord*				pRecord  = nullptr;
		CXTPReportRecordItemText*       pItemTxt = nullptr;

		CString strName(_T(""));
		double  dOccupance(0.0);
		double  dPCE(0.0);

		//urban report
		int nUrbanCount = pUrbanRecords->GetCount();
		for (int i = 0; i < nUrbanCount; i++)
		{
			pRecord = pUrbanRecords->GetAt(i);
			pItemTxt = (CXTPReportRecordItemText*)pRecord->GetItem(0);
			
			CString strModeName = pItemTxt->GetCaption(nullptr);
			int     nModeID     = pItemTxt->GetItemData();

			double dOccupance(0.0);
			if (strModeName != _T("Bus"))
			{
				pItemTxt = (CXTPReportRecordItemText*)pRecord->GetItem(1);
				CString strOccupance = pItemTxt->GetCaption(nullptr);
				dOccupance = _ttof(strOccupance);
			}
			else
			{
				dOccupance = 0.0;
			}

			pItemTxt = (CXTPReportRecordItemText*)pRecord->GetItem(2);
			CString strPCE = pItemTxt->GetCaption(nullptr);
			double dPCE = _ttof(strPCE);
			
			TInterModeDefine oInfo;
			oInfo.nModeID     = nModeID;
			oInfo.strModeName = strModeName;
			oInfo.dOccupancy  = dOccupance;
			oInfo.dPCE        = dPCE;
			m_vecModeDefineUrban.push_back(oInfo);
		}

		//region report
		int nRegionCount = pRegionRecords->GetCount();
		CString tmpKeyStr = _T("");
		if (KmzSystem::GetLanguage() == KEMKorea) {
			tmpKeyStr = _T("Auto");
		}
		else {
			tmpKeyStr = _T("PassengerCar");
		}
		for (int i = 0; i < nRegionCount; i++)
		{
			pRecord = pRegionRecords->GetAt(i);
			pItemTxt = (CXTPReportRecordItemText*)pRecord->GetItem(0);

			CString strModeName = pItemTxt->GetCaption(nullptr);
			int     nModeID     = pItemTxt->GetItemData();

			double dOccupance(0.0);
			if (strModeName == tmpKeyStr)
			{
				pItemTxt = (CXTPReportRecordItemText*)pRecord->GetItem(1);
				CString strOccupance = pItemTxt->GetCaption(nullptr);
				dOccupance = _ttof(strOccupance);
			}
			else
			{
				dOccupance = 0.0;
			}

			pItemTxt = (CXTPReportRecordItemText*)pRecord->GetItem(2);
			CString strPCE = pItemTxt->GetCaption(nullptr);
			double dPCE = _ttof(strPCE);

			TInterModeDefine oInfo;
			oInfo.nModeID     = nModeID;
			oInfo.strModeName = strModeName;
			oInfo.dOccupancy  = dOccupance;
			oInfo.dPCE        = dPCE;
			m_vecModeDefineRegion.push_back(oInfo);
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
	
	KResizeDialogEx::OnOK();
}


void KInterModeDefineDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}


void KInterModeDefineDlg::GetDefineModeData( double& a_dATime, std::vector<TInterModeDefine>& a_vecUrban,std::vector<TInterModeDefine>& a_vecRegion )
{
	a_dATime    = m_dAnalysisTime;
	a_vecUrban  = m_vecModeDefineUrban;
	a_vecRegion = m_vecModeDefineRegion;
}

void KInterModeDefineDlg::OnBnClickedButtonDef()
{
	try
	{
		KDBaseDefaultInterRunOption::LoadInterUrbanOccupancyPCE(m_pTarget, m_vecModeDefineUrban);
		KDBaseDefaultInterRunOption::LoadInterRegionalOccupancyPCE(m_pTarget, m_vecModeDefineRegion);
		KDBaseDefaultInterRunOption::LoadInterODDuration(m_pTarget, m_dAnalysisTime);

		UpdateREportDataRegion();
		UpdateReportDataUrban();

		CString strValue(_T(""));
		strValue.Format(_T("%.2f"), m_dAnalysisTime);
		SetDlgItemText(IDC_EDIT_ATIME, strValue);

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
