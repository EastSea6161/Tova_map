// MultiStatisticsDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "MultiStatisticsDlg.h"
#include "afxdialogex.h"
#include "ExpressionElementField.h"
#include "ExpressionElementFunction.h"
#include "ExpressionElementNumber.h"
#include "ExpressionElementOperator.h"
#include "GRegressionExpression.h"
#include "GenerationInfo.h"
#include "KDBaseStatistics.h"
//^#include "DBaseConnector.h"
//^^ #include "TableOwner.h"
//#include "CodeGroup.h"
//#include "CodeManager.h"
//^^ #include "IORecordset.h"
//^^ #include "IORow.h"
//^^ #include "IOItem.h"
//^#include "DevideDataInterval.h"
#include <fstream>
#include <iostream>

enum
{
	_0_PART_COLUMN_NAME = 0,    
	_1_PART_COLUMN_SUM,
	_2_PART_COLUMN_AVERAGE,
	_3_PART_COLUMN_MIN,
	_4_PART_COLUMN_MAX,
	_5_PART_COLUMN_VARIANCE,
	_6_PART_COLUMN_STDDEV,
	_7_PART_COLUMN_STDERR
};
#define PART_COLUMN_TOTAL_COUNT 8

enum
{
	_0_MAIN_COLUMN_CHECKNAME = 0,    
	_1_MAIN_COLUMN_RMSE,
	_2_MAIN_COLUMN_PRMSE,
	_3_MAIN_COLUMN_MARE,
	_4_MAIN_COLUMN_MAE,
	_5_MAIN_COLUMN_RSQUARE,
	_6_MAIN_COLUMN_CHISQUARE,
	_7_MAIN_COLUMN_TTEST
};
#define MAIN_COLUMN_TOTAL_COUNT 8


// KMultiStatisticsDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KMultiStatisticsDlg, CXTPResizeDialog)

KMultiStatisticsDlg::KMultiStatisticsDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(KMultiStatisticsDlg::IDD, pParent)
{
}

KMultiStatisticsDlg::~KMultiStatisticsDlg()
{
}

void KMultiStatisticsDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT2, m_ReportMainCtrl);
	DDX_Control(pDX, IDC_REPORT,  m_ReportPartCtrl);
}


BEGIN_MESSAGE_MAP(KMultiStatisticsDlg, CXTPResizeDialog)
	ON_BN_CLICKED(IDOK, &KMultiStatisticsDlg::OnBnClickedOk)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED,       IDC_REPORT2, OnMainReportCheckItem)
	ON_BN_CLICKED(IDC_BTN_EXPORT, &KMultiStatisticsDlg::OnBnClickedButtonExport)
END_MESSAGE_MAP()


// KMultiStatisticsDlg 메시지 처리기입니다.

