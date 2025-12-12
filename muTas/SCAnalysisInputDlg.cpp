// SCAnalysisInputDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SCAnalysisInputDlg.h"
#include "afxdialogex.h"

#include "Target.h"

#include "SCAnalysisInfo.h"
#include "Mode.h"

#include "CustomReportRecordItem.h"

// KSCAnalysisInputDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KSCAnalysisInputDlg, KDialogEx)

KSCAnalysisInputDlg::KSCAnalysisInputDlg(KTarget* a_pTarget, std::list<KSCAnalysisInfo*> a_lstSCAnlaysisInfo, CWnd* pParent /*=NULL*/)
	: KDialogEx(KSCAnalysisInputDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_lstSCAnlaysisInfo(a_lstSCAnlaysisInfo)
{
}

KSCAnalysisInputDlg::~KSCAnalysisInputDlg()
{
}

void KSCAnalysisInputDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM1, m_wndModeReport);
	DDX_Control(pDX, IDC_CUSTOM2, m_wndInputReport);
}


BEGIN_MESSAGE_MAP(KSCAnalysisInputDlg, KDialogEx)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_CUSTOM1, OnModeCheckItem)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_CUSTOM2, OnInputCheckItem)
	ON_BN_CLICKED(IDOK, &KSCAnalysisInputDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// KSCAnalysisInputDlg 메시지 처리기입니다.


BOOL KSCAnalysisInputDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	InitColumnMap();

	KReportCtrlSetting::Default(m_wndModeReport, TRUE, FALSE);
	KReportCtrlSetting::Default(m_wndInputReport, FALSE, FALSE, FALSE);

	InitDemandReportControl();
	UpdateDemandReportRecord();

	InitInputReportControl();
	UpdateInputReportRecord();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KSCAnalysisInputDlg::InitColumnMap()
{
	m_ColumnMap.clear();
	std::list<KSCAnalysisInfo*>::iterator itInput, itEnd = m_lstSCAnlaysisInfo.end();

	for (itInput = m_lstSCAnlaysisInfo.begin(); itInput != itEnd; ++itInput)
	{
		KSCAnalysisInfo*	pSCAnlaysisInfo	= *itInput;
		KIOColumn*			pColumn			= pSCAnlaysisInfo->InputColumn();

		if (NULL != pColumn)
			m_ColumnMap.insert(std::make_pair(pSCAnlaysisInfo, pColumn));
	}
}


void KSCAnalysisInputDlg::InitDemandReportControl( void )
{
	int nColumnItemIndex = 0;
	CXTPReportColumn* pColumn = NULL;

	pColumn = new CXTPReportColumn(nColumnItemIndex++, _T("MODE"), 5);
	pColumn = m_wndModeReport.AddColumn(pColumn);
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("수단명"));
    }

	pColumn = new CXTPReportColumn(nColumnItemIndex++, _T("Occupancy"), 5);
	pColumn = m_wndModeReport.AddColumn(pColumn);
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("재차인원"));
    }

	pColumn = new CXTPReportColumn(nColumnItemIndex++, _T("PCU(O/D)"), 5);
	pColumn = m_wndModeReport.AddColumn(pColumn);
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("O/D 교통량 PCE"));
    }
	
	pColumn = new CXTPReportColumn(nColumnItemIndex++, _T("Survey Volume"), 8);
	pColumn = m_wndModeReport.AddColumn(pColumn);
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("조사 링크교통량 컬럼"));
    }

	pColumn->GetEditOptions()->AddComboButton(TRUE);
	pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
	pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;
    
	/*Add Combo Item*/
	CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();
	CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();
	
	KIOTables* pIOTables		= m_pTarget->Tables();
	KIOTable* pTable			= pIOTables->FindTable(TABLE_LINK);
	const KIOColumns* pColumns	= pTable->Columns();
	int nColumnCount					= pColumns->ColumnCount();

	for (int i = 0; i < nColumnCount; ++i)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		
		if(KEMIODataTypeDouble == pColumn->DataType())
			pEditOptions->AddConstraint(pColumn->Caption(), (DWORD_PTR)pColumn);
	}
    
	pColumn = new CXTPReportColumn(nColumnItemIndex++, _T("PCU(Volume)"), 5);    
	pColumn = m_wndModeReport.AddColumn(pColumn);
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("조사 링크교통량 PCE"));
    }
}

