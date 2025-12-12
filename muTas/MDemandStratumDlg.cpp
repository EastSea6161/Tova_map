/**
 * @file MDemandStratumDlg.cpp
 * @brief KMDemandStratumDlg 구현 파일
 * @author 
 * @date 2011.06.02
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "MDemandStratumDlg.h"
#include "afxdialogex.h"
//^^ #include "IOColumn.h"
//^^ #include "IOColumns.h"
//^^ #include "IOTable.h"
//^^ #include "IOTables.h"
#include "Purpose.h"
#include "Target.h"



/* define column index */
#define MDEMAND_PURPOSE		0
#define MDEMAND_SELECTION	1

#define MINPUT_NAME			0
#define MINPUT_STARTDATE	1
#define MINPUT_ENDDATE		2


// KMDemandStratumDlg 대화 상자입니다.
IMPLEMENT_DYNAMIC(KMDemandStratumDlg, KDialogEx)

KMDemandStratumDlg::KMDemandStratumDlg(CWnd* pParent /*=NULL*/)
	:	KDialogEx(KMDemandStratumDlg::IDD, pParent),
		m_pTarget(NULL)
{
	
}

KMDemandStratumDlg::~KMDemandStratumDlg()
{
}

void KMDemandStratumDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
	DDX_Control(pDX, IDC_REPORT3, m_wndInput);
}


BEGIN_MESSAGE_MAP(KMDemandStratumDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KMDemandStratumDlg::OnBnClickedOk)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT, OnReportCheckItem)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT3, OnInputCheckItem)
END_MESSAGE_MAP()



void KMDemandStratumDlg::SetTarget(KTarget* pTarget)
{
	m_pTarget = pTarget;
	//initPurposeList();
}


void KMDemandStratumDlg::PurposeSet(std::set<KPurpose*> purposeSet)
{
	m_PurposeSet = purposeSet;
}


std::set<KPurpose*, TPurposeComparator>& KMDemandStratumDlg::SelectedPurpose(void)
{
	return m_SelectedPurposeSet;
}


void KMDemandStratumDlg::SelectedPurpose(std::set<KPurpose*, TPurposeComparator>& purposeSet)
{
	m_SelectedPurposeSet = purposeSet;
}


std::map<KPurpose*, KIOColumn*, TPurposeComparator>& KMDemandStratumDlg::InputMap(void)
{
	return m_InputMap;
}


void KMDemandStratumDlg::InputMap(std::map<KPurpose*, KIOColumn*, TPurposeComparator>& inputMap)
{
	m_InputMap = inputMap;
}


void KMDemandStratumDlg::initReportControl(void)
{
	// Add Columns
	CXTPReportColumn* pColumn = NULL;
	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(MDEMAND_PURPOSE, _T("Demand"), 150));
	pColumn->SetHeaderAlignment(DT_CENTER);

    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("통행목적"));
    }

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(MDEMAND_SELECTION, _T("Selection"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }
}


// void KMDemandStratumDlg::initPurposeList(void)
// {
// 	ASSERT(NULL != m_pTarget);
// 
// 	m_PurposeMap.clear();
// 
// 	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
// 	CString strSQL;
// 	strSQL = 
// 		_T(" SELECT detail_object_id, object_name ")
// 		_T(" FROM detail_object");
// 
// 	try
// 	{
// 		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
// 		while(spResult->Next())
// 		{
// 			int nID = spResult->GetValueInt(0);
// 			CString strName = (TCHAR*)(spResult->GetValueString(1));
// 
// 			m_PurposeMap.insert(std::make_pair(nID, strName));
// 		}
// 	}
// 	catch (...)
// 	{
// 		TxLogDebugException();
// 	}
// }


void KMDemandStratumDlg::updatePurposeRecord(void)
{
	//std::map<int, CString>::iterator itPurpose, itEnd = m_PurposeMap.end();
	//for(itPurpose = m_PurposeMap.begin(); itPurpose != itEnd; ++itPurpose)
	//{
	//	CXTPReportRecord* pRecord = m_wndReport.AddRecord(new CXTPReportRecord);
	//	CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText);
	//	pItem->SetCaption(itPurpose->second);
	//	pItem->SetItemData(itPurpose->first);
	//	
	//	pItem = pRecord->AddItem(new CXTPReportRecordItem);
	//	pItem->HasCheckbox(TRUE);
	//	if(m_SelectedPurposeMap.find(itPurpose->first) != m_SelectedPurposeMap.end())
	//	{
	//		pItem->SetChecked(TRUE);
	//	}
	//	else
	//	{
	//		pItem->SetChecked(FALSE);
	//	}
	//	
	//}
	//m_wndReport.Populate();

	std::list<KPurpose*>::iterator itPurpose, itEnd = m_lstPurpose.end();
	for(itPurpose = m_lstPurpose.begin(); itPurpose != itEnd; ++itPurpose)
	{
		KPurpose* pPurpose = *itPurpose;
		CXTPReportRecord* pRecord = m_wndReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetCaption(pPurpose->PurposeName());
		pItem->SetItemData((DWORD_PTR)pPurpose);
		pItem->SetAlignment(DT_CENTER);

		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);
		if(m_SelectedPurposeSet.find(pPurpose) != m_SelectedPurposeSet.end())
		{
			pItem->SetChecked(TRUE);
		}
		else
		{
			pItem->SetChecked(FALSE);
		}
	}
	m_wndReport.Populate();
}


