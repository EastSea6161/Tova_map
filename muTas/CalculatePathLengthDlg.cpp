// CalculatePathLengthDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CalculatePathLengthDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "Purpose.h"
#include "PurposeInfoCollection.h"
#include "IncludeIOTable.h"
#include "PurposeManager.h"
#include "PAPurpose.h"
#include "../kmzModel/KShortestPath.h"
#include "../kmzModel/KLinkCollection.h"
#include "../kmzModel/KCostLinkCollection.h"
#include "../kmzModel/KNodeArrivedMinLink.h"
#include "DBaseConnection.h"
#include "ShortestPathInfo.h"
#include "KBulkDBase.h"
#include "SimpleProgress.h"
#include "PathCalculateCommdityDlg.h"

/* define column index */
#define PathLength_PURPOSE		0
#define PathLength_SELECTION	1

#define Link_Type_Zone               0
#define LInk_Type_Highway            1
#define LInk_Type_Expressway         2
#define LInk_Type_GeneralRoad        3
#define LInk_Type_metropolitanRoad   4
#define LInk_Type_StateLocalRoad     5
#define LInk_Type_LocalRoad          6
#define LInk_Type_SiGunRoad          7
#define LInk_Type_AccessRoad         8
#define LInk_Type_RailWay            9
#define LInk_Type_ShippingWay        70
#define LInk_Type_Shuttle            71
								    
#define DEFUALT_DISTANCE  5.0

using namespace CalculatePathLength_Structure;

// KCalculatePathLengthDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KCalculatePathLengthDlg, CDialogEx)

KCalculatePathLengthDlg::KCalculatePathLengthDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(KCalculatePathLengthDlg::IDD, pParent)
	, m_strEdtAvailableCommodity(_T(""))
{

}

KCalculatePathLengthDlg::~KCalculatePathLengthDlg()
{
}

void KCalculatePathLengthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cboRailStartLength);
	DDX_Control(pDX, IDC_COMBO2, m_cboRailMiddleLength);
	DDX_Control(pDX, IDC_COMBO3, m_cboRailEndLength);
	DDX_Control(pDX, IDC_COMBO4, m_cboRoadDirectLength);
	DDX_Control(pDX, IDC_COMBO6, m_cboRoadFStartLength);
	DDX_Control(pDX, IDC_COMBO7, m_cboRoadFEndLength);
	DDX_Control(pDX, IDC_COMBO8, m_cboRailStartFacilityNode);
	DDX_Control(pDX, IDC_COMBO9, m_cboRailEndFacilityNode);
	DDX_Control(pDX, IDC_COMBO10, m_cboRoadFacilityNode);
	DDX_Control(pDX, IDC_COMBO11, m_cboRailStartFacilityZone);
	DDX_Control(pDX, IDC_COMBO12, m_cboRailEndFacilityZone);
	DDX_Control(pDX, IDC_COMBO13, m_cboRoadFacilityZone);

	DDX_Control(pDX, IDC_CHECK1, m_chkRailStartLength);
	DDX_Control(pDX, IDC_CHECK2, m_chkRailMiddleLength);
	DDX_Control(pDX, IDC_CHECK3, m_chkRailEndLength);
	DDX_Control(pDX, IDC_CHECK4, m_chkRoadDirectLength);
	DDX_Control(pDX, IDC_CHECK5, m_chkRoadFStartLength);
	DDX_Control(pDX, IDC_CHECK6, m_chkRoadFEndLength);
	DDX_Control(pDX, IDC_CHECK7, m_chkRailStartFacilityNode);
	DDX_Control(pDX, IDC_CHECK8, m_chkRailEndFacilityNode);
	DDX_Control(pDX, IDC_CHECK9, m_chkRoadFacilityNode);
	DDX_Control(pDX, IDC_CHECK10, m_chkRailStartFacilityZone);
	DDX_Control(pDX, IDC_CHECK11, m_chkRailEndFacilityZone);
	DDX_Control(pDX, IDC_CHECK12, m_chkRoadFacilityZone);
	DDX_Text(pDX, IDC_EDIT1, m_strEdtAvailableCommodity);
}


BEGIN_MESSAGE_MAP(KCalculatePathLengthDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &KCalculatePathLengthDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &KCalculatePathLengthDlg::OnBnClickedSelectCommdity)
	ON_BN_CLICKED(IDC_CHECK1, &KCalculatePathLengthDlg::OnBnClickedRailOrigin)
	ON_BN_CLICKED(IDC_CHECK2, &KCalculatePathLengthDlg::OnBnClickedCheckRail)
	ON_BN_CLICKED(IDC_CHECK3, &KCalculatePathLengthDlg::OnBnClickedRailDestination)
	ON_BN_CLICKED(IDC_CHECK4, &KCalculatePathLengthDlg::OnBnClickedRoadDirect)
	ON_BN_CLICKED(IDC_CHECK5, &KCalculatePathLengthDlg::OnBnClickedRoadOrigin)
	ON_BN_CLICKED(IDC_CHECK6, &KCalculatePathLengthDlg::OnBnClickedRoadDestination)
	ON_BN_CLICKED(IDC_CHECK7, &KCalculatePathLengthDlg::OnBnClickedRailOriginNode)
	ON_BN_CLICKED(IDC_CHECK8, &KCalculatePathLengthDlg::OnBnClickedRailDestinatinoNode)
	ON_BN_CLICKED(IDC_CHECK9, &KCalculatePathLengthDlg::OnBnClickedRoadFacilityNode)
	ON_BN_CLICKED(IDC_CHECK10, &KCalculatePathLengthDlg::OnBnClickedRailOriginZone)
	ON_BN_CLICKED(IDC_CHECK11, &KCalculatePathLengthDlg::OnBnClickedRailDestinationZone)
	ON_BN_CLICKED(IDC_CHECK12, &KCalculatePathLengthDlg::OnBnClickedRoadFacilityZone)
END_MESSAGE_MAP()



void KCalculatePathLengthDlg::SetInfo( KTarget* a_pTarget, KIOTable* a_pTable )
{
	m_pTarget = a_pTarget;
	m_pTable  = a_pTable;
}


BOOL KCalculatePathLengthDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetParameterDoubleColumnList();
	//GetParameterIntColumnList();
	insertCombobox();
	InitComboState();
	return TRUE;
}



void KCalculatePathLengthDlg::GetParameterDoubleColumnList()
{
	initColumnList();
	const KIOColumnCollection* pColumnCollection = m_pTable->Columns();
	for(int i = 0; i < pColumnCollection->ColumnCount(); ++i)
	{
		KIOColumn* pColumn = pColumnCollection->GetColumn(i);
		int nDataType = pColumn->DataType();
		if (nDataType == 2)
		{
			m_vecParameterDoubleColumn.push_back(pColumn);
		}
	}
}

void KCalculatePathLengthDlg::GetParameterIntColumnList()
{
	initColumnList();
	const KIOColumnCollection* pColumnCollection = m_pTable->Columns();
	for(int i = 0; i < pColumnCollection->ColumnCount(); ++i)
	{
		KIOColumn* pColumn = pColumnCollection->GetColumn(i);
		int nDataType = pColumn->DataType();
		bool bPrimaryKey = pColumn->PrimaryKey();
		if (nDataType == 1 && bPrimaryKey == false )
		{
			m_vecParameterIntColumn.push_back(pColumn);
		}
	}
}

void KCalculatePathLengthDlg::initColumnList()
{
	size_t nxCount = m_vecParameterDoubleColumn.size();
	for (size_t i=0; i<nxCount; i++)
	{
		 KIOColumn* pColumn = m_vecParameterDoubleColumn[i];
		 delete pColumn;
	}
	m_vecParameterDoubleColumn.clear();
}



