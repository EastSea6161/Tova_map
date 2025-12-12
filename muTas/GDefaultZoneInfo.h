
#pragma once

#include "../QBicExtStaticCppLib/tinyXmlLib/tinyxml.h"
#include "../QBicExtStaticCppLib/tinyXmlLib/tinystr.h"

class TiXmlElement;
class KZoneGroupValue;


class KGDefaultZoneInfo
{

public:
	KGDefaultZoneInfo(void);
	~KGDefaultZoneInfo(void);

public:

	void SetElement(TiXmlElement* a_pElement);
	void SetZoneGroupValue(KZoneGroupValue* a_pKZoneGroupValue);
	TiXmlElement* GetElement();
	KZoneGroupValue* GetZoneGroupValue();


protected:

	TiXmlElement*     m_pElement;
	KZoneGroupValue*  m_pZoneGroupValue;




};

