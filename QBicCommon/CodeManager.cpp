#include "StdAfx.h"
#include "CodeManager.h"
#include "CodeGroup.h"

const CString KCodeManager::TABLE_CODEGROUP = _T("KCode_Group");
const CString KCodeManager::TABLE_CODES     = _T("KCode_Define");

KCodeManager::KCodeManager(KDBaseConnectionPtr a_spDBaseConnection)
	         :m_spDBaseConnection(a_spDBaseConnection)
{
}


KCodeManager::KCodeManager(const KCodeManager& r)
{    
    m_spDBaseConnection = r.m_spDBaseConnection;

	std::map<int, KCodeGroup*>::const_iterator itCodeData, itEnd = r.m_mapCodeGroup.end();
	for(itCodeData = r.m_mapCodeGroup.begin(); itCodeData != itEnd; ++itCodeData)
	{
		KCodeGroup* pSrc = itCodeData->second;
		KCodeGroup* pDest = new KCodeGroup(*pSrc);
		m_mapCodeGroup.insert(std::make_pair(itCodeData->first, pDest));
	}
}


KCodeManager::~KCodeManager(void)
{
	ResetCodeManager();
}


KCodeManager& KCodeManager::operator=(const KCodeManager& r)
{
	RemoveAllCodeGroup();
	std::map<int, KCodeGroup*>::const_iterator itCodeData, itEnd = r.m_mapCodeGroup.end();
	for(itCodeData = r.m_mapCodeGroup.begin(); itCodeData != itEnd; ++itCodeData)
	{
		KCodeGroup* pSrc  = itCodeData->second;
		KCodeGroup* pDest = new KCodeGroup(*pSrc);
		m_mapCodeGroup.insert(std::make_pair(itCodeData->first, pDest));
	}

	return *this;
}

int KCodeManager::CodeGroupCount(void)
{
	return (int)(m_mapCodeGroup.size());
}

