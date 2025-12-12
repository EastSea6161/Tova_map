/**
 * @file DOutputDesignDlg.cpp
 * @brief KDOutputDesignDlg 구현 파일
 * @author 
 * @date 2011.05.23
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "DOutputDesignDlg.h"
#include "afxdialogex.h"
#include "DistributionInfo.h"
//^^ #include "IOColumn.h"
//^^ #include "IOColumns.h"
//^^ #include "IOTable.h"
//^^ #include "IOTables.h"
#include "Purpose.h"
#include "Target.h"
#include "KGChangeFieldName.h"


/* Report item index */
#define DOUTPUTDESIGN_PURPOSE			0
#define DOUTPUTDESIGN_STARTDATE			1
#define DOUTPUTDESIGN_ENDDATE			2
#define DOUTPUTDESIGN_REMARK			3


#define UM_EXPRESSIONEDIT		WM_USER + 1001



IMPLEMENT_DYNAMIC(KDOutputDesignDlg, CXTResizeDialog)

KDOutputDesignDlg::KDOutputDesignDlg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(KDOutputDesignDlg::IDD, pParent)
{

}

KDOutputDesignDlg::~KDOutputDesignDlg()
{
}

void KDOutputDesignDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
}


BEGIN_MESSAGE_MAP(KDOutputDesignDlg, CXTResizeDialog)
	ON_NOTIFY(NM_CLICK, IDC_REPORT, OnReportItemClick)
	ON_NOTIFY(XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT, OnItemButtonClick)
	ON_MESSAGE(UM_EXPRESSIONEDIT, OnExpressionEdit)
END_MESSAGE_MAP()



void KDOutputDesignDlg::SetTarget(KTarget* pTarget)
{
	m_pTarget = pTarget;
}


// KDOutputDesignDlg 메시지 처리기입니다.

//Step 1
BOOL KDOutputDesignDlg::OnInitDialog()
{
	CXTResizeDialog::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndReport);
	initReportControl();
	updateOutputRecord();

	SetResize(IDC_REPORT, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KDOutputDesignDlg::initReportControl(void)
{
	// Report Header 설정
	//m_wndReport.GetReportHeader()->AllowColumnRemove(FALSE);
	//m_wndReport.GetReportHeader()->AllowColumnSort(FALSE);
	//m_wndReport.GetReportHeader()->AllowColumnReorder(FALSE);
	//m_wndReport.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);

	// Report tree 설정
	m_wndReport.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureSolid;
// 	m_wndReport.GetPaintManager()->SetGridStyle(FALSE, xtpReportGridSolid);
// 	m_wndReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSmallDots);

	// Add Columns
	CXTPReportColumn* pColumn = NULL;
	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(DOUTPUTDESIGN_PURPOSE, _T("Purpose"),150));
	pColumn->SetTreeColumn(TRUE);
	pColumn->SetHeaderAlignment(DT_CENTER);

    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("통행목적"));
    }

	pColumn = m_wndReport.AddColumn(
		new CXTPReportColumn(DOUTPUTDESIGN_STARTDATE, _T("Created Date"), 100)
		);
	pColumn->SetHeaderAlignment(DT_CENTER);

    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("만든 날짜"));
    }

	pColumn = m_wndReport.AddColumn(
		new CXTPReportColumn(DOUTPUTDESIGN_ENDDATE, _T("Modified Date"), 100)
		);
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("수정한 날짜"));
    }

	pColumn = m_wndReport.AddColumn(
		new CXTPReportColumn(DOUTPUTDESIGN_REMARK, _T("Remarks"), 80)
		);
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("비고"));
    }

	// Report control 설정
	m_wndReport.AllowEdit(FALSE);
	m_wndReport.EditOnClick(FALSE);
	m_wndReport.SetMultipleSelection(FALSE);
	m_wndReport.SetTreeIndent(10);
	m_wndReport.SelectionEnable(FALSE);
}


void KDOutputDesignDlg::updateOutputRecord(void)
{
	std::list<KDistributionInfo*>::iterator itInfo, itEnd = m_DInfoList.end();
	for(itInfo = m_DInfoList.begin(); itInfo != itEnd; ++itInfo)
	{
		KDistributionInfo* pInfo = *itInfo;
		CXTPReportRecord* pRecord = NULL;
		CXTPReportRecordItem* pItem = NULL;

		pRecord = m_wndReport.AddRecord(new CXTPReportRecord);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetCaption(pInfo->PurposeName());
		pItem->SetItemData((DWORD_PTR)pInfo);
		pItem->SetBold(TRUE);
		pItem->SetBackgroundColor(RGB(191,239,254));

		pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));                
		pItem->SetBackgroundColor(RGB(191,239,254));

		pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));                
		pItem->SetBackgroundColor(RGB(191,239,254));

		pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));                
		pItem->SetBackgroundColor(RGB(191,239,254));

		addOutputColumns(pRecord, pInfo);
	}
	m_wndReport.Populate();
	m_wndReport.ExpandAll(TRUE);
}


