// SCOutputDesignDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SCAnalysisOutputDesignDlg.h"
#include "afxdialogex.h"

#include "Target.h"
#include "SCAnalysisInfo.h"
//^#include "DBaseConnector.h"
#include "Mode.h"


#include "SCChangeColumnName.h"

#define UM_EXPRESSIONEDIT		WM_USER + 1001

// KSCOutputDesignDlg 대화 상자입니다.
const	int		SCOUTPUT_COLUMN_IDX_MODE		= 0;
const	int		SCOUTPUT_COLUMN_IDX_STARTDATE	= 1;
const	int		SCOUTPUT_COLUMN_IDX_ENDDATE		= 2;
const	int		SCOUTPUT_COLUMN_IDX_REMARKS		= 3;

const	CString SCOUTPUT_COLUMN_NAME_MODE		= _T("Mode");
const	CString SCOUTPUT_COLUMN_NAME_STARTDATE	= _T("Created Date");
const	CString SCOUTPUT_COLUMN_NAME_ENDDATE	= _T("Modified Date");
const	CString SCOUTPUT_COLUMN_NAME_REMARKS	= _T("Remarks");

IMPLEMENT_DYNAMIC(KSCAnalysisOutputDesignDlg, CXTResizeDialog)

KSCAnalysisOutputDesignDlg::KSCAnalysisOutputDesignDlg(KTarget* a_pTarget, std::list<KSCAnalysisInfo*> a_lstSCAnlaysisInfo, CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(KSCAnalysisOutputDesignDlg::IDD, pParent)
	, m_pTaget(a_pTarget)
	, m_lstSCAnlaysisInfo(a_lstSCAnlaysisInfo)
{
}

KSCAnalysisOutputDesignDlg::~KSCAnalysisOutputDesignDlg()
{
}

void KSCAnalysisOutputDesignDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndOutputReport);
}


BEGIN_MESSAGE_MAP(KSCAnalysisOutputDesignDlg, CXTResizeDialog)
	ON_NOTIFY(NM_CLICK, IDC_REPORT, OnReportItemClick)
	ON_NOTIFY(XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT, OnItemButtonClick)
	ON_MESSAGE(UM_EXPRESSIONEDIT, OnExpressionEdit)
	ON_BN_CLICKED(IDOK, &KSCAnalysisOutputDesignDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KSCAnalysisOutputDesignDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KSCOutputDesignDlg 메시지 처리기입니다.


BOOL KSCAnalysisOutputDesignDlg::OnInitDialog()
{
	CXTResizeDialog::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	SetResize(IDC_REPORT, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR1, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	KReportCtrlSetting::Default(m_wndOutputReport, FALSE, FALSE, FALSE);

	InitOutputReportControl();
	UpdateOutputReportRecord();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KSCAnalysisOutputDesignDlg::InitOutputReportControl( void )
{
	/*Add Header Column*/
	CXTPReportColumn* pColumn = NULL;
	
	pColumn = m_wndOutputReport.AddColumn(
		new CXTPReportColumn(SCOUTPUT_COLUMN_IDX_MODE, SCOUTPUT_COLUMN_NAME_MODE, 1));
	pColumn->SetTreeColumn(TRUE);
	pColumn->SetHeaderAlignment(DT_CENTER);

    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("컬럼이름"));
    }

	pColumn = m_wndOutputReport.AddColumn(
		new CXTPReportColumn(SCOUTPUT_COLUMN_IDX_STARTDATE, SCOUTPUT_COLUMN_NAME_STARTDATE, 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("만든 날짜"));
    }

	pColumn = m_wndOutputReport.AddColumn(
		new CXTPReportColumn(SCOUTPUT_COLUMN_IDX_ENDDATE, SCOUTPUT_COLUMN_NAME_ENDDATE, 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("수정한 날짜"));
    }

	pColumn = m_wndOutputReport.AddColumn(
		new CXTPReportColumn(SCOUTPUT_COLUMN_IDX_REMARKS, SCOUTPUT_COLUMN_NAME_REMARKS, 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("비고"));
    }
}

void KSCAnalysisOutputDesignDlg::UpdateOutputReportRecord( void )
{
	CXTPReportRecord*		pCXTPRecord = NULL;
	CXTPReportRecordItem*	pCXTPItem	= NULL;
	
	std::list<KSCAnalysisInfo*>::iterator iter	= m_lstSCAnlaysisInfo.begin();
	std::list<KSCAnalysisInfo*>::iterator end	= m_lstSCAnlaysisInfo.end();

	while (iter != end)
	{
		KSCAnalysisInfo* pSCInfo = *iter;

		if (!pSCInfo->Selected())
		{
			++iter;
			continue;
		}
		pCXTPRecord = m_wndOutputReport.AddRecord(new CXTPReportRecord);

		pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText(_T("")));
		KMode* pMode = pSCInfo->Mode();
		pCXTPItem->SetBackgroundColor(RGB(191,239,254));
		pCXTPItem->SetCaption(pMode->ModeName());
		pCXTPItem->SetItemData((DWORD_PTR)pSCInfo);
		pCXTPItem->SetBold(TRUE);
		
		pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText(_T("")));
		pCXTPItem->SetBackgroundColor(RGB(191,239,254));

		pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText(_T("")));
		pCXTPItem->SetBackgroundColor(RGB(191,239,254));

		pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText(_T("")));
		pCXTPItem->SetBackgroundColor(RGB(191,239,254));

		int nOverwriteCount(0);

		AddOverwriteOutputColumns(pCXTPRecord, pSCInfo, nOverwriteCount);
		AddNewFieldOutputColumn(pCXTPRecord, pSCInfo, nOverwriteCount);
		++iter;
	}
	m_wndOutputReport.Populate();
	m_wndOutputReport.ExpandAll(TRUE);
}

