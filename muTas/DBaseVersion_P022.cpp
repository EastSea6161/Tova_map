#include "StdAfx.h"
#include "DBaseVersion_P022.h"


KDBaseVersion_P022::KDBaseVersion_P022(void)
{
}


KDBaseVersion_P022::~KDBaseVersion_P022(void)
{
}

void KDBaseVersion_P022::IolocalDBUpgrade( KDBaseConPtr spDBaseConnection )
{
	try
	{
		TableInterLinkOption(spDBaseConnection);
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KDBaseVersion_P022::TableInterLinkOption( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T(""));
		strSQLDrop.Format(_T("DROP TABLE inter_link_option "));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
		TxLogDebugException();
	}


	try
	{
		CString strCreateSQL(_T(""));
		strCreateSQL.AppendFormat(_T(" CREATE TABLE [inter_link_option] ( "));
		strCreateSQL.AppendFormat(_T(" [Type] Integer PRIMARY KEY, "));
		strCreateSQL.AppendFormat(_T(" [Travel_Time] VARCHAR,  "));
		strCreateSQL.AppendFormat(_T(" [VDFChoice] Integer, "));
		strCreateSQL.AppendFormat(_T(" [Alpha] double, "));
		strCreateSQL.AppendFormat(_T(" [Beta]  double, "));
		strCreateSQL.AppendFormat(_T(" [VDFFunction] Integer, "));
		strCreateSQL.AppendFormat(_T(" [LinkFn_Sql]   VARCHAR ) "));
		spDBaseConnection->ExecuteUpdate(strCreateSQL);

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw -1;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}
