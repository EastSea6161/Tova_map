/**
 * @file PurposeManager.cpp
 * @brief KPurposeManager 구현 파일
 * @author 
 * @date 2011.05.18
 * @remark
 */


#include "StdAfx.h"
#include "PurposeManager.h"
#include "PAPurpose.h"



KPurposeManager::KPurposeManager()
{

}


KPurposeManager::~KPurposeManager()
{
	Clear();
}

void KPurposeManager::LoadPurpose(KDBaseConPtr spDBConnection)
{
    Clear();

	CString strSQL;
	strSQL = 
		_T(" SELECT object_group_code, detail_object_id, object_name ")
		_T(" FROM detail_object");
    	
	try
	{
        KResultSetPtr spResultSet = spDBConnection->ExecuteQuery(strSQL);
        // spResultSet이 nullptr인지 확인
        if (spResultSet == nullptr) {
            // 쿼리 실행 실패 처리
            return ;
        }

        while(spResultSet->Next())
        {
            int     nObjectGroup = spResultSet->GetValueInt(0);
            int     nObjectID    = spResultSet->GetValueInt(1);
            CString strName      = spResultSet->GetValueString(2);

            KPAPurpose* pPurposeP = new KPAPurpose((KEMPurposeGroup)nObjectGroup, nObjectID, strName, KEMPATypeProduction);
            m_vecPurpose.push_back(pPurposeP);

            KPAPurpose* pPurposeA = new KPAPurpose((KEMPurposeGroup)nObjectGroup, nObjectID, strName, KEMPATypeAttraction);
            m_vecPurpose.push_back(pPurposeA);
        }
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
        throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
        throw -1;
	}
}


void KPurposeManager::Clear(void)
{
    for (size_t i=0; i<m_vecPurpose.size(); i++)
    {
        KPAPurpose* pPurpose = m_vecPurpose[i];
        delete pPurpose;
    }

    m_vecPurpose.clear();
}

int KPurposeManager::GetPurposeCount(void)
{
    int nCount = (int)m_vecPurpose.size();
	return nCount;
}

KPAPurpose* KPurposeManager::GetPurpose(int nIndex)
{
    int nCount = (int)m_vecPurpose.size();
    if (nIndex <0 || nIndex >= nCount)
        return nullptr;

	return m_vecPurpose[nIndex];
}