/**
 * @file DInputDlg.cpp
 * @brief KDInputDlg 구현 파일
 * @author 
 * @date 2011.05.19
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "DInputDlg.h"
#include "afxdialogex.h"
#include "DistributionInfo.h"
//^^ #include "IOColumn.h"
//^^ #include "IOColumns.h"
//^^ #include "IOTable.h"
//^^ #include "IOTables.h"
//#include "Purpose.h"
#include "PAPurpose.h"
#include "Target.h"
#include "KmzSystem.h"


/* Report item index */
#define DINPUT_NAME			0
#define DINPUT_STARTDATE	1
#define DINPUT_ENDDATE		2



IMPLEMENT_DYNAMIC(KDInputDlg, KDialogEx)

KDInputDlg::KDInputDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KDInputDlg::IDD, pParent)
{

}

KDInputDlg::~KDInputDlg()
{
}

void KDInputDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
}


BEGIN_MESSAGE_MAP(KDInputDlg, KDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_REPORT, OnReportItemClick)
END_MESSAGE_MAP()



//void KDInputDlg::AddModelPurpose(KPurpose* pPurpose)
//{
//	std::list<KPurpose*>::iterator itPurpose, itEnd = m_PurposeList.end();
//	for(itPurpose = m_PurposeList.begin(); itPurpose != itEnd; ++itPurpose)
//	{
//		if(*itPurpose == pPurpose)
//		{
//			return;
//		}
//	}
//
//	m_PurposeList.push_back(pPurpose);
//}
void KDInputDlg::AddModelPurpose(KPAPurpose* pPurpose)
{
	std::list<KPAPurpose*>::iterator itPurpose, itEnd = m_PurposeList.end();
	for(itPurpose = m_PurposeList.begin(); itPurpose != itEnd; ++itPurpose)
	{
		if(*itPurpose == pPurpose)
		{
			return;
		}
	}

	m_PurposeList.push_back(pPurpose);
}


//void KDInputDlg::RemoveModelPurpose(KPurpose* pPurpose)
//{
//	std::list<KPurpose*>::iterator itPurpose, itEnd = m_PurposeList.end();
//	for(itPurpose = m_PurposeList.begin(); itPurpose != itEnd; ++itPurpose)
//	{
//		if(*itPurpose == pPurpose)
//		{
//			m_PurposeList.erase(itPurpose);
//		}
//	}
//}
void KDInputDlg::RemoveModelPurpose(KPAPurpose* pPurpose)
{
	std::list<KPAPurpose*>::iterator itPurpose, itEnd = m_PurposeList.end();
	for(itPurpose = m_PurposeList.begin(); itPurpose != itEnd; ++itPurpose)
	{
		if(*itPurpose == pPurpose)
		{
			m_PurposeList.erase(itPurpose);
		}
	}
}


void KDInputDlg::RemoveAllModelPurpose(void)
{
	m_PurposeList.clear();
}


//void KDInputDlg::SetModelPurposeList(std::list<KPurpose*>& purposeList)
//{
//	m_PurposeList.clear();
//	m_PurposeList = purposeList;
//}
void KDInputDlg::SetModelPurposeList(std::list<KPAPurpose*>& purposeList)
{
	m_PurposeList.clear();
	m_PurposeList = purposeList;
}


void KDInputDlg::SetTarget(KTarget* pTarget)
{
	m_pTarget = pTarget;
}


void KDInputDlg::initReportColumn(void)
{
	// Report Header 설정
	/*m_wndReport.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndReport.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndReport.GetReportHeader()->AllowColumnReorder(FALSE);
	m_wndReport.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);*/

	// Report tree 설정
    m_wndReport.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureSolid;
//     m_wndReport.GetPaintManager()->SetGridStyle(FALSE, xtpReportGridSolid);
//     m_wndReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSmallDots);

	// Add Columns
	CXTPReportColumn* pColumn = NULL;
	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(DINPUT_NAME, _T("Input"), 150));
	pColumn->SetTreeColumn(TRUE);
	pColumn->SetHeaderAlignment(DT_CENTER);

    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("컬럼선택"));
    }

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(DINPUT_STARTDATE, _T("Created Date"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);

    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("만든 날짜"));
    }

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(DINPUT_ENDDATE, _T("Modified Date"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("수정한 날짜"));
    }

	// Report control 설정
	m_wndReport.AllowEdit(FALSE);
	m_wndReport.EditOnClick(FALSE);
	m_wndReport.SetMultipleSelection(FALSE);
	m_wndReport.SetTreeIndent(10);
    m_wndReport.SelectionEnable(FALSE);
}


