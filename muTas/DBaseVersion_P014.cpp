#include "StdAfx.h"
#include "DBaseVersion_P014.h"
#include "DefineNamesDefaultPara.h"

KDBaseVersion_P014::KDBaseVersion_P014(void)
{
}


KDBaseVersion_P014::~KDBaseVersion_P014(void)
{
}

void KDBaseVersion_P014::IolocalDBUpgrade( KDBaseConPtr spDBaseConnection )
{
	try
	{
		TableInterTerminalschedule(spDBaseConnection);
		TableLayerRender(spDBaseConnection);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KDBaseVersion_P014::TableInterTerminalschedule( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strAddColumn(_T("Alter Table terminal_schedule Add Column 'headway' DOUBLE DEFAULT 0;"));
		spDBaseConnection->ExecuteUpdate(strAddColumn);

		CString strAddColumnInfo(_T(""));
		strAddColumnInfo.Append(_T("Insert Into Column_Description ( "));
		strAddColumnInfo.Append(_T(" Table_Name , Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, "));
		strAddColumnInfo.Append(_T(" Display_Name, Column_Info, Display_Order, PK_YN, Use_Flag ) "));
		strAddColumnInfo.Append(_T(" Values ( "));
		strAddColumnInfo.Append(_T(" 'terminal_schedule', 'headway',  '2', '-1', '-1', 'Y', 'Y', "));
		strAddColumnInfo.Append(_T(" 'HeadWay', 'HeadWay', '9', 'N', '1' ) "));
		spDBaseConnection->ExecuteUpdate(strAddColumnInfo);

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KDBaseVersion_P014::ProjectDBUpgrade( KDBaseConPtr spDBaseConnection )
{
	try
	{
		TableRegionalModePara(spDBaseConnection);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KDBaseVersion_P014::TableRegionalModePara( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T(""));
		strSQLDrop.Format(_T("DROP TABLE %s "), KDefaultParaTable::TABLE_DEFAULT_REGIONAL_MODECHOICE_PARA);
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{

	}

	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [Default_Regional_ModeChoice_Para] (  "));
		strSQLCreate.Append(_T( "   [Master_Code] INTEGER NOT NULL,  "));
		strSQLCreate.Append(_T( "   [Object_ID] INTEGER NOT NULL,  "));
		strSQLCreate.Append(_T( "   [Alternative_ID] INTEGER NOT NULL,  "));
		strSQLCreate.Append(_T( "   [InVehecle_Time] DOUBLE,  "));
		strSQLCreate.Append(_T( "   [Waiting_Time] DOUBLE,  "));
		strSQLCreate.Append(_T( "   [Transfer_Time] DOUBLE,  "));
		strSQLCreate.Append(_T( "   [Process_Time] DOUBLE,  "));
		strSQLCreate.Append(_T( "   [Frequency] DOUBLE,  "));
		strSQLCreate.Append(_T( "   [Cost] DOUBLE,  "));
		strSQLCreate.Append(_T( "   [Dummy] DOUBLE,  "));
		strSQLCreate.Append(_T( "   CONSTRAINT [] PRIMARY KEY ([Master_Code], [Object_ID], [Alternative_ID]) ) "));

		spDBaseConnection->ExecuteUpdate(strSQLCreate);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KDBaseVersion_P014::TableLayerRender( KDBaseConPtr spDBaseConnection )
{
	CString strSQLDrop(_T(""));
	try
	{
		strSQLDrop.Format(_T("DROP TABLE LayerRender_Link "));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
	}

	try
	{
		strSQLDrop.Format(_T("DROP TABLE LayerRender_Node "));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
	}

	try
	{
		strSQLDrop.Format(_T("DROP TABLE LayerRender_Zone "));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
	}

	CString strSQLLink(_T(""));
	CString strSQLNode(_T(""));
	CString strSQLZone(_T(""));

	try
	{
		strSQLLink.Append(_T( " CREATE TABLE [LayerRender_Link] (  "));
		strSQLLink.Append(_T( "   [ColorR] INTEGER NOT NULL,  "));
		strSQLLink.Append(_T( "   [ColorG] INTEGER NOT NULL,  "));
		strSQLLink.Append(_T( "   [ColorB] INTEGER NOT NULL,  "));
		strSQLLink.Append(_T( "   [LineWidth] DOUBLE,  "));
		strSQLLink.Append(_T( "   [MinScale] DOUBLE,  "));
		strSQLLink.Append(_T( "   [MaxScale] DOUBLE,  "));
		strSQLLink.Append(_T( "   [OffsetScale] DOUBLE,  "));
		strSQLLink.Append(_T( "   [ArrowScale] DOUBLE,  "));
		strSQLLink.Append(_T( "   [LabelShow] DOUBLE,  "));
		strSQLLink.Append(_T( "   [LabelField] DOUBLE,  "));
		strSQLLink.Append(_T( "   [LabelScaleOn] DOUBLE,  "));
		strSQLLink.Append(_T( "   [LabelOnScale] DOUBLE,  "));
		strSQLLink.Append(_T( "   [UseType] DOUBLE,  "));
		strSQLLink.Append(_T( "   [Layer_On_Off] DOUBLE )  "));
		spDBaseConnection->ExecuteUpdate(strSQLLink);

		strSQLNode.Append(_T( " CREATE TABLE [LayerRender_Node] (  "));
		strSQLNode.Append(_T( "   [MarkerIndex] INTEGER NOT NULL,  "));
		strSQLNode.Append(_T( "   [ColorR] INTEGER NOT NULL,  "));
		strSQLNode.Append(_T( "   [ColorG] INTEGER NOT NULL,  "));
		strSQLNode.Append(_T( "   [ColorB] DOUBLE,  "));
		strSQLNode.Append(_T( "   [MarkerSize] DOUBLE,  "));
		strSQLNode.Append(_T( "   [MinScale] DOUBLE,  "));
		strSQLNode.Append(_T( "   [MaxScale] DOUBLE,  "));
		strSQLNode.Append(_T( "   [LabelShow] DOUBLE,  "));
		strSQLNode.Append(_T( "   [LabelField] DOUBLE,  "));
		strSQLNode.Append(_T( "   [LabelScaleOn] DOUBLE,  "));
		strSQLNode.Append(_T( "   [LabelOnScale] DOUBLE,  "));
		strSQLNode.Append(_T( "   [UseType] DOUBLE,  "));
		strSQLNode.Append(_T( "   [Layer_On_Off] DOUBLE )  "));
		spDBaseConnection->ExecuteUpdate(strSQLNode);

		strSQLZone.Append(_T( " CREATE TABLE [LayerRender_Zone] (  "));
		strSQLZone.Append(_T( "   [FillColorR] INTEGER NOT NULL,  "));
		strSQLZone.Append(_T( "   [FillColorG] INTEGER NOT NULL,  "));
		strSQLZone.Append(_T( "   [FillColorB] INTEGER NOT NULL,  "));
		strSQLZone.Append(_T( "   [LineColorR] DOUBLE,  "));
		strSQLZone.Append(_T( "   [LineColorG] DOUBLE,  "));
		strSQLZone.Append(_T( "   [LineColorB] DOUBLE,  "));
		strSQLZone.Append(_T( "   [LineWidth] DOUBLE,  "));
		strSQLZone.Append(_T( "   [MinScale] DOUBLE,  "));
		strSQLZone.Append(_T( "   [MaxScale] DOUBLE,  "));
		strSQLZone.Append(_T( "   [Layer_On_Off] DOUBLE )  "));
		spDBaseConnection->ExecuteUpdate(strSQLZone);

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}