void KSCAnalysisOutputDesignDlg::AddOverwriteOutputColumns( 
	CXTPReportRecord* a_pParent, KSCAnalysisInfo* a_pSCInfo, int& a_nOverwriteCount )
{
	KIOTables*			pIOTables	= m_pTaget->Tables();
	KIOTable*					pTable			= pIOTables->FindTable(TABLE_MODE_OD);
	const KIOColumns*	pColumns		= pTable->Columns();
	KMode*						pMode			= a_pSCInfo->Mode();
	
	KDBaseConPtr spDBaseConnection = m_pTaget->GetDBaseConnection();
	CString strQuery;
	strQuery.Format(
		_T(" SELECT mode_od_column_name, start_date, end_date FROM MODECHOICE_MODEL")
		_T(" WHERE mode_group_code = %d AND detail_mode_id = %d")
		_T(" ORDER BY end_date DESC"),
		pMode->ModeGroup(), pMode->ModeID()
		);
	
	try
	{
		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResult->Next())
		{
			CXTPReportRecord*		pCXTPRecord = a_pParent->GetChilds()->Add(new CXTPReportRecord);
			CXTPReportRecordItem*	pCXTPItem	= NULL;

			/*ColumnName*/
			pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText);
			KIOColumn* pColumn = pColumns->GetColumn(spResult->GetValueString(0));
						
			if (NULL != pColumn)
			{
				pCXTPItem->SetCaption(pColumn->Caption());
				pCXTPItem->SetItemData((DWORD_PTR)pColumn);
				pCXTPItem->HasCheckbox(TRUE);

				if (pColumn == a_pSCInfo->OutputColumn())
				{
					pCXTPItem->SetChecked(TRUE);
				}
			}

			/*Start Date*/
			pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText);
			pCXTPItem->SetCaption(spResult->GetValueString(1));

			/*End Date*/
			pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText);
			pCXTPItem->SetCaption(spResult->GetValueString(2));

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

void KSCAnalysisOutputDesignDlg::AddNewFieldOutputColumn( 
	CXTPReportRecord* a_pParent, KSCAnalysisInfo* a_pSCInfo, int& a_nOverwriteCount )
{
	KMode*		pMode				= a_pSCInfo->Mode();
	CString		strModeName			= pMode->ModeName();
	CString		strNewColumnName	= a_pSCInfo->OutputColumnName();
	KIOColumn*	pSCOutputColumn		= a_pSCInfo->OutputColumn();
	
	if (_T("") == strNewColumnName)
	{
		strNewColumnName.Format(_T("%s_%d"), strModeName, a_nOverwriteCount);

		while (true == IsNameExists(a_pParent, strNewColumnName))
		{
			strNewColumnName.Format(_T("%s_%d"), strModeName, ++a_nOverwriteCount);
		}
	}
	CXTPReportRecord*				pCXTPRecord = a_pParent->GetChilds()->Add(new CXTPReportRecord);
	CXTPReportRecordItem*			pCXTPItem	= pCXTPRecord->AddItem(new CXTPReportRecordItemText);
	CXTPReportRecordItemControl*	pCXTPButton = pCXTPItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
	
	if (NULL != pCXTPButton)
	{
		pCXTPButton->SetAlignment(xtpItemControlUnknown);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pCXTPButton->SetCaption(_T("수정"));
			pCXTPButton->SetSize(CSize(22,0));
		}
		else 
		{
			pCXTPButton->SetCaption(_T("..."));
			pCXTPButton->SetSize(CSize(22,0));
		}
	}
	pCXTPItem->SetCaption(strNewColumnName);
	pCXTPItem->HasCheckbox(TRUE);

	if (NULL == a_pSCInfo->OutputColumn())
		pCXTPItem->SetChecked(TRUE);
	
	/*Start Date*/
	pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText);	
	
	/*End Date*/
	pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText);
	
	/*Remarks*/
	pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText);
	pCXTPItem->SetCaption(_T("New Field"));
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pCXTPItem->SetCaption(_T("새 컬럼 생성"));
    }
}

