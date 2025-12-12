/**
 * @file GSelectModelDlg.cpp
 * @brief KGSelectModelDlg 선언 파일
 * @author 
 * @date 2011.05.03
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "GSelectModelDlg.h"
#include "GGroupExpressionContainer.h"
#include "GSimpleExpressionContainer.h"
#include "Purpose.h"
#include "KExRecordItem.h"
#include "GenerationInfoList.h"


/* Report item index */
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



// KGSelectModelDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KGSelectModelDlg, CXTResizeDialog)

KGSelectModelDlg::KGSelectModelDlg(KEMGHaveDBMode a_KEMGUSeDBMode, CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(KGSelectModelDlg::IDD, pParent)
{
	m_KEMGUSeDBMode = a_KEMGUSeDBMode;
}


KGSelectModelDlg::~KGSelectModelDlg()
{
}


void KGSelectModelDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
}


BEGIN_MESSAGE_MAP(KGSelectModelDlg, CXTResizeDialog)
	ON_NOTIFY(NM_CLICK, IDC_REPORT, OnReportItemClick)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT, OnReportCheckItem)
	ON_BN_CLICKED(IDCANCEL, &KGSelectModelDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KGSelectModelDlg 메시지 처리기입니다.

BOOL KGSelectModelDlg::OnInitDialog()
{
	CXTResizeDialog::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndReportCtrl, FALSE, FALSE);

	initReportColumn();
	switch (m_KEMGUSeDBMode)
	{
	case KEMHaveSed_All: 
		initReportRecordAll();
		break;

	case KEMHaveSed_OnlyPop:
		initReportRecordSedPop();
		break;

	case KEMHaveSed_OnlyVector:
		initReportRecordSedVector();
		break;

	}    

// 	m_wndReportCtrl.SetGridStyle(TRUE, xtpReportGridSolid);
// 	m_wndReportCtrl.SetGridStyle(FALSE, xtpReportGridSolid);
    m_wndReportCtrl.SetGroupRowsBold(TRUE);    
    m_wndReportCtrl.Populate();   

	SetResize(IDC_REPORT, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR1, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
}

//Step 1
void KGSelectModelDlg::initReportColumn(void)
{
	CXTPReportColumn* pKColumn = NULL;

	pKColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_PURPOSE_NAME, _T("Purpose")/*목적*/, 15));
	pKColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pKColumn->SetCaption(_T("목적"));
    }

	m_wndReportCtrl.GetColumns()->GetGroupsOrder()->Add(pKColumn);
	pKColumn->SetVisible(FALSE);

	pKColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_SELECTED, _T("Select"), 15));
	pKColumn->SetHeaderAlignment(DT_CENTER);
	pKColumn->SetAlignment(xtpColumnIconCenter);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pKColumn->SetCaption(_T("선택"));
    }

	pKColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_PATYPE, _T("P/A"), 17));
	pKColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pKColumn->SetCaption(_T("모형구분"));
    }

	pKColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_3_COLUMN_ZONEGROUP, _T("Zone Group"), 17));
	pKColumn->SetHeaderAlignment(DT_CENTER);
	pKColumn->SetAlignment(xtpColumnIconCenter);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pKColumn->SetCaption(_T("존 그룹별 적용"));
    }

	pKColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_4_COLUMN_UNIT, _T("Unit"), 17));
	pKColumn->SetHeaderAlignment(DT_CENTER);
	pKColumn->SetAlignment(xtpColumnIconCenter);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pKColumn->SetCaption(_T("원단위법"));
    }

	pKColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_5_COLUMN_REGRESSION, _T("Regression"), 17));
	pKColumn->SetHeaderAlignment(DT_CENTER);
	pKColumn->SetAlignment(xtpColumnIconCenter);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pKColumn->SetCaption(_T("회귀분석법"));
    }

	pKColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_6_COLUMN_CROSSCLASSIFICATION, _T("Cross-Classification"), 17));
	pKColumn->SetHeaderAlignment(DT_CENTER);
	pKColumn->SetAlignment(xtpColumnIconCenter);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pKColumn->SetCaption(_T("카테고리분석법"));
    }
}