BOOL KMultiStatisticsDlg::OnInitDialog()
{
	CXTPResizeDialog::OnInitDialog();    
	SetDlgItemText(IDC_STATIC_NAME, m_pTable->Caption());

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	InitReportHeader();
	m_ReportMainCtrl.SetGridStyle(TRUE, xtpReportGridSolid);
	m_ReportPartCtrl.SetGridStyle(TRUE, xtpReportGridSolid);	
	InitReportData();

    KReportCtrlSetting::Default(m_ReportMainCtrl, FALSE, FALSE, FALSE);
    KReportCtrlSetting::Default(m_ReportPartCtrl, FALSE, FALSE, FALSE);

	m_ReportMainCtrl.Populate();
	m_ReportPartCtrl.Populate();
	m_ReportMainCtrl.ExpandAll(TRUE);
	m_ReportPartCtrl.ExpandAll(TRUE);

	SetResize(IDC_REPORT2,      SZ_TOP_LEFT,     SZ_TOP_RIGHT);
	SetResize(IDC_REPORT,       SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR1,    SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDOK,             SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);	
	SetResize(IDC_STATIC_COUNT, SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_BTN_EXPORT,   SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KMultiStatisticsDlg::InitReportHeader()
{
	if(NULL == m_pGroupColumn)
	{
		InitMainReportNormalHeader();
		InitPartReportNormalHeader();
	}
	else
	{
		InitMainReportGroupHeader();
		InitPartReportGroupHeader();
	}
}


void KMultiStatisticsDlg::InitMainReportNormalHeader()
{
	CXTPReportColumn* pColumn = NULL;

	//m_ReportMainCtrl.FocusSubItems(FALSE);
	m_ReportMainCtrl.SelectionEnable(FALSE);

	pColumn = m_ReportMainCtrl.AddColumn(new CXTPReportColumn(_0_MAIN_COLUMN_CHECKNAME, _T("선택"),    23));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T(""));
    }

	pColumn = m_ReportMainCtrl.AddColumn(new CXTPReportColumn(_1_MAIN_COLUMN_RMSE,      _T("RMSE"),     11));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_ReportMainCtrl.AddColumn(new CXTPReportColumn(_2_MAIN_COLUMN_PRMSE,     _T("%RMSE"), 11));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_ReportMainCtrl.AddColumn(new CXTPReportColumn(_3_MAIN_COLUMN_MARE,      _T("MARE"),     11));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_ReportMainCtrl.AddColumn(new CXTPReportColumn(_4_MAIN_COLUMN_MAE,       _T("MAE"),     11));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_ReportMainCtrl.AddColumn(new CXTPReportColumn(_5_MAIN_COLUMN_RSQUARE,   _T("RSquare"), 11));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_ReportMainCtrl.AddColumn(new CXTPReportColumn(_6_MAIN_COLUMN_CHISQUARE, _T("ChiSquare"), 11));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_ReportMainCtrl.AddColumn(new CXTPReportColumn(_7_MAIN_COLUMN_TTEST,     _T("T-TEST"), 11));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetVisible(FALSE);
    
	CXTPReportHeader* pHeader = m_ReportMainCtrl.GetReportHeader();
	pHeader->AllowColumnRemove(FALSE);
	pHeader->AllowColumnReorder(FALSE);
}


void KMultiStatisticsDlg::InitPartReportNormalHeader()
{
	CXTPReportPaintManager* pPaint = m_ReportPartCtrl.GetPaintManager();
	pPaint->SetColumnStyle(xtpReportColumnFlat);
	pPaint->m_treeStructureStyle = xtpReportTreeStructureSolid;
	//pPaint->SetGridStyle(TRUE, xtpReportGridSolid);

	CXTPReportColumn* pColumn = NULL;

	//m_ReportPartCtrl.FocusSubItems(FALSE);
	m_ReportPartCtrl.SelectionEnable(FALSE);

	pColumn = m_ReportPartCtrl.AddColumn(new CXTPReportColumn(_0_PART_COLUMN_NAME,     _T("Name"),    24));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetTreeColumn(TRUE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T(""));
    }

	pColumn = m_ReportPartCtrl.AddColumn(new CXTPReportColumn(_1_PART_COLUMN_SUM,      _T("SUM"),     13));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("합계"));
    }

	pColumn = m_ReportPartCtrl.AddColumn(new CXTPReportColumn(_2_PART_COLUMN_AVERAGE,  _T("Average"), 10));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("평균"));
    }

	pColumn = m_ReportPartCtrl.AddColumn(new CXTPReportColumn(_3_PART_COLUMN_MIN,      _T("Min"),     10));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("최소"));
    }

	pColumn = m_ReportPartCtrl.AddColumn(new CXTPReportColumn(_4_PART_COLUMN_MAX,      _T("Max"),     10));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("최대"));
    }

	pColumn = m_ReportPartCtrl.AddColumn(new CXTPReportColumn(_5_PART_COLUMN_VARIANCE, _T("Variance"), 13));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("분산"));
    }

	pColumn = m_ReportPartCtrl.AddColumn(new CXTPReportColumn(_6_PART_COLUMN_STDDEV,   _T("Std.Deviation"), 10));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("표준편차"));
    }

	pColumn = m_ReportPartCtrl.AddColumn(new CXTPReportColumn(_7_PART_COLUMN_STDERR,   _T("Std.Error"), 10));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("표준오차"));
    }

	/*_7_PART_COLUMN_STDERR*/

	CXTPReportHeader* pHeader = m_ReportPartCtrl.GetReportHeader();
	pHeader->AllowColumnRemove(FALSE);
	pHeader->AllowColumnReorder(FALSE);
}


