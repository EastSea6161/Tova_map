#include "StdAfx.h"
#include "CodeGroup.h"

const CString KCodeGroup::TABLE_CODEDATA = _T("kcode_define");

KCodeGroup::KCodeGroup(KDBaseConnectionPtr a_spDBaseConnection, int nKey, LPCTSTR strGroupName, LPCTSTR strSystemCodeYN)
	       :m_spDBaseConnection(a_spDBaseConnection),
            m_nGroupKey(nKey),
	        m_strGroupName(strGroupName),
	        m_strSystemCodeYN(strSystemCodeYN)
{
}


KCodeGroup::KCodeGroup(const KCodeGroup& r)
           :m_spDBaseConnection(r.m_spDBaseConnection),
	        m_nGroupKey(r.m_nGroupKey),
		    m_strGroupName(r.m_strGroupName),
            m_strSystemCodeYN(r.m_strSystemCodeYN)
{
	m_mapCode = r.m_mapCode;
}


KCodeGroup::~KCodeGroup(void)
{	
}

KCodeGroup& KCodeGroup::operator=(const KCodeGroup& r)
{
	m_nGroupKey = r.m_nGroupKey;
	m_strGroupName = r.m_strGroupName;
	m_mapCode = r.m_mapCode;

	return *this;
}

void KCodeGroup::CodeGroupKey(int nKey)
{
	m_nGroupKey = nKey;
}

int KCodeGroup::CodeGroupKey(void)
{
	return m_nGroupKey;
}

void KCodeGroup::CodeGroupName(LPCTSTR strName)
{
	m_strGroupName = strName;
}

LPCTSTR KCodeGroup::CodeGroupName(void)
{
	return m_strGroupName;
}

void KCodeGroup::SystemCodeYN(LPCTSTR strSysCodeYN)
{
	m_strSystemCodeYN = strSysCodeYN;
}

LPCTSTR KCodeGroup::SystemCodeYN(void)
{

	return m_strSystemCodeYN;
}

int KCodeGroup::CodeCount(void)
{
	return (int)(m_mapCode.size());
}