// void KGSelectModelDlg::initReportColumn(void)
// {
// 	CXTPReportColumn* pKColumn = NULL;
// 
// 	pKColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_PURPOSE_NAME, _T("목적")/*목적*/, 50));
// 	pKColumn->SetHeaderAlignment(DT_CENTER);
// 
// 	m_wndReportCtrl.GetColumns()->GetGroupsOrder()->Add(pKColumn);
// 	pKColumn->SetVisible(FALSE);
// 
// 	pKColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_SELECTED, _T("Select"), 80));
// 	pKColumn->SetHeaderAlignment(DT_CENTER);
// 	pKColumn->SetAlignment(xtpColumnIconCenter);
// 
// 	pKColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_PATYPE, _T("P/A"), 80));
// 	pKColumn->SetHeaderAlignment(DT_CENTER);
// 
// 	pKColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_3_COLUMN_ZONEGROUP, _T("Zone Group"), 50));
// 	pKColumn->SetHeaderAlignment(DT_CENTER);
// 	pKColumn->SetAlignment(xtpColumnIconCenter);
// 
// 	pKColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_4_COLUMN_UNIT, _T("Unit"), 80));
// 	pKColumn->SetHeaderAlignment(DT_CENTER);
// 	pKColumn->SetAlignment(xtpColumnIconCenter);
// 
// 	pKColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_5_COLUMN_REGRESSION, _T("Regression"), 80));
// 	pKColumn->SetHeaderAlignment(DT_CENTER);
// 	pKColumn->SetAlignment(xtpColumnIconCenter);
// 
// 	pKColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_6_COLUMN_CROSSCLASSIFICATION, _T("Cross-Classification"), 80));
// 	pKColumn->SetHeaderAlignment(DT_CENTER);
// 	pKColumn->SetAlignment(xtpColumnIconCenter);
// 
// 	CXTPReportHeader* pKHeader = m_wndReportCtrl.GetReportHeader();
// 	pKHeader->AllowColumnRemove(FALSE);
// 	pKHeader->AllowColumnReorder(FALSE);
// 	pKHeader->AllowColumnSort(FALSE);
// }


void KGSelectModelDlg::initReportRecordAll(void)
{
	TxLogDebugStartMsg();
	CXTPReportRecord* pKRecord = NULL;
	CXTPReportRecordItem* pKItem = NULL;
	BOOL bUseGeneration = FALSE;

	std::list<KGenerationInfo*>::iterator itInfo, itEnd = m_lstGInfoList.end();
	int nNo = 0;
	for(itInfo = m_lstGInfoList.begin(); itInfo != itEnd; ++itInfo)
	{
		pKRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		KGenerationInfo* pKInfo = *itInfo;
		// 0 : 
		pKItem = pKRecord->AddItem(new KExRecordItem2GroupTextNo(pKInfo->GetName(), nNo));
		nNo ++;

		pKItem->SetCaption(pKInfo->GetName());
		pKItem->SetItemData((DWORD_PTR)pKInfo);        

		pKItem = pKRecord->AddItem(new CXTPReportRecordItemText());
		pKItem->HasCheckbox(TRUE);
		if(pKInfo->UseGeneration())
		{
			bUseGeneration = TRUE;
		}
		else
		{
			bUseGeneration = FALSE;
		}
		pKItem->SetChecked(bUseGeneration);

		if(pKInfo->GetPAType() == KEMPATypeProduction)
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pKItem = pKRecord->AddItem(new CXTPReportRecordItemText(_T("유출모형")));
			}
			else {
				pKItem = pKRecord->AddItem(new CXTPReportRecordItemText(PRODUCTION_STRING));
			}
		}
		else
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pKItem = pKRecord->AddItem(new CXTPReportRecordItemText(_T("유입모형")));
			}
			else {
				pKItem = pKRecord->AddItem(new CXTPReportRecordItemText(ATTRACTION_STRING));
			}
		}
		pKItem->SetEditable(bUseGeneration);
		pKItem->SetAlignment(DT_CENTER);

		// Zone group 과 model 종류 설정
		KGBaseExpressionContainer* pKExpressionContainer = pKInfo->ExpressionContainer();
		bool bGroup = false;
		KEMGenerationModel emGModel = pKInfo->GenerationModel();

		if(NULL != pKExpressionContainer)
		{
			if(pKExpressionContainer->ContainerType() == KEMGExpressionContainerZoneGroup)
			{
				bGroup = true;
			}
			emGModel = pKExpressionContainer->GenerationModel();
		}
		// 2. Zone group
		pKItem = pKRecord->AddItem(new CXTPReportRecordItemText());
		pKItem->HasCheckbox(TRUE);
		pKItem->SetChecked(pKInfo->ZoneGroup());
		pKItem->SetEditable(bUseGeneration);

		// 3. Unit model
		pKItem = pKRecord->AddItem(new CXTPReportRecordItemText());
		pKItem->HasCheckbox(TRUE);
		if(KEMGenerationUnit == emGModel)
		{
			pKItem->SetChecked(TRUE);
		}
		pKItem->SetEditable(bUseGeneration);

		// 4. Regression model
		pKItem = pKRecord->AddItem(new CXTPReportRecordItemText());
		pKItem->HasCheckbox(TRUE);
		if(KEMGenerationRegression == emGModel)
		{
			pKItem->SetChecked(TRUE);
		}
		pKItem->SetEditable(bUseGeneration);

		// 5.Cross-classfication model
		pKItem = pKRecord->AddItem(new CXTPReportRecordItemText());
		pKItem->HasCheckbox(TRUE);
		if(KEMGenerationCrossClassification == emGModel)
		{
			pKItem->SetChecked(TRUE);
		}
		pKItem->SetEditable(bUseGeneration);
	}
	TxLogDebugEndMsg();
}


