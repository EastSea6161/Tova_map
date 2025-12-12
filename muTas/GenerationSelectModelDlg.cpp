// GenerationSelectModelDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GenerationSelectModelDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "GenerationModelInfo.h"
#include "PAPurpose.h"
#include "KExRecordItem.h"

enum
{
	_0_COLUMN_PURPOSE_NAME = 0,	
	_1_COLUMN_SELECTED,
	_2_COLUMN_PATYPE,    
	_3_COLUMN_ZONEGROUP,    
	_4_COLUMN_UNIT,			
	_5_COLUMN_REGRESSION,
	_6_COLUMN_CROSSCLASSIFICATION 
};

// KGenerationSelectModelDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KGenerationSelectModelDlg, KResizeDialogEx)
	
KGenerationSelectModelDlg::KGenerationSelectModelDlg(KTarget* a_pTarget, std::map<int, KGenerationModelInfo>& a_mapGenerationModelInfo, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KGenerationSelectModelDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_mapGenerationModelInfo(a_mapGenerationModelInfo)
{

}

KGenerationSelectModelDlg::~KGenerationSelectModelDlg()
{
}

void KGenerationSelectModelDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
}


BEGIN_MESSAGE_MAP(KGenerationSelectModelDlg, KResizeDialogEx)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT, OnReportCheckItem)
	ON_BN_CLICKED(IDOK, &KGenerationSelectModelDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KGenerationSelectModelDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


BOOL KGenerationSelectModelDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndReportCtrl);

	InitReportHeader();
	UpdateReportData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KGenerationSelectModelDlg::InitReportHeader( void )
{
	try
	{
		CXTPReportColumn* pColumn = NULL;

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_PURPOSE_NAME, _T(""), 15));
		pColumn->SetHeaderAlignment(DT_CENTER);

		m_wndReportCtrl.GetColumns()->GetGroupsOrder()->Add(pColumn);
		pColumn->SetVisible(FALSE);

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_SELECTED, _T("Select"), 15));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(xtpColumnIconCenter);

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_PATYPE, _T("P/A"), 17));
		pColumn->SetHeaderAlignment(DT_CENTER);

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_3_COLUMN_ZONEGROUP, _T("Zone Group"), 17));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(xtpColumnIconCenter);

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_4_COLUMN_UNIT, _T("Unit"), 17));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(xtpColumnIconCenter);

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_5_COLUMN_REGRESSION, _T("Regression"), 17));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(xtpColumnIconCenter);

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_6_COLUMN_CROSSCLASSIFICATION, _T("Cross-Classification"), 17));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(xtpColumnIconCenter);

		CXTPReportHeader* pKHeader = m_wndReportCtrl.GetReportHeader();
		pKHeader->AllowColumnRemove(FALSE);
		pKHeader->AllowColumnReorder(FALSE);
		pKHeader->AllowColumnSort(FALSE);

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

void KGenerationSelectModelDlg::UpdateReportData( void )
{
	try
	{
		CXTPReportRecord*		pRecord = nullptr;
		CXTPReportRecordItem*	pItem	= nullptr;

		AutoType iter = m_mapGenerationModelInfo.begin();
		AutoType end  = m_mapGenerationModelInfo.end();
		int nIndex = 0;	
		while(iter != end)
		{
			KGenerationModelInfo oInfo = iter->second;

			pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
			pItem	= pRecord->AddItem(new KExRecordItem2GroupTextNo(oInfo.Purpose()->PurposeName(), nIndex));
			pItem	= pRecord->AddItem(new CXTPReportRecordItem());
			pItem->SetItemData((DWORD_PTR)iter->first);
			pItem->HasCheckbox(TRUE);

			BOOL BCheckData(FALSE);
			if ( true == oInfo.UseGeneration())
			{
				BCheckData = TRUE;
			}
			else
			{
				BCheckData = FALSE;
			}
			pItem->SetChecked(BCheckData);

			KPAPurpose* pPurpose = oInfo.Purpose();

			if (pPurpose->PAType() == KEMPATypeProduction)
			{
				pItem	= pRecord->AddItem(new CXTPReportRecordItemText(PRODUCTION_STRING));
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
			}
			else
			{
				pItem	= pRecord->AddItem(new CXTPReportRecordItemText(ATTRACTION_STRING));
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
			}

			pItem = pRecord->AddItem(new CXTPReportRecordItemText());
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(oInfo.UseZoneGroup());
			pItem->SetEditable(BCheckData);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText());
			pItem->HasCheckbox(TRUE);
			if (KEMGenerationUnit == oInfo.GenerationModel())
			{
				pItem->SetChecked(TRUE);
			}
			else
			{
				pItem->SetChecked(FALSE);
			}
			pItem->SetEditable(BCheckData);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText());
			pItem->HasCheckbox(TRUE);
			if (KEMGenerationRegression == oInfo.GenerationModel())
			{
				pItem->SetChecked(TRUE);
			}
			else
			{
				pItem->SetChecked(FALSE);
			}
			pItem->SetEditable(BCheckData);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText());
			pItem->HasCheckbox(TRUE);
			if (KEMGenerationCrossClassification == oInfo.GenerationModel())
			{
				pItem->SetChecked(TRUE);
			}
			else
			{
				pItem->SetChecked(FALSE);
			}
			pItem->SetEditable(BCheckData);
			nIndex++;
			++iter;
		}

		m_wndReportCtrl.Populate();
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

