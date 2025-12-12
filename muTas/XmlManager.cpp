#include "StdAfx.h"
#include "KmzSystem.h"
#include "XmlManager.h"

KXmlManager::KXmlManager(void)
{
	m_pxmlDocument = new TiXmlDocument();
}


KXmlManager::~KXmlManager(void)
{
	if(nullptr != m_pxmlDocument)
	{
		QBicDelete(m_pxmlDocument);
	}
}


bool KXmlManager::LoadTargetXml(LPCTSTR a_lpcszTargetPath, LPCTSTR a_lpcszFileName)
{
	CString strTargetFile(_T(""));
	strTargetFile.Format(_T("%s\\%s"), a_lpcszTargetPath, a_lpcszFileName);

	std::string szLoadName = UnicodeToMulitybyte(strTargetFile);	
	bool bResult = m_pxmlDocument->LoadFile(szLoadName);

	return bResult;
}


bool KXmlManager::SaveTargetXml(LPCTSTR a_lpcszTargetPath, LPCTSTR a_lpcszFileName)
{	
	CString strTargetFile(_T(""));
	strTargetFile.Format(_T("%s\\%s"), a_lpcszTargetPath, a_lpcszFileName);

	std::string strTargetConvert = UnicodeToMulitybyte(strTargetFile);
	bool bResult = m_pxmlDocument->SaveFile(strTargetConvert);

	return bResult;
}


void KXmlManager::CreateDeclaration(LPCTSTR a_strVersion, LPCTSTR a_strEncode, LPCTSTR a_strStandAlone)
{
	std::string szVersion    = UnicodeToMulitybyte(a_strVersion);
	std::string szEncode     = UnicodeToMulitybyte(a_strEncode);
	std::string szStandAlone = UnicodeToMulitybyte(a_strStandAlone);

	TiXmlDeclaration* pDecl = new TiXmlDeclaration(szVersion, szEncode, szStandAlone);

	m_pxmlDocument->LinkEndChild(pDecl);
}