void KGSelectModelDlg::initReportRecordSedPop(void)
{
	TxLogDebugStartMsg();
	CXTPReportRecord* pKRecord = NULL;
	CXTPReportRecordItem* pKItem = NULL;
	BOOL bUseGeneration = FALSE;

	std::list<KGenerationInfo*>::iterator itInfo, itEnd = m_lstGInfoList.end();
	int nNo = 0;
	for(itInfo = m_lstGInfoList.begin(); itInfo != itEnd; ++itInfo)
	{
		pKRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		KGenerationInfo* pKInfo = *itInfo;
		// 0 : 
		pKItem = pKRecord->AddItem(new KExRecordItem2GroupTextNo(pKInfo->GetName(), nNo));
		nNo ++;

		pKItem->SetCaption(pKInfo->GetName());
		pKItem->SetItemData((DWORD_PTR)pKInfo);        

		pKItem = pKRecord->AddItem(new CXTPReportRecordItemText());
		pKItem->HasCheckbox(TRUE);
		if(pKInfo->UseGeneration())
		{
			bUseGeneration = TRUE;
		}
		else
		{
			bUseGeneration = FALSE;
		}
		pKItem->SetChecked(bUseGeneration);

		if(pKInfo->GetPAType() == KEMPATypeProduction)
		{
            if (KmzSystem::GetLanguage() == KEMKorea) {
                pKRecord->AddItem(new CXTPReportRecordItemText(_T("유출모형")));
            }
            else {
                pKRecord->AddItem(new CXTPReportRecordItemText(PRODUCTION_STRING));
            }			
		}
		else
		{
            if (KmzSystem::GetLanguage() == KEMKorea) {
                pKRecord->AddItem(new CXTPReportRecordItemText(_T("유입모형")));
            }
            else {
                pKRecord->AddItem(new CXTPReportRecordItemText(ATTRACTION_STRING));
            }						
		}
		pKItem->SetEditable(bUseGeneration);

		// Zone group 과 model 종류 설정
		KGBaseExpressionContainer* pKExpressionContainer = pKInfo->ExpressionContainer();
		bool bGroup = false;
		KEMGenerationModel emGModel = pKInfo->GenerationModel();

		if(NULL != pKExpressionContainer)
		{
			if(pKExpressionContainer->ContainerType() == KEMGExpressionContainerZoneGroup)
			{
				bGroup = true;
			}
			emGModel = pKExpressionContainer->GenerationModel();
		}
		// 2. Zone group
		pKItem = pKRecord->AddItem(new CXTPReportRecordItemText());
		pKItem->HasCheckbox(TRUE);
		pKItem->SetChecked(pKInfo->ZoneGroup());
		pKItem->SetEditable(bUseGeneration);

		// 3. Unit model
		pKItem = pKRecord->AddItem(new CXTPReportRecordItemText());
		pKItem->SetAlignment(DT_CENTER);
		pKItem->SetCaption(_T("-"));
		if(KEMGenerationUnit == emGModel)
		{
			pKItem->SetChecked(TRUE);
		}
		pKItem->SetEditable(bUseGeneration);

		// 4. Regression model
		pKItem = pKRecord->AddItem(new CXTPReportRecordItemText());
		pKItem->SetAlignment(DT_CENTER);
		pKItem->SetCaption(_T("-"));
		if(KEMGenerationRegression == emGModel)
		{
			pKItem->SetChecked(TRUE);
		}
		pKItem->SetEditable(bUseGeneration);

		// 5.Cross-classfication model
		pKItem = pKRecord->AddItem(new CXTPReportRecordItemText());
		pKItem->HasCheckbox(TRUE);
		if(KEMGenerationCrossClassification == emGModel)
		{
			pKItem->SetChecked(TRUE);
		}
		pKItem->SetEditable(bUseGeneration);
	}
	TxLogDebugEndMsg();
}


