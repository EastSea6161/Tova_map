// CalculateRatioRegion.cpp : 구현 파일입니다.
//

#include <map>

#include "stdafx.h"
#include "CalculateRatioRegionDlg.h"
#include "afxdialogex.h"

#include "Target.h"
#include "KmzApp.h"
#include "model_common.h"
//^#include "DBaseConnector.h"
//^^#include "SEDData.h"

#include "RatioRegionInfo.h"
#include "CustomReportRecordItem.h"
#include "KBulkDBase.h"
#include "Project.h"
#include "Purpose.h"

#include "ChildFrm.h"
#include "MapView.h"
#include "MainFrameWnd.h"
#include "IOTableController.h"
#include "IOTableView.h"
#include "IOView.h"


// KCalculateRatioRegion 대화 상자입니다.


#define  _0_COLUMN_COMMODITY  0
#define  _1_COLUMN_SED        1


IMPLEMENT_DYNAMIC(KCalculateRatioRegionDlg, CXTResizeDialog)

KCalculateRatioRegionDlg::KCalculateRatioRegionDlg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(KCalculateRatioRegionDlg::IDD, pParent)
	, m_nTargetYear(0)
{

}

KCalculateRatioRegionDlg::~KCalculateRatioRegionDlg()
{

	std::vector<KRatioRegionInfo*> vectemp;
	vectemp.swap(m_vecpRatioInfo);

	std::list<CString> lsttemp;
	lsttemp.swap(m_lstColumnName);

}

void KCalculateRatioRegionDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_reportCommodity);
}


