#pragma once

#include "KEMEncoding.h"
#include "../QBicGIS/TxPoint.h"
#include "../QBicGIS/TxPolyline.h"
#include "../QBicGIS/TxPolygon.h"

class KTarget;

class KImportCommon
{
public:
	KImportCommon(void) {};
	~KImportCommon(void){};

public:
	static void DeleteNodeRelation        (KDBaseConPtr spDBase);
	static void DeleteZoneRelation        (KDBaseConPtr spDBase);
	static void DeleteLinkRelation        (KDBaseConPtr spDBase);
	static void DeleteIntersectionRelation(KDBaseConPtr spDBase);
	static void DeleteTransitInfoRelation (KDBaseConPtr spDBase);

public:
	static void NotifyNodeRelation        (KTarget* a_pTarget);
	static void NotifyTurnRelation        (KTarget* a_pTarget);
	static void NotifyZoneRelation        (KTarget* a_pTarget);
	static void NotifyLinkRelation        (KTarget* a_pTarget);
	static void NotifyIntersectionRelation(KTarget* a_pTarget);
	static void NotifyTransitInfoRelation (KTarget* a_pTarget);
	
public:	
	static bool IsNodeRelationColumn(CString a_strColunmName);
	static bool IsZoneRelationColumn(CString a_strColunmName);

public:
	static Integer MaxLinkID   (KDBaseConPtr spDBase, Integer a_nxStartID=0);
	static bool    IsEmptyTable(KTarget* a_pTarget, CString a_strTableName);
};