void KCalculatePathLengthDlg::insertCombobox()
{
	int nComboIndex;

	nComboIndex	= m_cboRailStartLength.InsertString(-1, _T("----OutPut Field 선택----"));
	m_cboRailStartLength.SetItemData(nComboIndex, (DWORD_PTR)NULL);

	nComboIndex	= m_cboRailMiddleLength.InsertString(-1, _T("----OutPut Field 선택----"));
	m_cboRailMiddleLength.SetItemData(nComboIndex, (DWORD_PTR)NULL);

	nComboIndex	= m_cboRailEndLength.InsertString(-1, _T("----OutPut Field 선택----"));
	m_cboRailEndLength.SetItemData(nComboIndex, (DWORD_PTR)NULL);

	nComboIndex	= m_cboRoadDirectLength.InsertString(-1, _T("----OutPut Field 선택----"));
	m_cboRoadDirectLength.SetItemData(nComboIndex, (DWORD_PTR)NULL);

	nComboIndex	= m_cboRoadFStartLength.InsertString(-1, _T("----OutPut Field 선택----"));
	m_cboRoadFStartLength.SetItemData(nComboIndex, (DWORD_PTR)NULL);

	nComboIndex	= m_cboRoadFEndLength.InsertString(-1, _T("----OutPut Field 선택----"));
	m_cboRoadFEndLength.SetItemData(nComboIndex, (DWORD_PTR)NULL);

	nComboIndex	= m_cboRailStartFacilityNode.InsertString(-1, _T("----OutPut Field 선택----"));
	m_cboRailStartFacilityNode.SetItemData(nComboIndex, (DWORD_PTR)NULL);

	nComboIndex	= m_cboRailEndFacilityNode.InsertString(-1, _T("----OutPut Field 선택----"));
	m_cboRailEndFacilityNode.SetItemData(nComboIndex, (DWORD_PTR)NULL);

	nComboIndex	= m_cboRoadFacilityNode.InsertString(-1, _T("----OutPut Field 선택----"));
	m_cboRoadFacilityNode.SetItemData(nComboIndex, (DWORD_PTR)NULL);

	nComboIndex	= m_cboRailStartFacilityZone.InsertString(-1, _T("----OutPut Field 선택----"));
	m_cboRailStartFacilityZone.SetItemData(nComboIndex, (DWORD_PTR)NULL);

	nComboIndex	= m_cboRailEndFacilityZone.InsertString(-1, _T("----OutPut Field 선택----"));
	m_cboRailEndFacilityZone.SetItemData(nComboIndex, (DWORD_PTR)NULL);

	nComboIndex	= m_cboRoadFacilityZone.InsertString(-1, _T("----OutPut Field 선택----"));
	m_cboRoadFacilityZone.SetItemData(nComboIndex, (DWORD_PTR)NULL);
	
	size_t nxdoubleCount = m_vecParameterDoubleColumn.size();
	for (size_t i = 0; i<nxdoubleCount; i++)
	{
		KIOColumn* pColumn = m_vecParameterDoubleColumn[i];

		nComboIndex	= m_cboRailStartLength.InsertString(-1, pColumn->Caption());
		m_cboRailStartLength.SetItemData(nComboIndex, (DWORD_PTR)NULL);

		nComboIndex	= m_cboRailMiddleLength.InsertString(-1,  pColumn->Caption());
		m_cboRailMiddleLength.SetItemData(nComboIndex, (DWORD_PTR)NULL);

		nComboIndex	= m_cboRailEndLength.InsertString(-1,  pColumn->Caption());
		m_cboRailEndLength.SetItemData(nComboIndex, (DWORD_PTR)NULL);

		nComboIndex	= m_cboRoadDirectLength.InsertString(-1,  pColumn->Caption());
		m_cboRoadDirectLength.SetItemData(nComboIndex, (DWORD_PTR)NULL);

		nComboIndex	= m_cboRoadFStartLength.InsertString(-1, pColumn->Caption());
		m_cboRoadFStartLength.SetItemData(nComboIndex, (DWORD_PTR)NULL);

		nComboIndex	= m_cboRoadFEndLength.InsertString(-1,  pColumn->Caption());
		m_cboRoadFEndLength.SetItemData(nComboIndex, (DWORD_PTR)NULL);

		nComboIndex	= m_cboRailStartFacilityNode.InsertString(-1,  pColumn->Caption());
		m_cboRailStartFacilityNode.SetItemData(nComboIndex, (DWORD_PTR)NULL);

		nComboIndex	= m_cboRailEndFacilityNode.InsertString(-1, pColumn->Caption());
		m_cboRailEndFacilityNode.SetItemData(nComboIndex, (DWORD_PTR)NULL);

		nComboIndex	= m_cboRoadFacilityNode.InsertString(-1,  pColumn->Caption());
		m_cboRoadFacilityNode.SetItemData(nComboIndex, (DWORD_PTR)NULL);

		nComboIndex	= m_cboRailStartFacilityZone.InsertString(-1,  pColumn->Caption());
		m_cboRailStartFacilityZone.SetItemData(nComboIndex, (DWORD_PTR)NULL);

		nComboIndex	= m_cboRailEndFacilityZone.InsertString(-1,  pColumn->Caption());
		m_cboRailEndFacilityZone.SetItemData(nComboIndex, (DWORD_PTR)NULL);

		nComboIndex	= m_cboRoadFacilityZone.InsertString(-1,  pColumn->Caption());
		m_cboRoadFacilityZone.SetItemData(nComboIndex, (DWORD_PTR)NULL);
		
	}	

}