void KMultiStatisticsDlg::InitMainReportGroupHeader()
{
	CXTPReportPaintManager* pPaint = m_ReportMainCtrl.GetPaintManager();
	pPaint->SetColumnStyle(xtpReportColumnFlat);
	pPaint->m_treeStructureStyle = xtpReportTreeStructureSolid;
	//pPaint->SetGridStyle(TRUE, xtpReportGridSolid);

	CXTPReportColumn* pColumn = NULL;

	m_ReportMainCtrl.SelectionEnable(FALSE);

	pColumn = m_ReportMainCtrl.AddColumn(new CXTPReportColumn(_0_MAIN_COLUMN_CHECKNAME, _T("선택"),    23));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetTreeColumn(TRUE);

	pColumn = m_ReportMainCtrl.AddColumn(new CXTPReportColumn(_1_MAIN_COLUMN_RMSE,      _T("RMSE"),     11));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_ReportMainCtrl.AddColumn(new CXTPReportColumn(_2_MAIN_COLUMN_PRMSE,     _T("%RMSE"), 11));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_ReportMainCtrl.AddColumn(new CXTPReportColumn(_3_MAIN_COLUMN_MARE,      _T("MARE"),     11));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_ReportMainCtrl.AddColumn(new CXTPReportColumn(_4_MAIN_COLUMN_MAE,       _T("MAE"),     11));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_ReportMainCtrl.AddColumn(new CXTPReportColumn(_5_MAIN_COLUMN_RSQUARE,   _T("RSquare"), 11));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_ReportMainCtrl.AddColumn(new CXTPReportColumn(_6_MAIN_COLUMN_CHISQUARE, _T("ChiSquare"), 11));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_ReportMainCtrl.AddColumn(new CXTPReportColumn(_7_MAIN_COLUMN_TTEST,     _T("T-TEST"), 11));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetVisible(FALSE);

	CXTPReportHeader* pHeader = m_ReportMainCtrl.GetReportHeader();
	pHeader->AllowColumnRemove(FALSE);
	pHeader->AllowColumnReorder(FALSE);
	pHeader->AllowColumnSort(FALSE);
}


void KMultiStatisticsDlg::InitPartReportGroupHeader()
{
	CXTPReportPaintManager* pPaint = m_ReportPartCtrl.GetPaintManager();
	pPaint->SetColumnStyle(xtpReportColumnFlat);
	pPaint->m_treeStructureStyle = xtpReportTreeStructureSolid;
	//pPaint->SetGridStyle(TRUE, xtpReportGridSolid);

	CXTPReportColumn* pColumn = NULL;

	m_ReportPartCtrl.SelectionEnable(FALSE);

	pColumn = m_ReportPartCtrl.AddColumn(new CXTPReportColumn(_0_PART_COLUMN_NAME,     _T("Name"),    24));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetTreeColumn(TRUE);

	pColumn = m_ReportPartCtrl.AddColumn(new CXTPReportColumn(_1_PART_COLUMN_SUM,      _T("SUM"),     13));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_ReportPartCtrl.AddColumn(new CXTPReportColumn(_2_PART_COLUMN_AVERAGE,  _T("Average"), 10));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_ReportPartCtrl.AddColumn(new CXTPReportColumn(_3_PART_COLUMN_MIN,      _T("Min"),     10));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_ReportPartCtrl.AddColumn(new CXTPReportColumn(_4_PART_COLUMN_MAX,      _T("Max"),     10));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_ReportPartCtrl.AddColumn(new CXTPReportColumn(_5_PART_COLUMN_VARIANCE, _T("Variance"), 13));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_ReportPartCtrl.AddColumn(new CXTPReportColumn(_6_PART_COLUMN_STDDEV,   _T("Std.Deviation"), 10));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_ReportPartCtrl.AddColumn(new CXTPReportColumn(_7_PART_COLUMN_STDERR,   _T("Std.Error"), 10));
	pColumn->SetHeaderAlignment(DT_CENTER);

	CXTPReportHeader* pHeader = m_ReportPartCtrl.GetReportHeader();
	pHeader->AllowColumnRemove(FALSE);
	pHeader->AllowColumnReorder(FALSE);
	pHeader->AllowColumnSort(FALSE);
}