void KDOutputDesignDlg::addOutputColumns(CXTPReportRecord* pParent, KDistributionInfo* pInfo)
{
	ASSERT(NULL != m_pTarget);

	bool bChecked = false;
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	CString strSQL;
	int nIndex = 0;

	strSQL.Format(
		_T(" SELECT purpose_od_column_name, start_date, end_date")
		_T(" FROM distribution_model")
		_T(" WHERE object_group_code = %d")
		_T(" AND detail_object_id = %d")
		_T(" ORDER BY end_date DESC" ),
		pInfo->PurposeGroupID(),
		pInfo->PurposeID()
		);

	KIOTables* pIOTables = m_pTarget->Tables();
	ASSERT(NULL != pIOTables);
	KIOTable* pTable = pIOTables->FindTable(_T("purpose_od"));
	ASSERT(NULL != pTable);
	const KIOColumns* pColumns = pTable->Columns();

	CString strExistingFieldName = pInfo->OutputPurposeODColumnCaption();

	try
	{
		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResult->Next())
		{
			CXTPReportRecord* pRecord = pParent->GetChilds()->Add(new CXTPReportRecord);
			CXTPReportRecordItem* pItem = NULL;

			// column name
			CString strColumn = spResult->GetValueString(0);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			KIOColumn* pColumn = pColumns->GetColumn(strColumn);
			if(NULL != pColumn)
			{
				pItem->SetCaption(pColumn->Caption());
				pItem->SetItemData((DWORD_PTR)pColumn);
			}

			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(FALSE);
				

			if (strExistingFieldName.CompareNoCase(pColumn->Caption()) == 0)
			{
				pItem->SetChecked(TRUE);
				bChecked = true;
			}

			// start date
			CString strStartTime = spResult->GetValueString(1);
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			pItem->SetCaption(strStartTime);

			// end date
			CString strEndTime = spResult->GetValueString(2);
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			pItem->SetCaption(strEndTime);

			// remark
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			pItem->SetCaption(_T("Overwrite"));
            if (KmzSystem::GetLanguage() == KEMKorea) {
                pItem->SetCaption(_T("덮어쓰기"));
            }
			++nIndex;
		}
	}
	catch (...)
	{

	}

	// 새 column 항목 추가
	CXTPReportRecord* pRecord = pParent->GetChilds()->Add(new CXTPReportRecord);
	CXTPReportRecordItem* pItem = NULL;

	CString strNewField = pInfo->PurposeName();

	if (strExistingFieldName == _T(""))
	{
		if(0 < nIndex)
		{
			CString strTempName;
			strTempName.Format(_T("%s_%d"), strNewField, nIndex);
			while(true == isNameExists(pParent, strTempName))
			{
				strTempName.Format(_T("%s_%d"), strNewField, ++nIndex);
			}
			strNewField = strTempName;
		}
	} 
	else
	{
		if (bChecked == true)
		{
			CString strTempName;
			strTempName.Format(_T("%s_%d"), strNewField, nIndex);
			while(true == isNameExists(pParent, strTempName))
			{
				strTempName.Format(_T("%s_%d"), strNewField, ++nIndex);
			}
			strNewField = strTempName;
		}
		else
		{
			strNewField = strExistingFieldName;
		}
	}

	pItem = pRecord->AddItem(new CXTPReportRecordItemText(strNewField));
	CXTPReportRecordItemControl* pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
	if(NULL != pButton)
	{
		pButton->SetAlignment(xtpItemControlUnknown);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pButton->SetCaption(_T("수정"));
            pButton->SetSize(CSize(25, 0));
        }
        else {
            pButton->SetCaption(_T("Edit"));
            pButton->SetSize(CSize(22, 0));
        }		
	}


	pItem->SetCaption(strNewField);
	if (strExistingFieldName.CompareNoCase(_T("")) == 0 || strExistingFieldName.CompareNoCase(strNewField) == 0)
	{
		pItem->SetChecked(TRUE);//
	}
	else
	{
		pItem->SetChecked(FALSE);//
	}
	
	pItem->HasCheckbox(TRUE);
	pItem = pRecord->AddItem(new CXTPReportRecordItemText);	// start date
	pItem = pRecord->AddItem(new CXTPReportRecordItemText);	// end date
	pItem = pRecord->AddItem(new CXTPReportRecordItemText);	// remark
	pItem->SetCaption(_T("New Field"));
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pItem->SetCaption(_T("새 컬럼 생성"));
    }
}