void KCalculatePathLengthDlg::SaveButtonControlInfo()
{
	// 정보를 담는 vec 초기화
	m_vecPathInfo.clear();

	//step 1. 셔틀및 도로거리 (기점) 정보 담기
	int           nCurIndex   = 0;
	CString   strColumnName   = _T("");
	STPathInfo                  oSTPathInfo;
	oSTPathInfo.eumPathCase   = KEMRailStartLength;
	if (m_chkRailStartLength.GetCheck() == 1)
	{
		oSTPathInfo.bSelected = true;
	}
	else
	{
		oSTPathInfo.bSelected = false;
	}
	nCurIndex  = m_cboRailStartLength.GetCurSel();
	if (nCurIndex != 0)
	{
		m_cboRailStartLength.GetLBText(nCurIndex,strColumnName);
		oSTPathInfo.strColumnName = strColumnName;
		oSTPathInfo.pColumn       = findDoubleIOColumnInfo(strColumnName);
		oSTPathInfo.nOutPutMathing = RailS;
	}
	m_vecPathInfo.push_back(oSTPathInfo);


	//step 2. 철도 운송 거리 
	InitPathInfo(oSTPathInfo);
	oSTPathInfo.eumPathCase   = KEMRailMiddleLength;
	if (m_chkRailMiddleLength.GetCheck() == 1)
	{
		oSTPathInfo.bSelected = true;
	}
	else
	{
		oSTPathInfo.bSelected = false;
	}
	nCurIndex  = m_cboRailMiddleLength.GetCurSel();
	if (nCurIndex != 0)
	{
		m_cboRailMiddleLength.GetLBText(nCurIndex,strColumnName);
		oSTPathInfo.strColumnName = strColumnName;
		oSTPathInfo.pColumn       = findDoubleIOColumnInfo(strColumnName);
		oSTPathInfo.nOutPutMathing = RailM;
	}
	m_vecPathInfo.push_back(oSTPathInfo);


	//step 3.셔틀및 도로거리 (종점)
	InitPathInfo(oSTPathInfo);
	oSTPathInfo.eumPathCase   = KEMRailEndLength;
	if (m_chkRailEndLength.GetCheck() == 1)
	{
		oSTPathInfo.bSelected = true;
	}
	else
	{
		oSTPathInfo.bSelected = false;
	}
	nCurIndex  = m_cboRailEndLength.GetCurSel();
	if (nCurIndex != 0)
	{
		m_cboRailEndLength.GetLBText(nCurIndex,strColumnName);
		oSTPathInfo.strColumnName = strColumnName;
		oSTPathInfo.pColumn       = findDoubleIOColumnInfo(strColumnName);
		oSTPathInfo.nOutPutMathing = RailE;
	}
	m_vecPathInfo.push_back(oSTPathInfo);


	//step 4.물류시설 비경유 거리
	InitPathInfo(oSTPathInfo);
	oSTPathInfo.eumPathCase   = KEMRailRoadDirectLength;
	if (m_chkRoadDirectLength.GetCheck() == 1)
	{
		oSTPathInfo.bSelected = true;
	}
	else
	{
		oSTPathInfo.bSelected = false;
	}
	nCurIndex = m_cboRoadDirectLength.GetCurSel();
	if (nCurIndex != 0)
	{
		m_cboRoadDirectLength.GetLBText(nCurIndex,strColumnName);
		oSTPathInfo.strColumnName = strColumnName;
		oSTPathInfo.pColumn       = findDoubleIOColumnInfo(strColumnName);
		oSTPathInfo.nOutPutMathing = RoadD;
	}

	m_vecPathInfo.push_back(oSTPathInfo);

	//step 5.물류시설 경유(기점)
	InitPathInfo(oSTPathInfo);
	oSTPathInfo.eumPathCase   = KEMRailRoadFStartLength;
	if (m_chkRoadFStartLength.GetCheck() == 1)
	{
		oSTPathInfo.bSelected = true;
	}
	else
	{
		oSTPathInfo.bSelected = false;
	}
	nCurIndex = m_cboRoadFStartLength.GetCurSel();
	if (nCurIndex != 0)
	{
		m_cboRoadFStartLength.GetLBText(nCurIndex,strColumnName);
		oSTPathInfo.strColumnName = strColumnName;
		oSTPathInfo.pColumn       = findDoubleIOColumnInfo(strColumnName);
		oSTPathInfo.nOutPutMathing = RoadS;
	}
	m_vecPathInfo.push_back(oSTPathInfo);

	//step 6. 물류시설 경유(종점)
	InitPathInfo(oSTPathInfo);
	oSTPathInfo.eumPathCase   = KEMRailRoadFEndLength;
	if (m_chkRoadFEndLength.GetCheck() == 1)
	{
		oSTPathInfo.bSelected = true;
	}
	else
	{
		oSTPathInfo.bSelected = false;
	}
	nCurIndex = m_cboRoadFEndLength.GetCurSel();
	if (nCurIndex != 0)
	{
		m_cboRoadFEndLength.GetLBText(nCurIndex,strColumnName);
		oSTPathInfo.strColumnName = strColumnName;
		oSTPathInfo.pColumn       = findDoubleIOColumnInfo(strColumnName);
		oSTPathInfo.nOutPutMathing = RoadE;
	}
	m_vecPathInfo.push_back(oSTPathInfo);


	//step 7. 철도 기점 물류시설 노드 정보
	InitPathInfo(oSTPathInfo);
	oSTPathInfo.eumPathCase   = KEMRailRailStartFacilityNode;
	if (m_chkRailStartFacilityNode.GetCheck() == 1)
	{
		oSTPathInfo.bSelected = true;
	}
	else
	{
		oSTPathInfo.bSelected = false;
	}
	nCurIndex = m_cboRailStartFacilityNode.GetCurSel();
	if (nCurIndex != 0)
	{
		m_cboRailStartFacilityNode.GetLBText(nCurIndex,strColumnName);
		oSTPathInfo.strColumnName = strColumnName;
		oSTPathInfo.pColumn       = findDoubleIOColumnInfo(strColumnName);
		oSTPathInfo.nOutPutMathing = RailFSN;
	}
	m_vecPathInfo.push_back(oSTPathInfo);


	//step 8. 철도 종점 물류시설 노드
	InitPathInfo(oSTPathInfo);
	oSTPathInfo.eumPathCase   = KEMRailEndFacilityNode;
	if (m_chkRailEndFacilityNode.GetCheck() == 1)
	{
		oSTPathInfo.bSelected = true;
	}
	else
	{
		oSTPathInfo.bSelected = false;
	}
	nCurIndex = m_cboRailEndFacilityNode.GetCurSel();
	if (nCurIndex != 0)
	{
		m_cboRailEndFacilityNode.GetLBText(nCurIndex,strColumnName);
		oSTPathInfo.strColumnName = strColumnName;
		oSTPathInfo.pColumn       = findDoubleIOColumnInfo(strColumnName);
		oSTPathInfo.nOutPutMathing = RailFEN;
	}
	m_vecPathInfo.push_back(oSTPathInfo);


	//step 9. 육송 물류시설 노드
	InitPathInfo(oSTPathInfo);
	oSTPathInfo.eumPathCase   = KEMRoadFacilityNode;
	if (m_chkRoadFacilityNode.GetCheck() == 1)
	{
		oSTPathInfo.bSelected = true;
	}
	else
	{
		oSTPathInfo.bSelected = false;
	}
	nCurIndex                 = m_cboRoadFacilityNode.GetCurSel();
	if (nCurIndex != 0)
	{
		m_cboRoadFacilityNode.GetLBText(nCurIndex,strColumnName);
		oSTPathInfo.strColumnName = strColumnName;
		oSTPathInfo.pColumn       = findDoubleIOColumnInfo(strColumnName);
		oSTPathInfo.nOutPutMathing = RoadFN;
	}
	m_vecPathInfo.push_back(oSTPathInfo);


	//step 10.철도 기점 물류시설 존 정보
	InitPathInfo(oSTPathInfo);
	oSTPathInfo.eumPathCase   = KEMRailStartFacilityZone;
	if (m_chkRailStartFacilityZone.GetCheck() == 1)
	{
		oSTPathInfo.bSelected = true;
	}
	else
	{
		oSTPathInfo.bSelected = false;
	}
	nCurIndex  = m_cboRailStartFacilityZone.GetCurSel();
	if (nCurIndex != 0)
	{
		m_cboRailStartFacilityZone.GetLBText(nCurIndex,strColumnName);
		oSTPathInfo.strColumnName = strColumnName;
		oSTPathInfo.pColumn       = findDoubleIOColumnInfo(strColumnName);
		oSTPathInfo.nOutPutMathing = RailFSZ;
	}
	m_vecPathInfo.push_back(oSTPathInfo);


	//step 11. 철도 종점 물류시설 존
	InitPathInfo(oSTPathInfo);
	oSTPathInfo.eumPathCase   = KEMRailEndFacilityZone;
	if (m_chkRailEndFacilityZone.GetCheck() == 1)
	{
		oSTPathInfo.bSelected = true;
	}
	else
	{
		oSTPathInfo.bSelected = false;
	}
	nCurIndex = m_cboRailEndFacilityZone.GetCurSel();
	if (nCurIndex != 0)
	{
		m_cboRailEndFacilityZone.GetLBText(nCurIndex,strColumnName);
		oSTPathInfo.strColumnName = strColumnName;
		oSTPathInfo.pColumn       = findDoubleIOColumnInfo(strColumnName);
		oSTPathInfo.nOutPutMathing = RailFEZ;
	}
	m_vecPathInfo.push_back(oSTPathInfo);


	//step 12.육송 물류시설 존
	InitPathInfo(oSTPathInfo);
	oSTPathInfo.eumPathCase   = KEMRoadFacilityZone;
	if (m_chkRoadFacilityZone.GetCheck() == 1)
	{
		oSTPathInfo.bSelected = true;
	}
	else
	{
		oSTPathInfo.bSelected = false;
	}
	nCurIndex  = m_cboRoadFacilityZone.GetCurSel();
	if (nCurIndex != 0)
	{
		m_cboRoadFacilityZone.GetLBText(nCurIndex,strColumnName);
		oSTPathInfo.strColumnName = strColumnName;
		oSTPathInfo.pColumn       = findDoubleIOColumnInfo(strColumnName);
		oSTPathInfo.nOutPutMathing = RoadFZ;

	}
	m_vecPathInfo.push_back(oSTPathInfo);

}


KIOColumn* KCalculatePathLengthDlg::findDoubleIOColumnInfo( CString a_strColumnName )
{
	size_t nxCount = m_vecParameterDoubleColumn.size();
	for (size_t i = 0; i<nxCount; i++)
	{
		KIOColumn* pColumn = m_vecParameterDoubleColumn[i];
		if (pColumn->Caption() == a_strColumnName)
		{
			return pColumn;
		}
	}
	return NULL;
}



KIOColumn* KCalculatePathLengthDlg::findIntIOColumnInfo( CString a_strColumnName )
{
	size_t nxCount = m_vecParameterDoubleColumn.size();
	for (size_t i = 0; i<nxCount; i++)
	{
		KIOColumn* pColumn = m_vecParameterDoubleColumn[i];
		if (pColumn->Caption() == a_strColumnName)
		{
			pColumn->DataType(KEMIODataTypeInteger);
			return pColumn;
		}
	}
	return NULL;
}


void KCalculatePathLengthDlg::InitPathInfo( STPathInfo& a_StPathInfo )
{
	a_StPathInfo.bSelected     = false;
	a_StPathInfo.eumPathCase   = KEMRailRoadNONE;
	a_StPathInfo.pColumn       = NULL;
	a_StPathInfo.strColumnName = _T("");
}


void KCalculatePathLengthDlg::OnBnClickedOk()
{
	KLogDebugStartMsg();
	SaveButtonControlInfo();

   if(CheckSelectCommodity() == false)
	{
		AfxMessageBox(_T("선택한 품목이 없습니다.")); 
		return;
	}
	else if (CheckUseField() == false)
	{
		AfxMessageBox(_T("중복된 필드가 있습니다."));
		return;
	}

   KSimpleProgressThread::ExecuteThread(CalaculatePathThreadCaller, this);
   KSimpleProgressThread::ExecuteThread(ClearPathThreadCaller, this);

   KLogDebugEndMsg();
   CDialogEx::OnOK();
}


bool KCalculatePathLengthDlg::CheckSelectCommodity()
{
	std::map<int, bool>::iterator iter = m_mapSelectPurpose.begin();
	std::map<int, bool>::iterator end  = m_mapSelectPurpose.end();
	/*while(iter != end)
	{
		if (iter->second == false)
		{
			return true;
		}
		iter++;
	}
	return false;*/

	int nSelCount = 0;
	while(iter != end)
	{
		if ( true == iter->second )
		{
			nSelCount++;
		}
		iter++;
	}

	if(nSelCount < 1)
	{
		return false;
	}

	return true;
}


bool KCalculatePathLengthDlg::CheckUseField()
{
	std::set<CString> setColumnName;

	size_t nxCount = m_vecPathInfo.size();
	for (size_t i=0; i<nxCount; i++)
	{
		STPathInfo oStPath = m_vecPathInfo[i];
		if (oStPath.bSelected == true)
		{
			std::set<CString>::iterator iterFind = setColumnName.find(oStPath.strColumnName);
			std::set<CString>::iterator iterend  = setColumnName.end();
			if (iterFind == iterend)
			{
				setColumnName.insert(oStPath.strColumnName);
			}
			else
			{
				return false;
			}
		}
	}
	return true;
}




