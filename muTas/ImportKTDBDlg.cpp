/**
 * @file ImportKTDBDlg.cpp
 * @brief KImportKTDBDlg 구현
 * @author jyk@qbicware.com
 * @date 2012.01.17
 * @remark
 */


#include "stdafx.h"
#include "ImportKTDBDlg.h"

#include "KmzSystem.h"
#include "KmzApp.h"
#include "Project.h"
#include "Scenario.h"
#include "Target.h"
#include "Purpose.h"
#include "Mode.h"
#include "ReportCtrlSetting.h"

/******************************************************************************
* KImportKTDBDlg                                                              *
******************************************************************************/
IMPLEMENT_DYNAMIC(KImportKTDBDlg, KDialogEx)

KImportKTDBDlg::KImportKTDBDlg( CWnd* pParent /*=NULL*/ )
	: KDialogEx(KImportKTDBDlg::IDD, pParent)
	, m_bTargetSingle(false)
{

}

KImportKTDBDlg::~KImportKTDBDlg()
{
    TxLogDebugVisitor();
}

void KImportKTDBDlg::SetScenario(KScenario* pScenario)
{
    m_pScenario = pScenario;
}

void KImportKTDBDlg::SetSingleTargetMode( KTarget* a_pTarget )
{
	m_bTargetSingle = true;
	m_pTargetSingle = a_pTarget;
}

void KImportKTDBDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRJSTC_SCENARIONAME, m_stcScenarioName);
	DDX_Control(pDX, IDC_REPORT1, m_reportNetwork);
	DDX_Control(pDX, IDC_REPORT2, m_reportOD);
}