void KMDemandStratumDlg::initInputReportControl(void)
{
	// Add Columns
	CXTPReportColumn* pColumn = NULL;
	pColumn = m_wndInput.AddColumn(new CXTPReportColumn(MINPUT_NAME, _T("Purpose"), 150));
	pColumn->SetTreeColumn(TRUE);
	pColumn->SetHeaderAlignment(DT_CENTER);

    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("O/D 데이터 컬럼"));
    }

	pColumn = m_wndInput.AddColumn(new CXTPReportColumn(MINPUT_STARTDATE, _T("Created Date"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("만든 날짜"));
    }

	pColumn = m_wndInput.AddColumn(new CXTPReportColumn(MINPUT_ENDDATE, _T("Modified Date"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("수정한 날짜"));
    }
}


void KMDemandStratumDlg::updateInputRecord(void)
{
	m_wndInput.GetRecords()->RemoveAll();
	std::set<KPurpose*, TPurposeComparator>::iterator itPurpose, itEnd = m_SelectedPurposeSet.end();
	for(itPurpose = m_SelectedPurposeSet.begin(); itPurpose != itEnd; ++itPurpose)
	{
		KPurpose* pPurpose = *itPurpose;
		CXTPReportRecord* pRecord = m_wndInput.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		pRecord->AddItem(new CXTPReportRecordItemText);
		pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetCaption(pPurpose->PurposeName());
		pItem->SetItemData((DWORD_PTR)pPurpose);

		updateInputPurposeRecord(pRecord, pPurpose);
	}

	m_wndInput.Populate();
	m_wndInput.ExpandAll(TRUE);
}


void KMDemandStratumDlg::updateInputPurposeRecord(CXTPReportRecord* pParent, KPurpose* pPurpose)
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	bool    bCheck = false;
	CString strSQL = _T("");

	KIOTables* pIOTables = m_pTarget->Tables();
	ASSERT(NULL != pIOTables);
	KIOTable* pPurposeODTable = pIOTables->FindTable(_T("purpose_od"));
	ASSERT(NULL != pPurposeODTable);
	const KIOColumns* pColumns = pPurposeODTable->Columns();
	KIOColumn* pInputColumn = NULL;
	std::map<KPurpose*, KIOColumn*, TPurposeComparator>::iterator itInput = m_InputMap.find(pPurpose);
	if(m_InputMap.end() != itInput)
	{
		pInputColumn = itInput->second;
	}

	strSQL.Format(
		_T(" SELECT purpose_od_column_name, start_date, end_date ")
		_T(" FROM distribution_model ")
		_T(" WHERE object_group_code = %d ")
		_T(" AND detail_object_id = %d ")
		_T(" ORDER BY end_date DESC "),
		pPurpose->PurposeGroup(),
		pPurpose->PurposeID()
		);

	try
	{
		KResultSetPtr      spResult = spDBaseConnection->ExecuteQuery(strSQL);

		CXTPReportRecords* pRecords = pParent->GetChilds();
		
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

			CString strEndDate   = spResult->GetValueString(2);
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			pItem->SetCaption(strEndDate);
		}
	}
	catch(...)
	{

	}

	if((false == bCheck) && (pParent->GetChilds()->GetCount() > 0))
	{
		CXTPReportRecord* pRecord = pParent->GetChilds()->GetAt(0);
		CXTPReportRecordItem* pItem = pRecord->GetItem(0);
		pItem->SetChecked(TRUE);
		KIOColumn* pColumn = (KIOColumn*)(pItem->GetItemData());
		m_InputMap[pPurpose] = pColumn;
	}
}


bool KMDemandStratumDlg::applySelectedPurpose(void)
{
	// update selected purpose
	m_SelectedPurposeSet.clear();
	CXTPReportRecords* pRecords = m_wndReport.GetRecords();
	int nRecordCount = pRecords->GetCount();
	int nSelectedCount = 0;
	
	for(int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord* pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem = pRecord->GetItem(MDEMAND_SELECTION);
		if(pItem->IsChecked() == TRUE)
		{
			pItem = pRecord->GetItem(MDEMAND_PURPOSE);
			KPurpose* pPurpose = (KPurpose*)(pItem->GetItemData());
			m_SelectedPurposeSet.insert(pPurpose);
			nSelectedCount++;
		}
	}

	if(nSelectedCount < 1)
	{
		AfxMessageBox(_T("선택한 데이터 정보가 없습니다."));
		return false;
	}

	return true;
}