unsigned __stdcall KCalculatePathLengthDlg::CalaculatePathThreadCaller( void* p )
{
	KSimpleProgressParameter* pParameter = (KSimpleProgressParameter*)p;
	KSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();
	pProgressWindow->SetStatus(_T("경로 탐색 중입니다."));

	KCalculatePathLengthDlg* pDlg = (KCalculatePathLengthDlg*)pParameter->GetParameter();
	pDlg->CalculatePathFind();
	pDlg->WriteOutPutData();
	return 0;
}


unsigned __stdcall KCalculatePathLengthDlg::ClearPathThreadCaller( void* p )
{
	KSimpleProgressParameter* pParameter = (KSimpleProgressParameter*)p;
	KSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();
	pProgressWindow->SetStatus(_T("데이터 초기화  중입니다."));

	KCalculatePathLengthDlg* pDlg = (KCalculatePathLengthDlg*)pParameter->GetParameter();
	pDlg->ClearNodeArrivedMInInfo();
	return 0;
}



void KCalculatePathLengthDlg::CalculatePathFind()
{
	KLogDebugStartMsg();

	GetZoneNodeInfo();
	GetRoadFacilitiesNodeInfo();
	GetRailFacilitiesNodeInfo();
	GetUsingCommdityNode();
	GetRoadLinkData();
	GetRailLinkData();
	SaveAllPathFindInfo();

	std::map<Integer, Integer>::iterator iter = m_mapZoneNodeInfo.begin();
	std::map<Integer, Integer>::iterator end  = m_mapZoneNodeInfo.end();
	while (iter != end)
	{
		Integer nxSNodeID = iter->first;
		Integer nxSZoneID = iter->second;
		GetFindPathDirectRoadResult(nxSNodeID, nxSZoneID);
		GetFindPathFacilitiesRoadResult(nxSNodeID, nxSZoneID);
		GetFindPathRailResult(nxSNodeID, nxSZoneID);
		++iter;
	}
	KLogDebugEndMsg();
}



void KCalculatePathLengthDlg::WriteOutPutData()
{
	KLogDebugStartMsg();
	size_t nxCount = m_vecPathInfo.size();
	for (size_t i=0; i<nxCount; i++)
	{
		STPathInfo oStPath = m_vecPathInfo[i];
		if (oStPath.bSelected == true)
		{
			KIOColumn* pOutputColumn = oStPath.pColumn;

			KIOColumnCollection pOutColumnCollection; 
			pOutColumnCollection.AddNewColumn(pOutputColumn);

			KODKeyDoubleRecords oResult(1);
			std::map<KODKey, KSDoubleRecord*>::iterator Iter = m_mapResultRecord.begin();
			std::map<KODKey, KSDoubleRecord*>::iterator end  = m_mapResultRecord.end();
			KSDoubleRecord* pRecord = NULL;
			for (Iter; Iter != end; ++Iter)
			{
				KODKey oKODKey = Iter->first;
				KSDoubleRecord* pOutPutResult = oResult.AddRecord(oKODKey);
				KSDoubleRecord* pRecord = Iter->second;
				double dValue = pRecord->GetAt(oStPath.nOutPutMathing);
				pOutPutResult->SetAt(0, dValue);
			}
			KBulkDBase::BulkODUpsert(m_pTable, pOutColumnCollection, oResult);
		}
	}
	m_pTable->Notify();
	KLogDebugEndMsg();
}

bool KCalculatePathLengthDlg::GetZoneNodeInfo()
{
	KLogDebugStartMsg();
	m_setZoneNode.clear();

	KDBaseConnection oDBaseConnetion(m_pTarget->GetDB());
	CString strSQL;
	strSQL.Format (_T(" SELECT Node_Id, Name, Node_Type,Zone_ID FROM %s ORDER BY Node_Id "), TABLE_NODE);

	KResultSetPtr pResult = oDBaseConnetion.ExecuteQuery(strSQL);
	if(pResult == NULL)
	{
		return false;
	}

	Integer nxNodeId		= 0;
	Integer nxZoneID        = 0;
	CString strNodeName		= _T("");
	int		nNodeType		= 0;
	
	while( pResult->Next() )
	{
		nxNodeId	= pResult->GetValueInt64 (0);
		strNodeName = pResult->GetValueString(1);
		nNodeType	= pResult->GetValueInt   (2);
		nxZoneID    = pResult->GetValueInt64 (3);
		m_setAllNode.insert(nxNodeId);

		if (nNodeType == 0 )
		{
			m_setZoneNode.insert(nxNodeId);
			m_mapZoneNodeInfo.insert(std::make_pair(nxNodeId, nxZoneID));
		}
	}
	KLogDebugEndMsg();
	return true;
}


bool KCalculatePathLengthDlg::GetRoadFacilitiesNodeInfo()
{
	KLogDebugStartMsg();
	m_setRoadFacilitiesNode.clear();
	m_mapRoadFacilitesNodeInfo.clear();

	CString strTrafficType = _T("");
	CString strElement      = _T("");

	KDBaseConnection oDBaseConnetion(m_pTarget->GetDB());
	CString strTrafficSQL;
	strTrafficSQL.Format (_T("SELECT Traffic_mean From %s Where Node_Type = 2 Group By Traffic_mean"), TABLE_NODE);
	KResultSetPtr pTrafficTypeResult = oDBaseConnetion.ExecuteQuery(strTrafficSQL);
	if (pTrafficTypeResult == NULL)
	{
		return false;
	}

	std::set<CString> setUseTrafficType;

	while (pTrafficTypeResult->Next())
	{
		strTrafficType = pTrafficTypeResult->GetValueString(0);

		CString strElement      = _T("");
		int     nSub           = 0;
		while(AfxExtractSubString(strElement, strTrafficType, nSub++, _T(',')))
		{
			if(strElement.IsEmpty() == TRUE)
			{
				continue;
			}
			if (strElement == _T("1"))
			{
				setUseTrafficType.insert(strTrafficType);
			}
		}
	}

	CString strTrafficTypeSQL = _T("");
	std::set<CString>::iterator iterTraffic = setUseTrafficType.begin();
	std::set<CString>::iterator endTraffic = setUseTrafficType.end();
	while(iterTraffic != endTraffic)
	{
		strTrafficTypeSQL += _T("'");
		strTrafficTypeSQL += *iterTraffic;
		strTrafficTypeSQL += _T("'");
		iterTraffic ++;
		if (iterTraffic != endTraffic)
		{
			strTrafficTypeSQL += _T(",");
		}
	}

	CString strSQL;
	strSQL.Format (_T(" SELECT Node_Id, Name, Node_Type, Zone_ID FROM %s where Node_type = 2 and traffic_mean in (%s) ORDER BY Node_Id "), TABLE_NODE, strTrafficTypeSQL);

	KResultSetPtr pResult = oDBaseConnetion.ExecuteQuery(strSQL);

	if(pResult == NULL)
	{
		return false;
	}

	Integer nxNodeId        = 0;
	int     nNodeType       = 0;
	Integer nxZoneId        = 0;
	CString strNodeName		= _T("");

	while( pResult->Next() )
	{
		nxNodeId	= pResult->GetValueInt64 (0);
		strNodeName = pResult->GetValueString(1);
		nNodeType	= pResult->GetValueInt   (2);
		nxZoneId    = pResult->GetValueInt64 (3);

		m_setRoadFacilitiesNode.insert(nxNodeId);
		m_mapRoadFacilitesNodeInfo.insert(std::make_pair(nxNodeId, nxZoneId));
	}

	KLogDebugEndMsg();
	return true;
}