void KMultiStatisticsDlg::InitReportData()
{
	if(NULL == m_pGroupColumn)
	{
		InitMainReportNormalData();
	}
	else
	{
		InitMainReportGroupData();
	}
}


void KMultiStatisticsDlg::InitMainReportNormalData()
{
	if(NULL == m_pColumns)
	{
		return;
	}

	if(false == ValidNormalResultDataSize())
	{
		return;
	}

	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	// 일반 데이터
	m_ReportMainCtrl.ResetContent(TRUE);

	for (size_t i= 0; i< m_vecMultiColumn.size(); i++)
	{
		TMultiColumn &oTMultiColumn = m_vecMultiColumn[i];

		CString strName = _T("");
		strName.Format(_T("%s - %s"), oTMultiColumn.pBaseColumn->Caption(), oTMultiColumn.pCompareColumn->Caption());

		// draw grid
		pRecord = m_ReportMainCtrl.AddRecord(new CXTPReportRecord());

		TStatisticResultMain &oTStatisticMain = m_vecStatisticMain[i];

		SetMainNormalRecordGrid(pRecord, strName, oTStatisticMain.vecData, oTMultiColumn);
	}    

	CString strDataCount;
	strDataCount.Format(_T("Recod(Count): %d, Data(Count): %d"), 0, 0);        
	SetDlgItemText(IDC_STATIC_COUNT, strDataCount);
}


void KMultiStatisticsDlg::InitPartReportNormalData()
{
	CXTPReportRecords*     pMainRecords = m_ReportMainCtrl.GetRecords();
	int nMainRecordCount = pMainRecords->GetCount();  

	if(NULL == m_pColumns)
	{
		return;
	}

	CXTPReportRecord*     pParentRecord = NULL;
	CXTPReportRecords*    pRecords= NULL;
	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	m_ReportPartCtrl.ResetContent(TRUE);

	for (int i= 0; i< nMainRecordCount; i++)
	{
		CXTPReportRecord*     pMainRecord    = pMainRecords->GetAt(i);
		CXTPReportRecordItem* pMainCheckItem = pMainRecord->GetItem(_0_MAIN_COLUMN_CHECKNAME);
		if(pMainCheckItem->IsChecked() == FALSE)
		{
			continue;
		}

		TMultiColumn* pTMultiColumn = (TMultiColumn*)pMainCheckItem->GetItemData();
		CString strBaseName    = pTMultiColumn->pBaseColumn->Caption();
		CString strCompareName = pTMultiColumn->pCompareColumn->Caption();
		CString strName = _T("");
		strName.Format(_T("%s - %s"), strBaseName, strCompareName);

		// draw grid
		pParentRecord = m_ReportPartCtrl.AddRecord(new CXTPReportRecord());
		pParentRecord->AddItem(new CXTPReportRecordItemText(strName));
		for (int m= 0; m< PART_COLUMN_TOTAL_COUNT-1; m++)
		{
			pParentRecord->AddItem(new CXTPReportRecordItemText(_T("")));
		}
		pRecords      = pParentRecord->GetChilds();

		std::map<int, std::vector<TStatisticResultPart>>::iterator iter = m_mapStatisticPart.find(i);
		if( iter == m_mapStatisticPart.end() )
		{
			continue;
		}

		std::vector<TStatisticResultPart> vecStatisticPart = iter->second;

		pRecord       = pRecords->Add((new CXTPReportRecord()));
		TStatisticResultPart &oTStatisticPart = vecStatisticPart[0];
		SetPartRecordGrid(pRecord, strBaseName,    oTStatisticPart.vecData );

		pRecord       = pRecords->Add((new CXTPReportRecord()));
		oTStatisticPart = vecStatisticPart[1];
		SetPartRecordGrid(pRecord, strCompareName, oTStatisticPart.vecData );
	}    
}


