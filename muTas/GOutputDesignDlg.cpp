/**
 * @file GOutputDesignDlg.cpp
 * @brief KGOutputDesignDlg 구현 파일
 * @author 
 * @date 2011.05.17
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "GOutputDesignDlg.h"
#include "afxdialogex.h"
#include "GenerationInfo.h"
//^^ #include "IOColumn.h"
//^^ #include "IOColumns.h"
//^^ #include "IOTable.h"
#include "Target.h"
#include "KmzSystem.h"
#include "KGChangeFieldName.h"


/* column Index define */
//#define GOUTPUT_COLUMNOBJ		0
#define GOUTPUT_COLUMNNAME		0
#define GOUTPUT_STARTDATE		1
#define GOUTPUT_ENDDATE			2
#define GOUTPUT_REMARK			3



#define UM_EXPRESSIONEDIT		WM_USER + 1001


IMPLEMENT_DYNAMIC(KGOutputDesignDlg, CXTResizeDialog)

KGOutputDesignDlg::KGOutputDesignDlg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(KGOutputDesignDlg::IDD, pParent)
{
	m_pZone = NULL;
}


KGOutputDesignDlg::~KGOutputDesignDlg()
{
}


void KGOutputDesignDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
}


BEGIN_MESSAGE_MAP(KGOutputDesignDlg, CXTResizeDialog)
	ON_BN_CLICKED(IDOK, &KGOutputDesignDlg::OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_REPORT, OnReportItemClick)
	ON_NOTIFY(XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT, OnItemButtonClick)
    ON_BN_CLICKED(IDCANCEL, &KGOutputDesignDlg::OnBnClickedCancel)
	ON_MESSAGE(UM_EXPRESSIONEDIT, OnExpressionEdit)
END_MESSAGE_MAP()



void KGOutputDesignDlg::SetTarget(KTarget* a_pTarget)
{
	m_pTarget = a_pTarget;
}


void KGOutputDesignDlg::SetZoneTable(KIOTable* a_pTable)
{
	m_pZone = a_pTable;
}


//Step 1
BOOL KGOutputDesignDlg::OnInitDialog()
{
	CXTResizeDialog::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndReport);
	initReportControl();
	updateReportControl();

	SetResize(IDC_REPORT, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KGOutputDesignDlg::initReportControl(void)
{
	//// Report Header 설정
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
	if (KmzSystem::IsPassengerSystem())
	{
		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(GOUTPUT_COLUMNNAME, _T("Purpose"), 150));
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("컬럼이름"));
        }
	}
	else
	{
		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(GOUTPUT_COLUMNNAME, _T("Item"), 150));
	}
	pColumn->SetTreeColumn(TRUE);
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(GOUTPUT_STARTDATE, _T("Created Date"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("만든 날짜"));
    }

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(GOUTPUT_ENDDATE, _T("Modified Date"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("수정한 날짜"));
    }

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(GOUTPUT_REMARK, _T("Remarks"), 80));
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


void KGOutputDesignDlg::updateReportControl(void)
{
	std::map<CString, CXTPReportRecord*> recordMap;
	std::map<CString, CXTPReportRecord*>::iterator itRecord;

	CXTPReportRecord* pRecord = NULL;
	CXTPReportRecordItem* pItem = NULL;
	std::list<KGenerationInfo*>::iterator itInfo, itEnd = m_lstGInfoList.end();
	for(itInfo = m_lstGInfoList.begin(); itInfo != itEnd; ++itInfo)
	{
		KGenerationInfo* pInfo = *itInfo;
		if (pInfo->UseGeneration())
		{
			// 목적의 record가 존재 하는지 확인
			itRecord = recordMap.find(pInfo->GetName());
			if(itRecord == recordMap.end())
			{
				CString strName = pInfo->GetName();
				pRecord = m_wndReport.AddRecord(new CXTPReportRecord);                
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
				pItem->SetCaption(pInfo->GetName());
				pItem->SetBold(TRUE);
				pItem->SetBackgroundColor(RGB(191,239,254));

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));                
				pItem->SetBackgroundColor(RGB(191,239,254));

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));                
				pItem->SetBackgroundColor(RGB(191,239,254));

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));                
				pItem->SetBackgroundColor(RGB(191,239,254));

				recordMap.insert(std::make_pair(pInfo->GetName(), pRecord));
			}
			else
			{
				pRecord = itRecord->second;
			}

			addPARecord(pRecord, pInfo);
		}
	}
	m_wndReport.Populate();
	m_wndReport.ExpandAll(TRUE);		
}