bool KCalculatePathLengthDlg::GetRailFacilitiesNodeInfo()
{
	KLogDebugStartMsg();
	m_setRailFacilitiesNode.clear();
	CString strTrafficType = _T("");
	CString strElement      = _T("");
	int     nSub           = 0;

	KDBaseConnection oDBaseConnetion(m_pTarget->GetDB());
	CString strTrafficSQL;
	strTrafficSQL.Format (_T("SELECT Traffic_mean From %s Where Node_Type = 2 Group By Traffic_mean"), TABLE_NODE);
	KResultSetPtr pTrafficTypeResult = oDBaseConnetion.ExecuteQuery(strTrafficSQL);
	if (pTrafficTypeResult == NULL)
	{
		return false;
	}

	std::set<CString> setUseTrafficType;

	while (pTrafficTypeResult->Next())
	{
		strTrafficType = pTrafficTypeResult->GetValueString(0);

		CString strElement      = _T("");
		int     nSub           = 0;
		while(AfxExtractSubString(strElement, strTrafficType, nSub++, _T(',')))
		{
			if(strElement.IsEmpty() == TRUE)
			{
				continue;
			}
			if (strElement == _T("2"))
			{
				setUseTrafficType.insert(strTrafficType);
			}
		}
	}

	CString strTrafficTypeSQL = _T("");
	std::set<CString>::iterator iterTraffic = setUseTrafficType.begin();
	std::set<CString>::iterator endTraffic = setUseTrafficType.end();
	while(iterTraffic != endTraffic)
	{
		strTrafficTypeSQL += _T("'");
		strTrafficTypeSQL += *iterTraffic;
		strTrafficTypeSQL += _T("'");
		iterTraffic ++;
		if (iterTraffic != endTraffic)
		{
			strTrafficTypeSQL += _T(",");
		}
	}

	CString strSQL;
	strSQL.Format (_T(" SELECT Node_Id, Name, Node_Type, Zone_id FROM %s where Node_type = 2 and traffic_mean in (%s) ORDER BY Node_Id "), TABLE_NODE, strTrafficTypeSQL);

	KResultSetPtr pResult = oDBaseConnetion.ExecuteQuery(strSQL);

	if(pResult == NULL)
	{
		return false;
	}

	Integer nxNodeId        = 0;
	Integer nxZoneId        = 0;
	int     nNodeType       = 0;
	CString strNodeName		= _T("");

	while( pResult->Next() )
	{
		nxNodeId	= pResult->GetValueInt64 (0);
		strNodeName = pResult->GetValueString(1);
		nNodeType	= pResult->GetValueInt   (2);
		nxZoneId    = pResult->GetValueInt64 (3);

		m_mapRailFacilitesNodeInfo.insert(std::make_pair(nxNodeId, nxZoneId));
		m_setRailFacilitiesNode.insert(nxNodeId);
	}

	KLogDebugEndMsg();
	return true;
}


bool KCalculatePathLengthDlg::GetRoadLinkData()
{
	KLogDebugStartMsg();
	m_oRoadColLink.RemoveAll();

	KDBaseConnection oDBaseConnetion(m_pTarget->GetDB());
	CString strSQL;
	strSQL.Format (_T(" SELECT Link_Id, FNode_Id, TNode_Id, %s FROM %s Where link_type in (%d, %d, %d, %d, %d)"),
		           _T("length"), TABLE_LINK , Link_Type_Zone, LInk_Type_Highway, 
		            LInk_Type_Expressway, LInk_Type_GeneralRoad, LInk_Type_AccessRoad);
	KResultSetPtr pResult = oDBaseConnetion.ExecuteQuery(strSQL);

	if(pResult == NULL)
	{
		return false;
	}

	LinkIDType nxLinkId		   = 0;
	Integer    nxFNodeId       = 0;
	Integer    nxTNodeId       = 0;
	Integer    nxExcludeNodeID = 0;
	Double     dCost           = 0.0;
	CString    strExcludeNode  = _T("");
	KLink*     pLink           = NULL;

	while( pResult->Next() )
	{

		nxLinkId	= pResult->GetValueInt64 (0);
		nxFNodeId	= pResult->GetValueInt64 (1);
		nxTNodeId	= pResult->GetValueInt64 (2);
		dCost	    = pResult->GetValueDouble(3);

		pLink = m_oRoadColLink.GetExistOrNew(nxLinkId);
		pLink->SetFromNodeID(nxFNodeId);
		pLink->SetToNodeID(nxTNodeId);
		pLink->SetCost(dCost);
	}

	std::set<Integer>::iterator iter = m_setExcludeNodeID.begin();
	std::set<Integer>::iterator end  = m_setExcludeNodeID.end();
	while (iter!= end)
	{
		nxExcludeNodeID          = *iter;
		CString strNodeID = _T("");
		strNodeID.Format(_T("%s"), nxExcludeNodeID);

		strExcludeNode += strNodeID;

		iter++;

		if (iter != end)
		{
			strExcludeNode += _T(", ");
		}
	}


	CString strExcludeSQL = _T("");
	strExcludeSQL.Format(_T(" Select Link_Id, FNode_Id, TNode_Id, %s FROM %s Where (fnode_id not in (%s) and link_type = %d) or (tnode_id not in (%s) and link_type =%d) "),
		_T("length"), TABLE_LINK, strExcludeNode, LInk_Type_Shuttle, strExcludeNode, LInk_Type_Shuttle);

	KResultSetPtr pExcludeResult = oDBaseConnetion.ExecuteQuery(strExcludeSQL);
	if(pExcludeResult == NULL)
	{
		return false;
	}

	while( pExcludeResult->Next() )
	{
		nxLinkId	= pExcludeResult->GetValueInt64 (0);
		nxFNodeId	= pExcludeResult->GetValueInt64 (1);
		nxTNodeId	= pExcludeResult->GetValueInt64 (2);
		dCost	    = pExcludeResult->GetValueDouble(3);

		pLink = m_oRoadColLink.GetExistOrNew(nxLinkId);
		pLink->SetFromNodeID(nxFNodeId);
		pLink->SetToNodeID(nxTNodeId);
		pLink->SetCost(dCost);

	}

	KLogDebugEndMsg();
	return true;
}


bool KCalculatePathLengthDlg::GetRailLinkData()
{
	KLogDebugStartMsg();
	m_oRailColLink.RemoveAll();

	KDBaseConnection oDBaseConnetion(m_pTarget->GetDB());

	CString strRailSQL;
	strRailSQL.Format (_T(" SELECT Link_Id, FNode_Id, TNode_Id, %s FROM %s Where link_type = %d"), 
		_T("length"), TABLE_LINK, LInk_Type_RailWay );

	KResultSetPtr pRailResult = oDBaseConnetion.ExecuteQuery(strRailSQL);
	if(pRailResult == NULL)
	{
		return false;
	}

	LinkIDType nxLinkId		= 0;
	Integer    nxFNodeId    = 0;
	Integer    nxTNodeId    = 0;
	Double     dCost        = 0.0;
	KLink*     pLink        = NULL;

	while( pRailResult->Next() )
	{
		nxLinkId	= pRailResult->GetValueInt64 (0);
		nxFNodeId	= pRailResult->GetValueInt64 (1);
		nxTNodeId	= pRailResult->GetValueInt64 (2);
		dCost	    = pRailResult->GetValueDouble(3);

		pLink = m_oRailColLink.GetExistOrNew(nxLinkId);
		pLink->SetFromNodeID(nxFNodeId);
		pLink->SetToNodeID(nxTNodeId);
		pLink->SetCost(dCost);
	}

	KLogDebugEndMsg();
	return true;
}



void KCalculatePathLengthDlg::SaveAllPathFindInfo()
{

	ClearNodeArrivedMInInfo();

	//시작 노드에서 모든 노드까지
	std::map<Integer, Integer>::iterator iterOriginRoad = m_mapZoneNodeInfo.begin();
	std::map<Integer, Integer>::iterator endOriginRoad  = m_mapZoneNodeInfo.end();
	while (iterOriginRoad != endOriginRoad)
	{
		Integer nxOrginNodeID = iterOriginRoad->first;
		KCostLinkCollection* pCostLinkCollection = new KCostLinkCollection;
		KNodeArrivedMinLink* pNodeArrivedMinLink = new KNodeArrivedMinLink;
		KShortestPath oShortestPath;
		oShortestPath.MuLinkBaseExecute(nxOrginNodeID, m_setAllNode, m_oRoadColLink, *pCostLinkCollection, *pNodeArrivedMinLink);
		m_mapRoadOriginMinLink.insert(std::make_pair(nxOrginNodeID, pNodeArrivedMinLink));
		delete pCostLinkCollection;
		iterOriginRoad++;
	}

	//육송 물류시설 부터 모든 노드까지 
	std::map<Integer, Integer>::iterator iterDestinationRoad = m_mapRoadFacilitesNodeInfo.begin();
	std::map<Integer, Integer>::iterator endDestiantionRoad  = m_mapRoadFacilitesNodeInfo.end();
	while(iterDestinationRoad != endDestiantionRoad)
	{
		Integer nxFNodeID = iterDestinationRoad->first;

		KCostLinkCollection oCostLinkCollection;
		KNodeArrivedMinLink* pNodeArrivedMinLink = new KNodeArrivedMinLink;
		KShortestPath oRoadFacilitiesPath;
		oRoadFacilitiesPath.MuLinkBaseExecute(nxFNodeID, m_setAllNode, m_oRoadColLink, oCostLinkCollection, *pNodeArrivedMinLink );
		m_mapRoadDestinationMinLink.insert(std::make_pair(nxFNodeID, pNodeArrivedMinLink));
		iterDestinationRoad++;
	}

	//철송 물류시설부터 모든노드까지
	std::map<Integer, Integer>::iterator iterDestinationRail = m_mapRailFacilitesNodeInfo.begin();
	std::map<Integer, Integer>::iterator endDestinationRail  = m_mapRailFacilitesNodeInfo.end();
	while(iterDestinationRail != endDestinationRail)
	{
		Integer nxFNodeID = iterDestinationRail->first;

		KCostLinkCollection oCostLinkCollection;
		KNodeArrivedMinLink* pNodeArrivedMinLink = new KNodeArrivedMinLink;
		KShortestPath oRailFacilitiesPath;
		oRailFacilitiesPath.MuLinkBaseExecute(nxFNodeID, m_setAllNode, m_oRoadColLink, oCostLinkCollection, *pNodeArrivedMinLink );
		m_mapRailDestinationMinLink.insert(std::make_pair(nxFNodeID, pNodeArrivedMinLink));
		iterDestinationRail++;
	}

	//철송 물류시설부터 철송 물류시설까지
	std::map<Integer, Integer>::iterator iterRaildStation = m_mapRailFacilitesNodeInfo.begin();
	std::map<Integer, Integer>::iterator endRailStation  = m_mapRailFacilitesNodeInfo.end();
	while(iterRaildStation != endRailStation)
	{
		Integer nxFNodeID = iterRaildStation->first;

		KCostLinkCollection oCostLinkCollection;
		KNodeArrivedMinLink* pNodeArrivedMinLink = new KNodeArrivedMinLink;
		KShortestPath oRailFacilitiesPath;
		oRailFacilitiesPath.MuLinkBaseExecute(nxFNodeID, m_setRailFacilitiesNode, m_oRailColLink, oCostLinkCollection, *pNodeArrivedMinLink );
		m_mapRailStationMinLink.insert(std::make_pair(nxFNodeID, pNodeArrivedMinLink));
		iterRaildStation++;
	}

}

