// TimeODOutputDesignDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TimeODOutputDesignDlg.h"
#include "Target.h"
//^^ #include "IOTable.h"
//^^ #include "IOColumns.h"
//^^ #include "IOColumn.h"
//^#include "DBaseConnector.h"
#include "KGChangeFieldName.h"

#define _0_COLUMN_OUTPUT_MODE		0
#define _1_COLUMN_OUTPUT_STARTDATE  1
#define _2_COLUMN_OUTPUT_ENDDATE	2
#define _3_COLUMN_OUTPUT_REMARK		3

#define UM_EXPRESSIONEDIT		WM_USER + 1001

// KTimeODOutputDesignDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTimeODOutputDesignDlg, CXTPResizeDialog)

KTimeODOutputDesignDlg::KTimeODOutputDesignDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(KTimeODOutputDesignDlg::IDD, pParent)
{

}

KTimeODOutputDesignDlg::~KTimeODOutputDesignDlg()
{
}

void KTimeODOutputDesignDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,  m_ReportCtrl);
}


BEGIN_MESSAGE_MAP(KTimeODOutputDesignDlg, CXTPResizeDialog)
	ON_BN_CLICKED(IDOK, &KTimeODOutputDesignDlg::OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_REPORT, OnReportItemClick)
	ON_NOTIFY(XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT, OnItemButtonClick)
	ON_MESSAGE(UM_EXPRESSIONEDIT, OnExpressionEdit)
END_MESSAGE_MAP()


// KTimeODOutputDesignDlg 메시지 처리기입니다.

BOOL KTimeODOutputDesignDlg::OnInitDialog()
{
	CXTPResizeDialog::OnInitDialog();    

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	InitReportHeader();
	InitReportData();
	m_ReportCtrl.Populate();

	SetResizeComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KTimeODOutputDesignDlg::SetResizeComponent()
{
	SetResize(IDC_REPORT,             SZ_TOP_LEFT,      SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC,             SZ_BOTTOM_LEFT,   SZ_BOTTOM_RIGHT);
	SetResize(IDOK,                   SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,               SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
}


void KTimeODOutputDesignDlg::InitReportHeader()
{
	// Report Header 설정
	KReportCtrlSetting::Default(m_ReportCtrl);
// 	m_ReportCtrl.GetReportHeader()->AllowColumnRemove(FALSE);
// 	m_ReportCtrl.GetReportHeader()->AllowColumnSort(FALSE);
// 	m_ReportCtrl.GetReportHeader()->AllowColumnReorder(FALSE);
// 	m_ReportCtrl.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);
// 
// 	// Report tree 설정
// 	m_ReportCtrl.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureSolid;
// 	//m_ReportCtrl.GetPaintManager()->SetGridStyle(FALSE, xtpReportGridSolid);
// 	m_ReportCtrl.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);
	
	// Add Columns
	CXTPReportColumn* pColumn = NULL;
	pColumn = m_ReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_OUTPUT_MODE,      _T("Mode"), 150));
	pColumn->SetTreeColumn(TRUE);
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("컬럼 이름"));
    }	

	pColumn = m_ReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_OUTPUT_STARTDATE, _T("Created Date"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("만든 날짜"));
    }	

	pColumn = m_ReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_OUTPUT_ENDDATE,   _T("Modified Date"),   100));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("수정한 날짜"));
    }	

	pColumn = m_ReportCtrl.AddColumn(new CXTPReportColumn(_3_COLUMN_OUTPUT_REMARK,    _T("Remarks"),    80));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("비고"));
    }	
	// Report control 설정
	m_ReportCtrl.AllowEdit(FALSE);
	m_ReportCtrl.EditOnClick(FALSE);
	m_ReportCtrl.SetMultipleSelection(FALSE);
	m_ReportCtrl.SetTreeIndent(10);
}