void KMultiStatisticsDlg::InitMainReportGroupData()
{
	if(NULL == m_pColumns)
	{
		return;
	}

	if(false == ValidGroupResultDataSize())
	{
		return;
	}

	KCodeGroup*       pCodeData       = NULL;
	KCodeManager*     pCodeManager    = NULL;
	pCodeManager    = m_pTable->GetOwner()->GetCodeManager();
	pCodeData       = pCodeManager->FindCodeGroup(m_pGroupColumn->CodeGroup());

	CXTPReportRecord*     pParentRecord = NULL;
	CXTPReportRecords*    pRecords= NULL;
	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	m_ReportMainCtrl.ResetContent(TRUE);
	for (size_t i= 0; i< m_vecMultiColumn.size(); i++)
	{
		TMultiColumn &oTMultiColumn = m_vecMultiColumn[i];

		CString strName = _T("");
		strName.Format(_T("%s - %s"), oTMultiColumn.pBaseColumn->Caption(), oTMultiColumn.pCompareColumn->Caption());

		// draw grid
		pParentRecord = m_ReportMainCtrl.AddRecord(new CXTPReportRecord());
		pItem = pParentRecord->AddItem(new CXTPReportRecordItemText(strName));
		for (int m= 0; m< MAIN_COLUMN_TOTAL_COUNT-1; m++)
		{
			pParentRecord->AddItem(new CXTPReportRecordItemText(_T("")));
		}
		
		pItem->SetAlignment(DT_LEFT);
		pItem->HasCheckbox(TRUE);
		pItem->SetItemData((DWORD_PTR) &oTMultiColumn);

		pRecords = pParentRecord->GetChilds();

		for(int k= 0; k< m_nGroupFieldTypeCount; k++)
		{
			int nIdx =  i * m_nGroupFieldTypeCount + k;

			pRecord = pRecords->Add(new CXTPReportRecord());
			TStatisticResultMain &oTStatisticMain = m_vecStatisticMain[nIdx];
			CString &strGroupName = pCodeData->SingleCodeValue((int)oTStatisticMain.nxGroupID);
			SetMainGroupRecordGrid(pRecord, strGroupName, oTStatisticMain.vecData );
		}
	}   

	// 일반 데이터
	CString strDataCount;
	strDataCount.Format(_T("Recod(Count): %d, Data(Count): %d"), 0, 0);        
	SetDlgItemText(IDC_STATIC_COUNT, strDataCount);
}


