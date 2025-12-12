#include "StdAfx.h"
#include "GDefaultSimpleFuntion.h"
#include "KmzApp.h"
#include "../kmzModel/KModelInclude.h"
#include "ExpressionElement.h"
#include "ExpressionElementField.h"
#include "GenerationInfo.h"
#include "GGroupExpressionContainer.h"
#include "GRegressionExpression.h"
#include "GSimpleExpressionContainer.h"
#include "GUnitExpression.h"
#include "IOTableController.h"
#include "PAPurpose.h"
#include "PurposeManager.h"
#include "Target.h"
#include "ZoneGroupColumn.h"
#include "ZoneGroupValue.h"
#include "IOTableView.h"
#include "IOView.h"
#include "../QBicExtStaticCppLib/tinyXmlLib/tinyxml.h"
#include "../QBicExtStaticCppLib/tinyXmlLib/tinystr.h"
#include "XmlManager.h"
#include <stack>
#include "KmzSystem.h"
//#include "CodeGroup.h"
//#include "CodeManager.h"
#include "GDefaultZoneGroupFunction.h"
#include "GDefaultZoneInfo.h"
#include "Target.h"
#include "DBaseGenerationParaModel.h"
//^#include "DBaseConnector.h"

KGDefaultZoneGroupFunction::KGDefaultZoneGroupFunction(KIOTable* a_pKTable, KGenerationInfo* a_pKInfo, KXmlManager* a_pXmlmanager)
	: m_pKTable(a_pKTable)
	, m_pKInfo(a_pKInfo)
	, m_pXmlManager(a_pXmlmanager)
{
}


KGDefaultZoneGroupFunction::~KGDefaultZoneGroupFunction(void)
{
	m_pKTable = NULL;
	m_pKInfo  = NULL;
	m_pXmlManager = NULL;
	m_pKExpressionRoot = NULL;
}


void KGDefaultZoneGroupFunction::SetDefaultZoneGroupExpresstion(TiXmlElement *a_pElement)
{

	TxLogDebugStartMsg();

	TiXmlElement* pElement = a_pElement->FirstChildElement();

	LoadGroupElement(pElement);

	while (m_vecParentValue.size() != 0)
	{
		RoofGroupElement();
	}

	if(NULL != m_pKExpressionRoot)
	{
		KGGroupExpressionContainer* pContainer = 
			(KGGroupExpressionContainer*)m_pKInfo->ExpressionContainer();
		if(NULL == pContainer)
		{
			pContainer = (KGGroupExpressionContainer*)(m_pKInfo->CreateExpression());
		}

		KZoneGroupColumn* pGroupRoot = pContainer->GetGroupRoot();
		if(NULL == pGroupRoot)
		{
			pGroupRoot = pContainer->CreateGroupRoot(m_pKExpressionRoot->ColumnName(), m_pKExpressionRoot->ColumnCaption());
		}

		*pGroupRoot = *m_pKExpressionRoot;
	}

	TxLogDebugEndMsg();

}


