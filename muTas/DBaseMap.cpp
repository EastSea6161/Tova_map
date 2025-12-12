#include "stdafx.h"
#include "DBaseMap.h"
#include "Target.h"

void KDBaseMap::MapLayerOnOffInfo( KTarget* a_pTarget, std::map<int, KLayerOnOff>& ar_mapLayerOnOff )
{
    try 
    {
        ar_mapLayerOnOff.clear();

    	CString strSQL(_T(" Select Layer_ID, On_Off From MapLayer "));
        KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
        KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);

        while ( spResultSet->Next() )
        {
            KLayerOnOff oLayerOnOff;
            oLayerOnOff.m_nLayerID    = spResultSet->GetValueInt(0);
            oLayerOnOff.m_nLayerOnOff = spResultSet->GetValueInt(1);

            ar_mapLayerOnOff.insert(std::make_pair(oLayerOnOff.m_nLayerID, oLayerOnOff));
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugException();
    }
}

void KDBaseMap::UpdateMapLayerOnOff( KTarget* a_pTarget, int a_nLayerID, int a_nLayerOnOff )
{
    try
    {
        CString strSQL(_T(""));
        strSQL.Format(_T("INSERT OR REPLACE INTO MapLayer(Layer_ID, On_Off) Values('%d','%d')"), a_nLayerID, a_nLayerOnOff);

        KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

        try
        {
            spDBaseConnection->BeginTransaction();
            spDBaseConnection->ExecuteUpdate(strSQL);
            spDBaseConnection->Commit();
        }
        catch (KExceptionPtr ex)
        {
            spDBaseConnection->RollBack();
            TxExceptionPrint(ex);
        }
        catch (...)
        {
            spDBaseConnection->RollBack();
            TxLogDebugWarning();
        }
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }
}
