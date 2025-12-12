
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
#include "ChildFrm.h"
#include "MapView.h"
#include "MainFrameWnd.h"
#include "IOTableView.h"
#include "IOView.h"
#include "../QBicExtStaticCppLib/tinyXmlLib/tinyxml.h"
#include "../QBicExtStaticCppLib/tinyXmlLib/tinystr.h"
#include "KUIDefGeneration.h"
#include "XmlManager.h"
#include <stack>
#include "KmzSystem.h"
//#include "CodeGroup.h"
//#include "CodeManager.h"
#include "Target.h"
#include "DBaseGeneration.h"


KGDefaultSimpleFuntion::KGDefaultSimpleFuntion(KGenerationInfo* a_pKGenerationInfo, KXmlManager* a_pKXmlManager)
	:m_pKInfo(a_pKGenerationInfo)
	,m_pXmlManager(a_pKXmlManager)
{

}


KGDefaultSimpleFuntion::~KGDefaultSimpleFuntion(void)
{

}



void KGDefaultSimpleFuntion::SetTarget( KTarget* a_pTarget )
{
	if (a_pTarget == nullptr)
	{
		return;
	}

	m_pTarget = a_pTarget;
}



TiXmlElement* KGDefaultSimpleFuntion::SetFindModeElement(TiXmlElement* a_pElement)
{
	try
	{
		TiXmlElement*      pElem;
		int nPurposeID  =  m_pKInfo->GetObjectID();

		CString strPAType;
		int nPAType = m_pKInfo->GetPAType();
		switch(nPAType)
		{
		case 1: 
			strPAType = _T("Production");
			break;
		case 2: 
			if(KmzSystem::IsPassengerSystem())
			{
				strPAType = _T("Attraction");
			}
			else
			{
				strPAType = _T("Consumption");
			}
			break;
		}

		TiXmlElement* pPurPoseElement = m_pXmlManager->AccessElment(_T("Purpose"), a_pElement);

		while (pPurPoseElement != nullptr)
		{
			int nElementID = m_pXmlManager->GetAttributeInt(_T("ID"), pPurPoseElement);

			if (nPurposeID == nElementID)
			{
				pElem = pPurPoseElement;
				break;
			}
			pPurPoseElement = pPurPoseElement->NextSiblingElement();
		}

		if (pPurPoseElement == nullptr)
		{
			return nullptr;
		}

		TiXmlElement* pPAtypeElement = m_pXmlManager->AccessElment(strPAType, pElem);

		if (pPAtypeElement == nullptr)
		{
			return nullptr;
		}


		TiXmlElement* pModeElement	 = m_pXmlManager->AccessElment(_T("Mode"), pPAtypeElement);

		while (pModeElement != nullptr)
		{
			int nZoneGroup = m_pXmlManager->GetAttributeInt(_T("ZoneGroup"), pModeElement);
			bool bZoneGroup = (nZoneGroup != 0);
			int nModelType = m_pXmlManager->GetAttributeInt(_T("Model"), pModeElement);

			if (m_pKInfo->GenerationModel() == nModelType && m_pKInfo->ZoneGroup() == bZoneGroup)
			{
				return pModeElement;
			}
			pModeElement = pModeElement->NextSiblingElement();
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}

	return nullptr;
}




void KGDefaultSimpleFuntion::SetDefaultSimpleUnitExpression(TiXmlElement* a_pElement)
{
	try
	{
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

		KGUnitExpression* pKExpression = new KGUnitExpression;
		pKExpression->Number(dExpressionValue);
		pKExpression->Field(strColumnName, strExpressionFieldName);

		KGSimpleExpressionContainer* pContainer = (KGSimpleExpressionContainer*) m_pKInfo->ExpressionContainer();
		if(NULL == pContainer)
		{
			pContainer = (KGSimpleExpressionContainer*)(m_pKInfo->CreateExpression());
		}

		KGUnitExpression* pKInfoExpression = (KGUnitExpression*)(pContainer->ExpressionObject());
		if(NULL == pKInfoExpression)
		{
			pKInfoExpression = (KGUnitExpression*)(pContainer->CreateExpressionObject());
		}

		*pKInfoExpression = *pKExpression;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KGDefaultSimpleFuntion::SetDefaultSimpleRegresstionExpresstion(TiXmlElement* a_pElement)
{
	try
	{
			
		TiXmlElement* a_pExpressionElement   = m_pXmlManager->AccessElment(_T("Expression"), a_pElement);
		CString strExpressionFieldName       = m_pXmlManager->GetAttributeStr(_T("Value"), a_pExpressionElement);
		std::tstring str                     = (LPCTSTR)strExpressionFieldName;


		KGRegressionExpression* pKExpression = new KGRegressionExpression;
		typedef boost::tokenizer<
			boost::char_separator<TCHAR>, 
			std::tstring::const_iterator, 
			std::tstring> ttokenizer;

		boost::char_separator<TCHAR> sep(_T(" "));
		ttokenizer tokens(str, sep);
		for(ttokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it)
		{
			std::tstring strToken = *it;
			if(isField(strToken.c_str()) == true)
			{
				std::tstring strName    = getFieldName(strToken.c_str());
				std::tstring strCaption = getFieldCaption(strToken.c_str());
				pKExpression->AddField(strName.c_str(), strCaption.c_str());
			}
			else if(isOperator(strToken.c_str()) == true)
			{
				pKExpression->AddOperator(strToken.at(0));
			}
			else if(isFunction(strToken.c_str()) == true)
			{
				pKExpression->AddFunction(strToken.c_str());
			}
			else
			{
				pKExpression->AddNumber(_tstof(strToken.c_str()));
			}
		}

		
		KGSimpleExpressionContainer* pContainer = (KGSimpleExpressionContainer*)m_pKInfo->ExpressionContainer();
		if(NULL == pContainer)
		{
			pContainer = (KGSimpleExpressionContainer*)(m_pKInfo->CreateExpression());
		}

		KGRegressionExpression* pKInfoExpression = (KGRegressionExpression*)(pContainer->ExpressionObject());
		if(NULL == pKInfoExpression)
		{
			pKInfoExpression = (KGRegressionExpression*)(pContainer->CreateExpressionObject());
		}

		*pKInfoExpression = *pKExpression;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


bool KGDefaultSimpleFuntion::isOperator(LPCTSTR a_strElement)
{
	TxLogDebugStartMsg();
	const static TCHAR* szOperators[] = 
	{
		_T("+"), _T("-"), _T("*"), _T("/"), 
		_T("^"), _T("("), _T(")")
	};

	std::tstring strValue           = a_strElement;
	static std::tstring strOperator = _T("+-*/^()");
	if(strValue.length() > 1)
	{
		TxLogDebugEndMsg();
		return false;
	}

	int nOperator = sizeof(szOperators) / sizeof(TCHAR*);
	for(int i = 0; i < nOperator; ++i)
	{
		if(strValue.compare(szOperators[i]) == 0)
		{
			TxLogDebugEndMsg();
			return true;
		}
	}

	TxLogDebugEndMsg();
	return false;
}


bool KGDefaultSimpleFuntion::isFunction(LPCTSTR a_strElement)
{
	TxLogDebugStartMsg();
	const static TCHAR* szFunctions[] = 
	{
		_T("SQRT"), _T("EXP"), _T("LN"), _T("LOG"), 
		_T("ABS"), _T("POW")
	};

	std::tstring strValue = a_strElement;
	int nFunction         = sizeof(szFunctions) / sizeof(TCHAR*);
	for(int i = 0; i < nFunction; ++i)
	{
		if(_tcsicmp(strValue.c_str(), szFunctions[i]) == 0)
		{
			TxLogDebugEndMsg();
			return true;
		}
	}
	TxLogDebugEndMsg();
	return false;
}

bool KGDefaultSimpleFuntion::isField(LPCTSTR a_strElement)
{
	TxLogDebugStartMsg();
	std::tstring strValue = a_strElement;
	if((strValue.find(_T("[")) != std::tstring::npos) || (strValue.find(_T("]")) != std::tstring::npos))
	{
		std::tstring::size_type strBracket = strValue.find_first_not_of(_T("["));
		strValue                           = (strBracket == std::tstring::npos) ? strValue : strValue.substr(strBracket, strValue.length());
		strBracket                         = strValue.find_last_not_of(_T("]"));
		strValue                           = (strBracket == std::tstring::npos) ? strValue : strValue.substr(0, strBracket + 1);

		TxLogDebugEndMsg();
		return true;
	}
	TxLogDebugEndMsg();
	return false;
}


std::tstring KGDefaultSimpleFuntion::getFieldCaption(LPCTSTR a_strElement)
{
	TxLogDebugStartMsg();
	std::tstring strFieldCaption = a_strElement;
	if((strFieldCaption.find(_T("[")) != std::tstring::npos) || (strFieldCaption.find(_T("]")) != std::tstring::npos))
	{
		std::tstring::size_type strBracket = strFieldCaption.find_first_not_of(_T("["));
		strFieldCaption                    = (strBracket == std::tstring::npos) ? strFieldCaption : 
			strFieldCaption.substr(strBracket, strFieldCaption.length());
		strBracket                         = strFieldCaption.find_last_not_of(_T("]"));
		strFieldCaption                    = (strBracket == std::tstring::npos) ? strFieldCaption : 
			strFieldCaption.substr(0, strBracket + 1);
	}
	TxLogDebugEndMsg();
	return strFieldCaption;

	
}

std::tstring KGDefaultSimpleFuntion::getFieldName(LPCTSTR a_strElement)
{
	TxLogDebugStartMsg();
	std::tstring strValue     = a_strElement;
	std::tstring strFieldName = _T("");

	if((strValue.find(_T("[")) != std::tstring::npos) || (strValue.find(_T("]")) != std::tstring::npos))
	{
		std::tstring::size_type strBracket = strValue.find_first_not_of(_T("["));
		strValue                           = (strBracket == std::tstring::npos) ? strValue : strValue.substr(strBracket, strValue.length());
		strBracket                         = strValue.find_last_not_of(_T("]"));
		strValue                           = (strBracket == std::tstring::npos) ? strValue : strValue.substr(0, strBracket + 1);
	}

	LPCTSTR strGetstr                   = strValue.c_str();

	KIOTables* pTables            = m_pTarget->Tables();
	KIOTable* pSEDVector                = pTables->FindTable(_T("sed_vector"));
	const KIOColumns* pColumns = pSEDVector->Columns();
	int nColumnCount = pColumns->ColumnCount();
	for (int i = 0; i < nColumnCount; i++)
	{
		KIOColumn* pSingleColumn            = pColumns->GetColumn(i);
		if (pSingleColumn->Caption() == strValue)
		{
			return (LPCTSTR)pSingleColumn->Caption();
		}
	}

	TxLogDebugEndMsg();
	return strFieldName;
}


void KGDefaultSimpleFuntion::SetDefaultSimpleClassificationExpresstion(TiXmlElement* a_pElement)
{
	TxLogDebugStartMsg();
	KGCategoryExpression* pKExpression = new KGCategoryExpression;

	TiXmlElement* pExpressionElement = m_pXmlManager->AccessElment(_T("Expression"), a_pElement);
	if (pExpressionElement == nullptr)
	{
		return;
	}

	TiXmlElement* pClassElement      = m_pXmlManager->AccessElment(_T("Class"), pExpressionElement);
	if (pClassElement != nullptr)
	{
		return;
	}
	
	int nCategoryID                  = m_pXmlManager->GetAttributeInt(_T("CategoryID"), pExpressionElement);

	std::map<KODKey, double> mapCategoryValues;
	KDBaseGeneration::GetCategoryClassValue(m_pTarget, nCategoryID, mapCategoryValues);

	pKExpression->m_category_id                         = nCategoryID;
	std::map <KCategoryKey, double>& categoryUnitValues = pKExpression->m_categoryUnitValues;
	categoryUnitValues.clear();
	
	AutoType iter = mapCategoryValues.begin();
	AutoType end  = mapCategoryValues.end();
	while(iter != end)
	{
		KODKey oODKey = iter->first;
		KCategoryKey oCKey((int)oODKey.OriginID, (int)oODKey.DestinationID);
		categoryUnitValues.insert(std::make_pair(oCKey, iter->second));
		++iter;
	}

	KGSimpleExpressionContainer* pContainer = (KGSimpleExpressionContainer*)m_pKInfo->ExpressionContainer();
	if(NULL == pContainer)
	{
		pContainer = (KGSimpleExpressionContainer*)(m_pKInfo->CreateExpression());
	}

	KGCategoryExpression* pKInfoExpression = (KGCategoryExpression*)(pContainer->ExpressionObject());
	if(NULL == pKInfoExpression)
	{
		pKInfoExpression = (KGCategoryExpression*)(pContainer->CreateExpressionObject());
	}

	*pKInfoExpression = *pKExpression;
	TxLogDebugEndMsg();
}

