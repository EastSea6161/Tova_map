#include "StdAfx.h"

#include "MainFrameWnd.h"
#include "MapView.h"
#include "ShortestPathBase.h"
#include "afxdialogex.h"
#include "Target.h"

//^#include "DBaseConnector.h"
#include "..\QBicCommon\ColorRangeGen.h"
#include "KEMNode.h"
#include "KExRecordItem.h"
//#include "StringChecker.h"
//^^ #include "IOColumn.h"
#include "KBulkDBase.h"
#include "KBulkDBase2File.h"


KShortestPathBase::KShortestPathBase(void)
{

}


KShortestPathBase::~KShortestPathBase(void)
{
}


void KShortestPathBase::SetTarget(KTarget* a_pTarget)
{
	m_pTarget = a_pTarget;
}

void KShortestPathBase::SetLinkTable( KIOTable* a_pLinkTable )
{
	m_pLinkTable = a_pLinkTable;
}

void KShortestPathBase::SetMapView(KMapView* a_pMapView)
{
	m_pMapView = a_pMapView;
}

CString KShortestPathBase::DisplayScenarioTitle( CString a_strTitle )
{
	CString strReturn = _T("");
	strReturn.Format(_T("[ %s ]"), a_strTitle);

	return strReturn;
}


void KShortestPathBase::MessageExistNode(bool a_bZoneNode)
{
	if(a_bZoneNode)
	{
		AfxMessageBox(_T("입력한 존 노드 ID가 등록되어 있지 않습니다."));
	}
	else
	{
		AfxMessageBox(_T("입력한 노드 ID가 등록되어 있지 않습니다."));
	}
}


bool KShortestPathBase::IsNodeID(Integer a_nxCheckValue, bool a_bZoneNode)
{
	KTDNodeDataMap* pmapNodeData;

	if(a_bZoneNode)
	{
		pmapNodeData = &m_mapZoneNodeData;
	}
	else 
	{
		pmapNodeData = &m_mapNodeData;
	}

	KTDNodeDataMap::iterator iter;
	iter = pmapNodeData->find(a_nxCheckValue);

	if(iter == pmapNodeData->end())
	{
		return false;
	}

	return true;
}


bool KShortestPathBase::IsNodeID(Integer a_nxCheckValue, bool a_bZoneNode, CString &a_strNodeName)
{
	KTDNodeDataMap* pmapNodeData;

	if(a_bZoneNode)
	{
		pmapNodeData = &m_mapZoneNodeData;
	}
	else 
	{
		pmapNodeData = &m_mapNodeData;
	}

	KTDNodeDataMap::iterator iter;
	iter = pmapNodeData->find(a_nxCheckValue);

	if(iter == pmapNodeData->end())
	{
		a_strNodeName = _T(" - ");
		return false;
	}

	a_strNodeName = iter->second;
	return true;
}


Integer KShortestPathBase::ValidNodeId( CComboBox* a_pcboNode, int a_nSelCur, bool a_bZoneNode, bool a_bMessage )
{
	Integer nxNodeId     = -1;

	if(a_nSelCur < 0)	// 사용자 입력
	{
		bool    bExist = true;
		Integer nxUserInputValue  = 0;
		CString strUserInputValue = _T("");

		a_pcboNode->GetWindowText(strUserInputValue);

		bExist =       QBicStringChecker::IsUnSignedInteger(strUserInputValue);

		if(bExist)
		{
			nxUserInputValue = _ttoi64(strUserInputValue);

			bExist =   IsNodeID(nxUserInputValue, a_bZoneNode);
		}

		if(!bExist)
		{
			if( a_bMessage)
			{
				MessageExistNode(a_bZoneNode);
				a_pcboNode->SetFocus();
			}

			nxNodeId = -1;
		}
		else
		{
			nxNodeId = nxUserInputValue;
		}

		return nxNodeId;
	}

	nxNodeId = a_pcboNode->GetItemData(a_nSelCur);

	return nxNodeId;
}


void KShortestPathBase::InitNodeStationStyle(TNodeSelectionInfo &a_oTNodeStation)
{
	a_oTNodeStation.TNodeID       = -1;
	a_oTNodeStation.TType           = 0;
	a_oTNodeStation.TCharacterIndex = NODE_POINT_CHAR_INDEX;
	a_oTNodeStation.TSize           = NODE_POINT_SIZE;
	a_oTNodeStation.TColorRGB       = NODE_POINT_COLOR;
	a_oTNodeStation.TLabel          = _T("");
}