void KGOutputDesignDlg::addPARecord(CXTPReportRecord* a_pParent, KGenerationInfo* a_pGInfo)
{
	ASSERT(NULL != a_pParent);
	ASSERT(NULL != a_pGInfo);

	// 목적 Record에 PA 타입 추가	
	CXTPReportRecord* pRecord = a_pParent->GetChilds()->Add(new CXTPReportRecord);
	CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText);

	KEMPAType emPAType = a_pGInfo->GetPAType();
	if(KEMPATypeProduction == emPAType)
	{
		pItem->SetCaption(PRODUCTION_STRING);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pItem->SetCaption(_T("유출모형"));
        }
	}
	else
	{
		if (KmzSystem::IsPassengerSystem())
		{
			pItem->SetCaption(ATTRACTION_STRING);
            if (KmzSystem::GetLanguage() == KEMKorea) {
                pItem->SetCaption(_T("유입모형"));
            }
		}
		else
		{
			pItem->SetCaption(CONSUMPTION_STRING);
		}

	}
	pItem->SetItemData((DWORD_PTR)a_pGInfo);
	pItem->SetBackgroundColor(RGB(240,240,240));

	pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));                
	pItem->SetBackgroundColor(RGB(240,240,240));

	pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));                
	pItem->SetBackgroundColor(RGB(240,240,240));

	pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));                
	pItem->SetBackgroundColor(RGB(240,240,240));
	addOutputColumns(pRecord, a_pGInfo);
}


