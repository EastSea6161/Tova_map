#include "StdAfx.h"
#include "DBaseConnector.h"

#include "ImChampDir.h"
#include "Project.h"
#include "ImChampFrameWindow.h"

KDBaseConPtr KDBase::GetApplicationDBConnection()
{
	CString strApplicationDirectory = ImChampDir::GetAppDirectory();
	KDBaseConPtr spDBaseConnection = KDBaseConPtr(new KSQLiteConnection());
	try
	{
        KConnectionString oCon;
        oCon.m_strUrl = strApplicationDirectory + _T("\\TOVA.db");
		if (spDBaseConnection->Connection(oCon))
		{
			return spDBaseConnection;
		}
		else
		{
			spDBaseConnection.reset();
		}
	}
	catch (...)
	{
		spDBaseConnection.reset();
	}

	return spDBaseConnection;
}

KDBaseConPtr KDBase::GetProjectDBConnection()
{	
	KDBaseConPtr spDBaseConnection = KDBaseConPtr(new KSQLiteConnection());
	KProject* pProject = ImChampFrameWindow::GetProject();
	try
	{
        KConnectionString oCon;
        oCon.m_strUrl = pProject->GetLocation() + _T("\\project.db");

		if (spDBaseConnection->Connection(oCon))
		{
			return spDBaseConnection;
		}
		else
		{
			spDBaseConnection.reset();
		}
	}
	catch (...)
	{
		spDBaseConnection.reset();
	}

	return spDBaseConnection;
}

KDBaseConPtr KDBase::GetSubODDBConnection( CString strDBFilePath )
{
	KDBaseConPtr spDBaseConnection = KDBaseConPtr(new KSQLiteConnection());
	
	KProject* pProject = ImChampFrameWindow::GetProject();
	try
	{
        KConnectionString oCon;
        oCon.m_strUrl = strDBFilePath;

		if (spDBaseConnection->Connection(oCon))
		{
			return spDBaseConnection;
		}
		else
		{
			spDBaseConnection.reset();
		}
	}
	catch (...)
	{
		spDBaseConnection.reset();
	}

	return spDBaseConnection;
}
