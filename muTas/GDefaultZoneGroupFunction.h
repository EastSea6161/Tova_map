


#pragma once

#include "model_common.h"
#include "stdafx.h"
#include "GenerationInfo.h"
#include "ZoneGroupValue.h"
#include "GUnitExpression.h"
#include "GRegressionExpression.h"
#include "KGCategoryExpression.h"

class KGBaseExpression;
class KIORecordset;
class KIOTable;
class KPAPurpose;
class KTarget;
class KZoneGroupBase;
class KXmlManager;
class TiXmlElement;
class KZoneGroupColumn;
class KGDefaultZoneInfo;
class KTarget;



class KGDefaultZoneGroupFunction
{
public:
	KGDefaultZoneGroupFunction(KIOTable* a_pKTable, KGenerationInfo* a_pKInfo, KXmlManager* a_pXmlmanager);
	~KGDefaultZoneGroupFunction(void);


public:

	void SetTarget(KTarget* a_pTarget);

	void RoofGroupElement();

	void SetDefaultZoneGroupExpresstion(TiXmlElement *a_pElement);

	void LoadGroupElement(TiXmlElement *a_pElement);
	
	TiXmlElement* FindMatchingElement(TiXmlElement *a_pElement, int a_nCodeValue);

	void FindGroupMode(TiXmlElement* a_pElement,KZoneGroupValue* a_pKZoneGroupValue);

	void SetDefaultZoneGroupUnitExpression(TiXmlElement* a_pElement, KGUnitExpression* a_pExpression);

	void SetDefaultZoneGroupRegresstionExpresstion(TiXmlElement* a_pElement, KGRegressionExpression* pKExpression);

	void SetDefaultZoneGroupClassificationExpresstion(TiXmlElement* a_pElement, KGCategoryExpression* a_pKExpression);

	CString GetClassificationName(int a_nCategoryID);



protected:

	KTarget*							m_pTarget;
	KIOTable*							m_pKTable;
	KGenerationInfo*					m_pKInfo;
	KXmlManager*						m_pXmlManager;
	std::vector<KGDefaultZoneInfo*>		m_vecParentValue;
	KZoneGroupColumn*					m_pKExpressionRoot;
};