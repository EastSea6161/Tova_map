#include "stdafx.h"
#include "PA2ODOutputDesignDlg.h"
#include "afxdialogex.h"
#include "PA2ODInfo.h"
#include "Purpose.h"
#include "Target.h"

IMPLEMENT_DYNAMIC(KPA2ODOutputDesignDlg, KDialogEx)

KPA2ODOutputDesignDlg::KPA2ODOutputDesignDlg(KTarget* a_pTarget,
	std::list<KPA2ODInfo*> a_lstOutput, CWnd* pParent /*=NULL*/)
	: KDialogEx(KPA2ODOutputDesignDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_lstOutput(a_lstOutput)
{

}

KPA2ODOutputDesignDlg::~KPA2ODOutputDesignDlg()
{
}

void KPA2ODOutputDesignDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndOutputReport);
}


BEGIN_MESSAGE_MAP(KPA2ODOutputDesignDlg, KDialogEx)
END_MESSAGE_MAP()


BOOL KPA2ODOutputDesignDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndOutputReport, FALSE, FALSE, FALSE);

	InitReportCtrl();
	UpdateReportCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KPA2ODOutputDesignDlg::InitReportCtrl( void )
{
	/*Add Header Column*/
	CXTPReportColumn* pReportColumn = NULL;

	pReportColumn = m_wndOutputReport.AddColumn(
		new CXTPReportColumn(0, _T("Purpose"), 1));
	pReportColumn->SetTreeColumn(TRUE);
	pReportColumn->SetHeaderAlignment(DT_CENTER);

	pReportColumn = m_wndOutputReport.AddColumn(
		new CXTPReportColumn(1, _T("Created Date"), 1));
	pReportColumn->SetHeaderAlignment(DT_CENTER);

	pReportColumn = m_wndOutputReport.AddColumn(
		new CXTPReportColumn(2, _T("Modified Date"), 1));
	pReportColumn->SetHeaderAlignment(DT_CENTER);

	pReportColumn = m_wndOutputReport.AddColumn(
		new CXTPReportColumn(3, _T("Remarks"), 1));
	pReportColumn->SetHeaderAlignment(DT_CENTER);
}

void KPA2ODOutputDesignDlg::UpdateReportCtrl( void )
{
	CXTPReportRecord*		pCXTPRecord = NULL;
	CXTPReportRecordItem*	pCXTPItem	= NULL;

	std::list<KPA2ODInfo*>::iterator iter, end = m_lstOutput.end();

	for (iter = m_lstOutput.begin(); iter != end; ++iter)
	{
		KPA2ODInfo* pPA2ODInfo = *iter;
		KPurpose*   pPurpose   = pPA2ODInfo->Purpose();
		if (!pPA2ODInfo->Selected())
			continue;

		pCXTPRecord = m_wndOutputReport.AddRecord(new CXTPReportRecord);

		pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText(_T("")));
		pCXTPItem->SetBackgroundColor(RGB(191,239,254));
		pCXTPItem->SetCaption(pPurpose->PurposeName());
		pCXTPItem->SetItemData((DWORD_PTR)pPA2ODInfo);
		pCXTPItem->SetBold(TRUE);
		
		pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText(_T("")));
		pCXTPItem->SetBackgroundColor(RGB(191,239,254));

		pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText(_T("")));
		pCXTPItem->SetBackgroundColor(RGB(191,239,254));

		pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText(_T("")));
		pCXTPItem->SetBackgroundColor(RGB(191,239,254));

		int nOverwriteCount(0);

		AddOverwriteOutputColumns(pCXTPRecord, pPA2ODInfo, nOverwriteCount);
	}


	m_wndOutputReport.Populate();
	m_wndOutputReport.ExpandAll(TRUE);
}

void KPA2ODOutputDesignDlg::AddOverwriteOutputColumns( CXTPReportRecord* a_pParent, KPA2ODInfo* a_pPA2ODInfo, int& a_nOverwriteCount )
{
	KIOTables*			pIOTables	= m_pTarget->Tables();
	KIOTable*					pTable			= pIOTables->FindTable(TABLE_PURPOSE_OD);
	const KIOColumns*	pColumns		= pTable->Columns();
	KPurpose*                   pPurpose        = a_pPA2ODInfo->Purpose();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	CString strQuery;
	strQuery.Format(
		_T(" SELECT purpose_od_column_name, start_date, end_date")
		_T(" FROM distribution_model")
		_T(" WHERE object_group_code = %d")
		_T(" AND detail_object_id = %d")
		_T(" ORDER BY end_date DESC" ),
		pPurpose->PurposeGroup(),
		pPurpose->PurposeID()
		);
	try
	{
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			CXTPReportRecord*		pCXTPRecord = a_pParent->GetChilds()->Add(new CXTPReportRecord);
			CXTPReportRecordItem*	pCXTPItem	= NULL;

			/*ColumnName*/
			pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText);
			KIOColumn* pColumn = pColumns->GetColumn(spResultSet->GetValueString(0));

			if (NULL != pColumn)
			{
				pCXTPItem->SetCaption(pColumn->Caption());
				pCXTPItem->SetItemData((DWORD_PTR)pColumn);
				pCXTPItem->HasCheckbox(TRUE);

// 				if (pColumn == a_pSCInfo->OutputColumn())
// 				{
// 					pCXTPItem->SetChecked(TRUE);
// 				}
				pCXTPItem->SetChecked(TRUE);
			}

			/*Start Date*/
			pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText);
			pCXTPItem->SetCaption(spResultSet->GetValueString(1));

			/*End Date*/
			pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText);
			pCXTPItem->SetCaption(spResultSet->GetValueString(2));

			/*Remarks*/
			pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText);
			pCXTPItem->SetCaption(_T("Overwrite"));
            if (KmzSystem::GetLanguage() == KEMKorea) {
                pCXTPItem->SetCaption(_T("덮어쓰기"));
            }
			++a_nOverwriteCount;
		}
	}
	catch(KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}