bool KCodeGroup::ReadCodeData()
{
	KDBaseConnectionPtr spDBaseConnection = m_spDBaseConnection;

	if (nullptr == spDBaseConnection)
	{
		return false;
	}

	CString strSQL(_T(""));
	strSQL.Format( 
		_T(" SELECT kcode_key, kcode_display ")
		_T(" FROM %s ")
		_T(" WHERE kcode_group_key = %d "),
		TABLE_CODEDATA, m_nGroupKey
		);
		
	try
	{
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResultSet->Next())
		{
			int     nCode        = spResultSet->GetValueInt   (0);
            CString strCodeValue = spResultSet->GetValueString(1);
			//TCHAR*  szCodeValue  = spResultSet->GetValueString(1);
			//CString strCodeValue = szCodeValue ? szCodeValue : _T("");

			m_mapCode.insert(std::make_pair(nCode, strCodeValue));
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}


bool KCodeGroup::AddCode(int nCode, LPCTSTR strValue)
{
	std::map<int, CString>::iterator itCode = m_mapCode.find(nCode);
	if(m_mapCode.end() != itCode)
	{
		return false;
	}

	KDBaseConnectionPtr spDBaseConnection = m_spDBaseConnection;

	if (nullptr == spDBaseConnection) {
		return false;
	}

	CString strSQL(_T(""));
	strSQL.Format(
		_T(" INSERT INTO %s(kcode_group_key, kcode_key, kcode_display) ")
		_T(" VALUES(%d, %d, '%s') "),
		TABLE_CODEDATA, m_nGroupKey, nCode, strValue
		);

	try
	{
		if (spDBaseConnection->ExecuteUpdate(strSQL) == SQLITE_ERROR)
		{
			throw 1;
		}
	}
	catch(...)
	{
		return false;
	}

	m_mapCode.insert(std::make_pair(nCode, strValue));
	return true;
}


bool KCodeGroup::UpdateCode(int nCode, LPCTSTR strValue)
{
	std::map<int, CString>::iterator itCode = m_mapCode.find(nCode);
	if(m_mapCode.end() == itCode)
	{
		return false;
	}

	KDBaseConnectionPtr spDBaseConnection = m_spDBaseConnection;

	if(nullptr == spDBaseConnection)
	{
		return false;
	}

	CString strSQL(_T(""));
	strSQL.Format(
		_T(" UPDATE %s SET kcode_display = '%s' ")
		_T(" WHERE kcode_group_key = %d ")
		_T(" AND kcode_key = %d "),
		TABLE_CODEDATA, strValue, m_nGroupKey, nCode
		);

	try
	{
		if (spDBaseConnection->ExecuteUpdate(strSQL) == SQLITE_ERROR)
		{
			throw 1;
		}
	}
	catch(...)
	{
		return false;
	}

	itCode->second = strValue;

	return true;
}

void KCodeGroup::RemoveCode(int nCode)
{
	std::map<int, CString>::iterator itCode = m_mapCode.find(nCode);
	if(m_mapCode.end() != itCode)
	{
		m_mapCode.erase(itCode);
	}

	KDBaseConnectionPtr spDBaseConnection = m_spDBaseConnection;

	if (nullptr == spDBaseConnection)
	{
		return;
	}

	CString strSQL(_T(""));
	strSQL.Format(
		_T(" DELETE FROM %s ")
		_T(" WHERE kcode_group_key = %d ")
		_T(" AND kcode_key = %d "),
		TABLE_CODEDATA, m_nGroupKey, nCode
		);

	try
	{
		if (spDBaseConnection->ExecuteUpdate(strSQL) == SQLITE_ERROR)
		{
			throw 1;
		}
	}
	catch(...)
	{
		throw 1;
	}
}

void KCodeGroup::RemoveAllCode(void)
{
	m_mapCode.clear();

	KDBaseConnectionPtr spDBaseConnection = m_spDBaseConnection;
	if (nullptr == spDBaseConnection)
	{
		return;
	}

	CString strSQL(_T(""));
	strSQL.Format(
		_T(" DELETE FROM %s ")
		_T(" WHERE kcode_group_key = %d "), 
		TABLE_CODEDATA, m_nGroupKey
		);

	try
	{
		if (spDBaseConnection->ExecuteUpdate(strSQL) == SQLITE_ERROR)
		{
			throw 1;
		}
	}
	catch(...)
	{
		throw 1;
	}
}


void KCodeGroup::ResetCodeData(void)
{
	m_mapCode.clear();
}

CString KCodeGroup::SingleCodeValue(int nCode, int nType)
{
    CString  strValue(_T(""));

	/*if(m_CodeMap.size()< 1)
	{
		strValue.Format(_T("%d:%s"), nCode, _T("(x)"));
		return strValue;
	}*/

    std::map<int, CString>::iterator itCode = m_mapCode.find(nCode);
    if(m_mapCode.end() != itCode)
    {
        if (nType != 0)
        {
            strValue.Format(_T("%d:%s"), nCode, itCode->second);
        }
        else
        {
            strValue = itCode->second;
        }
        
    }
    else
    {
        strValue.Format(_T("%d:%s"), nCode, _T("(x)"));
    }
    
    return strValue;
}

CString KCodeGroup::MultiCodeValue(LPCTSTR a_strCodes, int nType)
{
    int     nSub = 0;
    CString strElement;
    
    CString strOut;
    bool    bFirst = true;

    while(AfxExtractSubString(strElement, a_strCodes, nSub++, _T(',')))
    {
        if(strElement.IsEmpty() == TRUE)
        {
            continue;
        }
        
        if (bFirst)
        {
            strOut += SingleCodeValue(_ttoi(strElement), nType);
            bFirst  = false;
        }
        else
        {
            strOut = strOut + _T(",") + SingleCodeValue(_ttoi(strElement), nType);
        }
        
    }

    return strOut;
}

void KCodeGroup::GetCodes(std::vector<int>& codes)
{
	std::map<int, CString>::iterator itCode, itEnd = m_mapCode.end();
	for(itCode = m_mapCode.begin(); itCode != itEnd; ++itCode)
	{
		codes.push_back(itCode->first);
	}
}

void KCodeGroup::GetCodes( std::map<int, CString>& a_mapCode )
{
    a_mapCode.clear();

    std::map<int, CString>::iterator itCode, itEnd = m_mapCode.end();
    for(itCode = m_mapCode.begin(); itCode != itEnd; ++itCode)
    {
        a_mapCode.insert(std::make_pair(itCode->first, itCode->second));
    }
}