bool KMDemandStratumDlg::applyInputColumn(void)
{
	// update input
	m_InputMap.clear();
	CXTPReportRecords* pPurposeRecords = m_wndInput.GetRecords();
	int nPurposeCount = pPurposeRecords->GetCount();
	for(int i = 0; i < nPurposeCount; ++i)
	{
		CXTPReportRecord* pPurposeRecord = pPurposeRecords->GetAt(i);
		CXTPReportRecordItem* pPurposeItem = pPurposeRecord->GetItem(MINPUT_NAME);
		KPurpose* pPurpose = (KPurpose*)(pPurposeItem->GetItemData());

		CXTPReportRecords* pInputRecords = pPurposeRecord->GetChilds();
		int nInputCount = pInputRecords->GetCount();

		if( nInputCount< 1 )
		{
			CString strInputDataChkMsg = _T("");
			strInputDataChkMsg.Format(_T("%s에 대한 입력 데이터 관련 정보가 존재하지 않습니다."), pPurpose->PurposeName());
			AfxMessageBox(strInputDataChkMsg);
			return false;
		}

		for(int j = 0; j < nInputCount; ++j)
		{
			CXTPReportRecord* pInputRecord = pInputRecords->GetAt(j);
			CXTPReportRecordItem* pInputItem = pInputRecord->GetItem(MINPUT_NAME);
			if(pInputItem->IsChecked() == TRUE)
			{
				KIOColumn* pColumn = (KIOColumn*)(pInputItem->GetItemData());
				m_InputMap.insert(std::make_pair(pPurpose, pColumn));
				break;
			}
		}
	}

	return true;
}


// KMDemandStratumDlg 메시지 처리기입니다.
BOOL KMDemandStratumDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndReport, FALSE, FALSE, TRUE);
	KReportCtrlSetting::Default(m_wndInput, FALSE, FALSE, TRUE);
	m_wndReport.Populate();
	m_wndInput.Populate();

	initReportControl();
	updatePurposeRecord();

	initInputReportControl();
	updateInputRecord();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KMDemandStratumDlg::OnBnClickedOk()
{
	if( false == applySelectedPurpose() )
	{
		return;
	}
	if( false == applyInputColumn() )
	{
		return;
	}

	KDialogEx::OnOK();
}


void KMDemandStratumDlg::OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* result)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;

	CXTPReportRecord* pRecord = pItemNotify->pItem->GetRecord();
	CXTPReportRecordItem* pItem = pRecord->GetItem(MDEMAND_PURPOSE);
	KPurpose* pPurpose = (KPurpose*)(pItem->GetItemData());
	if(pItemNotify->pItem->IsChecked() == TRUE)
	{
		m_SelectedPurposeSet.insert(pPurpose);
	}
	else
	{
		std::set<KPurpose*, TPurposeComparator>::iterator itPurpose = m_SelectedPurposeSet.find(pPurpose);
		if(m_SelectedPurposeSet.end() != itPurpose)
		{
			m_SelectedPurposeSet.erase(itPurpose);
		}

		std::map<KPurpose*, KIOColumn*, TPurposeComparator>::iterator itInput = m_InputMap.find(pPurpose);
		if(m_InputMap.end() != itInput)
		{
			m_InputMap.erase(itInput);
		}
	}

	updateInputRecord();
}


void KMDemandStratumDlg::OnInputCheckItem(NMHDR* pNotifyStruct, LRESULT* result)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;

	pItemNotify->pItem->SetChecked(TRUE);

	CXTPReportRecord* pCurrentRecord = pItemNotify->pItem->GetRecord();
	
	CXTPReportRecord* pPurposeRecord = pCurrentRecord->GetParentRecord();
	CXTPReportRecordItem* pPurposeItem = pPurposeRecord->GetItem(MINPUT_NAME);
	KPurpose* pPurpose = (KPurpose*)(pPurposeItem->GetItemData());
	
	CXTPReportRecordItem* pCurrentItem = pCurrentRecord->GetItem(MINPUT_NAME);
	KIOColumn* pCurrentInput = (KIOColumn*)(pCurrentItem->GetItemData());
	m_InputMap[pPurpose] = pCurrentInput;

	CXTPReportRecords* pInputRecords = pPurposeRecord->GetChilds();
	int nInputCount = pInputRecords->GetCount();
	for(int i = 0; i < nInputCount; ++i)
	{
		CXTPReportRecord* pInputRecord = pInputRecords->GetAt(i);
		if(pInputRecord != pCurrentRecord)
		{
			CXTPReportRecordItem* pInputItem = pInputRecord->GetItem(MINPUT_NAME);
			pInputItem->SetChecked(FALSE);
		}
	}
}



void KMDemandStratumDlg::PurposeList( std::list<KPurpose*> a_lstPurpose )
{
	m_lstPurpose = a_lstPurpose;
}