void KCalculatePathLengthDlg::GetFindPathDirectRoadResult(Integer a_nxStartNodeID, Integer a_StartZoneID)
{
	KLogDebugStartMsg();
	
	std::map<Integer, KNodeArrivedMinLink*>::iterator iterRoad  = m_mapRoadOriginMinLink.find(a_nxStartNodeID);
	std::map<Integer, KNodeArrivedMinLink*>::iterator endRoad   = m_mapRoadOriginMinLink.end();
	if (iterRoad == endRoad)
	{
		return;
	}
	KNodeArrivedMinLink* pNodeArrivedMinLink;
	pNodeArrivedMinLink = iterRoad->second;

	std::map<Integer, Integer>::iterator iterDRoad = m_mapZoneNodeInfo.begin();
	std::map<Integer, Integer>::iterator endDRoad  = m_mapZoneNodeInfo.end();
	while (iterDRoad != endDRoad)
	{
		Integer nxEndNodeID     = iterDRoad->first;
		Integer nxEndZoneID     = iterDRoad->second;
		double dSumDRoadValue   = 0.0;
		
		KMinLinkInfo* pLinkInfo = pNodeArrivedMinLink->GetMinLink(nxEndNodeID);
		if (pLinkInfo != NULL)
		{
			if (a_StartZoneID == nxEndZoneID)
			{
				dSumDRoadValue = DEFUALT_DISTANCE;
			}
			else
			{
				dSumDRoadValue = pLinkInfo->Cost;
			}
		} else
		{
			dSumDRoadValue = 9999999999;
		}

		KODKey oODKey (a_StartZoneID, nxEndZoneID);
		std::map<KODKey, KSDoubleRecord*>::iterator finditer =  m_mapResultRecord.find(oODKey);
		std::map<KODKey, KSDoubleRecord*>::iterator findend  =  m_mapResultRecord.end();		
		KSDoubleRecord* pRecord = NULL;

		if (finditer == findend)
		{
			pRecord = new KSDoubleRecord(12);
			m_mapResultRecord.insert(std::make_pair(oODKey, pRecord));
		}
		else
		{
			pRecord = finditer->second;
		}
		pRecord->SetAt(RoadD, dSumDRoadValue);

		++iterDRoad;
	}
	KLogDebugEndMsg();
}


void KCalculatePathLengthDlg::GetFindPathFacilitiesRoadResult( Integer a_nxStrartNodeID, Integer a_nxStartZoneID )
{
	KLogDebugStartMsg();

	Integer nxEndNodeID = 0;
	Integer nxEndZoneID = 0;

	std::map<Integer, Integer>::iterator iterRoad = m_mapZoneNodeInfo.begin();
	std::map<Integer, Integer>::iterator endRoad  = m_mapZoneNodeInfo.end();
	while (iterRoad != endRoad)
	{
		nxEndNodeID = iterRoad->first;
		nxEndZoneID = iterRoad->second;

		bool    bRoadFindPath              = false;
		Integer nxMinFacilitiesID          = 0;
		Integer nxMinZoneFacilitiesID      = 0;
		double  dMinOriginValue            = 0.0;
		double  dMInDestinaionValue        = 0.0;
		double  dMinSumValue               = 0.0;

		std::map<Integer, KNodeArrivedMinLink*>::iterator iterORoad  = m_mapRoadOriginMinLink.find(a_nxStrartNodeID);
		std::map<Integer, KNodeArrivedMinLink*>::iterator endORoad   = m_mapRoadOriginMinLink.end();
		KNodeArrivedMinLink* pNodeArrivedMinLink = iterORoad->second;

		std::map<Integer, Integer>::iterator iterFacilities = m_mapRoadFacilitesNodeInfo.begin();
		std::map<Integer, Integer>::iterator EndFacilities  = m_mapRoadFacilitesNodeInfo.end();
		while(iterFacilities != EndFacilities)
		{
			Integer nxFacilitiesNodeID = iterFacilities->first;
			Integer nxFacilitiesZoneID = iterFacilities->second;

			double  dOrginValue       = 0.0;
			double  dDestinationValue = 0.0;
			double  dSumValue         = 0.0;

			std::set<Integer> setEndNode;
			setEndNode.insert(nxEndNodeID);

			KMinLinkInfo* pOriginLinkInfo = pNodeArrivedMinLink->GetMinLink(nxFacilitiesNodeID);
			if (pOriginLinkInfo != NULL)
			{
				dOrginValue = pOriginLinkInfo->Cost;

				std::map<Integer, KNodeArrivedMinLink*>::iterator iterDRoad  = m_mapRoadDestinationMinLink.find(nxFacilitiesNodeID);
				std::map<Integer, KNodeArrivedMinLink*>::iterator endDRoad   = m_mapRoadDestinationMinLink.end();
				KNodeArrivedMinLink* pDestiaNationMinLink = iterDRoad->second;

				KMinLinkInfo* pDestinationLinkInfo = pDestiaNationMinLink->GetMinLink(nxEndNodeID);
				if (pDestinationLinkInfo != NULL)
				{
					dDestinationValue = pDestinationLinkInfo->Cost;
					dSumValue  = dOrginValue + dDestinationValue;
					
					if (bRoadFindPath == false)
					{
						dMinOriginValue       = dOrginValue;
						dMInDestinaionValue   = dDestinationValue;
						nxMinFacilitiesID     = nxFacilitiesNodeID;
						nxMinZoneFacilitiesID = nxFacilitiesZoneID;
						dMinSumValue          = dSumValue;
						bRoadFindPath         = true;

					} else if (dMinSumValue > dSumValue)
					{
						dMinOriginValue       = dOrginValue;
						dMInDestinaionValue   = dDestinationValue;
						nxMinFacilitiesID     = nxFacilitiesNodeID;
						nxMinZoneFacilitiesID = nxFacilitiesZoneID;
						dMinSumValue          = dSumValue;
					}
				}
			}
			iterFacilities++;
		}

		if (a_nxStartZoneID == nxEndZoneID)
		{
			dMinOriginValue       = DEFUALT_DISTANCE;
			dMInDestinaionValue   = DEFUALT_DISTANCE;
			nxMinFacilitiesID     = 0;
			nxMinZoneFacilitiesID = 0;
		}
		else if (bRoadFindPath == false)
		{
			dMinOriginValue       = 9999999999;
			dMInDestinaionValue   = 9999999999;
			nxMinFacilitiesID     = 0;
			nxMinZoneFacilitiesID = 0;
		}
		
		KODKey oODKey (a_nxStartZoneID, nxEndZoneID);
		std::map<KODKey, KSDoubleRecord*>::iterator finditer =  m_mapResultRecord.find(oODKey);
		std::map<KODKey, KSDoubleRecord*>::iterator findend  =  m_mapResultRecord.end();		
		KSDoubleRecord* pRecord = NULL;

		if (finditer == findend)
		{
			pRecord = new KSDoubleRecord(12);
			m_mapResultRecord.insert(std::make_pair(oODKey, pRecord));
		}
		else
		{
			pRecord = finditer->second;
		}

		pRecord->SetAt(RoadS, dMinOriginValue);
		pRecord->SetAt(RoadE, dMInDestinaionValue);
		pRecord->SetAt(RoadFN,(double)nxMinFacilitiesID);
		pRecord->SetAt(RoadFZ,(double)nxMinZoneFacilitiesID);

		iterRoad++;
	}
	KLogDebugEndMsg();
}

