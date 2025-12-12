/**
 * @file MOutputDesignDlg.cpp
 * @brief KMOutputDesignDlg 구현 파일
 * @author 
 * @date 2011.06.02
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "MOutputDesignDlg.h"
#include "afxdialogex.h"
//^^ #include "IOColumn.h"
//^^ #include "IOColumns.h"
//^^ #include "IOTable.h"
//^^ #include "IOTables.h"
#include "Mode.h"
#include "MOutputInfo.h"
#include "Target.h"
#include "KGChangeFieldName.h"


/* Report column index define */
#define MOUTPUT_MODE		0
#define MOUTPUT_STARTDATE	1
#define MOUTPUT_ENDDATE		2
#define MOUTPUT_REMARK		3


#define UM_EXPRESSIONEDIT		WM_USER + 1001


// KMOutputDesignDlg 대화 상자입니다.
IMPLEMENT_DYNAMIC(KMOutputDesignDlg, CXTResizeDialog)

KMOutputDesignDlg::KMOutputDesignDlg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(KMOutputDesignDlg::IDD, pParent)
{

}


KMOutputDesignDlg::~KMOutputDesignDlg()
{
}


void KMOutputDesignDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
}


BEGIN_MESSAGE_MAP(KMOutputDesignDlg, CXTResizeDialog)
	ON_NOTIFY(NM_CLICK, IDC_REPORT, OnReportItemClick)
	ON_NOTIFY(XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT, OnItemButtonClick)
	ON_MESSAGE(UM_EXPRESSIONEDIT, OnExpressionEdit)
END_MESSAGE_MAP()



void KMOutputDesignDlg::Target(KTarget* pTarget)
{
	m_pTarget = pTarget;
}


void KMOutputDesignDlg::AddOutptuInfo(KMode* pMode, KMOutputInfo* pInfo)
{
	std::map<KMode*, KMOutputInfo*>::iterator itOutput = m_OutputMap.find(pMode);
	if(m_OutputMap.end() == itOutput)
	{
		m_OutputMap.insert(std::make_pair(pMode, pInfo));
	}
}


// KMOutputDesignDlg 메시지 처리기입니다.

//Step 1
BOOL KMOutputDesignDlg::OnInitDialog()
{
	CXTResizeDialog::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndReport, FALSE, FALSE);

	initReportControl();
	updateOutputRecord();

	SetResize(IDC_REPORT, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR1, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KMOutputDesignDlg::initReportControl(void)
{
// 	// Report Header 설정
// 	m_wndReport.GetReportHeader()->AllowColumnRemove(FALSE);
// 	m_wndReport.GetReportHeader()->AllowColumnSort(FALSE);
// 	m_wndReport.GetReportHeader()->AllowColumnReorder(FALSE);
// 	m_wndReport.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);
// 
// 	// Report tree 설정
// 	m_wndReport.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureSolid;
// 	//m_wndReport.GetPaintManager()->SetGridStyle(FALSE, xtpReportGridSolid);
// 	//m_wndReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);

	// Add Columns
	CXTPReportColumn* pColumn = NULL;
	pColumn = m_wndReport.AddColumn(
		new CXTPReportColumn(MOUTPUT_MODE, _T("Mode"),150)
		);
	pColumn->SetTreeColumn(TRUE);
	pColumn->SetHeaderAlignment(DT_CENTER);

    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("컬럼이름"));
    }

	pColumn = m_wndReport.AddColumn(
		new CXTPReportColumn(MOUTPUT_STARTDATE, _T("Created Date"), 100)
		);
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("만든 날짜"));
    }

	pColumn = m_wndReport.AddColumn(
		new CXTPReportColumn(MOUTPUT_ENDDATE, _T("Modified Date"), 100)
		);
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("수정한 날짜"));
    }
    
	pColumn = m_wndReport.AddColumn(
		new CXTPReportColumn(MOUTPUT_REMARK, _T("Remarks"), 80)
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
}


void KMOutputDesignDlg::updateOutputRecord(void)
{
	std::map<KMode*, KMOutputInfo*>::iterator itOutput, itEnd = m_OutputMap.end();
	for(itOutput = m_OutputMap.begin(); itOutput != itEnd; ++itOutput)
	{
		KMode* pMode = itOutput->first;
		CXTPReportRecord* pRecord = m_wndReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		                              pRecord->AddItem(new CXTPReportRecordItemText);
									  pRecord->AddItem(new CXTPReportRecordItemText);
									  pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetCaption(pMode->ModeName());
		pItem->SetItemData((DWORD_PTR)pMode);
		addOutputColumns(pRecord, pMode);
	}

	m_wndReport.Populate();
	m_wndReport.ExpandAll(TRUE);
}