TiXmlElement* KXmlManager::CreateRootElement(LPCTSTR a_strRootName)
{

	if (a_strRootName == _T(""))
	{
		return nullptr;
	}

	TiXmlElement* pRoot = nullptr;

	try
	{
		std::string   szRootName = UnicodeToMulitybyte(a_strRootName);

		pRoot      = new TiXmlElement(szRootName);

		TiXmlNode* pCreatedNode  = m_pxmlDocument->LinkEndChild(pRoot);

		if( nullptr == pCreatedNode ||  pRoot != pCreatedNode )
		{
			return nullptr;
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

	return pRoot;
}


TiXmlElement* KXmlManager::CreateChildElement(LPCTSTR a_strChdName, TiXmlElement* a_pParentElement)
{
	if (a_strChdName == _T("") || a_pParentElement == nullptr)
	{
		return nullptr;
	}

	TiXmlElement* pChdElement = nullptr;

	try
	{
		std::string   szChildName = UnicodeToMulitybyte(a_strChdName);

		pChdElement = new TiXmlElement(szChildName);

		TiXmlNode* pCreatedNode   = a_pParentElement->LinkEndChild(pChdElement);

		if( nullptr == pCreatedNode ||  pChdElement != pCreatedNode )
		{
			return nullptr;
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

	return pChdElement;
}


void KXmlManager::CreateComment(LPCTSTR a_strContents, TiXmlElement* a_pElement)
{
	std::string szContents = UnicodeToMulitybyte(a_strContents);

	TiXmlComment* pComment = new TiXmlComment();

	pComment->SetValue(szContents);

	a_pElement->LinkEndChild(pComment);
}


TiXmlElement* KXmlManager::AccessRootElement(LPCTSTR a_strRootName)
{	
	std::string szRootName    = UnicodeToMulitybyte(a_strRootName);
	TiXmlElement* pxmlElement = m_pxmlDocument->FirstChildElement(szRootName);
	if(pxmlElement == NULL)
	{
		CString strMessage; 
		strMessage.Format(_T("%s의 Element가 없습니다."), a_strRootName);
		return nullptr;
	}
	else
	{
		return pxmlElement;
	}
}


TiXmlElement* KXmlManager::AccessElment(LPCTSTR a_strElementName, TiXmlElement* a_pElement)
{
	if(a_pElement == NULL)
	{
		return NULL;
	}
	
	std::string szElementName = UnicodeToMulitybyte(a_strElementName);
	TiXmlElement* pxmlElement = a_pElement->FirstChildElement(szElementName);
	return pxmlElement;
}


TiXmlElement* KXmlManager::CheckRootElement(LPCTSTR a_strRootName)
{
	// 있는지 확인 
	TiXmlElement* pRoot = AccessRootElement(a_strRootName);

	if(nullptr == pRoot)
	{
		pRoot = CreateRootElement(a_strRootName);
	}

	return pRoot;
}


TiXmlElement* KXmlManager::CheckChildElement(LPCTSTR a_strChdName, TiXmlElement* a_pParentElement)
{
	TiXmlElement* pChdElement = AccessElment(a_strChdName, a_pParentElement);

	if(nullptr == pChdElement)
	{
		pChdElement = CreateChildElement(a_strChdName, a_pParentElement);
	}

	return pChdElement;
}


void KXmlManager::SetAttributeInt(LPCTSTR a_lpcszAttributeName, TiXmlElement* a_pElement, int a_nVal)
{
	try
	{
		std::string szConvertName = UnicodeToMulitybyte(a_lpcszAttributeName);
		a_pElement->SetAttribute(szConvertName, a_nVal);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		a_nVal = 0;
	}
	catch(...)
	{
		TxLogDebugException();
		a_nVal = 0;
	}
}


void KXmlManager::SetAttributeDouble(LPCTSTR a_lpcszAttributeName, TiXmlElement* a_pElement, double a_dVal)
{
	try
	{
		std::string szConvertName  = UnicodeToMulitybyte(a_lpcszAttributeName);
		a_pElement->SetDoubleAttribute(szConvertName, a_dVal);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		a_dVal = 0;
	}
	catch(...)
	{
		TxLogDebugException();
		a_dVal = 0;
	}
}


int KXmlManager::GetAttributeInt(LPCTSTR a_strAttributeName, TiXmlElement* a_pElement)
{
	int nValue = 0;
	try
	{
		std::string szConvertName = UnicodeToMulitybyte(a_strAttributeName);
		a_pElement->Attribute(szConvertName, &nValue);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
	if(a_pElement == NULL)
	{
		return 0;
	}
	return nValue;
}


double KXmlManager::GetAttributeDouble(LPCTSTR a_strAttributeName, TiXmlElement* a_pElement)
{
	double dValue = 0.0;

	if(a_pElement == nullptr)
	{
		return dValue;
	}

	try
	{
		std::string szConvertName = UnicodeToMulitybyte(a_strAttributeName);
		a_pElement->Attribute(szConvertName, &dValue);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
	
	return dValue;
}


CString KXmlManager::GetAttributeStr(LPCTSTR a_strAttributeName, TiXmlElement* a_pElement)
{
	if (a_pElement == nullptr)
	{
		return _T("");
	}

	CString strReConvertValue = _T("");
	try
	{
		std::string szConvertName  = UnicodeToMulitybyte(a_strAttributeName);
		const char* pszConvertName = szConvertName.c_str();
		std::string szValue        = ""; 
		if (a_pElement->Attribute(pszConvertName) != NULL)
		{
			szValue = a_pElement->Attribute(pszConvertName);
		}
		strReConvertValue  = MulitybyteToUnicode(szValue);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}

	return  strReConvertValue;
}


TiXmlElement* KXmlManager::NextElement(LPCTSTR a_strAttributeName, TiXmlElement* a_pElement)
{	
	if (a_pElement == nullptr)
	{
		return nullptr;
	}

	TiXmlElement* pXmlElemnt = nullptr;
	
	try
	{
		std::string szConvertName   = UnicodeToMulitybyte(a_strAttributeName);
		const char* pszConvertName  = szConvertName.c_str();
		pXmlElemnt = a_pElement->NextSiblingElement(pszConvertName);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
	
	return pXmlElemnt;
}


CString KXmlManager::GetElementName(TiXmlElement* a_pElement)
{
	CString strConvertName(_T(""));

	if (a_pElement == nullptr)
	{
		return strConvertName;
	}
 
	try
	{
		char* abc = (char*)a_pElement->Value();
		strConvertName = CA2T(abc);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}

	return strConvertName;
}

CString KXmlManager::MulitybyteToUnicode( std::string a_strName )
{
	LPWSTR wsStr;
	int alen = strlen(a_strName.c_str());
	int wlen = MultiByteToWideChar(CP_ACP,0,a_strName.c_str(),alen,0,0);
	wsStr =(LPWSTR)calloc(wlen+1, sizeof(TCHAR));
	MultiByteToWideChar(CP_ACP,0,a_strName.c_str(),alen,wsStr,wlen);
	wsStr[wlen]= 0;
	CString strResult = (CString)wsStr;
	
	return strResult;
}

std::string KXmlManager::UnicodeToMulitybyte( CString a_strConvert )
{
	int nlen = a_strConvert.GetLength();
	char* szResult = NULL;
	int wlen = WideCharToMultiByte(CP_ACP, 0, a_strConvert, -1, szResult, 0, NULL, NULL);
	szResult = new char[wlen +1];
	::WideCharToMultiByte(CP_ACP, 0, a_strConvert, -1, szResult, wlen, NULL, NULL);

	std::string strResult = (std::string)szResult;

	return strResult;
}

void KXmlManager::SetAttributeString( LPCTSTR a_lpcszAttrbuteName, TiXmlElement* a_pElement, LPCTSTR strVal )
{
	std::string szConvertName  = UnicodeToMulitybyte(a_lpcszAttrbuteName);
	std::string szConvertValue = UnicodeToMulitybyte(strVal);

	a_pElement->SetAttribute(szConvertName, szConvertValue);
}

TiXmlElement* KXmlManager::AccessParentElement( TiXmlElement* a_pChildElement )
{
	try
	{
		TiXmlNode* pParentNode =  a_pChildElement->Parent();
		return pParentNode->ToElement();
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