void KCalculatePathLengthDlg::GetFindPathRailResult( Integer a_nxStartNodeID, Integer a_nxstartZoneID )
{

	KLogDebugStartMsg();
	Integer nxEndNodeID = 0;
	Integer nxEndZoneID = 0;

	std::map<Integer, KNodeArrivedMinLink*>::iterator iterORoad  = m_mapRoadOriginMinLink.find(a_nxStartNodeID);
	std::map<Integer, KNodeArrivedMinLink*>::iterator endORoad   = m_mapRoadOriginMinLink.end();
	KNodeArrivedMinLink* pOriginArrivedMinLink = iterORoad->second;

	std::map<Integer, Integer>::iterator iterRoad = m_mapZoneNodeInfo.begin();
	std::map<Integer, Integer>::iterator endRoad  = m_mapZoneNodeInfo.end();
	while (iterRoad != endRoad)
	{
		nxEndNodeID = iterRoad->first;
		nxEndZoneID = iterRoad->second;

		std::set<Integer> setEndNode;
		setEndNode.insert(nxEndNodeID);

		Integer nxOriginFacilitiesNode      = 0;
		Integer nxOriginFacilitiesZone      = 0;
		Integer nxDestinationFacilitiesNode = 0;
		Integer nxDestinationFacilitiesZone = 0;
		double  dMinOriginValue             = 0.0;
		double  dMinDestinationValue        = 0.0;
		double  dMinRailValue               = 0.0;

		bool  bFindOriginPath      = false;
		bool  bFindDestinationPath = false;
		bool  bFindRailPath        = false;

		std::map<Integer, Integer>::iterator iterOFacilities = m_mapRailFacilitesNodeInfo.begin();
		std::map<Integer, Integer>::iterator EndOFacilities  = m_mapRailFacilitesNodeInfo.end();
		while (iterOFacilities != EndOFacilities)
		{
			Integer nxOFacilitiesNode = iterOFacilities->first;

			KMinLinkInfo* pOriginLinkInfo = pOriginArrivedMinLink->GetMinLink(nxOFacilitiesNode);
			if (pOriginLinkInfo != NULL)
			{
				double dOrginValue = pOriginLinkInfo->Cost; 
				if (bFindOriginPath == false)
				{
					nxOriginFacilitiesNode = nxOFacilitiesNode;
					nxOriginFacilitiesZone = iterOFacilities->second;
					dMinOriginValue        =  dOrginValue;
					bFindOriginPath = true;
				} 
				else if (dMinOriginValue > dOrginValue)
				{
					nxOriginFacilitiesNode = nxOFacilitiesNode;
					nxOriginFacilitiesZone = iterOFacilities->second;
					dMinOriginValue        =  dOrginValue;
				}

			}
			iterOFacilities++;
		}

		std::map<Integer, Integer>::iterator iterDFacilities = m_mapRailFacilitesNodeInfo.begin();
		std::map<Integer, Integer>::iterator EndDFacilities  = m_mapRailFacilitesNodeInfo.end();
		while(iterDFacilities != EndDFacilities)
		{
			Integer nxDFaciliteiesNode = iterDFacilities->first;
			
			if (nxDFaciliteiesNode != nxOriginFacilitiesNode)
			{
				std::map<Integer, KNodeArrivedMinLink*>::iterator iterDRail  = m_mapRailDestinationMinLink.find(nxDFaciliteiesNode);
				std::map<Integer, KNodeArrivedMinLink*>::iterator endDRail   = m_mapRailDestinationMinLink.end();
				KNodeArrivedMinLink* pDestinationArrivedMinLink = iterDRail->second;
			
				KMinLinkInfo* pDestinationLinkInfo = pDestinationArrivedMinLink->GetMinLink(nxEndNodeID);
				if (pDestinationLinkInfo != NULL)
				{
					double dDestinaionValue = pDestinationLinkInfo->Cost;
					if (bFindDestinationPath == false)
					{
						nxDestinationFacilitiesNode = nxDFaciliteiesNode;
						nxDestinationFacilitiesZone = iterDFacilities->second;
						dMinDestinationValue        = dDestinaionValue;
						bFindDestinationPath = true;
					} else if (dMinDestinationValue > dDestinaionValue)
					{
						nxDestinationFacilitiesNode = nxDFaciliteiesNode;
						nxDestinationFacilitiesZone = iterDFacilities->second;
						dMinDestinationValue        = dDestinaionValue;
					}
				}
			}
			iterDFacilities++;
		}

		if (a_nxstartZoneID == nxEndZoneID)
		{
			nxOriginFacilitiesNode      = 0;
			nxOriginFacilitiesZone      = 0;
			nxDestinationFacilitiesNode = 0;
			nxDestinationFacilitiesZone = 0;
			dMinOriginValue             = DEFUALT_DISTANCE;
			dMinDestinationValue        = DEFUALT_DISTANCE;
			dMinRailValue               = DEFUALT_DISTANCE;
		}
		else if (bFindOriginPath == false || bFindDestinationPath == false)
		{
			nxOriginFacilitiesNode      = 0;
			nxOriginFacilitiesZone      = 0;
			nxDestinationFacilitiesNode = 0;
			nxDestinationFacilitiesZone = 0;
			dMinOriginValue             = 9999999999;
			dMinDestinationValue        = 9999999999;
			dMinRailValue               = 9999999999;
			
		}
		else
		{
			std::map<Integer, KNodeArrivedMinLink*>::iterator iterStation   = m_mapRailStationMinLink.find(nxOriginFacilitiesNode);
			std::map<Integer, KNodeArrivedMinLink*>::iterator endDStation   = m_mapRailStationMinLink.end();
			KNodeArrivedMinLink* pNodeArrivedMinLink = iterStation->second;

			KMinLinkInfo* pRailLinkInfo = pNodeArrivedMinLink->GetMinLink(nxDestinationFacilitiesNode);
			if (pRailLinkInfo != NULL)
			{
				dMinRailValue = pRailLinkInfo->Cost;
			}
			else
			{
				nxOriginFacilitiesNode      = 0;
				nxOriginFacilitiesZone      = 0;
				nxDestinationFacilitiesNode = 0;
				nxDestinationFacilitiesZone = 0;
				dMinOriginValue             = 9999999999;
				dMinDestinationValue        = 9999999999;
				dMinRailValue               = 9999999999;
			}

		}

		KODKey oODKey (a_nxstartZoneID, nxEndZoneID);
		std::map<KODKey, KSDoubleRecord*>::iterator finditer =  m_mapResultRecord.find(oODKey);
		std::map<KODKey, KSDoubleRecord*>::iterator findend  =  m_mapResultRecord.end();		
		KSDoubleRecord* pRecord = NULL;

		if (finditer == findend)
		{
			pRecord = new KSDoubleRecord(12);
			m_mapResultRecord.insert(std::make_pair(oODKey, pRecord));
		}
		else
		{
			pRecord = finditer->second;
		}
		pRecord->SetAt(RailS, dMinOriginValue);
		pRecord->SetAt(RailM, dMinRailValue);
		pRecord->SetAt(RailE, dMinDestinationValue);
		pRecord->SetAt(RailFSN, (double)nxOriginFacilitiesNode);
		pRecord->SetAt(RailFEN, (double)nxDestinationFacilitiesNode);
		pRecord->SetAt(RailFSZ, (double)nxOriginFacilitiesZone);
		pRecord->SetAt(RailFEZ, (double)nxDestinationFacilitiesZone);

		iterRoad++;
	}
	KLogDebugEndMsg();
}

void KCalculatePathLengthDlg::ClearNodeArrivedMInInfo()
{
	std::map<Integer, KNodeArrivedMinLink*>::iterator iterRO =  m_mapRoadOriginMinLink.begin();
	std::map<Integer, KNodeArrivedMinLink*>::iterator endRO  =  m_mapRoadOriginMinLink.end();
	while (iterRO != endRO)
	{
		KNodeArrivedMinLink* pMinLink = iterRO->second;
		pMinLink->RemoveAll();
		delete pMinLink;
		iterRO++;
	}
	m_mapRoadOriginMinLink.clear();

	std::map<Integer, KNodeArrivedMinLink*>::iterator iterRD =  m_mapRoadDestinationMinLink.begin();
	std::map<Integer, KNodeArrivedMinLink*>::iterator endRD  =  m_mapRoadDestinationMinLink.end();
	while (iterRD != endRD)
	{
		KNodeArrivedMinLink* pMinLink = iterRD->second;
		pMinLink->RemoveAll();
		delete pMinLink;
		iterRD++;
	}
	m_mapRoadDestinationMinLink.clear();

	std::map<Integer, KNodeArrivedMinLink*>::iterator iterRiD =  m_mapRailDestinationMinLink.begin();
	std::map<Integer, KNodeArrivedMinLink*>::iterator endRiD  =  m_mapRailDestinationMinLink.end();
	while (iterRiD != endRiD)
	{
		KNodeArrivedMinLink* pMinLink = iterRiD->second;
		pMinLink->RemoveAll();
		delete pMinLink;
		iterRiD++;
	}
	m_mapRailDestinationMinLink.clear();

	std::map<Integer, KNodeArrivedMinLink*>::iterator iterRiS =  m_mapRailStationMinLink.begin();
	std::map<Integer, KNodeArrivedMinLink*>::iterator endRiS  =  m_mapRailStationMinLink.end();
	while (iterRiS != endRiS)
	{
		KNodeArrivedMinLink* pMinLink = iterRiS->second;
		pMinLink->RemoveAll();
		delete pMinLink;
		iterRiS++;
	}
	m_mapRailStationMinLink.clear();
}