void KMOutputDesignDlg::addOutputColumns(CXTPReportRecord* pParent, KMode* pMode)
{
	ASSERT(NULL != m_pTarget);

	bool bChecked = false;
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	CString strSQL;
	int nIndex = 0;

	strSQL.Format(
		_T(" SELECT mode_od_column_name, start_date, end_date")
		_T(" FROM modechoice_model")
		_T(" WHERE mode_group_code = %d")
		_T(" AND detail_mode_id = %d")
		_T(" ORDER BY end_date DESC" ),
		pMode->ModeGroup(),
		pMode->ModeID()
		);

	KIOTables* pIOTables = m_pTarget->Tables();
	ASSERT(NULL != pIOTables);
	KIOTable* pTable = pIOTables->FindTable(_T("mode_od"));
	ASSERT(NULL != pTable);
	const KIOColumns* pColumns = pTable->Columns();

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
				pItem->HasCheckbox(TRUE);
			}

			if(false == bChecked)
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

	pItem = pRecord->AddItem(new CXTPReportRecordItemText);
	CString strNewField = pMode->ModeName();

	std::map<KMode*, KMOutputInfo*>::iterator itOutput = m_OutputMap.find(pMode);
	ASSERT(m_OutputMap.end() != itOutput);
	KMOutputInfo* pOutput = itOutput->second;
	CString strExistingFieldName = pOutput->FieldCaption();

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

	

	CXTPReportRecordItemControl* pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
	if(NULL != pButton)
	{
		pButton->SetAlignment(xtpItemControlRight);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pButton->SetCaption(_T("수정 "));
            pButton->SetSize(CSize(25, 0));
        }
        else {
            pButton->SetCaption(_T("Edit"));
            pButton->SetSize(CSize(22, 0));
        }		
	}

	pItem->SetCaption(strNewField);
	pItem->HasCheckbox(TRUE);
	if(false == bChecked)
	{
		pItem->SetChecked(TRUE);
	}
	pItem = pRecord->AddItem(new CXTPReportRecordItemText);	// start date
	pItem = pRecord->AddItem(new CXTPReportRecordItemText);	// end date
	pItem = pRecord->AddItem(new CXTPReportRecordItemText);	// remark
	pItem->SetCaption(_T("New Field"));

    if (KmzSystem::GetLanguage() == KEMKorea) {
        pItem->SetCaption(_T("새 컬럼 생성"));
    }
}


bool KMOutputDesignDlg::isNameExists(CXTPReportRecord* pParent, LPCTSTR strName)
{
	CXTPReportRecords* pRecords = pParent->GetChilds();
	int nRecordCount = pRecords->GetCount();
	CXTPReportRecordItem* pItem = 
		pRecords->FindRecordItem(0, nRecordCount-1, 0, 0, 0, 0, strName, 
		xtpReportTextSearchExactPhrase);
	return (NULL != pItem);
}


//Step 2
void KMOutputDesignDlg::OnReportItemClick(NMHDR * pNotifyStruct, LRESULT * result)
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
void KMOutputDesignDlg::OnItemButtonClick(NMHDR * a_pNotifyStruct, LRESULT* a_pResult)
{
	XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) a_pNotifyStruct;
	if(!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
		return;

	CXTPReportRecord* pRecord = pItemNotify->pRow->GetRecord();
	CXTPReportRecordItem* pItem = pRecord->GetItem(0);

	PostMessage(UM_EXPRESSIONEDIT, (WPARAM)pItem);
}


LRESULT KMOutputDesignDlg::OnExpressionEdit (WPARAM a_wParam, LPARAM a_lParam)
{
	CXTPReportColumn *pColum = (CXTPReportColumn*)a_wParam;
	CXTPReportRecordItem* pItem = (CXTPReportRecordItem*)a_wParam;
	CString strName = pItem->GetCaption(pColum);
	KGChangeFieldName dlg(strName);
	dlg.DoModal();
	CString strNewFieldName = dlg.m_strNewName;
	pItem->SetCaption(strNewFieldName);
	return 0;

}


//Step 4
void KMOutputDesignDlg::OnOK()
{
	CXTPReportRecords* pRecords = m_wndReport.GetRecords();
	int nRecordCount = pRecords->GetCount();
	for(int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord* pRecord = pRecords->GetAt(i);
		if( false == applyOutputField(pRecord) )
		{
			return;
		}
	}

	CXTResizeDialog::OnOK();
}


bool KMOutputDesignDlg::applyOutputField(CXTPReportRecord* pModeRecord)
{
	CXTPReportRecordItem* pModeItem = pModeRecord->GetItem(MOUTPUT_MODE);
	KMode* pMode = (KMode*)(pModeItem->GetItemData());
	std::map<KMode*, KMOutputInfo*>::iterator itOutput = m_OutputMap.find(pMode);
	ASSERT(m_OutputMap.end() != itOutput);

	KMOutputInfo* pOutput = itOutput->second;

	CXTPReportRecords* pFieldRecords = pModeRecord->GetChilds();

	int nFieldCount = pFieldRecords->GetCount();

	if( nFieldCount< 1 )
	{
		CString strInputDataCheckMsg = _T("");
		strInputDataCheckMsg.Format(_T("\'%s\'에 대한 컬럼 관련 정보가 존재하지 않습니다."), pMode->ModeName());
		AfxMessageBox(strInputDataCheckMsg);
		return false;
	}

	for(int i = 0; i < nFieldCount; ++i)
	{
		CXTPReportRecord* pFieldRecord = pFieldRecords->GetAt(i);
		CXTPReportRecordItem* pFieldItem = pFieldRecord->GetItem(MOUTPUT_MODE);
		if(pFieldItem->IsChecked() == FALSE)
		{
			continue;
		}

		pOutput->FieldCaption(pFieldItem->GetCaption(NULL));

		KIOColumn* pColumn = (KIOColumn*)(pFieldItem->GetItemData());
		if(NULL != pColumn)
		{
			pOutput->FieldName(pColumn->Name());
			pOutput->NewField(false);
		}
		else
		{
			pOutput->FieldName(_T(""));
			pOutput->NewField(true);
		}
	}

	return true;
}
