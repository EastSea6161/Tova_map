#include "StdAfx.h"
#include "KDBaseStatistics.h"


KDBaseStatistics::KDBaseStatistics(void)
{
}


KDBaseStatistics::~KDBaseStatistics(void)
{
}


bool KDBaseStatistics::StatisticsLinkInfluence(KDBaseConPtr a_spDBaseConnection, std::vector<TLinkInfluence>& results)
{
    results.clear();
    
    CString strSQL  = _T( " select link_type, count(*) count, sum(length) length from link where influence = 1 group by link_type order by link_type ");    
    
    try
    {
        KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);
        while(spResultSet->Next())
        {
            TLinkInfluence record;
            record.link_type = spResultSet->GetValueInt(0);        
            record.count     = spResultSet->GetValueInt(1); 
            record.length    = spResultSet->GetValueDouble(2);

            results.push_back(record);
        }
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
        return false;
    }
    catch (...)
    {
    	TxLogDebugException();
        return false;
    }
    
    return true;
}