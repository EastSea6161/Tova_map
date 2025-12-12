#pragma once

#include "model_common.h"
#include "stdafx.h"


class KGenerationInfo;
class KXmlManager;
class TiXmlElement;
class KTarget;

class KGDefaultSimpleFuntion
{

public:
	KGDefaultSimpleFuntion(KGenerationInfo* a_pKGenerationInfo, KXmlManager* a_pKXmlManager);
	~KGDefaultSimpleFuntion(void);


public:

	void SetTarget(KTarget* a_pTarget);

	TiXmlElement* SetFindModeElement(TiXmlElement* a_pNextElement);

	void SetDefaultSimpleUnitExpression(TiXmlElement* a_pElement);

	void SetDefaultSimpleRegresstionExpresstion(TiXmlElement* a_pElement);

	void SetDefaultSimpleClassificationExpresstion(TiXmlElement* a_pElement);

	bool isOperator(LPCTSTR a_strElement);

	bool isFunction(LPCTSTR a_strElement);

	bool isField(LPCTSTR a_strElement);

	std::tstring getFieldName(LPCTSTR a_strElement);

	std::tstring getFieldCaption(LPCTSTR a_strElement);


protected:
	KTarget*         m_pTarget;
	KGenerationInfo* m_pKInfo;
	KXmlManager*     m_pXmlManager;
};

