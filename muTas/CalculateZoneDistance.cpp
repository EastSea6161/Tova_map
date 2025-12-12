// CalculateZoneDistance.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CalculateZoneDistance.h"
#include "afxdialogex.h"
#include "Target.h"

//^#include "DBaseConnector.h"
#include "ShortestPathInfo.h"
#include "KBulkDBase.h"
#include "PathCalculateCommdityDlg.h"

// KCalculateZoneDistance 대화 상자입니다.

IMPLEMENT_DYNAMIC(KCalculateZoneDistanceDlg, KDialogEx)

KCalculateZoneDistanceDlg::KCalculateZoneDistanceDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KCalculateZoneDistanceDlg::IDD, pParent)
{

}

KCalculateZoneDistanceDlg::~KCalculateZoneDistanceDlg()
{
}

void KCalculateZoneDistanceDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cboZoneDistance);
}


BEGIN_MESSAGE_MAP(KCalculateZoneDistanceDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KCalculateZoneDistanceDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KCalculateZoneDistanceDlg::OnBnClickedCancel)
END_MESSAGE_MAP()




void KCalculateZoneDistanceDlg::SetInfo( KTarget* a_pTarget, KIOTable* a_pTable )
{
	m_pTarget = a_pTarget;
	m_pTable  = a_pTable;

}

BOOL KCalculateZoneDistanceDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	initColumnList();
	GetParameterDolubleColumn();
	InsertComboBoxValue();

	return TRUE;
}



void KCalculateZoneDistanceDlg::GetParameterDolubleColumn()
{
	const KIOColumns* pColumnCollection = m_pTable->Columns();
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


void KCalculateZoneDistanceDlg::initColumnList()
{
	size_t nxCount = m_vecParameterDoubleColumn.size();
	for (size_t i=0; i<nxCount; i++)
	{
		KIOColumn* pColumn = m_vecParameterDoubleColumn[i];
		delete pColumn;
	}
	m_vecParameterDoubleColumn.clear();
}

void KCalculateZoneDistanceDlg::InsertComboBoxValue()
{
	int nComboIndex;

	nComboIndex	= m_cboZoneDistance.InsertString(-1, _T("----OutPut Field 선택----"));
	m_cboZoneDistance.SetItemData(nComboIndex, (DWORD_PTR)NULL);

	size_t nxdoubleCount = m_vecParameterDoubleColumn.size();
	for (size_t i = 0; i<nxdoubleCount; i++)
	{
		KIOColumn* pColumn = m_vecParameterDoubleColumn[i];

		nComboIndex	= m_cboZoneDistance.InsertString(-1, pColumn->Caption());
		m_cboZoneDistance.SetItemData(nComboIndex, (DWORD_PTR)NULL);

	}
}



void KCalculateZoneDistanceDlg::OnBnClickedOk()
{
	//선택 필드값구하기

	TxLogDebugStartMsg();
	int nCurIndex         = 0;
	CString strColumnName = _T("");
	KIOColumn* pColumn    = NULL;
	
	nCurIndex  = m_cboZoneDistance.GetCurSel();
	if (nCurIndex == 0)
	{
		AfxMessageBox(_T("선택한 컬럼이 없습니다."));
		return;
	}

	m_cboZoneDistance.GetLBText(nCurIndex,strColumnName);
	m_pColumn = findDoubleIOColumnInfo(strColumnName);

	if (m_pColumn == NULL)
	{
		AfxMessageBox(_T("매칭 가능한 컬럼이 존재하지 않습니다."));
		return;
	}

	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

	QBicSimpleProgressThread::ExecuteThread(CalaculatePathThreadCaller, this, false, nLang);

	QBicSimpleProgressThread::ExecuteThread(ClearPathThreadCaller, this, false, nLang);
	//m_pTable->Notify();

	TxLogDebugEndMsg();
	KDialogEx::OnOK();
}



KIOColumn* KCalculateZoneDistanceDlg::findDoubleIOColumnInfo( CString a_strColumnName )
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



void KCalculateZoneDistanceDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KDialogEx::OnCancel();
}

bool KCalculateZoneDistanceDlg::GetZoneNodeInfo()
{
	TxLogDebugStartMsg();
	m_setZoneNode.clear();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	CString strSQL;
	strSQL.Format (_T(" SELECT Node_Id, Name, Node_Type,Zone_ID FROM %s Where node_type = 0 "), TABLE_NODE);

	KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);
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
		m_setZoneNode.insert(nxNodeId);
		m_mapZoneNodeInfo.insert(std::make_pair(nxNodeId, nxZoneID));
	}
	TxLogDebugEndMsg();
	return true;
}

bool KCalculateZoneDistanceDlg::GetRoadLinkData()
{
	TxLogDebugStartMsg();
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	CString strSQL;
	strSQL.Format (_T(" SELECT Link_Id, FNode_Id, TNode_Id, %s FROM %s Where link_type != 9"),
		_T("length"), TABLE_LINK);
	KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);


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
	TxLogDebugEndMsg();
	return true;
}