void KDInputDlg::updateReportRecord(void)
{
	std::list<KDistributionInfo*>::iterator itInfo, itEndInfo = m_DInfoList.end();
	for(itInfo = m_DInfoList.begin(); itInfo != itEndInfo; ++itInfo)
	{
		KDistributionInfo* pInfo = *itInfo;
		CXTPReportRecord* pInfoRecord = m_wndReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pInfoItem = pInfoRecord->AddItem(new CXTPReportRecordItemText);
		pInfoItem->SetCaption(pInfo->PurposeName());
		pInfoItem->SetItemData((DWORD_PTR)pInfo);
        pInfoItem->SetBold(TRUE);
        pInfoItem->SetBackgroundColor(RGB(191,239,254));

        pInfoItem = pInfoRecord->AddItem(new CXTPReportRecordItemText(_T("")));                
        pInfoItem->SetBackgroundColor(RGB(191,239,254));

        pInfoItem = pInfoRecord->AddItem(new CXTPReportRecordItemText(_T("")));                
        pInfoItem->SetBackgroundColor(RGB(191,239,254));
	}

	std::list<KPAPurpose*>::iterator itPurpose, itEnd = m_PurposeList.end();
	for(itPurpose = m_PurposeList.begin(); itPurpose != itEnd; ++itPurpose)
	{
		KPAPurpose* pPurpose = *itPurpose;

		// 목적이 존재하는지 확인
		CXTPReportRecords* pRootRecords = m_wndReport.GetRecords();
		int nRecordCount = pRootRecords->GetCount();

		CXTPReportRecord* pRootRecord = NULL;
		CXTPReportRecordItem* pRootRecordItem = pRootRecords->FindRecordItem(
			0, nRecordCount, 0, 0, 0, 0, pPurpose->PurposeName(),
			xtpReportTextSearchExactPhrase | xtpReportTextSearchMatchCase);

		//if(NULL == pRootRecordItem)
		//{
		//	pRootRecord = m_wndReport.AddRecord(new CXTPReportRecord);
		//	pRootRecordItem = pRootRecord->AddItem(new CXTPReportRecordItemText);
		//	pRootRecordItem->SetCaption(pPurpose->Name());
		//}
		//else 
		//{
		//	pRootRecord = pRootRecordItem->GetRecord();
		//}
		if(NULL == pRootRecordItem)
		{
			continue;
		}

		pRootRecord = pRootRecordItem->GetRecord();

		CXTPReportRecord* pPARecord = pRootRecord->GetChilds()->Add(new CXTPReportRecord);
		CXTPReportRecordItem* pPARecordItem = pPARecord->AddItem(new CXTPReportRecordItemText);
		if(pPurpose->PAType() == KEMPATypeProduction)
		{
            if (KmzSystem::GetLanguage() == KEMKorea) {
                pPARecordItem->SetCaption(_T("유출모형"));
            }
            else {
                pPARecordItem->SetCaption(PRODUCTION_STRING);
            }			
		}
		else
		{
            if ( KmzSystem::IsPassengerSystem() )
            {
                if (KmzSystem::GetLanguage() == KEMKorea) {
                    pPARecordItem->SetCaption(_T("유입모형"));
                }
                else {
                    pPARecordItem->SetCaption(ATTRACTION_STRING);
                }			    
            }
            else
            {
                pPARecordItem->SetCaption(CONSUMPTION_STRING);
            }
		}
		pPARecordItem->SetItemData((DWORD_PTR)pPurpose);		
		updatePARecord(pPARecord, pPurpose);

        pPARecordItem->SetBackgroundColor(RGB(240,240,240));

        pPARecordItem = pPARecord->AddItem(new CXTPReportRecordItemText(_T("")));                
        pPARecordItem->SetBackgroundColor(RGB(240,240,240));

        pPARecordItem = pPARecord->AddItem(new CXTPReportRecordItemText(_T("")));                
        pPARecordItem->SetBackgroundColor(RGB(240,240,240));
	}
	m_wndReport.Populate();
	m_wndReport.ExpandAll(TRUE);
}


void KDInputDlg::updatePARecord(CXTPReportRecord* pPARecord, KPAPurpose* pPurpose)
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	bool    bCheck = false;
	CString strSQL = _T("");

	KIOTables* pIOTables = m_pTarget->Tables();
	ASSERT(NULL != pIOTables);
	KIOTable* pZoneTable = pIOTables->FindTable(_T("zone"));
	ASSERT(NULL != pZoneTable);
	const KIOColumns* pColumns = pZoneTable->Columns();

	strSQL.Format(
		_T(" SELECT zone_column_name, start_date, end_date, generation_run_seq")
		_T(" FROM generation_model")
		_T(" WHERE object_group_code = %d")
		_T(" AND detail_object_id = %d")
		_T(" AND pa_flag = '%c' ")
		_T(" ORDER BY end_date DESC"),
		pPurpose->PurposeGroup(),
		pPurpose->PurposeID(),
		pPurpose->PAType() == KEMPATypeProduction ? 'p' : 'a'
		);

	try
	{
		KResultSetPtr      spResult = spDBaseConnection->ExecuteQuery(strSQL);

		CXTPReportRecords* pRecords = pPARecord->GetChilds();
		
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

			if(false == bCheck)
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
}

