#include "StdAfx.h"
#include "KXmlDefaultGeneration.h"
#include "Target.h"
#include "XmlManager.h"

#include "ImChampDir.h"
#include "PAPurpose.h"
#include "ImChampFrameWindow.h"
#include "DefineNamesDefaultPara.h"
#include "Project.h"
//#include "CodeGroup.h"
//#include "CodeManager.h"

#include "GenerationInfo.h"
#include "GGroupExpressionContainer.h"
#include "GRegressionExpression.h"
#include "GSimpleExpressionContainer.h"
#include "GUnitExpression.h"

#include "DBaseGeneration.h"

KXmlDefaultGeneration::KXmlDefaultGeneration(KTarget* a_pTarget)
	:m_pTarget(a_pTarget)
{
	m_pXmlManager = new KXmlManager;
}


KXmlDefaultGeneration::~KXmlDefaultGeneration(void)
{
	if (m_pXmlManager != nullptr)
	{
		QBicDelete(m_pXmlManager);
	}
}

bool KXmlDefaultGeneration::GetDefaultData( std::map<KIntKey, KGenerationInfo*>& a_mapGenerationInfo )
{
	try
	{
		KProject* pProject = ImChampFrameWindow::GetProject();
		CString strDir = pProject->GetLocation();
		strDir.AppendFormat(_T("\\%s\\"),  KDefaultParaFolder::FOLDER_PARAMETER);

		if (m_pXmlManager->LoadTargetXml(strDir, KDefaultParaFile::FILE_GENERATION) == false)
		{
			return false;
		}

		TiXmlElement* pRootElement = m_pXmlManager->AccessRootElement(_T("Generation-Default"));
		if (pRootElement == nullptr)
		{
			return false;
		}

		TiXmlElement* pGroupElement = m_pXmlManager->AccessElment(_T("PurposeGroup"), pRootElement);
		if (pGroupElement == nullptr)
		{
			return false;
		}

		AutoType iter = a_mapGenerationInfo.begin();
		AutoType end  = a_mapGenerationInfo.end();
		while(iter != end)
		{
			KGenerationInfo* pInfo = iter->second;
			TiXmlElement* pSelModeElement = GetFindModeElement(pGroupElement, pInfo);
			if (pSelModeElement == nullptr)
			{
				++iter;
				continue;
			}

			if (pInfo->ZoneGroup() == true)
			{
				GetZoneGroupData(pSelModeElement, pInfo);
			}
			else
			{
				GetSimpleGroupData(pSelModeElement, pInfo);
			}
			++iter;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}

TiXmlElement* KXmlDefaultGeneration::GetFindModeElement( TiXmlElement* a_pElement, KGenerationInfo* a_pGenerationInfo )
{
	TiXmlElement* pSelModeElement = nullptr;

	try
	{
		int nPurposeID = a_pGenerationInfo->Purpose()->PurposeID();

		CString strPAType;
		KEMPAType emPAType = a_pGenerationInfo->GetPAType();
		switch(emPAType)
		{
		case KEMPATypeProduction :
			strPAType = PRODUCTION_STRING;
			break;

		case KEMPATypeAttraction :
			strPAType = ATTRACTION_STRING;
			break;
		}

		TiXmlElement* pPurPoseElement = m_pXmlManager->AccessElment(_T("Purpose"), a_pElement);

		TiXmlElement* pMathingPurElement = nullptr;

		while(pPurPoseElement != nullptr)
		{
			int nElementID = m_pXmlManager->GetAttributeInt(_T("ID"), pPurPoseElement);

			if (nPurposeID == nElementID)
			{
				pMathingPurElement = pPurPoseElement;
				break;
			}
			pPurPoseElement = pPurPoseElement->NextSiblingElement();
		}

		if (pMathingPurElement == nullptr)
		{
			return nullptr;
		}

		TiXmlElement* pPATypeElement = m_pXmlManager->AccessElment(strPAType, pMathingPurElement);
		if (pPATypeElement == nullptr)
		{
			return nullptr;
		}

		TiXmlElement* pModeElement = m_pXmlManager->AccessElment(_T("Mode"), pPATypeElement);
		
		while(pModeElement != nullptr)
		{
			int nZoneGroup = m_pXmlManager->GetAttributeInt(_T("ZoneGroup"), pModeElement);
			bool bZoneGroup(false);
			if (nZoneGroup == 1)
			{
				bZoneGroup = true;
			}

			int nModeType = m_pXmlManager->GetAttributeInt(_T("Model"), pModeElement);
			if (a_pGenerationInfo->GenerationModel() == nModeType && a_pGenerationInfo->ZoneGroup() == bZoneGroup)
			{
				pSelModeElement = pModeElement;
				break;
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
	return pSelModeElement;
}

TiXmlElement* KXmlDefaultGeneration::GetSimpleGroupData( TiXmlElement* a_pElement, KGenerationInfo* a_pGenertionInfo )
{
	try
	{
		if (a_pGenertionInfo->GenerationModel() == KEMGenerationUnit)
		{
			GetSimpleUnitExpression(a_pElement, a_pGenertionInfo);
		}
		else if (a_pGenertionInfo->GenerationModel() == KEMGenerationRegression)
		{
			GetSimpleRegressionExpression(a_pElement, a_pGenertionInfo);
		}
		else if (a_pGenertionInfo->GenerationModel() == KEMGenerationCrossClassification)
		{
			GetSimpleCategoryExpression(a_pElement, a_pGenertionInfo);
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

bool KXmlDefaultGeneration::GetSimpleUnitExpression( TiXmlElement* a_pElement, KGenerationInfo* a_pGenerationInfo )
{
	KGUnitExpression* pKExpression = new KGUnitExpression;

	try
	{
		if (GetUnitExpression(a_pElement, pKExpression) == false)
		{
			throw -1;
		}

		KGSimpleExpressionContainer* pContainer = (KGSimpleExpressionContainer*) a_pGenerationInfo->ExpressionContainer();
		if(nullptr == pContainer)
		{
			pContainer = (KGSimpleExpressionContainer*)(a_pGenerationInfo->CreateExpression());
		}

		KGUnitExpression* pKInfoExpression = (KGUnitExpression*)(pContainer->ExpressionObject());
		if(nullptr == pKInfoExpression)
		{
			pKInfoExpression = (KGUnitExpression*)(pContainer->CreateExpressionObject());
		}

		*pKInfoExpression = *pKExpression;
	}
	catch(...)
	{
		return false;
	}

	if (pKExpression != nullptr)
	{
		QBicDelete(pKExpression);
	}
	return true;
}


bool KXmlDefaultGeneration::GetUnitExpression( TiXmlElement* a_pElement, KGUnitExpression* a_pExpression )
{
	try
	{
		TiXmlElement* pExpressElement = m_pXmlManager->AccessElment(_T("Expression"), a_pElement);
		if (pExpressElement == nullptr)
		{
			return false;
		}

		double dExValue				= m_pXmlManager->GetAttributeDouble(_T("Value"), pExpressElement);
		CString strEXFieldCaption	= m_pXmlManager->GetAttributeStr(_T("Field"), pExpressElement);

		CString strColumnName(_T(""));
		KIOTables* pTables				= m_pTarget->Tables();
		KIOTable* pSEDVector            = pTables->FindTable(_T("sed_vector"));
		const KIOColumns* pColumns		= pSEDVector->Columns();

		int nCountCount = pColumns->ColumnCount();
		for (int i = 0; i < nCountCount; i++)
		{
			KIOColumn* pColumn = pColumns->GetColumn(i);
			if (strEXFieldCaption.CompareNoCase(pColumn->Caption()) == 0)
			{
				strColumnName = pColumn->Name();
			}
		}

		a_pExpression->Number(dExValue);
		a_pExpression->Field(strColumnName, strEXFieldCaption);
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}


bool KXmlDefaultGeneration::GetSimpleRegressionExpression( TiXmlElement* a_pElement, KGenerationInfo* a_pGenerationInfo )
{
	KGRegressionExpression* pKExpression = new KGRegressionExpression;

	try
	{
		if (GetRegressionExpression(a_pElement, pKExpression) == false)
		{
			throw -1;
		}
		
		KGSimpleExpressionContainer* pContainer = (KGSimpleExpressionContainer*)a_pGenerationInfo->ExpressionContainer();
		if(nullptr == pContainer)
		{
			pContainer = (KGSimpleExpressionContainer*)(a_pGenerationInfo->CreateExpression());
		}

		KGRegressionExpression* pKInfoExpression = (KGRegressionExpression*)(pContainer->ExpressionObject());
		if(nullptr == pKInfoExpression)
		{
			pKInfoExpression = (KGRegressionExpression*)(pContainer->CreateExpressionObject());
		}

		*pKInfoExpression = *pKExpression;
	}
	catch(...)
	{
		return false;
	}

	if (pKExpression != nullptr)
	{
		QBicDelete(pKExpression);
	}

	return true;
}

bool KXmlDefaultGeneration::GetRegressionExpression( TiXmlElement* a_pElement, KGRegressionExpression* a_pExpression )
{
	try
	{
		TiXmlElement* pExpressionElement   = m_pXmlManager->AccessElment(_T("Expression"), a_pElement);
		if (pExpressionElement == nullptr)
		{
			return false;
		}

		CString strExpressionFieldName       = m_pXmlManager->GetAttributeStr(_T("Value"), pExpressionElement);
		if (strExpressionFieldName.IsEmpty() == true)
		{
			return false;
		}

		a_pExpression->SetCaptionExpression(strExpressionFieldName);

		typedef boost::tokenizer<
			boost::char_separator<TCHAR>, 
			std::tstring::const_iterator, 
			std::tstring> ttokenizer;
		boost::char_separator<TCHAR> sep(_T(" "));

		std::tstring str                     = (LPCTSTR)strExpressionFieldName;
		ttokenizer tokens(str, sep);
		for(ttokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it)
		{
			std::tstring strToken = *it;
			if(isField(strToken.c_str()) == true)
			{
				std::tstring strName    = getFieldName(strToken.c_str());
				std::tstring strCaption = getFieldCaption(strToken.c_str());
				a_pExpression->AddField(strName.c_str(), strCaption.c_str());
			}
			else if(isOperator(strToken.c_str()) == true)
			{
				a_pExpression->AddOperator(strToken.at(0));
			}
			else if(isFunction(strToken.c_str()) == true)
			{
				a_pExpression->AddFunction(strToken.c_str());
			}
			else
			{
				a_pExpression->AddNumber(_tstof(strToken.c_str()));
			}
		}
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


bool KXmlDefaultGeneration::GetSimpleCategoryExpression( TiXmlElement* a_pElement, KGenerationInfo* a_pGenerationInfo )
{
	KGCategoryExpression* pKExpression = new KGCategoryExpression;

	try
	{
		if (GetCategoryExpressioin(a_pElement, pKExpression) == false)
		{
			throw -1;
		}

		KGSimpleExpressionContainer* pContainer = (KGSimpleExpressionContainer*)a_pGenerationInfo->ExpressionContainer();
		if(nullptr == pContainer)
		{
			pContainer = (KGSimpleExpressionContainer*)(a_pGenerationInfo->CreateExpression());
		}

		KGCategoryExpression* pKInfoExpression = (KGCategoryExpression*)(pContainer->ExpressionObject());
		if(nullptr == pKInfoExpression)
		{
			pKInfoExpression = (KGCategoryExpression*)(pContainer->CreateExpressionObject());
		}

		*pKInfoExpression = *pKExpression;

	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	if (pKExpression != nullptr)
	{
		QBicDelete(pKExpression);
	}

	return true;
}


bool KXmlDefaultGeneration::GetCategoryExpressioin( TiXmlElement* a_pElement, KGCategoryExpression* a_pExpression )
{
	try
	{
		TiXmlElement* pExpressionElement = m_pXmlManager->AccessElment(_T("Expression"), a_pElement);
		if (pExpressionElement == nullptr)
		{
			return false;
		}

		TiXmlElement* pClassElement      = m_pXmlManager->AccessElment(_T("Class"), pExpressionElement);
		if (pClassElement != nullptr)
		{
			return false;
		}

		int nCategoryID                  = m_pXmlManager->GetAttributeInt(_T("CategoryID"), pExpressionElement);
		if (nCategoryID == 0)
		{
			return false;
		}

		a_pExpression->m_category_id                         = nCategoryID;
		
		std::map <KCategoryKey, double>& categoryUnitValues = a_pExpression->m_categoryUnitValues;
		categoryUnitValues.clear();

		std::map<KODKey, double> mapCategoryValues;
		KDBaseGeneration::GetCategoryClassValue(m_pTarget, nCategoryID, mapCategoryValues);
		AutoType iter = mapCategoryValues.begin();
		AutoType end  = mapCategoryValues.end();
		while(iter != end)
		{
			KODKey oODKey = iter->first;
			KCategoryKey oCKey((int)oODKey.OriginID, (int)oODKey.DestinationID);
			categoryUnitValues.insert(std::make_pair(oCKey, iter->second));
			++iter;
		}
		
		CString strCategoryName(_T(""));
		KDBaseGeneration::GetCategoryClassName(m_pTarget, nCategoryID, strCategoryName);
		a_pExpression->SetCaption(strCategoryName);

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}


std::tstring KXmlDefaultGeneration::getFieldCaption(LPCTSTR a_strElement)
{
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
	return strFieldCaption;


}

std::tstring KXmlDefaultGeneration::getFieldName(LPCTSTR a_strElement)
{
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
	return strFieldName;
}

bool KXmlDefaultGeneration::isOperator(LPCTSTR a_strElement)
{
	const static TCHAR* szOperators[] = 
	{
		_T("+"), _T("-"), _T("*"), _T("/"), 
		_T("^"), _T("("), _T(")")
	};

	std::tstring strValue           = a_strElement;
	static std::tstring strOperator = _T("+-*/^()");
	if(strValue.length() > 1)
	{
		return false;
	}

	int nOperator = sizeof(szOperators) / sizeof(TCHAR*);
	for(int i = 0; i < nOperator; ++i)
	{
		if(strValue.compare(szOperators[i]) == 0)
		{
			return true;
		}
	}
	return false;
}


bool KXmlDefaultGeneration::isFunction(LPCTSTR a_strElement)
{
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
			return true;
		}
	}
	return false;
}

bool KXmlDefaultGeneration::isField(LPCTSTR a_strElement)
{
	std::tstring strValue = a_strElement;
	if((strValue.find(_T("[")) != std::tstring::npos) || (strValue.find(_T("]")) != std::tstring::npos))
	{
		std::tstring::size_type strBracket = strValue.find_first_not_of(_T("["));
		strValue                           = (strBracket == std::tstring::npos) ? strValue : strValue.substr(strBracket, strValue.length());
		strBracket                         = strValue.find_last_not_of(_T("]"));
		strValue                           = (strBracket == std::tstring::npos) ? strValue : strValue.substr(0, strBracket + 1);

		return true;
	}
	return false;
}


TiXmlElement* KXmlDefaultGeneration::GetZoneGroupData( TiXmlElement* a_pElement, KGenerationInfo* a_pGenertionInfo )
{
	KZoneGroupColumn* pRootGroupExpression = new KZoneGroupColumn;
	try
	{
		std::vector<TDefaultZoneGroupParent> vecGroupParent;
		
		if (GetGroupChildRoot(a_pElement, a_pGenertionInfo, vecGroupParent, pRootGroupExpression) == false)
		{
			throw -1;
		}
		
		while(0 != vecGroupParent.size())
		{
			TDefaultZoneGroupParent oPInfo = vecGroupParent.back();
			GetGroupChideLoof(oPInfo.pPElement, a_pGenertionInfo, vecGroupParent);
		}
		
		if (nullptr != pRootGroupExpression)
		{
			KGGroupExpressionContainer* pContainer = 
				(KGGroupExpressionContainer*)a_pGenertionInfo->ExpressionContainer();
			if(nullptr == pContainer)
			{
				pContainer = (KGGroupExpressionContainer*)(a_pGenertionInfo->CreateExpression());
			}

			KZoneGroupColumn* pGroupRoot = pContainer->GetGroupRoot();
			if(nullptr == pGroupRoot)
			{
				pGroupRoot = pContainer->CreateGroupRoot(pRootGroupExpression->ColumnName(), pRootGroupExpression->ColumnCaption());
			}

			*pGroupRoot = *pRootGroupExpression;

			int a = 10;
		}
	}
	catch(...)
	{
		CString strLog(_T(""));
		strLog.Format(_T("%d _ %d _ %d"),a_pGenertionInfo->Purpose()->PurposeID(), a_pGenertionInfo->GetPAType(), a_pGenertionInfo->GenerationModel());
		TxLogDebug(strLog);
		TxLogDebugException();
	}
	
	if (pRootGroupExpression != nullptr)
	{
		QBicDelete(pRootGroupExpression);
	}
	return nullptr;
}


bool KXmlDefaultGeneration::GetGroupChildRoot( TiXmlElement* a_pElement, KGenerationInfo* a_pGenertionInfo, std::vector<TDefaultZoneGroupParent>& a_vecGroupParents, KZoneGroupColumn* a_pZoneGroupColumn )
{
	//KZoneGroupColumn* pZoneGroupColumn = nullptr;
	try
	{
		TiXmlElement* pCategoryElement = m_pXmlManager->AccessElment(_T("Category"), a_pElement);
		if (pCategoryElement == nullptr)
		{
			return false;
		}
		CString strCategoryName = m_pXmlManager->GetAttributeStr(_T("ID"), pCategoryElement);
		if (strCategoryName.IsEmpty() == true)
		{
			return false;
		}
		KIOColumn* pCategoryColumn = GetZoneColumn(strCategoryName);
		if (pCategoryColumn == nullptr)
		{
			return false;
		}

		KCodeManager* pCodeManager = m_pTarget->CodeManager();
		KCodeGroup* pCodeGroup = pCodeManager->FindCodeGroup(pCategoryColumn->CodeGroup());
		if (pCodeGroup == nullptr)
		{
			return false;
		}

		a_pZoneGroupColumn->ColumnCaption(pCategoryColumn->Caption());
		a_pZoneGroupColumn->ColumnName(pCategoryColumn->Name());
		//pZoneGroupColumn = a_pZoneGroupColumn;

		std::vector<int> vecAllCodeList;
		pCodeGroup->GetCodes(vecAllCodeList);

// 		std::map<CString, int> mapParentCodeInfo;
// 		GetParentCodeData(pCategoryElement, mapParentCodeInfo);
// 		
// 		std::set<int> setCodeData;
// 		KDBaseGeneration::GetSubCode(m_pTarget, pCategoryColumn->Name(), mapParentCodeInfo, setCodeData);
// 
// 		std::vector<int> vecMatingCodeData;
// 		for (size_t i = 0; i < vecAllCodeList.size(); i++)
// 		{
// 			int nCode = vecAllCodeList[i];
// 			AutoType iter = setCodeData.find(nCode);
// 			AutoType end  = setCodeData.end();
// 			if (iter != end)
// 			{
// 				vecMatingCodeData.push_back(nCode);
// 			}
// 		}

		GetZoneNodeData(pCategoryElement, a_pGenertionInfo, a_pZoneGroupColumn, pCodeGroup, vecAllCodeList, a_vecGroupParents);

// 		if (mapParentCodeInfo.size() < 1)
// 		{
// 			GetZoneNodeData(pCategoryElement, a_pGenertionInfo, a_pZoneGroupColumn, pCodeGroup, vecAllCodeList, a_vecGroupParents);
// 		}
// 		else
// 		{
// 			GetZoneNodeData(pCategoryElement, a_pGenertionInfo, a_pZoneGroupColumn, pCodeGroup, vecMatingCodeData, a_vecGroupParents);
// 		}
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}


bool KXmlDefaultGeneration::GetGroupChideLoof( TiXmlElement* a_pElement, KGenerationInfo* a_pGenertionInfo, std::vector<TDefaultZoneGroupParent>& a_vecGroupParents )
{
	KZoneGroupColumn* pZoneGroupColumn = nullptr;

	try
	{
		TiXmlElement* pCategoryElement = m_pXmlManager->AccessElment(_T("Category"), a_pElement);
		if (pCategoryElement == nullptr)
		{
			return false;
		}
		CString strCategoryName = m_pXmlManager->GetAttributeStr(_T("ID"), pCategoryElement);
		if (strCategoryName.IsEmpty() == true)
		{
			return false;
		}
		KIOColumn* pCategoryColumn = GetZoneColumn(strCategoryName);
		if (pCategoryColumn == nullptr)
		{
			return false;
		}

		KCodeManager* pCodeManager = m_pTarget->CodeManager();
		KCodeGroup* pCodeGroup = pCodeManager->FindCodeGroup(pCategoryColumn->CodeGroup());
		if (pCodeGroup == nullptr)
		{
			return false;
		}

		TDefaultZoneGroupParent oZoneGroupP = a_vecGroupParents.back();
		a_vecGroupParents.pop_back();
		KZoneGroupValue* pGroupValue = oZoneGroupP.pZoneGroupValue;
		pZoneGroupColumn = pGroupValue->AddSubColumn(pCategoryColumn->Name(), pCategoryColumn->Caption());

		std::vector<int> vecAllCodeList;
		pCodeGroup->GetCodes(vecAllCodeList);

		std::map<CString, int> mapParentCodeInfo;
		GetParentCodeData(pCategoryElement, mapParentCodeInfo);

		std::set<int> setCodeData;
		KDBaseGeneration::GetSubCode(m_pTarget, pCategoryColumn->Name(), mapParentCodeInfo, setCodeData);

		std::vector<int> vecMatingCodeData;
		for (size_t i = 0; i < vecAllCodeList.size(); i++)
		{
			int nCode = vecAllCodeList[i];
			AutoType iter = setCodeData.find(nCode);
			AutoType end  = setCodeData.end();
			if (iter != end)
			{
				vecMatingCodeData.push_back(nCode);
			}
		}

		if (mapParentCodeInfo.size() < 1)
		{
			GetZoneNodeData(pCategoryElement, a_pGenertionInfo, pZoneGroupColumn, pCodeGroup, vecAllCodeList, a_vecGroupParents);
		}
		else
		{
			GetZoneNodeData(pCategoryElement, a_pGenertionInfo, pZoneGroupColumn, pCodeGroup, vecMatingCodeData, a_vecGroupParents);
		}
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}


KIOColumn* KXmlDefaultGeneration::GetZoneColumn( CString strColumnCaption )
{
	KIOColumn* pGetColumn = nullptr;
	try
	{
		KIOTable* pZoneTable = m_pTarget->Tables()->FindTable(TABLE_ZONE);
		const KIOColumns* pColumns = pZoneTable->Columns();
		int nColumnCount = pColumns->ColumnCount();
		for (int i = 0; i < nColumnCount; i++)
		{
			KIOColumn* pColumn = pColumns->GetColumn(i);
			if (strColumnCaption.CompareNoCase(pColumn->Caption()) == 0)
			{
				pGetColumn = pColumn;
				break;
			}
		}
	}
	catch(...)
	{
		TxLogDebugException();
		return nullptr;
	}

	return pGetColumn;
}

bool KXmlDefaultGeneration::GetParentCodeData( TiXmlElement* a_pElement, std::map<CString, int>& a_mapCode )
{
	try
	{
		TiXmlElement* pGroupElement = m_pXmlManager->AccessParentElement(a_pElement);
		if (pGroupElement == nullptr)
		{
			return false;
		}

		CString strEleGName = m_pXmlManager->GetElementName(pGroupElement);
		if (strEleGName.CompareNoCase(_T("Group")) != 0)
		{
			return false;
		}

		int nCode = m_pXmlManager->GetAttributeInt(_T("Code"), pGroupElement);

		TiXmlElement* pCategoryElement = m_pXmlManager->AccessParentElement(pGroupElement);
		if (pCategoryElement == nullptr)
		{
			return false;
		}

		CString strEleCName = m_pXmlManager->GetElementName(pCategoryElement);
		if (strEleCName.CompareNoCase(_T("Category")) != 0)
		{
			return false;
		}

		CString strColumnCaption = m_pXmlManager->GetAttributeStr(_T("ID"), pCategoryElement);
		if (strColumnCaption.CompareNoCase(_T("")) == 0)
		{
			return false;
		}

		KIOColumn* pColumn = GetZoneColumn(strColumnCaption);
		if (pColumn == nullptr)
		{
			return false;
		}

		a_mapCode.insert(std::make_pair(pColumn->Name(), nCode));

		if (GetParentCodeData(pCategoryElement, a_mapCode) == false)
		{
			return false;
		}

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}

bool KXmlDefaultGeneration::GetZoneNodeData( TiXmlElement* a_pElement, KGenerationInfo* a_pGenertionInfo, KZoneGroupColumn* a_pZoneGroupColumn, KCodeGroup* a_pCodeGroup, std::vector<int> a_vecCode, std::vector<TDefaultZoneGroupParent>& a_vecGroupParents )
{
	try
	{
		size_t nCodeCount = a_vecCode.size();
		for (size_t i = 0; i < nCodeCount; i++)
		{
			int nCode = a_vecCode[i];
			
			KZoneGroupValue* pNodeValue = a_pZoneGroupColumn->AddZoneGroupValue();
			pNodeValue->Value(nCode);
			pNodeValue->ValueName(a_pCodeGroup->SingleCodeValue(nCode));
			pNodeValue->CreateExpressionObject(a_pGenertionInfo->GenerationModel());
			
			TiXmlElement* pGroupElement = FindMatchingElement(a_pElement, nCode);
			if (pGroupElement == nullptr)
			{
				continue;
			}

			int nZoneGroup = m_pXmlManager->GetAttributeInt(_T("UseGroup"), pGroupElement);
			if (nZoneGroup == 1)
			{
				TDefaultZoneGroupParent oParent;
				oParent.pPElement = pGroupElement;
				oParent.pZoneGroupValue = pNodeValue;
				a_vecGroupParents.push_back(oParent);
			}
			else
			{
				if (GetNodeExpressionData(pGroupElement, a_pGenertionInfo, pNodeValue) == true)
				{
					pNodeValue->DoneExpression(true);
				}
			}
		}

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}


TiXmlElement* KXmlDefaultGeneration::FindMatchingElement(TiXmlElement *a_pElement, int a_nCodeValue)
{
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
	return pMatchingElemnt;

}

bool KXmlDefaultGeneration::GetNodeExpressionData( TiXmlElement* a_pElement, KGenerationInfo* a_pGenertionInfo, KZoneGroupValue* a_pKZoneGroupValue )
{
	try
	{
		if (KEMGenerationUnit == a_pGenertionInfo->GenerationModel())
		{
			KGUnitExpression* pExpression = (KGUnitExpression*)(a_pKZoneGroupValue->ExpressionObject());
			if (GetUnitExpression(a_pElement, pExpression) == false)
			{
				return false;
			}
		}
		else if (KEMGenerationRegression == a_pGenertionInfo->GenerationModel())
		{
			KGRegressionExpression* pExpression = (KGRegressionExpression*)(a_pKZoneGroupValue->ExpressionObject());
			if (GetRegressionExpression(a_pElement, pExpression) == false)
			{
				return false;
			}

		}
		else if (KEMGenerationCrossClassification == a_pGenertionInfo->GenerationModel())
		{
			KGCategoryExpression* pExpression = (KGCategoryExpression*)(a_pKZoneGroupValue->ExpressionObject());
			if (GetCategoryExpressioin(a_pElement, pExpression) == false)
			{
				return false;
			}
		}
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}

bool KXmlDefaultGeneration::WriteDefaultData( KXmlManager& a_oXmlManager, TiXmlElement* a_pElement, KGenerationInfo* a_pGenerationInfo )
{
	try
	{
		if (a_pGenerationInfo->GenerationModel() == KEMGenerationUnit && a_pGenerationInfo->ZoneGroup() == false)
		{
			WriteUnitData(a_oXmlManager, a_pElement, a_pGenerationInfo);
		}
		else if (a_pGenerationInfo->GenerationModel() == KEMGenerationRegression && a_pGenerationInfo->ZoneGroup() == false)
		{
			WriteRegressionData(a_oXmlManager, a_pElement, a_pGenerationInfo);
		}
		else if (a_pGenerationInfo->GenerationModel() == KEMGenerationCrossClassification && a_pGenerationInfo->ZoneGroup() == false)
		{
			WriteClassificationData(a_oXmlManager, a_pElement, a_pGenerationInfo);
		}
		else if (a_pGenerationInfo->ZoneGroup() == true)
		{
			WriteZoneGroupData(a_oXmlManager, a_pElement, a_pGenerationInfo);
		}
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	
	return true;
}

bool KXmlDefaultGeneration::WriteUnitData( KXmlManager& a_oXmlManager, TiXmlElement* a_pElement, KGenerationInfo* a_pGenerationInfo )
{
	try
	{
		KGBaseExpressionContainer* pBaseCont = a_pGenerationInfo->ExpressionContainer();
		if (nullptr == pBaseCont)
		{
			return false;
		}
		
		KGSimpleExpressionContainer* pContainer = (KGSimpleExpressionContainer*)pBaseCont;
		if (nullptr == pContainer)
		{
			return false;
		}

		KGBaseExpression* pBaseExp = pContainer->ExpressionObject();
		if (nullptr == pBaseExp)
		{
			return false;
		}

		KGUnitExpression* pExpression = (KGUnitExpression*)pBaseExp;
		if (nullptr == pExpression)
		{
			return false;
		}


		TiXmlElement* pModeElement = a_oXmlManager.CreateChildElement(_T("Mode"), a_pElement);
		if (nullptr == pModeElement )
		{
			return false;
		}

		a_oXmlManager.SetAttributeInt(_T("ZoneGroup"), pModeElement, 0);
		a_oXmlManager.SetAttributeInt(_T("Model"), pModeElement, 1);

		TiXmlElement* pExpElement = a_oXmlManager.CreateChildElement(_T("Expression"), pModeElement);
		if (nullptr == pExpElement)
		{
			return false;
		}
		a_oXmlManager.SetAttributeString(_T("Field"), pExpElement, pExpression->FieldCaption());
		a_oXmlManager.SetAttributeDouble(_T("Value"), pExpElement, pExpression->Number());

	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}

bool KXmlDefaultGeneration::WriteRegressionData( KXmlManager& a_oXmlManager, TiXmlElement* a_pElement, KGenerationInfo* a_pGenerationInfo )
{
	try
	{
		KGBaseExpressionContainer* pBaseCont = a_pGenerationInfo->ExpressionContainer();
		if (nullptr == pBaseCont)
		{
			return false;
		}

		KGSimpleExpressionContainer* pContainer = (KGSimpleExpressionContainer*)pBaseCont;
		if (nullptr == pContainer)
		{
			return false;
		}

		KGBaseExpression* pBaseExp = pContainer->ExpressionObject();
		if (nullptr == pBaseExp)
		{
			return false;
		}

		KGRegressionExpression* pExpression = (KGRegressionExpression*)pBaseExp;
		if (nullptr == pExpression)
		{
			return false;
		}

		TiXmlElement* pModeElement = a_oXmlManager.CreateChildElement(_T("Mode"), a_pElement);
		if (nullptr == pModeElement )
		{
			return false;
		}

		a_oXmlManager.SetAttributeInt(_T("ZoneGroup"), pModeElement, 0);
		a_oXmlManager.SetAttributeInt(_T("Model"), pModeElement, 2);

		TiXmlElement* pExpElement = a_oXmlManager.CreateChildElement(_T("Expression"), pModeElement);
		if (nullptr == pExpElement)
		{
			return false;
		}

		a_oXmlManager.SetAttributeString(_T("Value"), pExpElement, pExpression->GetCaption());

	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}

bool KXmlDefaultGeneration::WriteClassificationData( KXmlManager& a_oXmlManager, TiXmlElement* a_pElement, KGenerationInfo* a_pGenerationInfo )
{
	try
	{
		KGBaseExpressionContainer* pBaseCont = a_pGenerationInfo->ExpressionContainer();
		if (nullptr == pBaseCont)
		{
			return false;
		}

		KGSimpleExpressionContainer* pContainer = (KGSimpleExpressionContainer*)pBaseCont;
		if (nullptr == pContainer)
		{
			return false;
		}

		KGBaseExpression* pBaseExp = pContainer->ExpressionObject();
		if (nullptr == pBaseExp)
		{
			return false;
		}

		KGCategoryExpression* pExpression = (KGCategoryExpression*)pBaseExp;
		if (nullptr == pExpression)
		{
			return false;
		}

		TiXmlElement* pModeElement = a_oXmlManager.CreateChildElement(_T("Mode"), a_pElement);
		if (nullptr == pModeElement )
		{
			return false;
		}

		a_oXmlManager.SetAttributeInt(_T("ZoneGroup"), pModeElement, 0);
		a_oXmlManager.SetAttributeInt(_T("Model"), pModeElement, 3);

		TiXmlElement* pExpElement = a_oXmlManager.CreateChildElement(_T("Expression"), pModeElement);
		if (nullptr == pExpElement)
		{
			return false;
		}

		a_oXmlManager.SetAttributeInt(_T("CategoryID"), pExpElement, pExpression->m_category_id);
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}

bool KXmlDefaultGeneration::WriteZoneGroupData( KXmlManager& a_oXmlManager, TiXmlElement* a_pElement, KGenerationInfo* a_pGenerationInfo )
{
	try
	{
		KGBaseExpressionContainer* pBaseCont = a_pGenerationInfo->ExpressionContainer();
		if(nullptr == pBaseCont)
		{
			return false;
		}

		if(pBaseCont->ContainerType() != KEMGExpressionContainerZoneGroup)
		{
			return false;
		}

		KGGroupExpressionContainer* pContainer = (KGGroupExpressionContainer*)pBaseCont;
		if (nullptr == pContainer)
		{
			return false;
		}

		KZoneGroupColumn* pGroupRoot           = pContainer->GetGroupRoot();
		if (nullptr == pGroupRoot)
		{
			return false;
		}

		TiXmlElement* pModeElement = a_oXmlManager.CreateChildElement(_T("Mode"), a_pElement);
		if (nullptr == pModeElement )
		{
			return false;
		}

		a_oXmlManager.SetAttributeInt(_T("ZoneGroup"), pModeElement, 1);

		if (a_pGenerationInfo->GenerationModel() == KEMGenerationUnit)
		{
			a_oXmlManager.SetAttributeInt(_T("Model"), pModeElement, 1);
		}
		else if (a_pGenerationInfo->GenerationModel() == KEMGenerationRegression)
		{
			a_oXmlManager.SetAttributeInt(_T("Model"), pModeElement, 2);
		}
		else if (a_pGenerationInfo->GenerationModel() == KEMGenerationCrossClassification)
		{
			a_oXmlManager.SetAttributeInt(_T("Model"), pModeElement, 3);
		}
		else
		{
			return false;
		}

		WriteZoneGroupLoofData(a_oXmlManager, pModeElement, a_pGenerationInfo, pGroupRoot);

	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}

bool KXmlDefaultGeneration::WriteZoneGroupLoofData( KXmlManager& a_oXmlManager, TiXmlElement* a_pElement, KGenerationInfo* a_pGenerationInfo, KZoneGroupColumn* a_pGroupRoot )
{
	try
	{
		TiXmlElement* pCategoryElement = a_oXmlManager.CreateChildElement(_T("Category"), a_pElement);
		a_oXmlManager.SetAttributeString(_T("ID"), pCategoryElement, a_pGroupRoot->ColumnCaption());

		KZoneGroupValue* pValue = a_pGroupRoot->GetFirstZoneGroupValue();

		while(nullptr != pValue)
		{
			TiXmlElement* pGroupElement = a_oXmlManager.CreateChildElement(_T("Group"), pCategoryElement);

			if (pGroupElement == nullptr)
			{
				break;
			}

			a_oXmlManager.SetAttributeInt(_T("Code"), pGroupElement, pValue->Value());

			if (pValue->HasSubColumn() == true)
			{
				a_oXmlManager.SetAttributeInt(_T("UseGroup"), pGroupElement, 1);
				KZoneGroupColumn* pSubColumn = pValue->GetSubColumn();
				if (pSubColumn == nullptr)
				{
					pGroupElement->Clear();
				}

				WriteZoneGroupLoofData(a_oXmlManager, pGroupElement, a_pGenerationInfo, pSubColumn);
			}
			else
			{
				a_oXmlManager.SetAttributeInt(_T("UseGroup"), pGroupElement, 0);

				KGBaseExpression* pExpression = pValue->ExpressionObject();
				if (pExpression == nullptr)
				{
					pGroupElement->Clear();
					pValue = a_pGroupRoot->GetNextZoneGroupValue();
					continue;
				}

				TiXmlElement* pExElement = a_oXmlManager.CreateChildElement(_T("Expression"), pGroupElement);
				if (pExElement == nullptr)
				{
					pGroupElement->Clear();
				}
				else
				{
					if (a_pGenerationInfo->GenerationModel() == KEMGenerationUnit)
					{
						KGUnitExpression* pUnitExpression = (KGUnitExpression*)pExpression;
						if (nullptr == pUnitExpression)
						{
							pGroupElement->Clear();
						}

						a_oXmlManager.SetAttributeString(_T("Field"), pExElement, pUnitExpression->FieldCaption());
						a_oXmlManager.SetAttributeDouble(_T("Value"), pExElement, pUnitExpression->Number());

					}
					else if (a_pGenerationInfo->GenerationModel() == KEMGenerationRegression)
					{
						KGRegressionExpression* pRegExpression = (KGRegressionExpression*)pExpression;
						if (nullptr == pRegExpression)
						{
							pGroupElement->Clear();
						}

						a_oXmlManager.SetAttributeString(_T("Value"), pExElement, pRegExpression->GetCaption());

					}
					else if (a_pGenerationInfo->GenerationModel() == KEMGenerationCrossClassification)
					{
						KGCategoryExpression* pCategExpression = (KGCategoryExpression*)pExpression;
						if (nullptr == pCategExpression)
						{
							pGroupElement->Clear();
						}

						a_oXmlManager.SetAttributeInt(_T("CategoryID"), pExElement, pCategExpression->m_category_id);
					}
				}
			}

			pValue = a_pGroupRoot->GetNextZoneGroupValue();
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

	return true;
}