void KCalculatePathLengthDlg::OnBnClickedSelectCommdity()
{
	CString strEditBox = _T("");
	KPathCalculateCommdityDlg oPathCommdityDlg;
	oPathCommdityDlg.SetInfo(m_pTarget, m_mapSelectPurpose);
	if(oPathCommdityDlg.DoModal() == IDOK)
	{
		oPathCommdityDlg.SelectCommdityList(m_mapSelectPurpose, strEditBox);

		m_strEdtAvailableCommodity = strEditBox;
		UpdateData(FALSE);
	}
}


void KCalculatePathLengthDlg::OnBnClickedRailOrigin()
{
	if (m_chkRailStartLength.GetCheck())
	{
		m_cboRailStartLength.EnableWindow(TRUE);
	}
	else
	{
		m_cboRailStartLength.EnableWindow(FALSE);
	}
}


void KCalculatePathLengthDlg::OnBnClickedCheckRail()
{
	if (m_chkRailMiddleLength.GetCheck())
	{
		m_cboRailMiddleLength.EnableWindow(TRUE);
	}
	else
	{
		m_cboRailMiddleLength.EnableWindow(FALSE);
	}
}


void KCalculatePathLengthDlg::OnBnClickedRailDestination()
{
	if (m_chkRailEndLength.GetCheck())
	{
		m_cboRailEndLength.EnableWindow(TRUE);
	}
	else
	{
		m_cboRailEndLength.EnableWindow(FALSE);
	}
}


void KCalculatePathLengthDlg::OnBnClickedRoadDirect()
{
	if (m_chkRoadDirectLength.GetCheck())
	{
		m_cboRoadDirectLength.EnableWindow(TRUE);
	}
	else
	{
		m_cboRoadDirectLength.EnableWindow(FALSE);
	}
}


void KCalculatePathLengthDlg::OnBnClickedRoadOrigin()
{
	if (m_chkRoadFStartLength.GetCheck())
	{
		m_cboRoadFStartLength.EnableWindow(TRUE);
	}
	else
	{
		m_cboRoadFStartLength.EnableWindow(FALSE);
	}
}


void KCalculatePathLengthDlg::OnBnClickedRoadDestination()
{
	if (m_chkRoadFEndLength.GetCheck())
	{
		m_cboRoadFEndLength.EnableWindow(TRUE);
	}
	else
	{
		m_cboRoadFEndLength.EnableWindow(FALSE);
	}
}


void KCalculatePathLengthDlg::OnBnClickedRailOriginNode()
{
	if (m_chkRailStartFacilityNode.GetCheck())
	{
		m_cboRailStartFacilityNode.EnableWindow(TRUE);
	}
	else
	{
		m_cboRailStartFacilityNode.EnableWindow(FALSE);
	}
}


void KCalculatePathLengthDlg::OnBnClickedRailDestinatinoNode()
{
	if (m_chkRailEndFacilityNode.GetCheck())
	{
		m_cboRailEndFacilityNode.EnableWindow(TRUE);
	}
	else
	{
		m_cboRailEndFacilityNode.EnableWindow(FALSE);
	}
}


void KCalculatePathLengthDlg::OnBnClickedRoadFacilityNode()
{
	if (m_chkRoadFacilityNode.GetCheck())
	{
		m_cboRoadFacilityNode.EnableWindow(TRUE);
	}
	else
	{
		m_cboRoadFacilityNode.EnableWindow(FALSE);
	}
}


void KCalculatePathLengthDlg::OnBnClickedRailOriginZone()
{
	if (m_chkRailStartFacilityZone.GetCheck())
	{
		m_cboRailStartFacilityZone.EnableWindow(TRUE);
	}
	else
	{
		m_cboRailStartFacilityZone.EnableWindow(FALSE);
	}
}


void KCalculatePathLengthDlg::OnBnClickedRailDestinationZone()
{
	if (m_chkRailEndFacilityZone.GetCheck())
	{
		m_cboRailEndFacilityZone.EnableWindow(TRUE);
	}
	else
	{
		m_cboRailEndFacilityZone.EnableWindow(FALSE);
	}
}


void KCalculatePathLengthDlg::OnBnClickedRoadFacilityZone()
{
	if (m_chkRoadFacilityZone.GetCheck())
	{
		m_cboRoadFacilityZone.EnableWindow(TRUE);
	}
	else
	{
		m_cboRoadFacilityZone.EnableWindow(FALSE);
	}
}

void KCalculatePathLengthDlg::InitComboState()
{
	m_cboRailStartLength.EnableWindow(FALSE);
	m_cboRailStartLength.SetCurSel(0);
	m_cboRailMiddleLength.EnableWindow(FALSE);
	m_cboRailMiddleLength.SetCurSel(0);
	m_cboRailEndLength.EnableWindow(FALSE);
	m_cboRailEndLength.SetCurSel(0);
	m_cboRoadDirectLength.EnableWindow(FALSE);
	m_cboRoadDirectLength.SetCurSel(0);
	m_cboRoadFStartLength.EnableWindow(FALSE);
	m_cboRoadFStartLength.SetCurSel(0);
	m_cboRoadFEndLength.EnableWindow(FALSE);
	m_cboRoadFEndLength.SetCurSel(0);
	m_cboRailStartFacilityNode.EnableWindow(FALSE);
	m_cboRailStartFacilityNode.SetCurSel(0);
	m_cboRailEndFacilityNode.EnableWindow(FALSE);
	m_cboRailEndFacilityNode.SetCurSel(0);
	m_cboRoadFacilityNode.EnableWindow(FALSE);
	m_cboRoadFacilityNode.SetCurSel(0);
	m_cboRailStartFacilityZone.EnableWindow(FALSE);
	m_cboRailStartFacilityZone.SetCurSel(0);
	m_cboRailEndFacilityZone.EnableWindow(FALSE);
	m_cboRailEndFacilityZone.SetCurSel(0);
	m_cboRoadFacilityZone.EnableWindow(FALSE);
	m_cboRoadFacilityZone.SetCurSel(0);

	m_strEdtAvailableCommodity = _T("<----품목 선택---->");

	UpdateData(FALSE);


}

void KCalculatePathLengthDlg::GetUsingCommdityNode()
{
	KDBaseConnection oDBaseConnetion(m_pTarget->GetDB());

	CString strSQL;
	strSQL.Format (_T(" SELECT node_id, available_item FROM %s Where node_type = 2"), TABLE_NODE);

	KResultSetPtr pResult = oDBaseConnetion.ExecuteQuery(strSQL);
	if(pResult == NULL)
	{
		return;
	}

	Integer nxNodeID = 0;
	CString strAvailableItem = _T("");

	while (pResult->Next())
	{
		nxNodeID	     = pResult->GetValueInt64 (0);
		strAvailableItem = pResult->GetValueString(1);

		int     nSub       = 0;
		CString strElement = _T("");
		int     nCodeValue = 0;
		std::set<int> setCodeValue;


		while(AfxExtractSubString(strElement, strAvailableItem, nSub++, _T(',')))
		{
			if(strElement.IsEmpty() == TRUE)
			{
				continue;
			}
			nCodeValue = _ttoi(strElement);
			setCodeValue.insert(nCodeValue);
		}


		bool bMatchCode = true;

		std::map<int, bool>::iterator iter = m_mapSelectPurpose.begin();
		std::map<int, bool>::iterator end  = m_mapSelectPurpose.end();
		while(iter != end)
		{
			int nCode = iter->first;
			if (iter->second == true)
			{
				std::set<int>::iterator iterfind = setCodeValue.find(nCode);
				std::set<int>::iterator findend  = setCodeValue.end();
				if (iter == end)
				{
					bMatchCode = false;
				}
			}

			iter++;
		}
		if (bMatchCode == false)
		{
			m_setExcludeNodeID.insert(nxNodeID);
		}
	}
}