void KGSelectModelDlg::initReportRecordSedVector(void)
{
	TxLogDebugStartMsg();
	CXTPReportRecord* pKRecord = NULL;
	CXTPReportRecordItem* pKItem = NULL;
	BOOL bUseGeneration = FALSE;

	std::list<KGenerationInfo*>::iterator itInfo, itEnd = m_lstGInfoList.end();
	int nNo = 0;
	for(itInfo = m_lstGInfoList.begin(); itInfo != itEnd; ++itInfo)
	{
		pKRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		KGenerationInfo* pKInfo = *itInfo;
		// 0 : 
		pKItem = pKRecord->AddItem(new KExRecordItem2GroupTextNo(pKInfo->GetName(), nNo));
		nNo ++;

		pKItem->SetCaption(pKInfo->GetName());
		pKItem->SetItemData((DWORD_PTR)pKInfo);        

		pKItem = pKRecord->AddItem(new CXTPReportRecordItemText());
		pKItem->HasCheckbox(TRUE);
		if(pKInfo->UseGeneration())
		{
			bUseGeneration = TRUE;
		}
		else
		{
			bUseGeneration = FALSE;
		}
		pKItem->SetChecked(bUseGeneration);

		if(pKInfo->GetPAType() == KEMPATypeProduction)
		{
			pKRecord->AddItem(new CXTPReportRecordItemText(PRODUCTION_STRING));
		}
		else
		{
			pKRecord->AddItem(new CXTPReportRecordItemText(ATTRACTION_STRING));
		}
		pKItem->SetEditable(bUseGeneration);

		// Zone group 과 model 종류 설정
		KGBaseExpressionContainer* pKExpressionContainer = pKInfo->ExpressionContainer();
		bool bGroup = false;
		KEMGenerationModel emGModel = pKInfo->GenerationModel();

		if(NULL != pKExpressionContainer)
		{
			if(pKExpressionContainer->ContainerType() == KEMGExpressionContainerZoneGroup)
			{
				bGroup = true;
			}
			emGModel = pKExpressionContainer->GenerationModel();
		}
		// 2. Zone group
		pKItem = pKRecord->AddItem(new CXTPReportRecordItemText());
		pKItem->HasCheckbox(TRUE);
		pKItem->SetChecked(pKInfo->ZoneGroup());
		pKItem->SetEditable(bUseGeneration);

		// 3. Unit model
		pKItem = pKRecord->AddItem(new CXTPReportRecordItemText());
		pKItem->HasCheckbox(TRUE);
		if(KEMGenerationUnit == emGModel)
		{
			pKItem->SetChecked(TRUE);
		}
		pKItem->SetEditable(bUseGeneration);

		// 4. Regression model
		pKItem = pKRecord->AddItem(new CXTPReportRecordItemText());
		pKItem->HasCheckbox(TRUE);
		if(KEMGenerationRegression == emGModel)
		{
			pKItem->SetChecked(TRUE);
		}
		pKItem->SetEditable(bUseGeneration);

		// 5.Cross-classfication model
		pKItem = pKRecord->AddItem(new CXTPReportRecordItemText());
		pKItem->SetAlignment(DT_CENTER);
		pKItem->SetCaption(_T("-"));
		if(KEMGenerationCrossClassification == emGModel)
		{
			pKItem->SetChecked(TRUE);
		}
		pKItem->SetEditable(bUseGeneration);
	}
	TxLogDebugEndMsg();
}