BEGIN_MESSAGE_MAP(KCalculateRatioRegionDlg, CXTResizeDialog)
	ON_BN_CLICKED(IDOK, &KCalculateRatioRegionDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// KCalculateRatioRegion 메시지 처리기입니다.


BOOL KCalculateRatioRegionDlg::OnInitDialog()
{
	CXTResizeDialog::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );
	
	SetResize(IDC_REPORT, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC2, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	KReportCtrlSetting::Default(m_reportCommodity, TRUE);

	GetSEDYearData();
	InitSEDColumnList();
	InitCommodityList();
	InitCommoditReportColumn();
	UpdateData(FALSE);
	return TRUE;  
}


void KCalculateRatioRegionDlg::GetSEDYearData()
{
	TxLogDebugStartMsg();

	m_nTargetYear = m_pTarget->GetTargetYear();
	UpdateData(FALSE);
	TxLogDebugEndMsg();
}


void KCalculateRatioRegionDlg::InitSEDColumnList()
{
	KIOTables* pIOTables = m_pTarget->Tables();
	ASSERT(NULL != pIOTables);

	KIOTable* pTable = pIOTables->FindTable(_T("sed_vector"));
	ASSERT(NULL != pTable);

	const KIOColumns* pColumns = pTable->Columns();
	ASSERT(NULL != pColumns);

	m_lstColumnName.push_back(_T("-"));

	int nColumnCount = pColumns->ColumnCount();
	int nIndex = 0;
	for(nIndex; nIndex < nColumnCount; ++nIndex)
	{
		KIOColumn* pColumn = pColumns->GetColumn(nIndex);
		if (pColumn->ColumnType() == KEMIOColumnTypeUserDefine)
		{
			m_lstColumnName.push_back(pColumn->Caption());
		}
	}
}

void KCalculateRatioRegionDlg::InitCommodityList( void )
{
	TxLogDebugStartMsg();
	m_vecpRatioInfo.clear();

	KProject* pProject = (KProject*)( (CKmzApp*)AfxGetApp() )->GetProject();
	std::vector<KPurpose*> vecPurpose = pProject->GetPurpose();

	size_t nxCount = vecPurpose.size();
	for (size_t i=0; i<nxCount; i++)
	{
		KPurpose *pPurpose            = vecPurpose[i];
		KRatioRegionInfo* pKRatioInfo = new KRatioRegionInfo();

		pKRatioInfo->SetCommdityName(pPurpose->PurposeName());
		pKRatioInfo->SetCommdityCode(pPurpose->PurposeID());
		pKRatioInfo->SetCommdityGroupCode(pPurpose->PurposeGroup());
		m_vecpRatioInfo.push_back(pKRatioInfo);
	}

	TxLogDebugEndMsg();
}


void KCalculateRatioRegionDlg::InitCommoditReportColumn()
{
	TxLogDebugStartMsg();
	m_reportCommodity.GetColumns()->Clear();
	/*m_reportCommodity.GetReportHeader()->AllowColumnRemove(FALSE);
	m_reportCommodity.GetReportHeader()->AllowColumnSort(FALSE);
	m_reportCommodity.GetReportHeader()->AllowColumnReorder(FALSE);
	m_reportCommodity.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);
	m_reportCommodity.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);*/
	

	CXTPReportColumn* pColumn = NULL;
	pColumn = m_reportCommodity.AddColumn(new CXTPReportColumn(_0_COLUMN_COMMODITY, _T("품목"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_reportCommodity.AddColumn(new CXTPReportColumn(_1_COLUMN_SED, _T("사회경제지표"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->GetEditOptions()->AddComboButton(TRUE);
	pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
	pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;


	CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();
	std::list<CString>::iterator itcolum, itEnd = m_lstColumnName.end();
	int nCountIter = 1;
	for(itcolum = m_lstColumnName.begin(); itcolum != itEnd; ++itcolum, ++nCountIter)
	{
		CString strSheetName = *itcolum;
		pEditOptions->AddConstraint(strSheetName, nCountIter);
	}
	
	/*m_reportCommodity.AllowEdit(TRUE);
	m_reportCommodity.EditOnClick(TRUE);
	m_reportCommodity.SetMultipleSelection(FALSE);*/

	InitUpdateColumn();
	TxLogDebugEndMsg();
}


void KCalculateRatioRegionDlg::InitUpdateColumn()
{
	TxLogDebugStartMsg();
	m_reportCommodity.ResetContent(TRUE);

	CXTPReportColumn* pColumn                     = m_reportCommodity.GetColumns()->GetAt(_1_COLUMN_SED);
	CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();
	CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();

	size_t nxCount = m_vecpRatioInfo.size();
	for (size_t i=0; i<nxCount; i++)
	{
		KRatioRegionInfo* pKRatioInfo = m_vecpRatioInfo[i];
		CXTPReportRecord* pRecord     = m_reportCommodity.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem   = pRecord->AddItem(new CXTPReportRecordItemText);

		pItem->SetAlignment(DT_CENTER);
		pItem->SetCaption(pKRatioInfo->GetCommdityName());
		pItem->SetItemData((DWORD_PTR)pKRatioInfo);
		pItem->SetEditable(FALSE);

		int nCommdityCode          = pKRatioInfo->GetCommdityCode();
		CString strCommdityCaption = pKRatioInfo->GetCommdityName();

		pItem = pRecord->AddItem(new CCustomReportRecordItem);
		pItem->SetAlignment(DT_CENTER);
		
		//권역비율 Combo에 Default로 나타나는 값을 사업체수->종사자수로 변경
		//홍다희 박사와 회의로 변경 14.09.02
		//int nIndex = i + 2;
		int nIndex(0);
        if (nxCount == 12)
            nIndex = i + 13;
        else
		    nIndex = i + 31 + 2;

		if (m_lstColumnName.size() > i)
		{
			pItem->SetItemData(nIndex);
		}
		else
		{
			pItem->SetItemData(m_lstColumnName.size());
		}
	}
	m_reportCommodity.Populate();
	TxLogDebugEndMsg();
}


void KCalculateRatioRegionDlg::OnBnClickedOk()
{
	TxLogDebugStartMsg();
	CXTPReportRecords* pRecords = m_reportCommodity.GetRecords();

	int nRecordCount  = pRecords->GetCount();
	if (nRecordCount <= 0)
	{
		AfxMessageBox(_T("선택한 파일이 없습니다."));
		return;
	}

	for (int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord* pRecord   = pRecords->GetAt(i);
		CXTPReportColumn* pColumn   = m_reportCommodity.GetColumns()->GetAt(_1_COLUMN_SED);
		CXTPReportRecordItem* pItem = NULL;
		pItem = pRecord->GetItem(_1_COLUMN_SED);
		CString strSEDName = pItem->GetCaption(pColumn);
		CString strSEDCode = _T("");
		if (strSEDName != _T(""))
		{
			FindSEDCode(strSEDName, strSEDCode);
		}
		
		KRatioRegionInfo* pRatioInfo = new KRatioRegionInfo;
		pRatioInfo = m_vecpRatioInfo.at(i);
		pRatioInfo->SetSEDCode(strSEDCode);
		pRatioInfo->SetSEDCaption(strSEDName);
	}
	CString strYear = _T("");
	

	DeleteRegionInfo();
	DelExistiongDB();
	SetRatioRegionBaseDB();
	RunRatioRegion();
	ViewRatioRegionTable();

	TxLogDebugEndMsg();
	CXTResizeDialog::OnOK();
}

bool KCalculateRatioRegionDlg::FindSEDCode(CString a_strSEDName, CString& a_strSEDCode)
{
	TxLogDebugStartMsg();

	KIOTables* pIOTables = m_pTarget->Tables();
	ASSERT(NULL != pIOTables);

	KIOTable* pTable = pIOTables->FindTable(_T("sed_vector"));
	ASSERT(NULL != pTable);

	const KIOColumns* pColumns = pTable->Columns();
	ASSERT(NULL != pColumns);

	int nColumnCount = pColumns->ColumnCount();
	int nIndex = 0;
	for(nIndex; nIndex < nColumnCount; ++nIndex)
	{
		KIOColumn* pColumn = pColumns->GetColumn(nIndex);
		if (pColumn->ColumnType() == 1)
		{
			if(pColumn->Caption() == a_strSEDName)
			{
				a_strSEDCode = pColumn->Name(); 
				TxLogDebugEndMsg();
				return true ;
			}
		}
	}
	TxLogDebugEndMsg();
	return false;
}


void KCalculateRatioRegionDlg::DeleteRegionInfo()
{
	TxLogDebugStartMsg();
	std::vector<KRatioRegionInfo*>::iterator itRinfo;
	for(itRinfo = m_vecpRatioInfo.begin(); itRinfo < m_vecpRatioInfo.end();)
	{
		KRatioRegionInfo* pKReginInfo = *itRinfo;
		CString strSEDName = pKReginInfo->GetSEDCaption();
		if (strSEDName == _T("-"))
		{
			itRinfo = m_vecpRatioInfo.erase(itRinfo);
		}
		else
		{
			itRinfo++;
		}
	}
	TxLogDebugEndMsg();
}


void KCalculateRatioRegionDlg::DelExistiongDB()
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	try
	{
		CString strDelSQL = _T("");
		strDelSQL.Format(_T("Delete From Ratio_Region "));
		spDBaseConnection->ExecuteUpdate(strDelSQL);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KCalculateRatioRegionDlg::WriteMatchingTable()
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	try
	{
		CString strDelMatchingSQL = _T("");
		//strDelMatchingSQL.Format(_T("Delete From Region_Ratio_Matching where year = %d"),m_nSEDYear);
		strDelMatchingSQL.Format(_T("Delete From Region_Ratio_Matching "));
		spDBaseConnection->ExecuteUpdate(strDelMatchingSQL);

		size_t nxCount = m_vecpRatioInfo.size();
		for (size_t i = 0; i < nxCount; i++)
		{
			KRatioRegionInfo* pRatioInfo = m_vecpRatioInfo[i];
			int nCommdityCode            = pRatioInfo->GetCommdityCode();
			CString strRegionColumn      = pRatioInfo->GetRatioReionCode();
			CString strSQL = _T("");
			/*strSQL.Format(_T("Insert Into Region_Ratio_Matching(Object_ID, Region_Ratio_Column) Values ('%d','%d','%s')"),
								nCommdityCode, m_nSEDYear, strRegionColumn);*/
			strSQL.Format(_T("Insert Into Region_Ratio_Matching(Object_ID,Region_Ratio_Column) Values ('%d','%s')"),
				nCommdityCode, strRegionColumn);
			spDBaseConnection->ExecuteUpdate(strSQL);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KCalculateRatioRegionDlg::SetRatioRegionBaseDB()
{
	TxLogDebugStartMsg();
	std::vector<TFixedSEDVector> vecSEDVector;

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	KBulkDBase::BulkSEDVectorSelect(spDBaseConnection, vecSEDVector);

	size_t nxCount = vecSEDVector.size();
	for (size_t i = 0; i< nxCount; i++ )
	{
		TFixedSEDVector TSEDVector = vecSEDVector[i];
		int nZone_ID               = (int)TSEDVector.nZone_id;
		CString strName            = TSEDVector.strName;
		CString strFullName        = TSEDVector.strFull_Name;
		int nMCategory             = TSEDVector.nMCategory_id;

		CString strSQL = _T("");
		strSQL.Format(_T("Insert into Ratio_Region(Zone_ID, Name, Full_Name, Main_Category) Values ('%d', '%s', '%s', '%d')"),
			                                       nZone_ID, strName, strFullName, nMCategory);
		spDBaseConnection->ExecuteUpdate(strSQL);
	}
	TxLogDebugEndMsg();	
}


void KCalculateRatioRegionDlg::RunRatioRegion()
{
	TxLogDebugStartMsg();
	size_t nxCount = m_vecpRatioInfo.size();
	for (size_t i = 0; i < nxCount; i++)
	{
		KRatioRegionInfo* pRatioInfo = m_vecpRatioInfo[i];
		CalculateSumCategory(pRatioInfo);

		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		CString strSEDCode = pRatioInfo->GetSEDCode();
		CString strSQL = _T("");
		strSQL.Format(_T("Select Zone_ID, Main_Category, %s from sed_Vector"), strSEDCode);
		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		while (spResult->Next())
		{
			TRatioRegionManagement oTratioRegion;

			int nZone_ID      = spResult->GetValueInt(0);
			int nMCategory    = spResult->GetValueInt(1);
			double dItemValue = spResult->GetValueDouble(2);
			double dSumValue  = pRatioInfo->m_mapCategorySum[nMCategory];

			oTratioRegion.nZoneID           = nZone_ID;
			oTratioRegion.nZoneCategoryCode = nMCategory;
			oTratioRegion.dItemValue        = dItemValue;
			if(dItemValue == 0 || dSumValue == 0 )
			{
				oTratioRegion.dResultValue  = 0;
			}
			else
			{
				oTratioRegion.dResultValue   = dItemValue / dSumValue;
			}
			
			pRatioInfo->m_lstRatioReionManage.push_back(oTratioRegion);

		}
		AddRatioRegionColumn(pRatioInfo);

	}
	WriteMatchingTable();
	TxLogDebugEndMsg();	
		
}


void KCalculateRatioRegionDlg::CalculateSumCategory(KRatioRegionInfo* a_pRatioInfo)
{
	TxLogDebugStartMsg();

	CString strSEDName = a_pRatioInfo->GetSEDCode();
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQL = _T("");
	/*strSQL.Format(_T("Select Main_Category, Sum(%s) from sed_vector group by Year ='%d', Main_Category "),
		strSEDName, m_nSEDYear);*/
	strSQL.Format(_T("Select Main_Category, Sum(%s) from sed_vector group by Main_Category "), strSEDName);

	KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
	while (spResult ->Next())
	{
		int nCategroyGroup     = spResult->GetValueInt(0);
		double dGroupSumValue  = spResult->GetValueDouble(1);
		a_pRatioInfo->m_mapCategorySum[nCategroyGroup] = dGroupSumValue;
	}
	TxLogDebugEndMsg();	
}


bool KCalculateRatioRegionDlg::AddRatioRegionColumn( KRatioRegionInfo* a_pRatioInfo )
{
	TxLogDebugStartMsg();
	bool bResult                  = true;
	KIOTables* pTableManger = m_pTarget->Tables();
	
	ASSERT(NULL != pTableManger);
	if (NULL == pTableManger)
	{
		AfxMessageBox((_T("사회경제지표 데이터가 존재하지 않습니다.")));
	}
	KIOTable * pTable = pTableManger->FindTable(_T("Ratio_Region"));
	ASSERT(NULL != pTable);

	const KIOColumns* pColumns = pTable->Columns();
	ASSERT(NULL != pColumns);

	int nCommdityCode          = a_pRatioInfo->GetCommdityCode();
	CString strRatioRegionCode = _T("");
	CString strRatioCaption    = a_pRatioInfo->GetSEDCaption();
	strRatioRegionCode.Format(_T("r%d"),nCommdityCode);
	a_pRatioInfo->SetRatioReionCode(strRatioRegionCode);

	KIOColumn* pOutputColumn = pColumns->GetColumn(strRatioRegionCode);
	if(NULL == pOutputColumn)
	{
		KIOColumn oColumn;
		oColumn.Name(strRatioRegionCode);
		oColumn.Caption(strRatioCaption);
		oColumn.CodeType(KEMIOCodeTypeIsNothing);
		oColumn.ColumnType(KEMIOColumnTypeUserDefine);
		oColumn.DataType(KEMIODataTypeDouble);
		oColumn.DisplayOrder(pColumns->ColumnCount()+1);
		oColumn.PrimaryKey(false);
		oColumn.Visible(true);
		pOutputColumn = pTable->AddColumn(oColumn);
		if (NULL == pOutputColumn)
		{
			AfxMessageBox(_T("컬럼 추가에 실패하였습니다."));
			TxLogDebugEndMsg();
			return false;
		}
	}
	else
	{
		pOutputColumn->Caption(strRatioCaption);
		KIOColumn oColumn = *pOutputColumn;
		pTable->UpdateColumn(oColumn);
		//pTable->Commit();
	}

	KIOColumns oColumnCollection;
	oColumnCollection.AddNewColumn(pOutputColumn);
	std::list<TRatioRegionManagement>::iterator ItTInfo;
	for(ItTInfo = a_pRatioInfo->m_lstRatioReionManage.begin(); ItTInfo != a_pRatioInfo->m_lstRatioReionManage.end(); ++ItTInfo)
	{
		TRatioRegionManagement TRatioRegion = * ItTInfo;
		KIORow resultRow;
		KIOItem* pResultItem = resultRow.AddNewItem();
		int nZoneID          = TRatioRegion.nZoneID;
		double dResultValue  = TRatioRegion.dResultValue;
		pResultItem->SetValueDetails(dResultValue);

		CString strFilter;
		//strFilter.Format(_T("Zone_ID = %d and Year = %d"), nZoneID, m_nSEDYear);
		strFilter.Format(_T("Zone_ID = %d"), nZoneID);
		pTable->UpdateRow(&oColumnCollection, &resultRow, strFilter);

	}
	TxLogDebugEndMsg();
	return bResult;
}

void KCalculateRatioRegionDlg::ViewRatioRegionTable()
{
	TxLogDebugStartMsg();

	KIOTables* pTableManger = m_pTarget->Tables();
	KIOTable * pTable             = pTableManger->FindTable(_T("Ratio_Region"));

	CChildFrame* pChildFrame = (CChildFrame*)((CMainFrameWnd*)AfxGetMainWnd())->MDIGetActive();
	pChildFrame->ShowIOView( true );
	KIOView* pIOView = pChildFrame->GetIOView();

	if(NULL != pIOView)
	{
		KIOTableView* pTableView = pIOView->FindTableView(pTable->Name());
		if(NULL == pTableView)
		{
			pTableView = pIOView->CreateNewTableView(pTable);
			KIOTableController* pController = new KIOTableController(m_pTarget);
			pController->Attach(pTable, pTableView);
		}
		pIOView->SetActiveTableView(pTable->Name());
	}
	pTable->Notify();

	TxLogDebugEndMsg();
}

void KCalculateRatioRegionDlg::SetTarget( KTarget* a_pTarget )
{
	if (a_pTarget == NULL)
	{
		return;
	}

	m_pTarget = a_pTarget;
}