bool KCodeManager::LoadCodeGroups()
{
    ResetCodeManager();
    	
	if (nullptr == m_spDBaseConnection) {
		return false;
	}

	CString strSQL(_T(""));
	strSQL = 
		_T(" SELECT KCode_Group_Key, KCode_Group_Name, System_Code_YN ")
		_T(" FROM KCode_Group ");

	try
	{
		KResultSetPtr spResultSet = m_spDBaseConnection->ExecuteQuery(strSQL);

		// spResultSet이 nullptr인지 확인
		if (spResultSet == nullptr) {
			// 쿼리 실행 실패 처리
			return false;
		}
		while(spResultSet->Next())
		{
			int     nCodeGroupKey    = spResultSet->GetValueInt   (0);
			CString strCodeGroupName = spResultSet->GetValueString(1);
            CString strSystemCodeYN  = spResultSet->GetValueString(2);
			//TCHAR*  szCodeGroupName  = spResultSet->GetValueString(1);
			//CString strCodeGroupName = szCodeGroupName ? szCodeGroupName: _T("");

			//TCHAR*  szSystemCodeYN   = spResultSet->GetValueString(2);
			//CString strSystemCodeYN  = szSystemCodeYN ? szSystemCodeYN: _T("");

			if(strCodeGroupName.IsEmpty() == FALSE) {
				KCodeGroup* pCodeGroupData = new KCodeGroup(m_spDBaseConnection, nCodeGroupKey, strCodeGroupName, strSystemCodeYN);
				if(pCodeGroupData->ReadCodeData() == true) {
					m_mapCodeGroup.insert(std::make_pair(pCodeGroupData->CodeGroupKey(), pCodeGroupData));
				}
			}
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool KCodeManager::AddCodeGroup(int a_nGroupKey, LPCTSTR a_strCodeGroupName)
{
	return InsertCodeGroupData(a_nGroupKey, a_strCodeGroupName);
}


bool KCodeManager::InsertCodeGroupData(int a_nGroupKey, LPCTSTR a_strCodeGroupName)
{
	std::map<int, KCodeGroup*>::iterator itCodeGroup = m_mapCodeGroup.find(a_nGroupKey);
	if(m_mapCodeGroup.end() != itCodeGroup)
	{
		return false;
	}

	KDBaseConnectionPtr spDBaseConnection = m_spDBaseConnection;

	if (nullptr == spDBaseConnection)
	{
		return false;
	}

	CString strSQL(_T(""));
	CString strSystemCodeYN(_T("N"));		// 사용자 입력 디폴트 'N' ( Y: group code가 편집/삭제 불가능한 SystemGroup, N: 편집/삭제 가능한 UserDefine Group )

	strSQL.Format(
		_T(" INSERT INTO %s(kcode_group_key, kcode_group_name, system_code_yn) ")
		_T(" VALUES(%d, '%s', '%s') "),
		TABLE_CODEGROUP, a_nGroupKey, a_strCodeGroupName, strSystemCodeYN
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

	// KCodeGroup 생성
	KCodeGroup* pCodeData = new KCodeGroup(m_spDBaseConnection, a_nGroupKey, a_strCodeGroupName, strSystemCodeYN);
	m_mapCodeGroup.insert(std::make_pair(a_nGroupKey, pCodeData));

	return true;
}


void KCodeManager::RemoveCodeGroupKey(int nGroupKey)
{	
    std::map<int, KCodeGroup*>::iterator itCodeGroup = m_mapCodeGroup.find(nGroupKey);
	if(m_mapCodeGroup.end() != itCodeGroup)
	{
		KCodeGroup* pCodeData = itCodeGroup->second;
		pCodeData->RemoveAllCode();
		delete pCodeData;
		m_mapCodeGroup.erase(itCodeGroup);
	}

	KDBaseConnectionPtr spDBaseConnection = m_spDBaseConnection;
	if(nullptr == spDBaseConnection)
	{
		return;
	}

	CString strSQL(_T(""));
	strSQL.Format(
		_T(" DELETE FROM %s WHERE kcode_group_key = '%d' "),
		TABLE_CODEGROUP, nGroupKey
		);

	try
	{
		if (spDBaseConnection->ExecuteUpdate(strSQL) == SQLITE_ERROR)
		{
			throw 1;
		}
		else
		{
			//삭제된 CodeGroup에 속한 code 정보들도 삭제 한다.
 			CString strSubSQL;
			strSubSQL.Format(
				_T(" DELETE FROM %s ")
				_T(" WHERE kcode_group_key = %d "),
				TABLE_CODES, nGroupKey
				);

			try
			{
				if (spDBaseConnection->ExecuteUpdate(strSubSQL) == SQLITE_ERROR)
				{
					throw 1;
				}
			}
			catch(...)
			{
				throw 1;
			}
		}
	}
	catch(...)
	{
		throw 1;
	}
}


bool KCodeManager::RenameCodeGroup(int a_nGroupKey, LPCTSTR strCodeGroupName, LPCTSTR strNewName)
{
	std::map<int, KCodeGroup*>::iterator itCodeGroup = m_mapCodeGroup.find(a_nGroupKey);
	if(m_mapCodeGroup.end() == itCodeGroup)
	{
		return false;
	}

	KDBaseConnectionPtr spDBaseConnection = m_spDBaseConnection;

	if (nullptr == spDBaseConnection)
	{
		return false;
	}

	CString strSQL(_T(""));
	strSQL.Format(
		_T(" UPDATE %s SET kcode_group_name = '%s' ")
		_T(" WHERE kcode_group_key = '%d' "),
		TABLE_CODEGROUP, strNewName, a_nGroupKey
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

	// KCodeGroup 정보 수정
	KCodeGroup* pCodeData = itCodeGroup->second;
	pCodeData->CodeGroupName(strNewName);
	m_mapCodeGroup.erase(itCodeGroup);
	m_mapCodeGroup.insert(std::make_pair(a_nGroupKey, pCodeData));

	return true;
}


void KCodeManager::RemoveAllCodeGroup(void)
{
	ResetCodeManager();

	KDBaseConnectionPtr spDBaseConnection = m_spDBaseConnection;
	if (nullptr == spDBaseConnection)
	{
		return;
	}

	CString strSQL(_T(""));
	strSQL.Format(
		_T(" DELETE FROM %s "),
		TABLE_CODEGROUP
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


void KCodeManager::ResetCodeManager(void)
{
    try
    {
        std::map<int, KCodeGroup*>::iterator itCodeGroup, itEnd = m_mapCodeGroup.end();
        for(itCodeGroup = m_mapCodeGroup.begin(); itCodeGroup != itEnd; ++itCodeGroup)
        {
            KCodeGroup* pCodeData = itCodeGroup->second;
            delete pCodeData;
        }
        m_mapCodeGroup.clear();
    }
    catch (KExceptionPtr ex)
    {        
    }
    catch (...)
    {
    }
}


KCodeGroup* KCodeManager::FindCodeGroup(int nKey)
{
	std::map<int, KCodeGroup*>::iterator itCodeGroup, itEnd = m_mapCodeGroup.end();
	for(itCodeGroup = m_mapCodeGroup.begin(); itCodeGroup != itEnd; ++itCodeGroup)
	{
		KCodeGroup* pCodeData = itCodeGroup->second;
		if(pCodeData->CodeGroupKey() == nKey)
		{
			return pCodeData;
		}
	}

	return NULL;
}

void KCodeManager::GetCodeGroups(std::vector<KCodeGroup*>& codeGroups)
{
	std::map<int, KCodeGroup*>::iterator itCodeGroup, itEnd = m_mapCodeGroup.end();
	for(itCodeGroup = m_mapCodeGroup.begin(); itCodeGroup != itEnd; ++itCodeGroup)
	{
		codeGroups.push_back(itCodeGroup->second);
	}
}

int KCodeManager::NextGroupKey()
{
    int nNewCodeGroupID(100);
    try
    {
        CString strSQL(_T(""));
        strSQL.Append(_T(" Select ifnull(Max(KCode_Group_Key)+1, 100) From KCode_Group "));

        KDBaseConnectionPtr spDBaseConnection = m_spDBaseConnection;
        KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);
        if (spResultset->Next())
        {
            nNewCodeGroupID = spResultset->GetValueInt(0);
        }
    }
    catch (KExceptionPtr ex)
    {
    }
    catch (...)
    {
    }

    return nNewCodeGroupID;
}
