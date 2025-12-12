#include "StdAfx.h"
#include "KDBaseZonePASum.h"


KDBaseZonePASum::KDBaseZonePASum(void)
{
}


KDBaseZonePASum::~KDBaseZonePASum(void)
{
    
}

CString KDBaseZonePASum::GetSumValue(KTarget* pTarget, CString strField)
{
    CString strOut = _T("");

	try
	{
		KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();

		CString strSQL  = _T( " Select sum (") + strField + _T(" ) From zone ");    

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		if (spResult->Next())
		{   
			strOut = spResult->GetValueString(0);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
    
    return strOut;
}

CString KDBaseZonePASum::GetSumValue(KTarget* pTarget, KPurposeKey& key)
{
    CString strOut = _T("");

	KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();
    
	CString 
        strSQL  = _T( " Select zone_column_name From generation_model Where ");    

    // TODO : fixed field yn으로 변경 필요.. 현재는 마지막...
    CString strTemp;
    strTemp.Format(_T(" object_group_code = %d And detail_object_id = %d And pa_flag = '%s' order by end_date desc"), key.GroupCode, key.ObjectID, key.PAKey == 1 ?_T("p"): _T("a"));

    strSQL += strTemp;

	KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL); 
    if (spResult->Next())
    {   
        strOut = spResult->GetValueString(0);
    }
    
    if (strOut.GetLength() > 0)
    {
        strOut = GetSumValue(pTarget, strOut);
    }
    
    return strOut;
}

bool KDBaseZonePASum::PASum(KTarget* pTarget, std::map<KPurposeKey, CString>& results)
{
    std::map<KPurposeKey, CString>::iterator iter = results.begin();
    std::map<KPurposeKey, CString>::iterator end  = results.end();

    while (iter != end)
    {
        KPurposeKey  key    = iter->first;        
        CString&     result = iter->second;

        result = GetSumValue(pTarget, key);
        ++iter;
    }
    
    return true;
}