// KDInputDlg 메시지 처리기입니다.
BOOL KDInputDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );
	
	KReportCtrlSetting::Default(m_wndReport);
	initReportColumn();
	updateReportRecord();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KDInputDlg::OnOK()
{
	CXTPReportRecords* pRecords = m_wndReport.GetRecords();
	int nRecordCount = pRecords->GetCount();
	for(int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord*   pRecord = pRecords->GetAt(i);	// 목적 이름
		CXTPReportRecords*  pChilds = pRecord->GetChilds();
		int nChildCount =   pChilds->GetCount();
		for(int j = 0; j < nChildCount; ++j)
		{
			CXTPReportRecord* pRecPurpose = pChilds->GetAt(j);
			if( false == setInputData(pRecPurpose) )
			{
				return;
			}
		}
	}

	if(nRecordCount < 1)
	{
		AfxMessageBox(_T("목적 통행 데이터가 존재하지 않습니다."));
		return;
	}
	
	KDialogEx::OnOK();
}


bool KDInputDlg::setInputData(CXTPReportRecord* pRecord)
{
	ASSERT(NULL != pRecord);

	// todo : 아래 코드 수정(2011.05.30);
	CXTPReportRecord* pParent = pRecord->GetParentRecord();
	ASSERT(NULL != pParent);
	CXTPReportRecordItem* pParentItem = pParent->GetItem(0);
	KDistributionInfo* pDInfo = (KDistributionInfo*)(pParentItem->GetItemData());

	CXTPReportRecordItem* pItem = pRecord->GetItem(0);
	KPAPurpose* pPurpose = (KPAPurpose*)(pItem->GetItemData());
	ASSERT(NULL != pPurpose);

	CXTPReportRecords*  pChilds = pRecord->GetChilds();
	int nCount = pChilds->GetCount();

	if(nCount < 1)
	{
		CString strEmptyPAType = _T("");

		if(KEMPATypeProduction == pPurpose->PAType())
		{
			strEmptyPAType = PRODUCTION_STRING;
		}
		else
		{
			strEmptyPAType = (KmzSystem::IsPassengerSystem() == true) ? ATTRACTION_STRING : CONSUMPTION_STRING;
		}

		CString strInputDataCheckMsg = _T("");
		strInputDataCheckMsg.Format(_T("%s에 대한 %s 관련 데이터가 존재하지 않습니다."), pPurpose->PurposeName(), strEmptyPAType);
		AfxMessageBox(strInputDataCheckMsg);
		return false;
	}

	for(int i = 0; i < nCount; ++i)
	{
		CXTPReportRecord* pChild = pChilds->GetAt(i);
		CXTPReportRecordItem* pChildItem = pChild->GetItem(0);
		if(pChildItem->IsChecked() == FALSE)
		{
			continue;
		}

		KIOColumn* pColumn = (KIOColumn*)(pChildItem->GetItemData());
		ASSERT(NULL != pColumn);

		if(pPurpose->PAType() == KEMPATypeProduction)
		{
			pDInfo->ZoneProductionColumnCaption(pChildItem->GetCaption(NULL));
			pDInfo->ZoneProductionColumnName(pColumn->Name());
		}
		else
		{
			pDInfo->ZoneAttractionColumnCaption(pChildItem->GetCaption(NULL));
			pDInfo->ZoneAttractionColumnName(pColumn->Name());
		}
	}

	return true;
}


void KDInputDlg::OnReportItemClick(NMHDR * pNotifyStruct, LRESULT * result)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;
	if(!pItemNotify->pRow || !pItemNotify->pColumn)
	{
		return;
	}

	if(pItemNotify->pColumn->GetItemIndex() == 0)
	{
		CXTPReportRecord* pCurrent = pItemNotify->pItem->GetRecord();
		if(pCurrent->HasChildren() == FALSE)
		{
			pItemNotify->pItem->SetChecked(TRUE);
			CXTPReportRecord* pParent = pCurrent->GetParentRecord();
			if(NULL != pParent)
			{
				CXTPReportRecords* pRecords = pParent->GetChilds();
				int nRecordCount = pRecords->GetCount();
				for(int i = 0; i < nRecordCount; ++i)
				{
					CXTPReportRecord* pRecord = pRecords->GetAt(i);
					if(pRecord == pCurrent)
					{
						continue;
					}
					CXTPReportRecordItem* pItem = pRecord->GetItem(0);
					pItem->SetChecked(FALSE);
				}
			}
		}
	}
}