void KSCAnalysisInputDlg::UpdateDemandReportRecord( void )
{
	std::list<KSCAnalysisInfo*>::iterator itInput, itEnd = m_lstSCAnlaysisInfo.end();

	for (itInput = m_lstSCAnlaysisInfo.begin(); itInput != itEnd; ++itInput)
	{
		KSCAnalysisInfo* pSCAnlaysisInfo	= *itInput;
		KMode* pMode						= pSCAnlaysisInfo->Mode();
		CXTPReportRecord* pRecord			= m_wndModeReport.AddRecord(new CXTPReportRecord);
		
		/*Mode*/
		CXTPReportRecordItem* pItem	= pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetCaption(pMode->ModeName());
		pItem->HasCheckbox(TRUE);

		if(true == pSCAnlaysisInfo->Selected())
			pItem->SetChecked(TRUE);
		pItem->SetItemData((DWORD_PTR)pSCAnlaysisInfo);

		/*Occupancy*/
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pSCAnlaysisInfo->Occupancy(), _T("%.3f")));
		
		/*OD_PCU*/
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pSCAnlaysisInfo->ODPCU(), _T("%.3f")));

		/*Survey Volume*/
		pItem = pRecord->AddItem(new CCustomReportRecordItem);
		pItem->SetItemData((DWORD_PTR)pSCAnlaysisInfo->SurveyColumn());

		/*Volume PCU*/
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pSCAnlaysisInfo->VolumePCU(), _T("%.3f")));
	}
	m_wndModeReport.Populate();
}

void KSCAnalysisInputDlg::InitInputReportControl( void )
{
	int nColumnItemIndex = 0;
	CXTPReportColumn* pColumn = NULL;

	pColumn = new CXTPReportColumn(nColumnItemIndex++, _T("Mode"), 1);
	pColumn = m_wndInputReport.AddColumn(pColumn);
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetTreeColumn(TRUE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("컬럼 이름"));
    }

	pColumn = new CXTPReportColumn(nColumnItemIndex++, _T("Created Date"), 1);
	pColumn = m_wndInputReport.AddColumn(pColumn);
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("만든 날짜"));
    }

	pColumn = new CXTPReportColumn(nColumnItemIndex++, _T("Modified Date"), 1);
	pColumn = m_wndInputReport.AddColumn(pColumn);
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("수정한 날짜"));
    }
}

void KSCAnalysisInputDlg::UpdateInputReportRecord( void )
{
	m_wndInputReport.ResetContent();
	CXTPReportRecords* pModeRecords = m_wndModeReport.GetRecords();
	int nRecordCount = pModeRecords->GetCount();

	for (int i = 0; i  < nRecordCount; ++i)
	{
		CXTPReportRecord*		pModeRecord = pModeRecords->GetAt(i);
		CXTPReportRecordItem*	pModeItem	= pModeRecord->GetItem(0);

		if (pModeItem->IsChecked() == FALSE)
			continue;
		
		KSCAnalysisInfo*		pInputInfo	= (KSCAnalysisInfo*)pModeItem->GetItemData();
		KMode*					pMode		= pInputInfo->Mode();
		CXTPReportRecord*		pRecord		= m_wndInputReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem*	pItem		= pRecord->AddItem(new CXTPReportRecordItem);
											  pRecord->AddItem(new CXTPReportRecordItem);
											  pRecord->AddItem(new CXTPReportRecordItem);
		pItem->SetCaption(pMode->ModeName());
		pItem->SetItemData((DWORD_PTR)pInputInfo);

		UpdateInputReportColumnRecord(pRecord, pInputInfo);
	}
	m_wndInputReport.Populate();
	m_wndInputReport.ExpandAll(TRUE);
}


