#pragma once
#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"
#include <fstream>
#include <iostream>
//#include "StdioFileEx.h"
//^^ #include "IOColumns.h"
#include "ShortestPathInfo.h"
#include "NodeSelectionInfo.h"
#include "ImChampIDFind.h"

/* Forward declarations */
class KTarget;
class KIOTable;
class KMapView;
class KIOColumns;

/* type definitions */
typedef std::map<Integer, CString> KTDNodeDataMap;

#define NODEITEM_LENGTH		50000
#define ZONENODEITEM_LENGTH 30000
#define DATA_SIZE			20

enum 
{
	NODE_POINT_SIZE       = 15,
	NODE_POINT_CHAR_INDEX = 74
};

class KShortestPathBase
{
public:
	KShortestPathBase(void);
	virtual ~KShortestPathBase(void) = 0;

public:
	void SetTarget   (KTarget* a_pTarget);
	void SetLinkTable(KIOTable* a_pLinkTable);
	void SetMapView  (KMapView* a_pMapView);

public:
	static CString DisplayScenarioTitle( CString a_strTitle );

protected:
	void MessageExistNode(bool a_bZoneNode);
	bool IsNodeID(Integer a_nxCheckValue, bool a_bZoneNode);
	bool IsNodeID(Integer a_nxCheckValue, bool a_bZoneNode, CString &a_strNodeName);
	Integer ValidNodeId( CComboBox* a_pcboNode, int a_nSelCur, bool a_bZoneNode, bool a_bMessage = true );
	void InitNodeStationStyle(TNodeSelectionInfo &a_oTNodeStation);

protected:

	KTarget*  m_pTarget;
	KIOTable* m_pLinkTable;
	KMapView* m_pMapView;

	KTDNodeDataMap m_mapNodeData;
	KTDNodeDataMap m_mapZoneNodeData;

	KTDNodeDataMap m_mapEndNodeData;
	KTDNodeDataMap m_mapEndZoneNodeData;

	std::vector<KIDCaption> m_vecNodeIDCaption;
	std::vector<KIDCaption> m_vecEndNodeIDCaption;
	std::vector<KIDCaption> m_vecZoneNodeIDCaption;
	std::vector<KIDCaption> m_vecEndZoneNodeIDCaption;

	std::vector<Integer>    m_vecEndZoneNodeID;

	std::vector<TNodeSelectionInfo> m_vecTNodeStation;
	std::vector<TNodeSelectionInfo> m_vecTZoneNodeStation;

	KIOColumns m_oUserColumnsLink;
	int					m_nCriterionIndex;
	int					m_nLinkColumnCount;	// index total count
	Integer				m_nxStartNodeID;
	Integer				m_nxEndNodeID;
};