void KMultiStatisticsDlg::InitPartReportGroupData()
{
	CXTPReportRecords*     pMainRecords = m_ReportMainCtrl.GetRecords();
	int nMainRecordCount = pMainRecords->GetCount();  

	if(NULL == m_pColumns)
	{
		return;
	}

	KCodeGroup*       pCodeData       = NULL;
	KCodeManager*     pCodeManager    = NULL;
	pCodeManager    = m_pTable->GetOwner()->GetCodeManager();
	pCodeData   =     pCodeManager->FindCodeGroup(m_pGroupColumn->CodeGroup());

	CXTPReportRecord*     pParentRecord = NULL;
	CXTPReportRecords*    pMidRecords   = NULL;
	CXTPReportRecord*     pMidRecord    = NULL;
	CXTPReportRecords*    pRecords      = NULL;
	CXTPReportRecord*     pRecord       = NULL;
	CXTPReportRecordItem* pItem         = NULL;

	m_ReportPartCtrl.ResetContent(TRUE);

	for (int i= 0; i< nMainRecordCount; i++)
	{
		CXTPReportRecord*     pMainRecord    = pMainRecords->GetAt(i);
		CXTPReportRecordItem* pMainCheckItem = pMainRecord->GetItem(_0_MAIN_COLUMN_CHECKNAME);
		if(pMainCheckItem->IsChecked() == FALSE)
		{
			continue;
		}

		TMultiColumn* pTMultiColumn = (TMultiColumn*)pMainCheckItem->GetItemData();
		CString strBaseName    = pTMultiColumn->pBaseColumn->Caption();
		CString strCompareName = pTMultiColumn->pCompareColumn->Caption();
		CString strName = _T("");
		strName.Format(_T("%s - %s"), strBaseName, strCompareName);

		// draw grid
		pParentRecord = m_ReportPartCtrl.AddRecord(new CXTPReportRecord());
		pParentRecord->AddItem(new CXTPReportRecordItemText(strName));
		for (int m= 0; m< PART_COLUMN_TOTAL_COUNT-1; m++)
		{
			pParentRecord->AddItem(new CXTPReportRecordItemText(_T("")));
		}
		pMidRecords   = pParentRecord->GetChilds();

		std::map<int, std::vector<TStatisticResultPart>>::iterator iter = m_mapStatisticPart.find(i);
		if( iter == m_mapStatisticPart.end() )
		{
			continue;
		}

		std::vector<TStatisticResultPart> vecStatisticPart = iter->second;

		// Base 
		pMidRecord = pMidRecords->Add((new CXTPReportRecord()));
		pMidRecord->AddItem(new CXTPReportRecordItemText(strBaseName));
		for (int m= 0; m< PART_COLUMN_TOTAL_COUNT-1; m++)
		{
			pMidRecord->AddItem(new CXTPReportRecordItemText(_T("")));
		}
		pRecords   = pMidRecord->GetChilds();

		int nIdx = 0;
		for(int k= 0; k< m_nGroupFieldTypeCount; k++)
		{
			pRecord    = pRecords->Add((new CXTPReportRecord()));
			TStatisticResultPart &oTStatisticPart = vecStatisticPart[nIdx];

			CString &strGroupName = pCodeData->SingleCodeValue((int)oTStatisticPart.nxGroupID);

			SetPartRecordGrid(pRecord, strGroupName,    oTStatisticPart.vecData );
			nIdx++;
		}

		// Compare
		pMidRecord = pMidRecords->Add((new CXTPReportRecord()));
		pMidRecord->AddItem(new CXTPReportRecordItemText(strCompareName));
		for (int m= 0; m< PART_COLUMN_TOTAL_COUNT-1; m++)
		{
			pMidRecord->AddItem(new CXTPReportRecordItemText(_T("")));
		}
		pRecords   = pMidRecord->GetChilds();
	
		for(int k= 0; k< m_nGroupFieldTypeCount; k++)
		{
			pRecord    = pRecords->Add((new CXTPReportRecord()));
			TStatisticResultPart &oTStatisticPart = vecStatisticPart[nIdx];

			CString &strGroupName = pCodeData->SingleCodeValue((int)oTStatisticPart.nxGroupID);

			SetPartRecordGrid(pRecord, strGroupName,    oTStatisticPart.vecData );
			nIdx++;
		}
	}    
}


bool KMultiStatisticsDlg::ValidNormalResultDataSize()
{
	// 있어야 할 데이터 크기와 결과로 받은 데이터 크기가 다른지 체크
	if( m_vecMultiColumn.size() != m_vecStatisticMain.size())
	{
		return false;
	}

	return true;
}


bool KMultiStatisticsDlg::ValidGroupResultDataSize()
{
	// 있어야 할 데이터 크기와 결과로 받은 데이터 크기가 다른지 체크
	if( (m_vecMultiColumn.size() * m_nGroupFieldTypeCount)  != ( m_vecStatisticMain.size()) )
	{
		return false;
	}

	return true;
}