void KSCAnalysisInputDlg::UpdateInputReportColumnRecord( CXTPReportRecord* a_pParent, KSCAnalysisInfo* pInputInfo )
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	bool bCheck = false;
	CString strSQL = _T("");

	KMode* pMode = pInputInfo->Mode();

	KIOTables* pIOTables = m_pTarget->Tables();
	ASSERT(NULL != pIOTables);

	KIOTable* pTable = pIOTables->FindTable(TABLE_MODE_OD);
	ASSERT(NULL != pTable);

	const KIOColumns* pColumns = pTable->Columns();
	KIOColumn* pInputColumn = NULL;
	std::map<KSCAnalysisInfo*, KIOColumn*>::iterator itInput = m_ColumnMap.find(pInputInfo);
	if(m_ColumnMap.end() != itInput)
	{
		pInputColumn = itInput->second;
	}

	strSQL.Format(
		_T(" SELECT mode_od_column_name, start_date, end_date ")
		_T(" FROM modechoice_model ")
		_T(" WHERE mode_group_code = %d ")
		_T(" AND detail_mode_id = %d ")
		_T(" ORDER BY end_date DESC "),
		pMode->ModeGroup(),
		pMode->ModeID()
		);

	try
	{
		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		CXTPReportRecords* pRecords = a_pParent->GetChilds();
		while(spResult->Next())
		{
			CXTPReportRecord* pRecord = pRecords->Add(new CXTPReportRecord);
			CXTPReportRecordItem* pItem = NULL;

			CString strColumn = spResult->GetValueString(0);
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			KIOColumn* pColumn = pColumns->GetColumn(strColumn);
			if(NULL != pColumn)
			{
				pItem->SetCaption(pColumn->Caption());
				pItem->SetItemData((DWORD_PTR)pColumn);
			}
			else
			{
				pItem->SetCaption(strColumn);
			}

			pItem->HasCheckbox(TRUE);

			if((NULL != pInputColumn) && (pInputColumn == pColumn))
			{
				pItem->SetChecked(TRUE);
				bCheck = true;
			}

			CString strStartDate = spResult->GetValueString(1);
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			pItem->SetCaption(strStartDate);

			CString strEndDate = spResult->GetValueString(2);
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			pItem->SetCaption(strEndDate);
		}
	}
	catch(...)
	{

	}

	if((false == bCheck) && (a_pParent->GetChilds()->GetCount() > 0))
	{
		CXTPReportRecord* pRecord = a_pParent->GetChilds()->GetAt(0);
		CXTPReportRecordItem* pItem = pRecord->GetItem(0);
		pItem->SetChecked(TRUE);
		KIOColumn* pColumn = (KIOColumn*)(pItem->GetItemData());
		m_ColumnMap[pInputInfo] = pColumn;
	}
}


void KSCAnalysisInputDlg::OnBnClickedOk()
{

	try
	{
		ApplyInputInfo();
		ApplyInputColumn();
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		TxLogDebugException();
		return;
	}
	KDialogEx::OnOK();
}

void KSCAnalysisInputDlg::ApplyInputInfo( void )
{
	CXTPReportRecords* pRecords 		  = m_wndModeReport.GetRecords();
	int 			   nRecordCount 	  = pRecords->GetCount();
	int 			   nModeSelectedCount = 0;

	for(int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord*     pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem = pRecord->GetItem(0);
		KSCAnalysisInfo* pInputInfo = (KSCAnalysisInfo*)(pItem->GetItemData());

		//Output Design 초기화.
		pInputInfo->OutputColumn(NULL);
		pInputInfo->OutputColumnName(_T(""));

		if(pItem->IsChecked() == FALSE)
		{
			pInputInfo->Selected(false);
			pInputInfo->InputColumn(NULL);
			continue;
		}
		nModeSelectedCount++;
		pInputInfo->Selected(true);

		CXTPReportRecordItemNumber* pNumber = (CXTPReportRecordItemNumber*)(pRecord->GetItem(1));
		pInputInfo->Occupancy(pNumber->GetValue());

		pNumber = (CXTPReportRecordItemNumber*)(pRecord->GetItem(2));
		pInputInfo->ODPCU(pNumber->GetValue());

		pItem = pRecord->GetItem(3);
		pInputInfo->SurveyColumn((KIOColumn*)pItem->GetItemData());

		pNumber = (CXTPReportRecordItemNumber*)(pRecord->GetItem(4));
		pInputInfo->VolumePCU(pNumber->GetValue());
	}

	if (0 == nModeSelectedCount)
		ThrowException(_T("수단을 선택하세요."));
}