void KGOutputDesignDlg::addOutputColumns(CXTPReportRecord* a_pParent, KGenerationInfo* a_pGInfo)
{
	ASSERT(NULL != m_pTarget);

	bool bChecked = false;
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	CString strSQL(_T(""));
	int nIndex = 0;

	strSQL.Format(
		_T(" SELECT zone_column_name, start_date, end_date, generation_run_seq")
		_T(" FROM generation_model")
		_T(" WHERE object_group_code = %d")
		_T(" AND detail_object_id = %d")
		_T(" AND pa_flag = '%s' ")
		_T(" ORDER BY end_date DESC "),
		a_pGInfo->GetObjectGroupID(),
		a_pGInfo->GetObjectID(),
		a_pGInfo->GetPAType() == KEMPATypeProduction ? _T("p") : _T("a")
		);

	CString strExistingFieldName = a_pGInfo->OutputColumnCaption();
	try
	{
		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		const KIOColumns* pColumns = m_pZone->Columns();
		while(spResult->Next())
		{
			CXTPReportRecord* pRecord = a_pParent->GetChilds()->Add(new CXTPReportRecord);
			CXTPReportRecordItem* pItem = NULL;

			CString strColumn = spResult->GetValueString(GOUTPUT_COLUMNNAME);

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
						
			CString strStartTime = spResult->GetValueString(GOUTPUT_STARTDATE);
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			pItem->SetCaption(strStartTime);
						
			CString strEndTime = spResult->GetValueString(GOUTPUT_ENDDATE);
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			pItem->SetCaption(strEndTime);
						
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
	CXTPReportRecord* pRecord = a_pParent->GetChilds()->Add(new CXTPReportRecord);
	CXTPReportRecordItem* pItem = NULL;

	pItem = pRecord->AddItem(new CXTPReportRecordItemText);
	CString strNewField = a_pGInfo->GetName();

	if (KmzSystem::IsPassengerSystem())
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strNewField += a_pGInfo->GetPAType() == KEMPATypeProduction ? _T("(유출)") : _T("(유입)");
		}
		else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
			strNewField += a_pGInfo->GetPAType() == KEMPATypeProduction ? _T("(Out)") : _T("(In)");
		}
	}
	else
	{
		strNewField += a_pGInfo->GetPAType() == KEMPATypeProduction ? _T("(P)") : _T("(C)");
	}

	if (strExistingFieldName == _T(""))
	{
		if(0 < nIndex)
		{
			CString strTempName;
			strTempName.Format(_T("%s_%d"), strNewField, nIndex);
			while(true == isNameExists(a_pParent, strTempName))
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
			while(true == isNameExists(a_pParent, strTempName))
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

	pItem->SetEditable(TRUE);
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


bool KGOutputDesignDlg::isNameExists(CXTPReportRecord* a_pParent, LPCTSTR a_strName)
{
	CXTPReportRecords* pRecords = a_pParent->GetChilds();
	int nRecordCount = pRecords->GetCount();
	CXTPReportRecordItem* pItem = 
		pRecords->FindRecordItem(0, nRecordCount-1, 0, 0, 0, 0, a_strName, 
		xtpReportTextSearchExactPhrase);
	return (NULL != pItem);
}


void KGOutputDesignDlg::OnItemButtonClick(NMHDR * a_pNotifyStruct, LRESULT* a_pResult)
{
	XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) a_pNotifyStruct;
	if(!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
		return;

	CXTPReportRecord* pRecord = pItemNotify->pRow->GetRecord();
	CXTPReportRecordItem* pItem = pRecord->GetItem(0);

	PostMessage(UM_EXPRESSIONEDIT, (WPARAM)pItem);
}


LRESULT KGOutputDesignDlg::OnExpressionEdit(WPARAM a_wParam, LPARAM a_lParam)
{
	CXTPReportColumn *pColum = (CXTPReportColumn *)a_wParam;
	CXTPReportRecordItem* pItem = (CXTPReportRecordItem*)a_wParam;
	CString strName = pItem->GetCaption(pColum);
	KGChangeFieldName dlg(strName);
	dlg.DoModal();
	pItem->SetCaption(dlg.m_strNewName);
	return 0;
}


//Step 2
void KGOutputDesignDlg::OnReportItemClick(NMHDR * a_pNotifyStruct, LRESULT * a_result)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) a_pNotifyStruct;
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
void KGOutputDesignDlg::OnBnClickedOk()
{
	CXTPReportRecords* pRecords = m_wndReport.GetRecords();
	int nRecordCount = pRecords->GetCount();
	for(int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord*  pRecord = pRecords->GetAt(i);	// 목적 이름
		CXTPReportRecords* pChilds = pRecord->GetChilds();
		int nChildCount = pChilds->GetCount();

		/*if(nChildCount == 1)
		{
			CXTPReportRecordItem*    pParentItem = pRecord->GetItem(0);
			CString strPurposeName = pParentItem->GetCaption(NULL);

			CXTPReportRecord*     pRecPurpose = pChilds->GetAt(0);
			CXTPReportRecordItem* pItem       = pRecPurpose->GetItem(0);
			KGenerationInfo*      pGInfo      = (KGenerationInfo*)pItem->GetItemData();
			KEMPAType emPAType  = pGInfo->GetPAType();

			CString strEmptyPAType = _T("");
			// 없는 type 찾기 (반대 타입)
			if(KEMPATypeProduction == emPAType)
			{
				strEmptyPAType = (KmzSystem::IsPassengerSystem() == true) ? ATTRACTION_STRING : CONSUMPTION_STRING;
			}
			else
			{
				strEmptyPAType = PRODUCTION_STRING;
			}

			CString strMsg(_T(""));
			strMsg.Format(_T("%s 의 %s 정보가 존재 하지 않습니다. "), strPurposeName, strEmptyPAType);

			AfxMessageBox(strMsg);
			return;
		}
		else */
		if(nChildCount == 0)
		{
			CXTPReportRecordItem*    pParentItem = pRecord->GetItem(0);
			CString strPurposeName = pParentItem->GetCaption(NULL);

			CString strAttraction = (KmzSystem::IsPassengerSystem() == true) ? ATTRACTION_STRING : CONSUMPTION_STRING;
			CString strEmptyPAType = _T("");
			strEmptyPAType.Format(_T("%s, %s"), PRODUCTION_STRING, strAttraction);

			CString strMsg(_T(""));
			strMsg.Format(_T("%s의 %s 관련 정보가 존재하지 않습니다."), strPurposeName, strEmptyPAType);

			AfxMessageBox(strMsg);
			return;
		}

		for(int j = 0; j < nChildCount; ++j)
		{
			CXTPReportRecord* pRecPurpose = pChilds->GetAt(j);
			setPurposeOutput(pRecPurpose);
		}
	}

	CXTResizeDialog::OnOK();
}


void KGOutputDesignDlg::setPurposeOutput(CXTPReportRecord* a_pRecord)
{
	ASSERT(NULL != a_pRecord);

	CXTPReportRecordItem* pItem = a_pRecord->GetItem(0);
	KGenerationInfo* pGInfo = (KGenerationInfo*)(pItem->GetItemData());
	ASSERT(NULL != pGInfo);

	CXTPReportRecords*  pChilds = a_pRecord->GetChilds();
	int nCount = pChilds->GetCount();
	for(int i = 0; i < nCount; ++i)
	{
		CXTPReportRecord* pChild = pChilds->GetAt(i);
		CXTPReportRecordItem* pChildItem = pChild->GetItem(0);
		if(pChildItem->IsChecked() == FALSE)
		{
			continue;
		}
		pGInfo->OutputColumnCaption(pChildItem->GetCaption(NULL));

		KIOColumn* pColumn = (KIOColumn*)(pChildItem->GetItemData());
		if(NULL != pColumn)
		{
			pGInfo->OutputColumnName(pColumn->Name());
		}
	}
}


//Step 4
void KGOutputDesignDlg::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CXTResizeDialog::OnCancel();
}
