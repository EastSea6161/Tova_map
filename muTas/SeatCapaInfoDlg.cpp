// SeatCapaInfoDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SeatCapaInfoDlg.h"
#include "afxdialogex.h"
#include "Target.h"

// KSeatCapaInfoDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KSeatCapaInfoDlg, KResizeDialogEx)

KSeatCapaInfoDlg::KSeatCapaInfoDlg(KTarget* a_pTarget, int a_nType, std::vector<Integer> a_vecLineID, double a_dCapaRatio, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KSeatCapaInfoDlg::IDD, pParent),
	m_pTarget(a_pTarget),
	m_nSeatCapaType(a_nType),
	m_vecTransitLine(a_vecLineID),
	m_dCapaRatio(a_dCapaRatio)
{

}

KSeatCapaInfoDlg::~KSeatCapaInfoDlg()
{
}

void KSeatCapaInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
}


BEGIN_MESSAGE_MAP(KSeatCapaInfoDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDCANCEL, &KSeatCapaInfoDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KSeatCapaInfoDlg 메시지 처리기입니다.


BOOL KSeatCapaInfoDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndReport, TRUE);

	IniteReportHeader();
	UpdateReportData();

	ResizeComponet();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KSeatCapaInfoDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

void KSeatCapaInfoDlg::IniteReportHeader()
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(0, _T("Trasit ID"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("노선 ID"));
    }

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(1, _T("Name"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("노선 명칭"));
    }

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(2, _T("Value"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("좌석수/용량"));
    }
}

void KSeatCapaInfoDlg::UpdateReportData()
{
	m_wndReport.ResetContent(TRUE);

	try
	{
		for (size_t i = 0; i < m_vecTransitLine.size(); i++)
		{
			Integer nxTransitID = m_vecTransitLine[i];

			CString strTransitID(_T(""));
			strTransitID.Format(_T("%I64d"), nxTransitID);

			CXTPReportRecord*		pRecord = m_wndReport.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem*   pItem	= nullptr;

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strTransitID));
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

			CString strTransitName(_T(""));
			GetTransitName(nxTransitID, strTransitName);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strTransitName));
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

			double dValue(0);
			if (m_nSeatCapaType == 0)
			{
				GetSeatValue(nxTransitID, dValue);
			}
			else
			{
				GetCapacityValue(nxTransitID, dValue);
			}

			CString strValue(_T(""));
			strValue.Format(_T("%.2f"), dValue);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strValue));
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		}
		m_wndReport.Populate();

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

void KSeatCapaInfoDlg::GetSeatValue(Integer a_nxLineID, double& a_dSetValue )
{
	CString strSQL(_T(""));
	try
	{
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
		strSQL.Format(_T("Select seat_count, total_frequency From %s Where transit_ID = '%I64d'"),TABLE_TRANSIT, a_nxLineID);
		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);
		if (spResultset->Next())
		{
			double dSeat = spResultset->GetValueDouble(0);
			double dFreq = spResultset->GetValueDouble(1);

			a_dSetValue = dSeat * dFreq;
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

void KSeatCapaInfoDlg::GetCapacityValue(Integer a_nxLineID,  double& a_dCapaValue )
{
	CString strSQL(_T(""));
	try
	{
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
		strSQL.Format(_T("Select capacity, total_frequency From %s Where transit_ID = '%I64d'"),TABLE_TRANSIT, a_nxLineID);
		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);
		if (spResultset->Next())
		{
			double dSeat = spResultset->GetValueDouble(0);
			double dFreq = spResultset->GetValueDouble(1);

			a_dCapaValue = dSeat * dFreq * (m_dCapaRatio / 100);
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

void KSeatCapaInfoDlg::GetTransitName( Integer a_nxLineID, CString& a_strTransitName )
{
	CString strSQL(_T(""));
	try
	{
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
		strSQL.Format(_T("Select name From %s Where transit_ID = '%I64d'"),TABLE_TRANSIT, a_nxLineID);
		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);
		if (spResultset->Next())
		{
			a_strTransitName = spResultset->GetValueString(0);
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

void KSeatCapaInfoDlg::ResizeComponet()
{
	try
	{
		SetResize(IDC_STATIC1,			SZ_TOP_LEFT,      SZ_TOP_LEFT);
		SetResize(IDC_REPORT,			SZ_TOP_LEFT,      SZ_BOTTOM_RIGHT);
		SetResize(IDC_STATIC2,	     	SZ_BOTTOM_LEFT,   SZ_BOTTOM_RIGHT);
		SetResize(IDCANCEL,	     		SZ_BOTTOM_RIGHT,   SZ_BOTTOM_RIGHT);
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