bool KDOutputDesignDlg::isNameExists(CXTPReportRecord* pParent, LPCTSTR strName)
{
	CXTPReportRecords* pRecords = pParent->GetChilds();
	int nRecordCount = pRecords->GetCount();
	CXTPReportRecordItem* pItem = 
		pRecords->FindRecordItem(0, nRecordCount-1, 0, 0, 0, 0, strName, 
		xtpReportTextSearchExactPhrase);
	return (NULL != pItem);
}


//Step 2
void KDOutputDesignDlg::OnReportItemClick(NMHDR * pNotifyStruct, LRESULT * result)
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


//Step 3
void KDOutputDesignDlg::OnItemButtonClick(NMHDR * a_pNotifyStruct, LRESULT* a_pResult)
{
	XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) a_pNotifyStruct;
	if(!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
		return;

	CXTPReportRecord* pRecord = pItemNotify->pRow->GetRecord();
	CXTPReportRecordItem* pItem = pRecord->GetItem(0);

	PostMessage(UM_EXPRESSIONEDIT, (WPARAM)pItem);
}


LRESULT KDOutputDesignDlg::OnExpressionEdit (WPARAM a_wParam, LPARAM a_lParam)
{
	CXTPReportColumn *pColum = (CXTPReportColumn*)a_wParam;
	CXTPReportRecordItem* pItem = (CXTPReportRecordItem*)a_wParam;
	CXTPReportRecordItemText* pItemText = (CXTPReportRecordItemText*)a_wParam;
	CString strName = pItem->GetCaption(pColum);
	KGChangeFieldName dlg(strName);
	dlg.DoModal();
	CString strNewFieldName = dlg.m_strNewName;
	pItem->SetCaption(strNewFieldName);
	pItemText->SetValue(strNewFieldName);
	return 0;

}


//Step 4
void KDOutputDesignDlg::OnOK()
{
	CXTPReportRecords* pRecords = m_wndReport.GetRecords();
	int nRecordCount = pRecords->GetCount();
	for(int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord* pRecord = pRecords->GetAt(i);	
		if( false == setDistributionOutput(pRecord) )
		{
			return;
		}
	}

	if( nRecordCount< 1 )
	{
		AfxMessageBox(_T("목적 통행 데이터가 존재하지 않습니다."));
		return;
	}

	CXTResizeDialog::OnOK();
}


bool KDOutputDesignDlg::setDistributionOutput(CXTPReportRecord* pRecord)
{
	ASSERT(NULL != pRecord);

	CXTPReportRecordItem* pItem = pRecord->GetItem(0);
	KDistributionInfo* pDInfo = (KDistributionInfo*)(pItem->GetItemData());
	ASSERT(NULL != pDInfo);

	CXTPReportRecords*  pChilds = pRecord->GetChilds();
	int nCount = pChilds->GetCount();

	if( nCount< 1 )
	{
		CString strInputDataCheckMsg = _T("");
		strInputDataCheckMsg.Format(_T("%s에 대한 컬럼 데이터가 존재하지 않습니다."), pDInfo->PurposeName());
		AfxMessageBox(strInputDataCheckMsg);
		return false;
	}

	for(int i = 0; i < nCount; ++i)
	{
		CXTPReportRecord* pChild = pChilds->GetAt(i);
		//CXTPReportRecordItem* pChildItem = pChild->GetItem(0);
		CXTPReportRecordItemText* pChildItem = (CXTPReportRecordItemText*)pChild->GetItem(0);
		if(pChildItem->IsChecked() == FALSE)
		{
			continue;
		}

		//pDInfo->OutputColumnCaption(pChildItem->GetCaption(NULL));
		pDInfo->OutputPurposeODColumnCaption(pChildItem->GetValue());

		KIOColumn* pColumn = (KIOColumn*)(pChildItem->GetItemData());
		if(NULL != pColumn)
		{
			pDInfo->OutputPurposeODColumnName(pColumn->Name());
            pDInfo->OutputPurposeODColumnCaption(pColumn->Caption());
		}
		else
		{
			pDInfo->OutputPurposeODColumnName(_T(""));
		}
	}

	return true;
}