void KCalculateZoneDistanceDlg::PathFindAllZoneInfo()
{
	TxLogDebugStartMsg();

	std::map<Integer, Integer>::iterator iterOriginRoad = m_mapZoneNodeInfo.begin();
	std::map<Integer, Integer>::iterator endOriginRoad  = m_mapZoneNodeInfo.end();
	while (iterOriginRoad != endOriginRoad)
	{
		Integer nxOrginNodeID = iterOriginRoad->first;
		KCostLinkCollection* pCostLinkCollection = new KCostLinkCollection;
		KNodeArrivedMinLink* pNodeArrivedMinLink = new KNodeArrivedMinLink;
		KShortestPath oShortestPath;
		oShortestPath.MuLinkBaseExecute(nxOrginNodeID, m_setZoneNode, m_oRoadColLink, *pCostLinkCollection, *pNodeArrivedMinLink);
		m_mapRoadOriginMinLink.insert(std::make_pair(nxOrginNodeID, pNodeArrivedMinLink));
		delete pCostLinkCollection;

		iterOriginRoad++;
	}
	TxLogDebugEndMsg();
}

void KCalculateZoneDistanceDlg::GetPathFindDistanceResult(Integer a_nxStartNodeID, Integer a_StartZoneID)
{
	//TxLogDebugStartMsg();
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
				dSumDRoadValue = 0.0;
			}
			else
			{
				dSumDRoadValue = pLinkInfo->Cost;
			}
		} 
		else
		{
			dSumDRoadValue = FREIGHT_MAX_DISTANCE;
		}

		KODKey oODKey (a_StartZoneID, nxEndZoneID);
		std::map<KODKey, KSDoubleRecord*>::iterator finditer =  m_mapResultRecord.find(oODKey);
		std::map<KODKey, KSDoubleRecord*>::iterator findend  =  m_mapResultRecord.end();		
		KSDoubleRecord* pRecord = NULL;

		if (finditer == findend)
		{
			pRecord = new KSDoubleRecord(1);
			m_mapResultRecord.insert(std::make_pair(oODKey, pRecord));
		}
		else
		{
			pRecord = finditer->second;
		}
		pRecord->SetAt(0, dSumDRoadValue);

		++iterDRoad;
	}

	//TxLogDebugEndMsg();
}

void KCalculateZoneDistanceDlg::ClearInfoData()
{
	TxLogDebugStartMsg();
	std::map<Integer, KNodeArrivedMinLink*>::iterator iter =  m_mapRoadOriginMinLink.begin();
	std::map<Integer, KNodeArrivedMinLink*>::iterator end  =  m_mapRoadOriginMinLink.end();
	while (iter != end)
	{
		KNodeArrivedMinLink* pMinLink = iter->second;
		pMinLink->RemoveAll();
		delete pMinLink;
		++iter;
	}
	m_mapRoadOriginMinLink.clear();

	initColumnList();
	
	std::map<KODKey, KSDoubleRecord*>::iterator iterResult =  m_mapResultRecord.begin();
	std::map<KODKey, KSDoubleRecord*>::iterator endResult  =  m_mapResultRecord.end();
	while (iterResult != endResult)
	{
		KSDoubleRecord* pRecord = iterResult->second;
		pRecord->RemoveAll();
		delete pRecord;
		iterResult++;
	}

	m_oRoadColLink.RemoveAll();
	TxLogDebugEndMsg();
}

unsigned __stdcall KCalculateZoneDistanceDlg::CalaculatePathThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();
	pProgressWindow->SetStatus(_T("존간거리 계산 중입니다."));

	KCalculateZoneDistanceDlg* pDlg = (KCalculateZoneDistanceDlg*)pParameter->GetParameter();
	pDlg->CalculatePathFind();
	return 0;
}

void KCalculateZoneDistanceDlg::CalculatePathFind()
{
	GetZoneNodeInfo();
	GetRoadLinkData();
	PathFindAllZoneInfo();

	//존간 거리값 계산하기 
	std::map<Integer, Integer>::iterator iterZone = m_mapZoneNodeInfo.begin();
	std::map<Integer, Integer>::iterator endZone  = m_mapZoneNodeInfo.end();
	while (iterZone != endZone)
	{
		Integer nxSNodeID = iterZone->first;
		Integer nxSZoneID = iterZone->second;
		GetPathFindDistanceResult(nxSNodeID, nxSZoneID);
		++iterZone;
	}


	//계산값 저장하기
	KIOColumns pOutColumnCollection; 
	pOutColumnCollection.AddNewColumn(m_pColumn);

	KODKeyDoubleRecords oResult(1);
	std::map<KODKey, KSDoubleRecord*>::iterator Iter = m_mapResultRecord.begin();
	std::map<KODKey, KSDoubleRecord*>::iterator end  = m_mapResultRecord.end();
	KSDoubleRecord* pRecord = NULL;
	for (Iter; Iter != end; ++Iter)
	{
		KODKey oKODKey = Iter->first;
		KSDoubleRecord* pOutPutResult = oResult.AddRecord(oKODKey);
		KSDoubleRecord* pRecord = Iter->second;
		double dValue = pRecord->GetAt(0);
		pOutPutResult->SetAt(0, dValue);
	}

	KBulkDBase::BulkODUpsert(m_pTable, pOutColumnCollection, oResult);

}

unsigned __stdcall KCalculateZoneDistanceDlg::ClearPathThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();
	pProgressWindow->SetStatus(_T("저장된 데이터 삭제중입니다."));

	KCalculateZoneDistanceDlg* pDlg = (KCalculateZoneDistanceDlg*)pParameter->GetParameter();
	pDlg->ClearInfoData();
	return 0;
}