void KGDefaultZoneGroupFunction::LoadGroupElement(TiXmlElement *a_pElement)
{
	TxLogDebugStartMsg();

	TiXmlElement* pCateroryElement = a_pElement;
	CString strExpressionFieldName = m_pXmlManager->GetElementName(pCateroryElement);
	if(strExpressionFieldName != _T("Category"))
	{
		return;
	}

	KZoneGroupColumn* pGroupColumn      = NULL;
	KCodeManager* pKCodeManager         = NULL;
	KGDefaultZoneInfo* pKAcceptZoneInfo = NULL;
	CString strFieldName                = m_pXmlManager->GetAttributeStr(_T("ID"),pCateroryElement);
	const KIOColumns* pColumns = m_pKTable->Columns();
	int nColumnCount = pColumns->ColumnCount();

	KIOColumn* pSingleColumn  = nullptr;

	for (int i = 0; i < nColumnCount; i++)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		if (strFieldName.CompareNoCase(pColumn->Caption()) == 0)
		{
			pSingleColumn = pColumn;
			break;
		}
	};

	if (pSingleColumn == nullptr)
	{
		return;
	}

	KIOTableOwner* pOwner                 = m_pKTable->GetOwner();
	if(NULL != pOwner)
	{
		pKCodeManager = pOwner->GetCodeManager();
	}

	KCodeGroup* pCodeData = pKCodeManager->FindCodeGroup(pSingleColumn->CodeGroup());
	ASSERT(NULL != pCodeData);

	if (m_vecParentValue.size() == 0)
	{
		m_pKExpressionRoot = new KZoneGroupColumn;
		m_pKExpressionRoot->ColumnCaption(pSingleColumn->Caption());
		m_pKExpressionRoot->ColumnName(pSingleColumn->Name());
		pGroupColumn = m_pKExpressionRoot;
	} 
	else
	{
		pKAcceptZoneInfo = m_vecParentValue.back();
		m_vecParentValue.pop_back();
		KZoneGroupValue* pGroupValue = pKAcceptZoneInfo->GetZoneGroupValue();
		pGroupColumn = pGroupValue->AddSubColumn(pSingleColumn->Name(),pSingleColumn->Caption());
	}

	std::vector<int> vecCodeList;
	std::vector<int>::iterator itCode, itEnd;
	pCodeData->GetCodes(vecCodeList);

	itEnd = vecCodeList.end();

	for (itCode = vecCodeList.begin(); itCode != itEnd; ++itCode)
	{
		KZoneGroupValue* pSubValue = pGroupColumn->AddZoneGroupValue();
		pSubValue->Value(*itCode);
		pSubValue->ValueName(pCodeData->SingleCodeValue(*itCode));
		pSubValue->CreateExpressionObject(m_pKInfo->GenerationModel());
		int nCodeValue = *itCode;

		TiXmlElement* pGroupElemnet = NULL;
		pGroupElemnet = FindMatchingElement(pCateroryElement, nCodeValue);

		bool bUseGroup = ( m_pXmlManager->GetAttributeInt(_T("UseGroup"), pGroupElemnet) == 1 );
		if (bUseGroup)
		{
			KGDefaultZoneInfo* GDefaultZoneInfo = new KGDefaultZoneInfo();
			TiXmlElement* pGroupFirstchildElement = pGroupElemnet->FirstChildElement();
			GDefaultZoneInfo->SetElement(pGroupFirstchildElement);
			GDefaultZoneInfo->SetZoneGroupValue(pSubValue);
			m_vecParentValue.push_back(GDefaultZoneInfo);
		} 
		else
		{
			FindGroupMode(pGroupElemnet, pSubValue);
		}
	}
	TxLogDebugEndMsg();
}


void KGDefaultZoneGroupFunction::RoofGroupElement()
{
	TxLogDebugStartMsg();

	KGDefaultZoneInfo* pAcceptZoneInfo = m_vecParentValue.back();
	TiXmlElement* pElement = pAcceptZoneInfo->GetElement();
	LoadGroupElement(pElement);

	TxLogDebugEndMsg();
}


TiXmlElement* KGDefaultZoneGroupFunction::FindMatchingElement(TiXmlElement *a_pElement, int a_nCodeValue)
{
	TxLogDebugStartMsg();
	TiXmlElement* pColumnElemnt = a_pElement->FirstChildElement();
	TiXmlElement* pMatchingElemnt = NULL;

	while (pColumnElemnt)
	{
		int ElementCode = m_pXmlManager->GetAttributeInt(_T("Code"), pColumnElemnt);
		if (ElementCode == a_nCodeValue)
		{
			pMatchingElemnt = pColumnElemnt;
			break;
		}
		pColumnElemnt = pColumnElemnt->NextSiblingElement();
	}
	
	TxLogDebugEndMsg();
	return pMatchingElemnt;
	
}


void KGDefaultZoneGroupFunction::FindGroupMode(TiXmlElement* a_pElement, KZoneGroupValue* a_pKZoneGroupValue)
{
	TxLogDebugStartMsg();

	if (KEMGenerationUnit == m_pKInfo->GenerationModel())
	{
		KGUnitExpression* pExpression = (KGUnitExpression*)(a_pKZoneGroupValue->ExpressionObject());
		SetDefaultZoneGroupUnitExpression(a_pElement, pExpression);
	} 
	else if (KEMGenerationRegression == m_pKInfo->GenerationModel())
	{
		KGRegressionExpression* pExpression = (KGRegressionExpression*)(a_pKZoneGroupValue->ExpressionObject());
		SetDefaultZoneGroupRegresstionExpresstion(a_pElement, pExpression);
	}
	else if (KEMGenerationCrossClassification == m_pKInfo->GenerationModel())
	{
		 KGCategoryExpression* pExpression = (KGCategoryExpression*)(a_pKZoneGroupValue->ExpressionObject());
		 SetDefaultZoneGroupClassificationExpresstion(a_pElement, pExpression);
	}

	TxLogDebugEndMsg();
}