bool KSCAnalysisOutputDesignDlg::IsNameExists(CXTPReportRecord* pParent, LPCTSTR strName)
{
	CXTPReportRecords* pRecords = pParent->GetChilds();
	int nRecordCount = pRecords->GetCount();
	CXTPReportRecordItem* pItem = 
		pRecords->FindRecordItem(0, nRecordCount-1, 0, 0, 0, 0, strName, 
		xtpReportTextSearchExactPhrase);
	return (NULL != pItem);
}

void KSCAnalysisOutputDesignDlg::OnReportItemClick( NMHDR * pNotifyStruct, LRESULT * result )
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

void KSCAnalysisOutputDesignDlg::OnItemButtonClick( NMHDR * a_pNotifyStruct, LRESULT* a_pResult )
{
	XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) a_pNotifyStruct;
	if(!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
		return;

	CXTPReportRecord* pRecord = pItemNotify->pRow->GetRecord();
	CXTPReportRecordItem* pItem = pRecord->GetItem(0);

	PostMessage(UM_EXPRESSIONEDIT, (WPARAM)pItem);
}

LRESULT KSCAnalysisOutputDesignDlg::OnExpressionEdit( WPARAM a_wParam, LPARAM a_lParam )
{
	CXTPReportColumn *pColum = (CXTPReportColumn*)a_wParam;
	CXTPReportRecordItem* pItem = (CXTPReportRecordItem*)a_wParam;
	CString strName = pItem->GetCaption(pColum);
	KSCChangeColumnName dlg(strName);
	dlg.DoModal();
	pItem->SetCaption(dlg.m_strNewName);
	return 0;
}


void KSCAnalysisOutputDesignDlg::OnBnClickedOk()
{
	CXTPReportRecords* pCXTPRecords = m_wndOutputReport.GetRecords();
	int nRecordCount = pCXTPRecords->GetCount();

	for (int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord*		pCXTPRecord		= pCXTPRecords->GetAt(i);
		CXTPReportRecordItem*	pCXTPItem		= pCXTPRecord->GetItem(SCOUTPUT_COLUMN_IDX_MODE);
		CXTPReportRecords*		pOutputRecords	= pCXTPRecord->GetChilds();
		
		KSCAnalysisInfo*		pSCAnalysisInfo = (KSCAnalysisInfo*)pCXTPItem->GetItemData();

		int nOutputCount = pOutputRecords->GetCount();

		for (int j = 0; j < nOutputCount; ++j)
		{
			CXTPReportRecord* pOutputRecord = pOutputRecords->GetAt(j);
			CXTPReportRecordItem* pOutputRecordItem = 
				pOutputRecord->GetItem(SCOUTPUT_COLUMN_IDX_MODE);

			if (pOutputRecordItem->IsChecked() == FALSE)
				continue;
			KIOColumn* pOutputColumn = (KIOColumn*)pOutputRecordItem->GetItemData();

			if (NULL == pOutputColumn)
			{
				CString strOutputColumnName = pOutputRecordItem->GetCaption(NULL);
				pSCAnalysisInfo->OutputColumnName(strOutputColumnName);
				pSCAnalysisInfo->OutputColumn(NULL);
			}
			else
			{
				pSCAnalysisInfo->OutputColumnName(_T(""));
				pSCAnalysisInfo->OutputColumn(pOutputColumn);
			}
		}
	}
	CXTResizeDialog::OnOK();
}


void KSCAnalysisOutputDesignDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CXTResizeDialog::OnCancel();
}
