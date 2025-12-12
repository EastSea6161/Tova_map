/**
 * @file ModeManager.cpp
 * @brief KModeManager 구현 파일
 * @author 
 * @date 2011.06.03
 * @remark
 */


#include "StdAfx.h"
#include "ModeManager.h"
#include "Mode.h"


KModeManager::KModeManager()
{
}


KModeManager::~KModeManager()
{
    Clear();
}

void KModeManager::Clear()
{
    for (size_t i=0; i<m_vecMode.size(); i++)
    {
        KMode* pMode = m_vecMode[i];
        delete pMode;
    }

    m_vecMode.clear();
}

void KModeManager::LoadMode(KDBaseConPtr spDBConnection)
{
	Clear();

	CString strSQL;
	strSQL = 
		_T(" SELECT mode_group_code, detail_mode_id, mode_name ")
		_T(" FROM detail_mode");

	try
	{
        KResultSetPtr spResultSet = spDBConnection->ExecuteQuery(strSQL);
        // spResultSet이 nullptr인지 확인
        if (spResultSet == nullptr) {
            // 쿼리 실행 실패 처리
            return;
        }
        while(spResultSet->Next())
        {
            int  nModeGroup = spResultSet->GetValueInt(0);
            int  nModeID    = spResultSet->GetValueInt(1);
            CString strName = spResultSet->GetValueString(2);

            // todo : Mode Group 수정
            KMode* pMode = new KMode(KEMModeGroupODBase, nModeID, strName);
            m_vecMode.push_back(pMode);
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

int KModeManager::GetModeCount()
{
    int nCount = (int)m_vecMode.size();
	return nCount;
}

KMode* KModeManager::GetMode(int nIndex)
{
    int nCount = (int)m_vecMode.size();
    if (nIndex <0 || nIndex >= nCount)
        return nullptr;

    return m_vecMode[nIndex];
}