void KMultiStatisticsDlg::SetMainNormalRecordGrid(CXTPReportRecord* a_pRecord, LPCTSTR a_lpcszName, std::vector<double> &a_vecData, TMultiColumn &a_oTMultiColumn)
{
	CXTPReportRecordItem* pItem   = NULL;

	pItem = a_pRecord->AddItem(new CXTPReportRecordItemText   ( a_lpcszName ));
	pItem->SetAlignment(DT_LEFT);
	pItem->HasCheckbox(TRUE);
	pItem->SetItemData((DWORD_PTR) &a_oTMultiColumn);

	for(size_t i= 0; i< a_vecData.size(); i++)
	{
		pItem = a_pRecord->AddItem( new CXTPReportRecordItemNumber( a_vecData[i],      _T("%.3f")) );
		pItem->SetAlignment(DT_RIGHT);
	}
}


void KMultiStatisticsDlg::SetMainGroupRecordGrid(CXTPReportRecord* a_pRecord, LPCTSTR a_lpcszName, std::vector<double> &a_vecData)
{
	CXTPReportRecordItem* pItem   = NULL;

	pItem = a_pRecord->AddItem(new CXTPReportRecordItemText   ( a_lpcszName ));
	pItem->SetAlignment(DT_LEFT);

	for(size_t i= 0; i< a_vecData.size(); i++)
	{
		pItem = a_pRecord->AddItem( new CXTPReportRecordItemNumber( a_vecData[i],      _T("%.3f")) );
		pItem->SetAlignment(DT_RIGHT);
	}
}


void KMultiStatisticsDlg::SetPartRecordGrid(CXTPReportRecord* a_pRecord, LPCTSTR a_lpcszName, std::vector<double> &a_vecData)
{
	CXTPReportRecordItem* pItem   = NULL;

	pItem = a_pRecord->AddItem(new CXTPReportRecordItemText   ( a_lpcszName ));
	pItem->SetAlignment(DT_LEFT);

	for(size_t i= 0; i< a_vecData.size(); i++)
	{
		pItem = a_pRecord->AddItem( new CXTPReportRecordItemNumber( a_vecData[i],      _T("%.3f")) );
		pItem->SetAlignment(DT_RIGHT);
	}
}


void KMultiStatisticsDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CXTPResizeDialog::OnOK();
}


void KMultiStatisticsDlg::OnMainReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{

	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	if(NULL == m_pGroupColumn)
	{
		InitPartReportNormalData();
	}
	else
	{
		InitPartReportGroupData();
	}
	m_ReportPartCtrl.Populate();
	m_ReportPartCtrl.ExpandAll(TRUE);
}

void KMultiStatisticsDlg::OnBnClickedButtonExport()
{
	try
	{
		CFileDialog fileDlg(FALSE, NULL, NULL, OFN_FILEMUSTEXIST, _T("(*.txt)|*txt|"), NULL);

		fileDlg.m_ofn.lpstrTitle = _T("Export Statistics Result");
		//fileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME) + 12;

		if (fileDlg.DoModal() != IDOK)
			return;

		CString strExportFile = fileDlg.GetPathName();
		strExportFile.Replace(_T(".txt"), _T(""));
		m_strFile.Format(_T("%s.txt"), strExportFile);


		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}
		QBicSimpleProgressThread::ExecuteThread(ExportThreadCaller, this, false, nLang);

		if (!m_strErrMsg.IsEmpty())
		{
			TxLogDebugException();
			AfxMessageBox(m_strErrMsg);
			return;
		}

		AfxMessageBox(_T("데이터 내보내기를 완료하였습니다."));
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("데이터 불러오기에 실패하였습니다."));
	}
}

unsigned __stdcall KMultiStatisticsDlg::ExportThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	KMultiStatisticsDlg* pDlg = (KMultiStatisticsDlg*)pParameter->GetParameter();
	pDlg->ExportData();

	return 0;
}