void KTimeODOutputDesignDlg::InitReportData()
{
	for(size_t i= 0; i< m_vecOutputModeInfo.size(); i++)
	{
		TOutputModeInfo     &oTOutputModeInfo = m_vecOutputModeInfo[i];

		TModeBase            &oTMode       = oTOutputModeInfo.oTMode;
		KIOColumn*           pModeODColumn = oTOutputModeInfo.pModeODColumn;
		int                  &nModeGroup   = oTOutputModeInfo.nModeGroup;
		int                  &nModeID      = oTOutputModeInfo.nDetailModeID;

		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		CString strSQL;
		strSQL.Format(
			_T(" SELECT mode_group_code, detail_mode_id ")
			_T(" FROM modechoice_model ")
			_T(" WHERE mode_od_column_name = '%s' "),
				pModeODColumn->Name()
		);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		if(nullptr == spResult)
		{
			continue;
		}

		while( spResult->Next() )
		{
			nModeGroup = spResult->GetValueInt(0);
			nModeID    = spResult->GetValueInt(1);
		}

		CXTPReportRecord*     pRecord      = m_ReportCtrl.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem        = pRecord->AddItem(new CXTPReportRecordItemText);
		                                     pRecord->AddItem(new CXTPReportRecordItemText);
											 pRecord->AddItem(new CXTPReportRecordItemText);
											 pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetCaption(oTMode.strModeName);
		pItem->SetItemData((DWORD_PTR) &oTOutputModeInfo);

		AddOutputColumns(pRecord, oTOutputModeInfo);
	}

	m_ReportCtrl.Populate();
	m_ReportCtrl.ExpandAll(TRUE);
}


void KTimeODOutputDesignDlg::AddOutputColumns(CXTPReportRecord* a_pParent, TOutputModeInfo &a_oTOutModeInfo)
{
	// 새 column 항목 추가
	CXTPReportRecord*     pRecord = a_pParent->GetChilds()->Add(new CXTPReportRecord);
	CXTPReportRecordItem* pItem   = NULL;
	
	pItem               = pRecord->AddItem(new CXTPReportRecordItemText);

	CString strNewField        = a_oTOutModeInfo.oTMode.strModeName;
	CString strExistOutputName = a_oTOutModeInfo.strOutputName;

	if(strExistOutputName.CompareNoCase(_T("")) == 0)
	{
		//column 에 대한 새 ID 할당
		CString strTempName(_T(""));
		strTempName.Format(_T("%s_%s"), _T("New"), strNewField);
		/*strTempName.Format(_T("%s_%d"), strNewField, nIndex);
		while(true == isNameExists(pParent, strTempName))
		{
			strTempName.Format(_T("%s_%d"), strNewField, ++nIndex);
		}*/

		strNewField = strTempName;
	}
	else
	{
		strNewField = strExistOutputName;
	}

	CXTPReportRecordItemControl* pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
	if(NULL != pButton)
	{
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pButton->SetAlignment(xtpItemControlUnknown);
            pButton->SetCaption(_T("수정"));
            pButton->SetSize(CSize(25, 0));
        }	
        else {
            pButton->SetAlignment(xtpItemControlUnknown);
            pButton->SetCaption(_T("Edit"));
            pButton->SetSize(CSize(22, 0));
        }		
	}

	pItem->SetCaption(strNewField);
	pItem->HasCheckbox(TRUE);
	pItem->SetChecked(TRUE);

	pItem = pRecord->AddItem(new CXTPReportRecordItemText);	// start date
	pItem = pRecord->AddItem(new CXTPReportRecordItemText);	// end date
	pItem = pRecord->AddItem(new CXTPReportRecordItemText);	// remark
	pItem->SetCaption(_T("New Field"));
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pItem->SetCaption(_T("새 컬럼 생성"));
    }
}


bool KTimeODOutputDesignDlg::IsNameExists(CXTPReportRecord* a_pParent, LPCTSTR a_strName)
{
	CXTPReportRecords* pRecords = a_pParent->GetChilds();
	int nRecordCount = pRecords->GetCount();
	CXTPReportRecordItem* pItem = 
		pRecords->FindRecordItem(0, nRecordCount-1, 0, 0, 0, 0, a_strName, 
		xtpReportTextSearchExactPhrase);
	return (NULL != pItem);
}