BEGIN_MESSAGE_MAP(KImportKTDBDlg, KDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_REPORT1, OnNetworkDataClick)
	ON_NOTIFY(NM_CLICK, IDC_REPORT2, OnODDataClick)
	ON_BN_CLICKED(IDC_PRJBTN_NETWORKDEFAULT, &KImportKTDBDlg::OnBnClickedPrjbtnNetworkdefault)
	ON_BN_CLICKED(IDC_PRJBTN_ODDEFAULT, &KImportKTDBDlg::OnBnClickedPrjbtnODdefault)
	ON_BN_CLICKED(IDOK, &KImportKTDBDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KImportKTDBDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


BOOL KImportKTDBDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	if( m_pScenario )
	{
		CString strScenarioName;
		m_stcScenarioName.GetWindowText( strScenarioName );
		strScenarioName += m_pScenario->GetName();
		m_stcScenarioName.SetWindowText( strScenarioName );

		if (m_bTargetSingle)
		{
			m_mapTarget.insert(std::make_pair(m_pTargetSingle->GetObjectID(), m_pTargetSingle));
		}
		else
		{
			m_mapTarget = m_pScenario->GetTargetMap();
		}

		// jyk임시
		InitAnalysisArea();
		InitNetworkListControl();
		InitODDataListControl();
		UpdateNetworkListControl();
		UpdateODDataListControl();

		m_reportNetwork.Populate();
		m_reportOD.Populate();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void KImportKTDBDlg::InitAnalysisArea()
{
	KProject* pProject = (KProject*)( (CKmzApp*)AfxGetApp() )->GetProject();
	ASSERT( pProject );

	const KTDStringList& lstAnalysisArea = pProject->getAnalysisAreaList();

	if(lstAnalysisArea.size() == 0)
	{
		return;
	}

	m_strAnalysisArea = lstAnalysisArea.front();
}


void KImportKTDBDlg::InitNetworkListControl( void )
{
    KReportCtrlSetting::Default(m_reportNetwork, FALSE, FALSE, FALSE);
// 	m_reportNetwork.GetReportHeader()->AllowColumnRemove(FALSE);
// 	m_reportNetwork.GetReportHeader()->AllowColumnSort(FALSE);
// 	m_reportNetwork.GetReportHeader()->AllowColumnReorder(FALSE);
// 	m_reportNetwork.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);
// 	m_reportNetwork.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);

	// Add Columns
	CXTPReportColumn* pColumn = NULL;
	pColumn = m_reportNetwork.AddColumn(new CXTPReportColumn(0, _T("Target Year"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	pColumn->SetAlignment(DT_CENTER);

	KProject* pProject = (KProject*)( (CKmzApp*)AfxGetApp() )->GetProject();
	ASSERT( pProject );

	int nColumnIndex = 1;
	const KTDStringList& dataYearList = pProject->getDataYearList();
	KTDStringList::const_iterator citYear = dataYearList.begin();
	while( dataYearList.end() != citYear )
	{
		m_mapDataYear[nColumnIndex] =  _ttoi( (*citYear) );
        CString strDataHeader;
        strDataHeader.Format(_T("%s"), *citYear);
		pColumn = m_reportNetwork.AddColumn(new CXTPReportColumn(nColumnIndex, strDataHeader/*(*citYear)*/, 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);
		pColumn->SetAlignment(DT_CENTER);

		++citYear;
		++nColumnIndex;
	}

	pColumn = m_reportNetwork.AddColumn(new CXTPReportColumn(nColumnIndex, _T("Status"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	pColumn->SetAlignment(DT_CENTER);

	// Report control 설정
	//m_reportNetwork.SetMultipleSelection(TRUE);
	//m_reportNetwork.AllowEdit(TRUE);
	//m_reportNetwork.EditOnClick(TRUE);
}


void KImportKTDBDlg::InitODDataListControl( void )
{
    KReportCtrlSetting::Default(m_reportOD, FALSE, FALSE, FALSE);
// 	m_reportOD.GetReportHeader()->AllowColumnRemove(FALSE);
// 	m_reportOD.GetReportHeader()->AllowColumnSort(FALSE);
// 	m_reportOD.GetReportHeader()->AllowColumnReorder(FALSE);
// 	m_reportOD.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);
// 	m_reportOD.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);

	// Add Columns
	CXTPReportColumn* pColumn = NULL;
	pColumn = m_reportOD.AddColumn(new CXTPReportColumn(0, _T("Target Year"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	pColumn->SetAlignment(DT_CENTER);

	KProject* pProject = (KProject*)( (CKmzApp*)AfxGetApp() )->GetProject();
	ASSERT( pProject );

	int nColumnIndex = 1;
	const KTDStringList& dataYearList = pProject->getDataYearList();
	KTDStringList::const_iterator citYear = dataYearList.begin();
	while( dataYearList.end() != citYear )
	{
        CString strDataHeader;
        strDataHeader.Format(_T("%s"), *citYear);
		pColumn = m_reportOD.AddColumn(new CXTPReportColumn(nColumnIndex, strDataHeader, 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);
		pColumn->SetAlignment(DT_CENTER);

		++citYear;
		++nColumnIndex;
	}

	pColumn = m_reportOD.AddColumn(new CXTPReportColumn(nColumnIndex, _T("Status"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	pColumn->SetAlignment(DT_CENTER);

	// Report control 설정
	//m_reportOD.SetMultipleSelection(FALSE);
	//m_reportOD.AllowEdit(TRUE);
	//m_reportOD.EditOnClick(FALSE);
}


void KImportKTDBDlg::UpdateNetworkListControl( void )

{
	KProject* pProject = (KProject*)( (CKmzApp*)AfxGetApp() )->GetProject();
	ASSERT( pProject );

	const KTDStringList& dataYearList = pProject->getDataYearList();

	/// List row 추가
// 	const KTDTargetMap& targetMap = m_pScenario->GetTargetMap();
// 	KTDTargetMap::const_iterator citTargetMap = targetMap.begin();

	AutoType citTargetMap = m_mapTarget.begin();

	int nRow = 0;
	while( m_mapTarget.end() != citTargetMap )
	{
		KTarget* pTarget       = citTargetMap->second;
		CString  strTargetName = pTarget->GetName();
		int      nTartgetYear  = _ttoi(strTargetName);

		CXTPReportRecord* pRecord = m_reportNetwork.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem = NULL;

		// field - Target Year
		pRecord->SetEditable(FALSE);
		pItem = pRecord->AddItem( new CXTPReportRecordItemText(strTargetName) );
        pItem->SetBackgroundColor(RGB(229, 239, 255));
		// fields - Data Year
		TCheckDataInfoByRow* pTCheckDataInfo = new TCheckDataInfoByRow;
		KEMImportKTDB emImportKTDB = KEMImportNone;
		GetNetworkCheckInfo(pRecord, nTartgetYear, dataYearList, pTCheckDataInfo, emImportKTDB);
		// 'v' check
		m_mapCheckNetworkData[nRow] = pTCheckDataInfo;
		SetDataCheck(pTCheckDataInfo, pRecord);

		// field - 'Status'
		CString strStatus = GetStatus(emImportKTDB);
		pItem = pRecord->AddItem( new CXTPReportRecordItemText(strStatus) );

		++citTargetMap;
		++nRow;
	}
}

void KImportKTDBDlg::UpdateODDataListControl( void )
{
	KProject* pProject = (KProject*)( (CKmzApp*)AfxGetApp() )->GetProject();
	ASSERT( pProject );

	const KTDStringList& dataYearList = pProject->getDataYearList();

	/// List row 추가
// 	const KTDTargetMap& targetMap = m_pScenario->GetTargetMap();
// 	KTDTargetMap::const_iterator citTargetMap = targetMap.begin();

	AutoType citTargetMap = m_mapTarget.begin();

	int nRow = 0;
	while( m_mapTarget.end() != citTargetMap )
	{
		KTarget* pTarget = citTargetMap->second;
		CString strTargetName = pTarget->GetName();
		int nTartgetYear = _ttoi(strTargetName);

		CXTPReportRecord* pRecord = m_reportOD.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem;		

		// field - 'Target Year'
		pRecord->SetEditable(FALSE);
		pItem = pRecord->AddItem( new CXTPReportRecordItemText(strTargetName) );
		pItem->SetBackgroundColor(RGB(229, 239, 255));

		// fields - 'Data Year'
		TCheckDataInfoByRow* pTCheckDataInfo = new TCheckDataInfoByRow;
		KEMImportKTDB emImportKTDB = KEMImportNone;
		GetODCheckInfo(pRecord, nTartgetYear, dataYearList, pTCheckDataInfo, emImportKTDB);
		// 'v' check
		m_mapCheckODData[nRow] = pTCheckDataInfo;
		SetDataCheck(pTCheckDataInfo, pRecord);

		// field - 'Status'
		CString strStatus = GetStatus(emImportKTDB);
		pRecord->SetEditable(FALSE);
		pItem = pRecord->AddItem( new CXTPReportRecordItemText(strStatus) );

		++citTargetMap;
		++nRow;
	}
}


void KImportKTDBDlg::GetNetworkCheckInfo(CXTPReportRecord* a_pRecord, int a_nTartgetYear, const std::list< CString > &a_dataYearList,	// in
							TCheckDataInfoByRow* a_pTCheckDataInfo, KEMImportKTDB &a_emImportKTDB)										// out
{
	if(a_dataYearList.size() == 0)
	{
		a_emImportKTDB = KEMImportNone;
		a_pTCheckDataInfo = NULL;

		return;
	}

	int nGapMin = 10000;
	int nCopyYear = 0;
	int nColumnIndex = 1;
	int nCopyColumnIndex = 0;
	KTDStringList::const_iterator citYear = a_dataYearList.begin();
	while( a_dataYearList.end() != citYear )
	{
		int nDataYear = _ttoi( (*citYear) );
		int nGap = abs(nDataYear - a_nTartgetYear);

		CXTPReportRecordItem* pItem = a_pRecord->AddItem( new CXTPReportRecordItemText(DATA_UNCHECK) );
		if(nGapMin >= nGap)
		{
			nCopyColumnIndex = nColumnIndex;
			nCopyYear		 = nDataYear;
			nGapMin = nGap;
		}

		++citYear;
		++nColumnIndex;
	}

	a_emImportKTDB = KEMImportCopy;

	a_pTCheckDataInfo->nTotalCheckedCnt = 1;
	a_pTCheckDataInfo->mapCheckedDataYear[nCopyColumnIndex] = nCopyYear;
	a_pTCheckDataInfo->nTargetYear = a_nTartgetYear;
	a_pTCheckDataInfo->emImportKTDB = a_emImportKTDB;
}


void KImportKTDBDlg::GetODCheckInfo(CXTPReportRecord* a_pRecord, int a_nTartgetYear, const std::list< CString > &a_dataYearList,	// in
							TCheckDataInfoByRow* a_pTCheckDataInfo, KEMImportKTDB &a_emImportKTDB)									// out
{
	if(a_dataYearList.size() == 0)
	{
		a_emImportKTDB = KEMImportNone;
		a_pTCheckDataInfo = NULL;

		return;
	}

	std::vector<int> vecHighYear;
	std::vector<int> vecLowYear;
	int nColumnIndex = 1;
	int nCopyColumnIndex = 0;

	KTDStringList::const_iterator citYear = a_dataYearList.begin();
	while( a_dataYearList.end() != citYear )
	{
		CXTPReportRecordItem* pItem = a_pRecord->AddItem( new CXTPReportRecordItemText(DATA_UNCHECK) );

		int nDataYear = _ttoi( (*citYear) );
		if(nDataYear == a_nTartgetYear || a_dataYearList.size() == 1)
		{
			nCopyColumnIndex = nColumnIndex;
			a_emImportKTDB = KEMImportCopy;
		}
		else if(nDataYear > a_nTartgetYear)
		{
			vecHighYear.push_back( nColumnIndex );
		}
		else // nDataYear < a_nTargetYear 
		{
			vecLowYear.push_back( nColumnIndex );
		}

		++citYear;
		++nColumnIndex;
	} // end while

	if(a_emImportKTDB == KEMImportCopy)
	{
		a_pTCheckDataInfo->nTotalCheckedCnt = 1;
		//a_pTCheckDataInfo->mapCheckedDataYear[nCopyColumnIndex] = a_nTartgetYear;
		a_pTCheckDataInfo->mapCheckedDataYear[nCopyColumnIndex] = m_mapDataYear[nCopyColumnIndex];
		a_pTCheckDataInfo->nTargetYear = a_nTartgetYear;
		a_pTCheckDataInfo->emImportKTDB = a_emImportKTDB;

		return;
	}

	std::vector<int>::iterator itIndexStart, itIndexEnd;
 	a_emImportKTDB = GetInterExtra(vecHighYear, vecLowYear, itIndexStart, itIndexEnd);

	a_pTCheckDataInfo->nTotalCheckedCnt = 2;
	a_pTCheckDataInfo->mapCheckedDataYear[*itIndexStart] = m_mapDataYear[*itIndexStart];	// 체크된 colum index에 대한 년도 가져오기
	a_pTCheckDataInfo->mapCheckedDataYear[*itIndexEnd]   = m_mapDataYear[*itIndexEnd];
	a_pTCheckDataInfo->nTargetYear = a_nTartgetYear;
	a_pTCheckDataInfo->emImportKTDB = a_emImportKTDB;
}


KEMImportKTDB KImportKTDBDlg::GetInterExtra(std::vector<int> &a_vecHighYear, std::vector<int> &a_vecLowYear,
	std::vector<int>::iterator &a_itIndexStart, std::vector<int>::iterator &a_itIndexEnd)
{
	KEMImportKTDB emImportKTDB;

	if(a_vecHighYear.size() == 0)
	{
		emImportKTDB = KEMImportExtrapolation;

		a_itIndexEnd	= a_vecLowYear.end() -1;
		a_itIndexStart = a_vecLowYear.end() -2;
	}
	else if(a_vecLowYear.size() == 0)
	{
		emImportKTDB = KEMImportExtrapolation;

		a_itIndexStart = a_vecHighYear.begin();
		a_itIndexEnd	= a_vecHighYear.begin() +1;
	}
	else 
	{
		emImportKTDB = KEMImportInterpolation;

		a_itIndexStart = a_vecLowYear.end() -1;
		a_itIndexEnd	= a_vecHighYear.begin();
	}

	return emImportKTDB;
}


void KImportKTDBDlg::SetDataCheck(TCheckDataInfoByRow* a_pTCheckDataInfo, CXTPReportRecord* pRecord)
{
	KTDDataYearMap mapDataYear =  a_pTCheckDataInfo->mapCheckedDataYear;

	KTDDataYearMap::iterator itData, itEnd = mapDataYear.end();
	for(itData = mapDataYear.begin(); itData != itEnd; ++itData)
	{
		CXTPReportRecordItem *pItem =  pRecord->GetItem(itData->first);
		pItem->SetCaption(DATA_CHECK);
	}
}


CString KImportKTDBDlg::GetStatus(KEMImportKTDB a_emImportKTDB)
{
	CString strStatus = _T("");

	switch(a_emImportKTDB)
	{
	case KEMImportNone:
		strStatus = _T("-");
		break;
	case KEMImportCopy:
		strStatus = _T("Copy");
		break;
	case KEMImportInterpolation:
		strStatus = _T("Inter");
		break;
	case KEMImportExtrapolation:
		strStatus = _T("Extra");
		break;
	}

	return strStatus;
}

/**************************************
* Message handlers                    *
**************************************/
void KImportKTDBDlg::OnNetworkDataClick(NMHDR * a_pNotifyStruct, LRESULT * a_result)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) a_pNotifyStruct;
	if(!pItemNotify->pRow || !pItemNotify->pColumn)
	{
		return;
	}

	int nSelectedColIndex = pItemNotify->pColumn->GetItemIndex();
	int nSelectedRowIndex = pItemNotify->pRow->GetIndex();

	CXTPReportRecord* pRecord = pItemNotify->pRow->GetRecord();
	int nColumnCount = pRecord->GetItemCount();

	// 'target', 'status' 필드 는 제외 
	if( nSelectedColIndex == 0 || nSelectedColIndex == (nColumnCount -1) )
	{
		return;
	}

	CXTPReportRecordItem* pItem = pRecord->GetItem(nSelectedColIndex);
	if( DATA_CHECK == pItem->GetCaption(NULL) )
	{
		return;
	}

	for(int i= 1; i< nColumnCount-1; i++)
	{
		CString strSetting = DATA_UNCHECK;
		if(i == nSelectedColIndex)
		{
			strSetting = DATA_CHECK;
		}
		else 
		{
			strSetting = DATA_UNCHECK;
		}
		pItem = pRecord->GetItem(i);
		pItem->SetCaption(strSetting);
	}

	TCheckDataInfoByRow* pTCheckData = m_mapCheckNetworkData[nSelectedRowIndex];
	pTCheckData->mapCheckedDataYear.clear();
	pTCheckData->mapCheckedDataYear[nSelectedColIndex] = m_mapDataYear[nSelectedColIndex];
}

void KImportKTDBDlg::OnODDataClick(NMHDR * a_pNotifyStruct, LRESULT * a_result)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) a_pNotifyStruct;
	if(!pItemNotify->pRow || !pItemNotify->pColumn)
	{
		return;
	}

	int nSelectedColIndex = pItemNotify->pColumn->GetItemIndex();
	int nSelectedRowIndex = pItemNotify->pRow->GetIndex();

	CXTPReportRecord* pRecord = pItemNotify->pRow->GetRecord();
	int nColumnCount = pRecord->GetItemCount();

	// 'target', 'status' 필드 는 제외 
	if( nSelectedColIndex == 0 || nSelectedColIndex == (nColumnCount -1) )
	{
		return;
	}
	
	TCheckDataInfoByRow* pTCheckData = m_mapCheckODData[nSelectedRowIndex];
	CXTPReportRecordItem* pItem = pRecord->GetItem(nSelectedColIndex);
	if( DATA_CHECK == pItem->GetCaption(NULL) )
	{
		pItem->SetCaption(DATA_UNCHECK);

		pTCheckData->nTotalCheckedCnt--;
		pTCheckData->mapCheckedDataYear.erase(nSelectedColIndex);
	} 
	else 
	{
		if(pTCheckData->nTotalCheckedCnt >= 2)
		{
			return;
		}
		pItem->SetCaption(DATA_CHECK);

		pTCheckData->nTotalCheckedCnt++; 
		pTCheckData->mapCheckedDataYear[nSelectedColIndex] = m_mapDataYear[nSelectedColIndex];
	}

	//-- status check
	KEMImportKTDB emImportKTDB = SetODDataStatus(pTCheckData);
	pTCheckData->emImportKTDB = emImportKTDB;

	CString strStatus = GetStatus(emImportKTDB);
	pItem = pRecord->GetItem(nColumnCount -1);	// 'status' field
	pItem->SetCaption(strStatus);
}


KEMImportKTDB KImportKTDBDlg::SetODDataStatus(TCheckDataInfoByRow* a_pTCheckData)
{
	KEMImportKTDB emImportKTDB;
	if(0 == a_pTCheckData->nTotalCheckedCnt)
	{
		emImportKTDB = KEMImportNone;
		return emImportKTDB;
	}
	else if(1 == a_pTCheckData->nTotalCheckedCnt)
	{
		emImportKTDB = KEMImportCopy;
		return emImportKTDB;
	}

	int nTargetYear = a_pTCheckData->nTargetYear;
	int nFlag = -1;  // 0:작다, 1:크다, 2:같다.
	int nTemp = -1;

	KTDDataYearMap::iterator itCheckYear, itEnd = a_pTCheckData->mapCheckedDataYear.end();
	for(itCheckYear = a_pTCheckData->mapCheckedDataYear.begin(); itCheckYear != itEnd; ++itCheckYear )
	{
		int nCheckedDataYear = itCheckYear->second;

		if( nCheckedDataYear > nTargetYear)
		{
			nFlag = 1;
		}
		else if(nCheckedDataYear < nTargetYear) 
		{
			nFlag = 0;
		}
		else 
		{
			nFlag = 2;
		}

		if(nTemp == nFlag)
		{
			emImportKTDB = KEMImportExtrapolation;
		}
		else 
		{
			emImportKTDB = KEMImportInterpolation;
		}
		nTemp = nFlag;
	}

	return emImportKTDB;
}

void KImportKTDBDlg::OnBnClickedPrjbtnNetworkdefault()
{
	CXTPReportRecords *pRecords = m_reportNetwork.GetRecords();
	pRecords->RemoveAll();

	KTDCheckDataMap::iterator itData, itEnd = m_mapCheckNetworkData.end();
	for(itData = m_mapCheckNetworkData.begin(); itData != itEnd; ++itData)
	{
		delete itData->second;
	}
	m_mapCheckNetworkData.clear();

	UpdateNetworkListControl();
	m_reportNetwork.Populate();
}


void KImportKTDBDlg::OnBnClickedPrjbtnODdefault()
{
	CXTPReportRecords *pRecords = m_reportOD.GetRecords();
	pRecords->RemoveAll();

	KTDCheckDataMap::iterator itData, itEnd = m_mapCheckODData.end();
	for(itData = m_mapCheckODData.begin(); itData != itEnd; ++itData)
	{
		delete itData->second;
	}
	m_mapCheckODData.clear();

	UpdateODDataListControl();
	m_reportOD.Populate();
}


void KImportKTDBDlg::OnBnClickedOk()
{
	KProject* pProject = (KProject*)( (CKmzApp*)AfxGetApp() )->GetProject();
	ASSERT( pProject );

	const KTDStringList &lstDataYear = pProject->getDataYearList(); 
	if(lstDataYear.size() == 0)
	{
		KDialogEx::OnOK();
		return;
	}

	KTDCheckDataMap::iterator iterData, itEnd = m_mapCheckODData.end();
	
	for(iterData = m_mapCheckODData.begin(); iterData != itEnd; ++iterData)
	{
		TCheckDataInfoByRow* pTCheckODData = iterData->second;
		KEMImportKTDB &emImportKTDB = pTCheckODData->emImportKTDB;

		if(KEMImportExtrapolation == emImportKTDB || KEMImportInterpolation == emImportKTDB || KEMImportCopy == emImportKTDB )
		{
			MakeODSelectedYearList(pTCheckODData->mapCheckedDataYear);
		}
	
		if(KEMImportNone == emImportKTDB)		// OD status 가 'none' 이면 network 에서 선택한 year를 기준으로 'copy' 할 수 있도록 재설정 한다.
		{
			int nIdx = iterData->first;
			TCheckDataInfoByRow* pTCheckNetworkData = m_mapCheckNetworkData[nIdx];
			
			*pTCheckODData = *pTCheckNetworkData;
			MakeODSelectedYearList(pTCheckODData->mapCheckedDataYear);
		}
	}

	KTDCheckDataMap::iterator iter, end = m_mapCheckNetworkData.end();
	
	for(iter = m_mapCheckNetworkData.begin(); iter != end; ++iter)
	{
		TCheckDataInfoByRow* pTCheckNetworkData = iter->second;
		KEMImportKTDB &emImportKTDB = pTCheckNetworkData->emImportKTDB;

		if(KEMImportCopy == emImportKTDB )
		{
			MakeNetworkSelectedYearList(pTCheckNetworkData->mapCheckedDataYear);
		}
	}

	m_vecPurpose = pProject->GetPurpose();
	m_vecMode	 = pProject->GetMode();

	KDialogEx::OnOK();
}

void KImportKTDBDlg::OnBnClickedCancel()
{
	KDialogEx::OnCancel();
}


void KImportKTDBDlg::MakeODSelectedYearList(KTDDataYearMap &a_mapCheckedDataYear)
{
	CString strYear = _T("");

	KTDDataYearMap::iterator iterData, itEnd = a_mapCheckedDataYear.end();
	for(iterData = a_mapCheckedDataYear.begin(); iterData != itEnd; ++iterData)
	{
	
		strYear.Format(_T("%d"), iterData->second);
		m_mapTotalODSelectedYear[strYear] = iterData->second;
	}
}


void KImportKTDBDlg::MakeNetworkSelectedYearList(KTDDataYearMap &a_mapCheckedDataYear)
{
	CString strYear = _T("");

	KTDDataYearMap::iterator iterData, itEnd = a_mapCheckedDataYear.end();
	for(iterData = a_mapCheckedDataYear.begin(); iterData != itEnd; ++iterData)
	{

		strYear.Format(_T("%d"), iterData->second);
		m_mapTotalNetworkSelectedYear[strYear] = iterData->second;
	}
}