void KMultiStatisticsDlg::ExportData()
{
	m_strErrMsg = _T("");

	UINT         nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx outFile(m_strFile, nOpenMode);
	CString      strLine;

	CXTPReportRecord* pRecord = nullptr;

	try
	{
		//위
		ExportMainReport(outFile);

		outFile.WriteString(_T("======================================================================\r\n"));

		//아래
		ExportPartReport(outFile);

		outFile.Close();
	}
	catch (KExceptionPtr ex)
	{
		outFile.Close();
		m_strErrMsg = ex->GetErrorMessage();
	}
	catch (...)
	{
		outFile.Close();
		m_strErrMsg = _T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오.");
	}
}

void KMultiStatisticsDlg::ExportMainReport( CStdioFileEx& of )
{
	CString strLine = _T("");

	//export header
	CXTPReportColumn* pColumn = nullptr;
	int               nColumn = m_ReportMainCtrl.GetColumns()->GetCount();
	
	for (int i = 0; i < nColumn; ++i)
	{
		pColumn = m_ReportMainCtrl.GetColumns()->GetAt(i);

		if (i == 0)
		{
			strLine = pColumn->GetCaption();
			continue;
		}

		strLine.AppendFormat(_T("\t%s"), pColumn->GetCaption());
	}

	if (strLine.IsEmpty())
		ThrowException(_T("ReportControl 설정이 잘못되었습니다."));

	of.WriteString(strLine);
	of.WriteString(_T("\r\n"));

	//export data
	CXTPReportRecord* pRecord = nullptr;
	int               nRecord = m_ReportMainCtrl.GetRecords()->GetCount();

	for (int i = 0; i < nRecord; ++i)
	{
		pRecord = m_ReportMainCtrl.GetRecords()->GetAt(i);

		WriteRecord(pRecord, of);
	}
}

void KMultiStatisticsDlg::ExportPartReport( CStdioFileEx& of )
{
	CString strLine = _T("");

	//export header
	CXTPReportColumn* pColumn = nullptr;
	int               nColumn = m_ReportPartCtrl.GetColumns()->GetCount();

	for (int i = 0; i < nColumn; ++i)
	{
		pColumn = m_ReportPartCtrl.GetColumns()->GetAt(i);

		if (i == 0)
		{
			strLine = pColumn->GetCaption();
			continue;
		}

		strLine.AppendFormat(_T("\t%s"), pColumn->GetCaption());
	}

	if (strLine.IsEmpty())
		ThrowException(_T("ReportControl 설정이 잘못되었습니다."));

	of.WriteString(strLine);
	of.WriteString(_T("\r\n"));

	//export data
	CXTPReportRecord* pRecord = nullptr;
	int               nRecord = m_ReportPartCtrl.GetRecords()->GetCount();

	for (int i = 0; i < nRecord; ++i)
	{
		pRecord = m_ReportPartCtrl.GetRecords()->GetAt(i);

		WriteRecord(pRecord, of);
	}
}

void KMultiStatisticsDlg::WriteRecord( CXTPReportRecord* a_pRecord, CStdioFileEx& of )
{
	CString strLine;
	CXTPReportRecordItemText*   pText   = nullptr;
	CXTPReportRecordItemNumber* pNumber = nullptr;

	int nItem = a_pRecord->GetItemCount();

	for (int i = 0; i < nItem; ++i)
	{
		if (i == 0)
		{
			pText   = (CXTPReportRecordItemText*)a_pRecord->GetItem(i);
			strLine = pText->GetValue();				

			continue;
		}

		if (a_pRecord->HasChildren())
			break;

		pNumber = (CXTPReportRecordItemNumber*)a_pRecord->GetItem(i);
		strLine.AppendFormat(_T("\t%.3f"), pNumber->GetValue());
	}

	of.WriteString(strLine);
	of.WriteString(_T("\r\n"));
	
	if (a_pRecord->HasChildren())
	{
		CXTPReportRecords* pRecords = a_pRecord->GetChilds();
		CXTPReportRecord*  pRecord  = nullptr;
		int                nRecord  = pRecords->GetCount();

		for (int i = 0; i < nRecord; ++i)
		{
			pRecord = pRecords->GetAt(i);
			
			WriteRecord(pRecord, of);
		}
	}	
}