void KGenerationSelectModelDlg::OnReportCheckItem( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
		ASSERT(NULL != pItemNotify);

		int nIndex = pItemNotify->pColumn->GetItemIndex();

		if(nIndex >= _3_COLUMN_ZONEGROUP && nIndex<= _6_COLUMN_CROSSCLASSIFICATION)
		{
			if( FALSE == pItemNotify->pRow->GetRecord()->GetItem(_1_COLUMN_SELECTED)->IsChecked() )
			{
				return;
			}
		}

		if( _1_COLUMN_SELECTED == nIndex)
		{
			if( TRUE == pItemNotify->pRow->GetRecord()->GetItem(_1_COLUMN_SELECTED)->IsChecked() )
			{
				pItemNotify->pRow->GetRecord()->GetItem(_3_COLUMN_ZONEGROUP)->SetEditable(TRUE);
				pItemNotify->pRow->GetRecord()->GetItem(_4_COLUMN_UNIT)->SetEditable(TRUE);
				pItemNotify->pRow->GetRecord()->GetItem(_5_COLUMN_REGRESSION)->SetEditable(TRUE);
				pItemNotify->pRow->GetRecord()->GetItem(_6_COLUMN_CROSSCLASSIFICATION)->SetEditable(TRUE);
			}
			else
			{
				pItemNotify->pRow->GetRecord()->GetItem(_3_COLUMN_ZONEGROUP)->SetEditable(FALSE);
				pItemNotify->pRow->GetRecord()->GetItem(_4_COLUMN_UNIT)->SetEditable(FALSE);
				pItemNotify->pRow->GetRecord()->GetItem(_5_COLUMN_REGRESSION)->SetEditable(FALSE);
				pItemNotify->pRow->GetRecord()->GetItem(_6_COLUMN_CROSSCLASSIFICATION)->SetEditable(FALSE);
			}
		}

		if(_4_COLUMN_UNIT == nIndex)
		{
			pItemNotify->pItem->SetChecked(TRUE);
			pItemNotify->pRow->GetRecord()->GetItem(_5_COLUMN_REGRESSION)->SetChecked(FALSE);
			pItemNotify->pRow->GetRecord()->GetItem(_6_COLUMN_CROSSCLASSIFICATION)->SetChecked(FALSE);
		}
		else if(_5_COLUMN_REGRESSION == nIndex)
		{
			pItemNotify->pItem->SetChecked(TRUE);
			pItemNotify->pRow->GetRecord()->GetItem(_4_COLUMN_UNIT)->SetChecked(FALSE);
			pItemNotify->pRow->GetRecord()->GetItem(_6_COLUMN_CROSSCLASSIFICATION)->SetChecked(FALSE);
		}
		else if(_6_COLUMN_CROSSCLASSIFICATION == nIndex)
		{
			pItemNotify->pItem->SetChecked(TRUE);
			pItemNotify->pRow->GetRecord()->GetItem(_4_COLUMN_UNIT)->SetChecked(FALSE);
			pItemNotify->pRow->GetRecord()->GetItem(_5_COLUMN_REGRESSION)->SetChecked(FALSE);
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


void KGenerationSelectModelDlg::OnBnClickedOk()
{
	try
	{
		if (CheckUseData() == false)
		{
			AfxMessageBox(_T("선택한 정보가 없습니다."));
			return;
		}

		CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i < nRecordCount; i++)
		{
			CXTPReportRecord*		pRecord = pRecords->GetAt(i);
			CXTPReportRecordItem*	pItem	= pRecord->GetItem(1);
			int nCode = (int)pItem->GetItemData();
			AutoType find = m_mapGenerationModelInfo.find(nCode);
			AutoType end  = m_mapGenerationModelInfo.end();
			if (find != end)
			{
				KGenerationModelInfo& oInfo = find->second;
				if (pItem->IsChecked()== TRUE)
				{
					oInfo.UseGeneration(true);
				}
				else
				{
					oInfo.UseGeneration(false);
				}

				pItem = pRecord->GetItem(3);
				if (pItem->IsChecked() == TRUE)
				{
					oInfo.UseZoneGroup(true); 
				}
				else
				{
					oInfo.UseZoneGroup(false);
				}

				pItem = pRecord->GetItem(4);
				if (pItem->IsChecked() == TRUE)
				{
					oInfo.GenerationModel(KEMGenerationUnit); 
				}

				pItem = pRecord->GetItem(5);
				if (pItem->IsChecked() == TRUE)
				{
					oInfo.GenerationModel(KEMGenerationRegression); 
				}

				pItem = pRecord->GetItem(6);
				if (pItem->IsChecked() == TRUE)
				{
					oInfo.GenerationModel(KEMGenerationCrossClassification); 
				}
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("데이터 설정중 오류"));
	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("데이터 설정중 오류"));
	}

	KResizeDialogEx::OnOK();
}


void KGenerationSelectModelDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

bool KGenerationSelectModelDlg::CheckUseData( void )
{
	try
	{
		CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i < nRecordCount; i++)
		{
			CXTPReportRecord*		pRecord = pRecords->GetAt(i);
			CXTPReportRecordItem*	pItem	= pRecord->GetItem(1);
			if (TRUE == pItem->IsChecked())
			{
				return true;
			}		
		}
	}
	catch(...)
	{
		TxLogDebugException();
	}
	return false;
}
