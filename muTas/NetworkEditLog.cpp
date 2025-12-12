#include "StdAfx.h"
#include "NetworkEditLog.h"
#include "Target.h"

/*
      a_nEditType =>	 0 : Delete   1 : Add   2 : Update
						 0      .           2          ?
						 1      -           ?          1
						 2      0           ?          2

*/

//Layer -               -1 :  Import    | 0 : Node   | 1 : Link | 2 : UrbanTransit | 3 : RegionalTransit   
//CheckData              0 :  NotCheck  | 1 : Check


KNetworkEditLog::KNetworkEditLog(void)
{
}


KNetworkEditLog::~KNetworkEditLog(void)
{
}

void KNetworkEditLog::WriteNetworkEditLog( KTarget* a_pTarget, int a_nLayer, Integer a_nxID, int a_nEditType )
{
	try
	{
        KDBaseConPtr spDBase = a_pTarget->GetDBaseConnection();

		CString strSQL(_T("")); {
            strSQL.Append(_T(" Insert Or Replace Into "));  
            strSQL.AppendFormat(_T("  %s(Layer, Key_Value, Edit_Type, Check_Data) "), TABLE_NETWORKEDIT_LOG);   
            strSQL.AppendFormat(_T(" Values ('%d', '%I64d', '%d', '%d') "), a_nLayer, a_nxID, a_nEditType, 0);  
        }
		
		spDBase->ExecuteUpdate(strSQL);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KNetworkEditLog::NetworkEditLog( KTarget* a_pTarget, std::vector<TNetworkEditLog>& a_vecNetworkEditLog )
{
	try
	{
		a_vecNetworkEditLog.clear();

		CString strSQL(_T(""));
		strSQL.Append(_T(" Select "));
		strSQL.Append(_T("  Layer, Key_Value, Edit_Type, Check_Data "));
		strSQL.Append(_T(" From "));
		strSQL.Append(_T("  NetworkEditLog "));  

		KDBaseConPtr spDBaseConnection(a_pTarget->GetDBaseConnection());
		KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);

		while (spResultSet->Next())
		{
			TNetworkEditLog oNetworkEditLog;
			oNetworkEditLog.TLayer    = spResultSet->GetValueInt(0);
			oNetworkEditLog.TID       = spResultSet->GetValueInt64(1);
			oNetworkEditLog.TEditType = spResultSet->GetValueInt(2);
			oNetworkEditLog.TCheckData= spResultSet->GetValueInt(3);
			a_vecNetworkEditLog.push_back(oNetworkEditLog);
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

void KNetworkEditLog::NetworkEditLog( KTarget* a_pTarget, int a_nLayer, std::vector<TNetworkEditLog>& a_vecNetworkEditLog )
{
	try
	{
		a_vecNetworkEditLog.clear();

		CString strSQL(_T(""));
		strSQL.Append(_T(" Select "));
		strSQL.Append(_T("  Layer, Key_Value, Edit_Type, Check_Data "));
		strSQL.Append(_T(" From "));
		strSQL.Append(_T("  NetworkEditLog "));  
		strSQL.Append(_T(" Where "));
		strSQL.AppendFormat(_T(" Layer = '%d' "), a_nLayer); 

		KDBaseConPtr spDBaseConnection(a_pTarget->GetDBaseConnection());
		KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);

		while (spResultSet->Next())
		{
			TNetworkEditLog oNetworkEditLog;
			oNetworkEditLog.TLayer    = spResultSet->GetValueInt(0);
			oNetworkEditLog.TID       = spResultSet->GetValueInt64(1);
			oNetworkEditLog.TEditType = spResultSet->GetValueInt(2);
			oNetworkEditLog.TCheckData= spResultSet->GetValueInt(3);
			a_vecNetworkEditLog.push_back(oNetworkEditLog);
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

void KNetworkEditLog::GetLastNetWorkLog( KTarget* a_pTarget, int& a_nCheckData )
{
	CString strSQL(_T(""));
	strSQL.Format(_T("Select Check_Data From %s Order By edit_time DESC LIMIT 1"), TABLE_NETWORKEDIT_LOG);

	try
	{
		KDBaseConPtr spDBaseConnection(a_pTarget->GetDBaseConnection());
		KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		if (spResultSet->Next())
		{
			a_nCheckData = spResultSet->GetValueInt(0);
		}
		else
		{
			a_nCheckData = 1;
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
}

void KNetworkEditLog::SetImportNetworkLog( KTarget* a_pTarget )
{

	CString strSQlDelete(_T(""));
	strSQlDelete.Format(_T("Delete From %s "), TABLE_NETWORKEDIT_LOG);

	CString strSQLInsert(_T(""));
	strSQLInsert.Append(_T(" Insert Into "));
	strSQLInsert.AppendFormat(_T(" %s(Layer, Key_Value, Edit_Type, Check_Data) "), TABLE_NETWORKEDIT_LOG );
	strSQLInsert.Append(_T(" Values ('-1', '0', '1', '0')"));

	KDBaseConPtr spDBaseConnection(a_pTarget->GetDBaseConnection());
	spDBaseConnection->BeginTransaction();

	try
	{	
		spDBaseConnection->ExecuteUpdate(strSQlDelete);
		spDBaseConnection->ExecuteUpdate(strSQLInsert);
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex) 
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
	}
}

void KNetworkEditLog::SetNetworkLogUpdateInspection( KTarget* a_pTarget )
{
	CString strSQL(_T(""));
	try
	{
		strSQL.Format(_T("Update %s Set Check_Data = '1'"), TABLE_NETWORKEDIT_LOG);

		KDBaseConPtr spDBaseConeection(a_pTarget->GetDBaseConnection());
		spDBaseConeection->ExecuteUpdate(strSQL);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

CTime KNetworkEditLog::GetLastEditTime(KTarget* pTarget)
{
    CTime oTime(1972, 9, 29, 22, 30, 10);

    CString strSQL(_T("")); {
        strSQL.Append(_T(" Select strftime('%Y%m%d%H%M%S', Max(Edit_Time)) as MaxTime From NetworkEditLog "));
    }

    KDBaseConPtr  spDBase  = pTarget->GetDBaseConnection();
    KResultSetPtr spResult = spDBase->ExecuteQuery(strSQL);
    if (spResult->Next()) {
        CString strTime = spResult->GetValueStr(0);
		if (strTime.GetLength() == 14)
		{
			int nYear(0); {
				nYear = _ttoi(strTime.Mid(0, 4));
			}

			int nMonth(0); {
				nMonth = _ttoi(strTime.Mid(4, 2));
			}
				
			int nDay(0); {
				nDay = _ttoi(strTime.Mid(6, 2));
			}

			int nHour(0); {
				nHour = _ttoi(strTime.Mid(8, 2));
			}

			int nMin(0); {
				nMin = _ttoi(strTime.Mid(10, 2));
			}

			int nSec(0); {
				nSec = _ttoi(strTime.Mid(12, 2));
			}

			oTime = CTime(nYear, nMonth, nDay, nHour, nMin, nSec);

		}
    }
    
    return oTime;
}
