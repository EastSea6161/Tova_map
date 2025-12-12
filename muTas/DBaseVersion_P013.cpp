#include "StdAfx.h"
#include "DBaseVersion_P013.h"


KDBaseVersion_P013::KDBaseVersion_P013(void)
{
}


KDBaseVersion_P013::~KDBaseVersion_P013(void)
{
}

void KDBaseVersion_P013::IolocalDBUpgrade( KDBaseConPtr spDBaseConnection )
{
	TableMapView(spDBaseConnection);
}

void KDBaseVersion_P013::TableMapView( KDBaseConPtr spDBaseConnetion )
{
	try
	{
		CString strSQLDrop (_T("DROP TABLE "));
		spDBaseConnetion->ExecuteUpdate(strSQLDrop);
	}
	catch (...)
	{
	}

	CString strSQLCreate(_T(""));
	strSQLCreate.Append(_T( " CREATE TABLE [MapLayer] (  "));
	strSQLCreate.Append(_T( "   [Layer_ID] INTEGER PRIMARY KEY,  "));
	strSQLCreate.Append(_T( "   [On_Off] INT ) "));
	spDBaseConnetion->ExecuteUpdate(strSQLCreate);

	CString strSQLInsert(_T(""));
	strSQLInsert.Format(_T("Insert Into MapLayer Values (0,1)"));
	spDBaseConnetion->ExecuteUpdate(strSQLInsert);

	strSQLInsert.Format(_T("Insert Into MapLayer Values (1,1)"));
	spDBaseConnetion->ExecuteUpdate(strSQLInsert);

	strSQLInsert.Format(_T("Insert Into MapLayer Values (2,0)"));
	spDBaseConnetion->ExecuteUpdate(strSQLInsert);
}
