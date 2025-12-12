#include "stdafx.h"
#include "BulkDBaseTargetRegion.h"
#include "Target.h"
#include "TCoordinate.h"
//^#include "DBaseConnector.h"

void KBulkDBaseTargetRegion::InsertTargetRegion( KTarget* a_pTarget, int a_nTargetID, std::vector<TCoordinate>& ar_vecCoordinate )
{
    try
    {
    	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

        CString strDelete(_T(""));
        strDelete.Format(_T(" Delete From Target_Region_Vertex Where ID = '%d'"), a_nTargetID);

        spDBaseConnection->ExecuteUpdate(strDelete);

        for (size_t i=0; i<ar_vecCoordinate.size(); i++)
        {
            TCoordinate& oCoordinate = ar_vecCoordinate[i];
            CString strInsert(_T(""));
            strInsert.Format(_T(" Insert Into Target_Region_Vertex(ID, Seq, X, Y) Values ('%d', '%d', '%f', '%f') "), a_nTargetID, i, oCoordinate.Tx, oCoordinate.Ty);
            spDBaseConnection->ExecuteUpdate(strInsert);
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

void KBulkDBaseTargetRegion::GetTargetRegionInfo( KTarget* a_pTarget, int a_nTargetID, std::vector<TCoordinate>& ar_vecCoordinate )
{
    try
    {
        ar_vecCoordinate.clear();

        KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

        CString strSQL(_T(""));
        strSQL.Format(_T(" Select ID, Seq, X, Y From Target_Region_Vertex Where ID = '%d' Order By Seq "), a_nTargetID);

        KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
        while(spResultSet->Next())
        {
            double X = spResultSet->GetValueDouble(2);
            double Y = spResultSet->GetValueDouble(3);

            TCoordinate oCoordinate(X, Y);
            ar_vecCoordinate.push_back(oCoordinate);
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