void KTimeODOutputDesignDlg::OnBnClickedOk()
{
	CXTPReportRecords*    pRecords = m_ReportCtrl.GetRecords();
	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;

	for(int i= 0; i< pRecords->GetCount(); i++)
	{
		pRecord = pRecords->GetAt(i);
		if( false == ApplyOutputField(pRecord) )
		{
			return;
		}
	}

	/*CXTPReportRecords*    pRecords = m_ReportCtrl.GetRecords();
	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;

	int nSelectedCount = 0;

	for( int i= 0; i< pRecords->GetCount(); i++ )
	{
		pRecord = pRecords->GetAt(i);
		pItem   = pRecord->GetItem(_0_COLUMN_CHECKOD);

		TInputModeInfo* pTInputModeInfo = (TInputModeInfo* )pItem->GetItemData();
		pTInputModeInfo->bSelected = pItem->IsChecked();

		if(TRUE == pItem->IsChecked())
		{
			nSelectedCount++;
		}

		pItem = pRecord->GetItem(_1_COLUMN_FIELDNAME);
		KIOColumn* pModeODColumn = (KIOColumn*)pItem->GetItemData();

		pItem = pRecord->GetItem(_2_COLUMN_TIME);
		KIOColumn* pParaODColumn = (KIOColumn*)pItem->GetItemData();

		pTInputModeInfo->pModeODColumn       = pModeODColumn;
		pTInputModeInfo->pParaODColumn       = pParaODColumn;
	}

	if( nSelectedCount < 1)
	{
		AfxMessageBox(_T("선택한 기종점 통행 데이터가 없습니다."));
		return;
	}*/

	CXTPResizeDialog::OnOK();
}


bool KTimeODOutputDesignDlg::ApplyOutputField(CXTPReportRecord* a_pModeRecord)
{
	CXTPReportRecordItem* pModeItem   = a_pModeRecord->GetItem(_0_COLUMN_OUTPUT_MODE);
	TOutputModeInfo*      pOutputMode = (TOutputModeInfo*)(pModeItem->GetItemData());

	/*std::map<KMode*, KMOutputInfo*>::iterator itOutput = m_OutputMap.find(pMode);
	ASSERT(m_OutputMap.end() != itOutput);
	KMOutputInfo* pOutput = itOutput->second;*/

	CXTPReportRecords*  pFieldRecords = a_pModeRecord->GetChilds();
	int nFieldCount  =  pFieldRecords->GetCount();

	if( nFieldCount< 1 )
	{
		CString strInputDataCheckMsg = _T("");
		strInputDataCheckMsg.Format(_T("\'%s\'에 대한 컬럼 정보가 존재하지 않습니다."), pOutputMode->oTMode.strModeName);
		AfxMessageBox(strInputDataCheckMsg);
		return false;
	}

	for(int i = 0; i < nFieldCount; ++i)
	{
		CXTPReportRecord* pFieldRecord = pFieldRecords->GetAt(i);
		CXTPReportRecordItem* pFieldItem = pFieldRecord->GetItem(_0_COLUMN_OUTPUT_MODE);
		if(pFieldItem->IsChecked() == FALSE)
		{
			continue;
		}

		pOutputMode->strOutputName = pFieldItem->GetCaption(NULL); 

		/*pOutput->FieldCaption(pFieldItem->GetCaption(NULL));
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
		}*/
	}

	return true;
}


void KTimeODOutputDesignDlg::OnReportItemClick(NMHDR * pNotifyStruct, LRESULT * result)
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
					CXTPReportRecordItem* pItem = pRecord->GetItem(_0_COLUMN_OUTPUT_MODE);
					pItem->SetChecked(FALSE);
				}
			}
		}
	}
}


void KTimeODOutputDesignDlg::OnItemButtonClick(NMHDR * a_pNotifyStruct, LRESULT* a_pResult)
{
	XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) a_pNotifyStruct;
	if(!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
		return;

	CXTPReportRecord*     pRecord = pItemNotify->pRow->GetRecord();
	CXTPReportRecordItem* pItem   = pRecord->GetItem(_0_COLUMN_OUTPUT_MODE);

	PostMessage(UM_EXPRESSIONEDIT, (WPARAM)pItem);
}


LRESULT KTimeODOutputDesignDlg::OnExpressionEdit (WPARAM a_wParam, LPARAM a_lParam)
{
	CXTPReportColumn *pColumn = (CXTPReportColumn*)a_wParam;
	CXTPReportRecordItem* pItem = (CXTPReportRecordItem*)a_wParam;
	CString strName = pItem->GetCaption(pColumn);
	KGChangeFieldName dlg(strName);
	dlg.DoModal();
	CString strNewFieldName = dlg.m_strNewName;
	pItem->SetCaption(strNewFieldName);
	return 0;

}