//Step 2
void KGSelectModelDlg::OnReportItemClick(NMHDR * a_pNotifyStruct, LRESULT* a_pResult)
{
	XTP_NM_REPORTRECORDITEM* pTItemNotify = (XTP_NM_REPORTRECORDITEM*) a_pNotifyStruct;

	if (!pTItemNotify->pRow || !pTItemNotify->pColumn)
		return;

/*
	int nIndex = pTItemNotify->pColumn->GetItemIndex();
	
	if(_4_COLUMN_UNIT == nIndex)
	{
		pTItemNotify->pItem->SetChecked(TRUE);
		pTItemNotify->pRow->GetRecord()->GetItem(_5_COLUMN_REGRESSION)->SetChecked(FALSE);
		pTItemNotify->pRow->GetRecord()->GetItem(_6_COLUMN_CROSSCLASSIFICATION)->SetChecked(FALSE);
	}
	else if(_5_COLUMN_REGRESSION == nIndex)
	{
		pTItemNotify->pItem->SetChecked(TRUE);
		pTItemNotify->pRow->GetRecord()->GetItem(_4_COLUMN_UNIT)->SetChecked(FALSE);
		pTItemNotify->pRow->GetRecord()->GetItem(_6_COLUMN_CROSSCLASSIFICATION)->SetChecked(FALSE);
	}
	else if(_6_COLUMN_CROSSCLASSIFICATION == nIndex)
	{
		pTItemNotify->pItem->SetChecked(TRUE);
		pTItemNotify->pRow->GetRecord()->GetItem(_4_COLUMN_UNIT)->SetChecked(FALSE);
		pTItemNotify->pRow->GetRecord()->GetItem(_5_COLUMN_REGRESSION)->SetChecked(FALSE);
	}*/
}


void KGSelectModelDlg::OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
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


//Step 3
void KGSelectModelDlg::OnOK()
{
	TxLogDebugStartMsg();
	CXTPReportRecords* pKRecords = m_wndReportCtrl.GetRecords();
	int nRecordCount = pKRecords->GetCount();

    int nSelectedCount = 0;
    for(int i = 0; i < nRecordCount; ++i)
    {
        KGenerationInfo* pKInfo = NULL;
        CXTPReportRecord* pKRecord = pKRecords->GetAt(i);
        CXTPReportRecordItem* pKItem = pKRecord->GetItem(_0_COLUMN_PURPOSE_NAME);

        pKInfo = (KGenerationInfo*)(pKItem->GetItemData());

        pKItem = pKRecord->GetItem(_1_COLUMN_SELECTED);
        if (pKItem->IsChecked() == TRUE )
        {
            nSelectedCount ++;
        }
    }

    if (nSelectedCount < 1)
    {
        AfxMessageBox(_T("선택한 정보가 없습니다."));
        return;
    }

	for(int i = 0; i < nRecordCount; ++i)
	{
		bool bZoneGroup = false;
		KEMGenerationModel emGModel;
		KGenerationInfo* pKInfo = NULL;
		CXTPReportRecord* pKRecord = pKRecords->GetAt(i);
		CXTPReportRecordItem* pKItem = pKRecord->GetItem(_0_COLUMN_PURPOSE_NAME);

		pKInfo = (KGenerationInfo*)(pKItem->GetItemData());
		pKInfo->DeleteExpression();

        pKItem = pKRecord->GetItem(_1_COLUMN_SELECTED);
        if (pKItem->IsChecked() == TRUE )
        {
            pKInfo->UseGeneration(true);
        }
        else
        {
            pKInfo->UseGeneration(false);
        }
        //pKInfo->UseGeneration(pKItem->IsChecked() == TRUE ? true : false);

		pKItem = pKRecord->GetItem(_3_COLUMN_ZONEGROUP);
		if(pKItem->IsChecked() == TRUE)
		{
			bZoneGroup = true;
		}

		pKItem = pKRecord->GetItem(_4_COLUMN_UNIT);
		if(pKItem->IsChecked() == TRUE)
		{
			emGModel = KEMGenerationUnit;
		}
	
		pKItem = pKRecord->GetItem(_5_COLUMN_REGRESSION);
		if(pKItem->IsChecked() == TRUE)
		{
			emGModel = KEMGenerationRegression;
		}

		pKItem = pKRecord->GetItem(_6_COLUMN_CROSSCLASSIFICATION);
		if(pKItem->IsChecked() == TRUE)
		{
			emGModel = KEMGenerationCrossClassification;
		}
		
		pKInfo->GenerationModel(emGModel);
		pKInfo->ZoneGroup(bZoneGroup);

		KGBaseExpressionContainer* pBaseCont = pKInfo->ExpressionContainer();
		KEMGenerationModel emGModelEXpression;
		if(pBaseCont != NULL)
		{
			emGModelEXpression = pBaseCont->GenerationModel();
			if(emGModel != emGModelEXpression)
			{
				pKInfo->DeleteExpression();

			}
		}
		
	}
	TxLogDebugEndMsg();

    CXTResizeDialog::OnOK();    
}


//Step 4
void KGSelectModelDlg::OnBnClickedCancel()
{
    CXTResizeDialog::OnCancel();
}