void KGDefaultZoneGroupFunction::SetDefaultZoneGroupUnitExpression(TiXmlElement* a_pElement, KGUnitExpression* a_pExpression)
{

	TxLogDebugStartMsg();

	if (a_pElement == NULL)
	{
		return;
	}

	TiXmlElement* a_pExpressionElement  = m_pXmlManager->AccessElment(_T("Expression"), a_pElement);
	double dExpressionValue             = m_pXmlManager->GetAttributeDouble(_T("Value"), a_pExpressionElement);
	CString strExpressionFieldName      = m_pXmlManager->GetAttributeStr(_T("Field"), a_pExpressionElement);
	
	KIOTables* pTables            = m_pTarget->Tables();
	KIOTable* pSEDVector                = pTables->FindTable(_T("sed_vector"));
	const KIOColumns* pColumns = pSEDVector->Columns();

	CString strColumnName(_T(""));
	int nColumnCount = pColumns->ColumnCount();
	for (int i = 0; i < nColumnCount; i++)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		if (pColumn->Caption()== strExpressionFieldName)
		{
			strColumnName = pColumn->Name();
			break;
		}
	}

	a_pExpression->Number(dExpressionValue);
	a_pExpression->Field(strColumnName, strExpressionFieldName);

	TxLogDebugEndMsg();

}


void KGDefaultZoneGroupFunction::SetDefaultZoneGroupRegresstionExpresstion(TiXmlElement* a_pElement, KGRegressionExpression* a_pKExpression)
{
	TxLogDebugStartMsg();

	TiXmlElement* a_pExpressionElement   = m_pXmlManager->AccessElment(_T("Expression"), a_pElement);
	CString strExpressionFieldName       = m_pXmlManager->GetAttributeStr(_T("Value"), a_pExpressionElement);
	std::tstring str                     = (LPCTSTR)strExpressionFieldName;

	typedef boost::tokenizer<
		boost::char_separator<TCHAR>, 
		std::tstring::const_iterator, 
		std::tstring> ttokenizer;

	boost::char_separator<TCHAR> sep(_T(" "));
	ttokenizer tokens(str, sep);
	for(ttokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it)
	{
		std::tstring strToken = *it;
// 		if(KDBaseGenerationParaModel::isField(strToken.c_str()) == true)
// 		{
// 			std::tstring strName    = KDBaseGenerationParaModel::getFieldName(m_pTarget, strToken.c_str());
// 			std::tstring strCaption = KDBaseGenerationParaModel::getFieldCaption(m_pTarget, strToken.c_str());
// 			a_pKExpression->AddField(strName.c_str(), strCaption.c_str());
// 		}
// 		else if(KDBaseGenerationParaModel::isOperator(strToken.c_str()) == true)
// 		{
// 			a_pKExpression->AddOperator(strToken.at(0));
// 		}
// 		else if(KDBaseGenerationParaModel::isFunction(strToken.c_str()) == true)
// 		{
// 			a_pKExpression->AddFunction(strToken.c_str());
// 		}
// 		else
// 		{
// 			a_pKExpression->AddNumber(_tstof(strToken.c_str()));
// 		}
	}

	TxLogDebugEndMsg();

}


void KGDefaultZoneGroupFunction::SetDefaultZoneGroupClassificationExpresstion(TiXmlElement* a_pElement, KGCategoryExpression* a_pKExpression)
{

	TxLogDebugStartMsg();

	TiXmlElement* pExpressionElement = m_pXmlManager->AccessElment(_T("Expression"), a_pElement);
	TiXmlElement* pClassElement      = m_pXmlManager->AccessElment(_T("Class"), pExpressionElement);
	
	int nCategoryID                  = m_pXmlManager->GetAttributeInt(_T("CategoryID"), pExpressionElement);
	CString strCategoryName          = GetClassificationName(nCategoryID);
	a_pKExpression->m_strInfo        = strCategoryName;                 
	a_pKExpression->m_category_id    = nCategoryID;
	
	std::map <KCategoryKey, double>& categoryUnitValues = a_pKExpression->m_categoryUnitValues;
	categoryUnitValues.clear();

	while(pClassElement != NULL)
	{
		int nSexKey   = m_pXmlManager->GetAttributeInt(_T("SexKey"), pClassElement);
		int nAgeSeq   = m_pXmlManager->GetAttributeInt(_T("AgeSeq"), pClassElement);
		double dValue = m_pXmlManager->GetAttributeDouble(_T("Value"), pClassElement);

		KCategoryKey key(nSexKey, nAgeSeq);
		categoryUnitValues.insert(std::make_pair(key, dValue));
		pClassElement = m_pXmlManager->NextElement(_T("Class"), pClassElement);
	}

	TxLogDebugEndMsg();
}


CString KGDefaultZoneGroupFunction::GetClassificationName(int a_nCategoryID)
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQL;
	strSQL.Format (_T("SELECT Name FROM Category_Class Where category_id = %d "), a_nCategoryID);
	
	KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
	if(nullptr == spResult)
	{
		return _T("");
	}
	
	bool bExist = spResult->Next();
	if (!bExist)
	{
		return _T("");
	}

	CString strResultValue = spResult->GetValueString(0);

	TxLogDebugEndMsg();
	return strResultValue;

}

void KGDefaultZoneGroupFunction::SetTarget( KTarget* a_pTarget )
{
	if (a_pTarget == NULL)
	{
		return;
	}

	m_pTarget  = a_pTarget;
}