void KSCAnalysisInputDlg::ApplyInputColumn( void )
{
	CXTPReportRecords*	pModeRecords		= m_wndInputReport.GetRecords();
	int					nModeRecordCount	= pModeRecords->GetCount();

	for(int i = 0; i < nModeRecordCount; ++i)
	{
		CXTPReportRecord*		pModeRecord = pModeRecords->GetAt(i);
		CXTPReportRecordItem*	pModeItem   = pModeRecord->GetItem(0);
		KSCAnalysisInfo*		pInputInfo	= (KSCAnalysisInfo*)(pModeItem->GetItemData());
		CXTPReportRecords*		pColumnRecords = pModeRecord->GetChilds();
		int						nColumnCount = pColumnRecords->GetCount();

		if (nColumnCount == 0)
			pInputInfo->Selected(false);
	
		for(int j = 0; j < nColumnCount; ++j)
		{
			CXTPReportRecord* pColumnRecord = pColumnRecords->GetAt(j);
			CXTPReportRecordItem* pColumnItem = pColumnRecord->GetItem(0);
			if(pColumnItem->IsChecked() == TRUE)
			{
				KIOColumn* pColumn = (KIOColumn*)(pColumnItem->GetItemData());
				pInputInfo->InputColumn(pColumn);
				break;
			}
		}
	}
}


void KSCAnalysisInputDlg::OnModeCheckItem( NMHDR* pNotifyStruct, LRESULT* result )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;

	CXTPReportRecord* pRecord	= pItemNotify->pItem->GetRecord();
	CXTPReportRecordItem* pItem = pRecord->GetItem(0);
	KSCAnalysisInfo* pInputInfo	= (KSCAnalysisInfo*)pItem->GetItemData();

	if (pItemNotify->pItem->IsChecked() == FALSE)
	{
		std::map<KSCAnalysisInfo*, KIOColumn*>::iterator itColumn = m_ColumnMap.find(pInputInfo);

		if(m_ColumnMap.end() != itColumn)
			m_ColumnMap.erase(itColumn);
	}
	UpdateInputReportRecord();
}


void KSCAnalysisInputDlg::OnInputCheckItem( NMHDR* pNotifyStruct, LRESULT* result )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	pItemNotify->pItem->SetChecked(TRUE);

	CXTPReportRecord*		pCurrentRecord	= pItemNotify->pItem->GetRecord();
	CXTPReportRecord*		pModeRecord		= pCurrentRecord->GetParentRecord();
	CXTPReportRecordItem*	pModeItem		= pModeRecord->GetItem(0);
	KSCAnalysisInfo*			pInputInfo		= (KSCAnalysisInfo*)pModeItem->GetItemData();

	CXTPReportRecordItem*	pCurrentItem	= pCurrentRecord->GetItem(0);
	KIOColumn*				pCurrentInput	= (KIOColumn*)pCurrentItem->GetItemData();
	m_ColumnMap[pInputInfo] = pCurrentInput;

	CXTPReportRecords* pInputRecords = pModeRecord->GetChilds();
	int nInputCount = pInputRecords->GetCount();

	for(int i = 0; i < nInputCount; ++i)
	{
		CXTPReportRecord* pInputRecord = pInputRecords->GetAt(i);

		if(pInputRecord != pCurrentRecord)
		{
			CXTPReportRecordItem* pInputItem = pInputRecord->GetItem(0);
			pInputItem->SetChecked(FALSE);
		}
	}
}