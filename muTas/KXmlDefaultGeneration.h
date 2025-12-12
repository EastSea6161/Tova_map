#pragma once

#include "model_common.h"

class KProject;
class KTarget;
class KIOColumn;
class KCodeGroup;

class KGenerationInfo;
class KGUnitExpression;
class KGRegressionExpression;
class KGCategoryExpression;
class KZoneGroupColumn;
class KZoneGroupValue;

class TDefaultZoneGroupParent;

class KXmlManager;
class TiXmlElement;


class KXmlDefaultGeneration
{
public:
	KXmlDefaultGeneration(KTarget* a_pTarget);
	~KXmlDefaultGeneration(void);

protected:
	KTarget*			m_pTarget;
	KXmlManager*		m_pXmlManager;

public:
	bool GetDefaultData(std::map<KIntKey, KGenerationInfo*>& a_mapGenerationInfo);

protected:
	TiXmlElement* GetFindModeElement(TiXmlElement* a_pElement, KGenerationInfo* a_pGenerationInfo);

	TiXmlElement* GetSimpleGroupData(TiXmlElement* a_pElement, KGenerationInfo* a_pGenertionInfo);

		bool GetSimpleUnitExpression(TiXmlElement* a_pElement, KGenerationInfo* a_pGenerationInfo);
			bool GetUnitExpression(TiXmlElement* a_pElement, KGUnitExpression* a_pExpression);
		
		bool GetSimpleRegressionExpression(TiXmlElement* a_pElement, KGenerationInfo* a_pGenerationInfo);
			bool GetRegressionExpression(TiXmlElement* a_pElement, KGRegressionExpression* a_pExpression);

		bool GetSimpleCategoryExpression(TiXmlElement* a_pElement, KGenerationInfo* a_pGenerationInfo);
			bool GetCategoryExpressioin(TiXmlElement* a_pElement, KGCategoryExpression* a_pExpression);

	TiXmlElement* GetZoneGroupData(TiXmlElement* a_pElement, KGenerationInfo* a_pGenertionInfo);
	bool GetGroupChildRoot(TiXmlElement* a_pElement, KGenerationInfo* a_pGenertionInfo, std::vector<TDefaultZoneGroupParent>& a_vecGroupParents, KZoneGroupColumn* a_pZoneGroupColumn = nullptr);
	bool GetGroupChideLoof(TiXmlElement* a_pElement, KGenerationInfo* a_pGenertionInfo, std::vector<TDefaultZoneGroupParent>& a_vecGroupParents);
		bool GetParentCodeData(TiXmlElement* a_pElement, std::map<CString, int>& a_mapCode);
		bool GetZoneNodeData(TiXmlElement* a_pElement, KGenerationInfo* a_pGenertionInfo, KZoneGroupColumn* a_pZoneGroupColumn, KCodeGroup* a_pCodeGroup, std::vector<int> a_vecCode, std::vector<TDefaultZoneGroupParent>& a_vecGroupParents);
		TiXmlElement* FindMatchingElement(TiXmlElement *a_pElement, int a_nCodeValue);
		bool GetNodeExpressionData(TiXmlElement* a_pElement, KGenerationInfo* a_pGenertionInfo, KZoneGroupValue* a_pKZoneGroupValue);

	KIOColumn* GetZoneColumn(CString strColumnCaption);
		
	std::tstring getFieldCaption(LPCTSTR a_strElement);
	std::tstring getFieldName(LPCTSTR a_strElement);
	bool isField(LPCTSTR a_strElement);
	bool isFunction(LPCTSTR a_strElement);
	bool isOperator(LPCTSTR a_strElement);


	//¾²±â
public:
	bool WriteDefaultData(KXmlManager& a_oXmlManager, TiXmlElement* a_pElement, KGenerationInfo* a_pGenerationInfo);
		bool WriteUnitData(KXmlManager& a_oXmlManager, TiXmlElement* a_pElement, KGenerationInfo* a_pGenerationInfo);
		bool WriteRegressionData(KXmlManager& a_oXmlManager, TiXmlElement* a_pElement, KGenerationInfo* a_pGenerationInfo);
		bool WriteClassificationData(KXmlManager& a_oXmlManager, TiXmlElement* a_pElement, KGenerationInfo* a_pGenerationInfo);
		bool WriteZoneGroupData(KXmlManager& a_oXmlManager, TiXmlElement* a_pElement, KGenerationInfo* a_pGenerationInfo);
			bool WriteZoneGroupLoofData(KXmlManager& a_oXmlManager, TiXmlElement* a_pElement, KGenerationInfo* a_pGenerationInfo, KZoneGroupColumn* a_pGroupRoot);
};

typedef std::shared_ptr<KXmlDefaultGeneration> KXmlDefaultGenerationPtr;
