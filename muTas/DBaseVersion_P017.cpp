#include "StdAfx.h"
#include "DBaseVersion_P017.h"


KDBaseVersion_P017::KDBaseVersion_P017(void)
{
}


KDBaseVersion_P017::~KDBaseVersion_P017(void)
{
}

void KDBaseVersion_P017::IolocalDBUpgrade( KDBaseConPtr spDBaseConnection )
{
	try
	{
		TableTerminalResult(spDBaseConnection);
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

void KDBaseVersion_P017::TableTerminalResult( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T(""));
		strSQLDrop.Format(_T("DROP TABLE %s "), TABLE_TERMINAL_RESULT);
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{

	}

	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [terminal_result] (  "));
		strSQLCreate.Append(_T( "   [terminal_id] Integer PRIMARY KEY  NOT NULL ,  "));
		strSQLCreate.Append(_T( "   [name] VARCHAR2 )  "));

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

void KDBaseVersion_P017::ProjectDBUpgrade( KDBaseConPtr spDBaseConnection )
{
	try
	{
		TableFunctionGroup(spDBaseConnection);

		TableLinkTemplate(spDBaseConnection);
		TableLinkTemplateType(spDBaseConnection);
		TableLinkTemplateValue(spDBaseConnection);
		TableLinkTypeDefineParent(spDBaseConnection);
		TableLinkTypeDefineChild(spDBaseConnection);
		TableNavigation(spDBaseConnection);
		TableNetworkEditTemplate(spDBaseConnection);
		TableNodeTemplate(spDBaseConnection);
		TableNodeTemplateType(spDBaseConnection);
		TableNodeTemplateValue(spDBaseConnection);
		TableNodeTypeDefineParent(spDBaseConnection);
		TableNodeTypeDefineChild(spDBaseConnection);
		TableTypeDefineDefault(spDBaseConnection);
		TableZoneTemplate(spDBaseConnection);
		TableZoneTemplateType(spDBaseConnection);
		TableZoneTemplateValue(spDBaseConnection);

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

void KDBaseVersion_P017::TableFunctionGroup( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T("DROP TABLE Function_Group"));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
		TxLogDebugException();
	}

	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [Function_Group] (  "));
		strSQLCreate.Append(_T( "   [Function_Group_ID] Integer PRIMARY KEY  NOT NULL ,  "));
		strSQLCreate.Append(_T( "   [Function_Group_Name] VARCHAR2,  "));
		strSQLCreate.Append(_T( "   [use_alpha_flag] INTEGER,  "));
		strSQLCreate.Append(_T( "   [alpha_column_name] VARCHAR2,  "));
		strSQLCreate.Append(_T( "   [use_beta_flag] INTEGER,  "));
		strSQLCreate.Append(_T( "   [beta_column_name] VARCHAR2,  "));
		strSQLCreate.Append(_T( "   [use_a_flag] INTEGER,  "));
		strSQLCreate.Append(_T( "   [a_column_name] VARCHAR2,  "));
		strSQLCreate.Append(_T( "   [use_b_flag] INTEGER,  "));
		strSQLCreate.Append(_T( "   [b_column_name] VARCHAR2 )  "));
		
		spDBaseConnection->ExecuteUpdate(strSQLCreate);

		//Function_Group »èÁ¦ ½Ã, Function Å×ÀÌºí µ¥ÀÌÅÍµµ ÇÔ²² »èÁ¦ÇÔ
		//Fucntion_GroupÀÇ °íÀ¯Å°¸¦ È®ÀÎÇÏ°í µ¥ÀÌÅÍ¸¦ Ãß°¡ÇÏ±â¿¡, function¿¡ µ¥ÀÌÅÍ°¡ ÀÖÀ» ½Ã Insert ¿À·ù°¡ ¹ß»ýÇÔ

		strSQLCreate = _T("DELETE FROM FUNCTION");

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

void KDBaseVersion_P017::TableLinkTemplate( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T("DROP TABLE LinkTemplate"));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
		TxLogDebugException();
	}

	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [LinkTemplate] (  "));
		strSQLCreate.Append(_T( "   [ID] Integer NOT NULL ,  "));
		strSQLCreate.Append(_T( "   [Type] Integer NOT NULL ,  "));
		strSQLCreate.Append(_T( "   [Name] VARCHAR2(255) NOT NULL,  "));
		strSQLCreate.Append(_T( "    CONSTRAINT [] PRIMARY KEY ([ID], [Type]))  "));

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

void KDBaseVersion_P017::TableLinkTemplateType( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T("DROP TABLE LinkTemplateType"));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
		TxLogDebugException();
	}

	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [LinkTemplateType] (  "));
		strSQLCreate.Append(_T( "   [ID] Integer NOT NULL ,  "));
		strSQLCreate.Append(_T( "   [Seq] Integer NOT NULL ,  "));
		strSQLCreate.Append(_T( "   [LineType] Integer ,  "));
		strSQLCreate.Append(_T( "   [Width] DOUBLE ,  "));
		strSQLCreate.Append(_T( "   [RGB] VARCHAR2(255) NOT NULL,  "));
		strSQLCreate.Append(_T( "    CONSTRAINT [] PRIMARY KEY ([ID], [Seq]))  "));

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

void KDBaseVersion_P017::TableLinkTemplateValue( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T("DROP TABLE LinkTemplateValue"));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
		TxLogDebugException();
	}

	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [LinkTemplateValue] (  "));
		strSQLCreate.Append(_T( "   [ID] Integer NOT NULL ,  "));
		strSQLCreate.Append(_T( "   [Seq] Integer NOT NULL ,  "));
		strSQLCreate.Append(_T( "   [Min] VARCHAR2 ,  "));
		strSQLCreate.Append(_T( "   [Max] VARCHAR2 ,  "));
		strSQLCreate.Append(_T( "   [LineType] Integer ,  "));
		strSQLCreate.Append(_T( "   [Width] DOUBLE ,  "));
		strSQLCreate.Append(_T( "   [RGB] VARCHAR2(255) NOT NULL,  "));
		strSQLCreate.Append(_T( "    CONSTRAINT [sqlite_autoindex_LinkTemplateValue_1] PRIMARY KEY ([ID], [Seq]))  "));

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

void KDBaseVersion_P017::TableLinkTypeDefineChild( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T("DROP TABLE LinkTypeDefineChild"));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
		TxLogDebugException();
	}

	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [LinkTypeDefineChild] (  "));
		strSQLCreate.Append(_T( "   [ID] Integer NOT NULL ,  "));
		strSQLCreate.Append(_T( "   [Link_Type] Integer ,  "));
		strSQLCreate.Append(_T( "   [Symbol] Integer ,  "));
		strSQLCreate.Append(_T( "   [Color] VARCHAR2 ,  "));
		strSQLCreate.Append(_T( "   [Size] DOUBLE ,  "));
		strSQLCreate.Append(_T( "   [Draw] INTEGER DEFAULT 1,   "));
		strSQLCreate.Append(_T( "    CONSTRAINT [sqlite_autoindex_LinkTypeDefineChild_1] PRIMARY KEY ([ID], [Link_Type]))  "));

		spDBaseConnection->ExecuteUpdate(strSQLCreate);

		CString strSQLDefaultData(_T(""));
		strSQLDefaultData = _T(" insert into LinkTypeDefineChild values( 1, 0, 0, '194,179,131', 1.3, 1) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		strSQLDefaultData = _T(" insert into LinkTypeDefineChild values( 1, 1, 0, '48,115,206', 2.0, 1) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		strSQLDefaultData = _T(" insert into LinkTypeDefineChild values( 1, 2, 0, '35,104,219', 1.5, 1) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		strSQLDefaultData = _T(" insert into LinkTypeDefineChild values( 1, 3, 0, '199,181,132', 1.0, 1) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		strSQLDefaultData = _T(" insert into LinkTypeDefineChild values( 1, 4, 0, '199,181,132', 1.0, 1) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		strSQLDefaultData = _T(" insert into LinkTypeDefineChild values( 1, 5, 0, '217,217,217', 1.0, 1) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		strSQLDefaultData = _T(" insert into LinkTypeDefineChild values( 1, 6, 0, '199,181,132', 1.0, 1) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		strSQLDefaultData = _T(" insert into LinkTypeDefineChild values( 1, 7, 0, '215,198,170', 1.0, 1) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		strSQLDefaultData = _T(" insert into LinkTypeDefineChild values( 1, 8, 0, '217,217,217', 1.0, 1) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		strSQLDefaultData = _T(" insert into LinkTypeDefineChild values( 1, 10, 0, '216,133,186', 1.2, 1) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		strSQLDefaultData = _T(" insert into LinkTypeDefineChild values( 1, 11, 0, '216,133,186', 1.2, 1) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		strSQLDefaultData = _T(" insert into LinkTypeDefineChild values( 1, 20, 0, '228,187,147', 1.3, 1) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		strSQLDefaultData = _T(" insert into LinkTypeDefineChild values( 1, 21, 0, '213,186,134', 1.3, 1) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
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

void KDBaseVersion_P017::TableLinkTypeDefineParent( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T("DROP TABLE LinkTypeDefineParent"));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
		TxLogDebugException();
	}

	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [LinkTypeDefineParent] (  "));
		strSQLCreate.Append(_T( "   [ID] Integer  PRIMARY KEY ,  "));
		strSQLCreate.Append(_T( "   [Name] VARCHAR2 ,  "));
		strSQLCreate.Append(_T( "   [Flag] INTEGER DEFAULT 0 )   "));

		spDBaseConnection->ExecuteUpdate(strSQLCreate);

		CString strSQLDefaultData(_T(""));
		/*strSQLDefaultData = _T(" insert into LinkTypeDefineParent values( 1, 'Color', 1) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		strSQLDefaultData = _T(" insert into LinkTypeDefineParent values( 2, 'Gray', 0) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		strSQLDefaultData = _T(" insert into LinkTypeDefineParent values( 3, 'User(1)', 0) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		strSQLDefaultData = _T(" insert into LinkTypeDefineParent values( 4, 'User(2)', 0) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		strSQLDefaultData = _T(" insert into LinkTypeDefineParent values( 5, 'User(3)', 0) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		strSQLDefaultData = _T(" insert into LinkTypeDefineParent values( 6, 'User(4)', 0) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		strSQLDefaultData = _T(" insert into LinkTypeDefineParent values( 7, 'User(5)', 0) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);*/
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strSQLDefaultData = _T(" insert into LinkTypeDefineParent values( 1, 'ÅÆÇÃ·¿(1)', 1) ");
			spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
			strSQLDefaultData = _T(" insert into LinkTypeDefineParent values( 2, 'ÅÆÇÃ·¿(2)', 0) ");
			spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
			strSQLDefaultData = _T(" insert into LinkTypeDefineParent values( 3, 'ÅÆÇÃ·¿(3)', 0) ");
			spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
			strSQLDefaultData = _T(" insert into LinkTypeDefineParent values( 4, 'ÅÆÇÃ·¿(4)', 0) ");
			spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
			strSQLDefaultData = _T(" insert into LinkTypeDefineParent values( 5, 'ÅÆÇÃ·¿(5)', 0) ");
			spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
			strSQLDefaultData = _T(" insert into LinkTypeDefineParent values( 6, 'ÅÆÇÃ·¿(6)', 0) ");
			spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
			strSQLDefaultData = _T(" insert into LinkTypeDefineParent values( 7, 'ÅÆÇÃ·¿(7)', 0) ");
			spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		}
		else
		{
			strSQLDefaultData = _T(" insert into LinkTypeDefineParent values( 1, 'Template(1)', 1) ");
			spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
			strSQLDefaultData = _T(" insert into LinkTypeDefineParent values( 2, 'Template(2)', 0) ");
			spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
			strSQLDefaultData = _T(" insert into LinkTypeDefineParent values( 3, 'Template(3)', 0) ");
			spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
			strSQLDefaultData = _T(" insert into LinkTypeDefineParent values( 4, 'Template(4)', 0) ");
			spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
			strSQLDefaultData = _T(" insert into LinkTypeDefineParent values( 5, 'Template(5)', 0) ");
			spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
			strSQLDefaultData = _T(" insert into LinkTypeDefineParent values( 6, 'Template(6)', 0) ");
			spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
			strSQLDefaultData = _T(" insert into LinkTypeDefineParent values( 7, 'Template(7)', 0) ");
			spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		}
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

void KDBaseVersion_P017::TableNavigation( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T("DROP TABLE Navigation"));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
		TxLogDebugException();
	}

	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [Navigation] (  "));
		strSQLCreate.Append(_T( "   [Navi_ID] Integer PRIMARY KEY,  "));
		strSQLCreate.Append(_T( "   [Navi_Name] VARCHAR2 ,  "));
		strSQLCreate.Append(_T( "   [Min_X] REAL ,  "));
		strSQLCreate.Append(_T( "   [Min_Y] REAL ,  "));
		strSQLCreate.Append(_T( "   [Max_X] REAL ,  "));
		strSQLCreate.Append(_T( "   [Max_Y] REAL )  "));

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

void KDBaseVersion_P017::TableNetworkEditTemplate( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T("DROP TABLE NetworkEdit_Template"));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
		TxLogDebugException();
	}

	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [NetworkEdit_Template] (  "));
		strSQLCreate.Append(_T( "   [ID] Integer ,  "));
		strSQLCreate.Append(_T( "   [network_type] Integer ,  "));
		strSQLCreate.Append(_T( "   [template_name] VARCHAR2(100),   "));
		strSQLCreate.Append(_T( "   [key_value]  VARCHAR2(1000),  "));
		strSQLCreate.Append(_T( "    CONSTRAINT [] PRIMARY KEY ([ID], [network_type]))  "));

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

void KDBaseVersion_P017::TableNodeTemplate( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T("DROP TABLE NodeTemplate"));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
		TxLogDebugException();
	}

	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [NodeTemplate] (  "));
		strSQLCreate.Append(_T( "   [ID] Integer NOT NULL ,  "));
		strSQLCreate.Append(_T( "   [Type] Integer NOT NULL ,  "));
		strSQLCreate.Append(_T( "   [Name] VARCHAR2(255) NOT NULL,  "));
		strSQLCreate.Append(_T( "    CONSTRAINT [] PRIMARY KEY ([ID], [Type]))  "));

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

void KDBaseVersion_P017::TableNodeTemplateType( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T("DROP TABLE NodeTemplateType"));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
		TxLogDebugException();
	}

	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [NodeTemplateType] (  "));
		strSQLCreate.Append(_T( "   [ID] Integer NOT NULL ,  "));
		strSQLCreate.Append(_T( "   [Seq] Integer NOT NULL ,  "));
		strSQLCreate.Append(_T( "   [Symbol] Integer ,  "));
		strSQLCreate.Append(_T( "   [Size] DOUBLE ,  "));
		strSQLCreate.Append(_T( "   [RGB] VARCHAR2(255) NOT NULL,  "));
		strSQLCreate.Append(_T( "    CONSTRAINT [] PRIMARY KEY ([ID], [Seq]))  "));

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

void KDBaseVersion_P017::TableNodeTemplateValue( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T("DROP TABLE NodeTemplateValue"));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
		TxLogDebugException();
	}

	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [NodeTemplateValue] (  "));
		strSQLCreate.Append(_T( "   [ID] Integer NOT NULL ,  "));
		strSQLCreate.Append(_T( "   [Seq] Integer NOT NULL ,  "));
		strSQLCreate.Append(_T( "   [Min] VARCHAR2 ,  "));
		strSQLCreate.Append(_T( "   [Max] VARCHAR2 ,  "));
		strSQLCreate.Append(_T( "   [Symbol] Integer ,  "));
		strSQLCreate.Append(_T( "   [Size] DOUBLE ,  "));
		strSQLCreate.Append(_T( "   [RGB] VARCHAR2(255) NOT NULL,  "));
		strSQLCreate.Append(_T( "    CONSTRAINT [sqlite_autoindex_NodeTemplateValue_1] PRIMARY KEY ([ID], [Seq]))  "));

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

void KDBaseVersion_P017::TableNodeTypeDefineChild( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T("DROP TABLE NodeTypeDefineChild"));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
		TxLogDebugException();
	}

	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [NodeTypeDefineChild] (  "));
		strSQLCreate.Append(_T( "   [ID] Integer NOT NULL ,  "));
		strSQLCreate.Append(_T( "   [node_type] Integer ,  "));
		strSQLCreate.Append(_T( "   [Symbol] Integer ,  "));
		strSQLCreate.Append(_T( "   [Color] VARCHAR2 ,  "));
		strSQLCreate.Append(_T( "   [Size] DOUBLE ,  "));
		strSQLCreate.Append(_T( "   [Draw] INTEGER DEFAULT 1,   "));
		strSQLCreate.Append(_T( "    CONSTRAINT [sqlite_autoindex_NodeTypeDefineChild_1] PRIMARY KEY ([ID], [node_type]))  "));

		spDBaseConnection->ExecuteUpdate(strSQLCreate);

		CString strSQLDefaultData(_T(""));
		strSQLDefaultData = _T(" insert into NodeTypeDefineChild values( 1, 0, 48, '83,135,171', 7.0, 1) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		strSQLDefaultData = _T(" insert into NodeTypeDefineChild values( 1, 1, 48, '181,180,156', 5.0, 1) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		strSQLDefaultData = _T(" insert into NodeTypeDefineChild values( 1, 2, 48, '199,147,100', 5.0, 1) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		strSQLDefaultData = _T(" insert into NodeTypeDefineChild values( 1, 3, 48, '176,148,111', 5.0, 1) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
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

void KDBaseVersion_P017::TableNodeTypeDefineParent( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T("DROP TABLE NodeTypeDefineParent"));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
		TxLogDebugException();
	}

	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [NodeTypeDefineParent] (  "));
		strSQLCreate.Append(_T( "   [ID] Integer  PRIMARY KEY ,  "));
		strSQLCreate.Append(_T( "   [Name] VARCHAR2 ,  "));
		strSQLCreate.Append(_T( "   [Flag] INTEGER DEFAULT 0 )   "));

		spDBaseConnection->ExecuteUpdate(strSQLCreate);

		CString strSQLDefaultData(_T(""));
		/*strSQLDefaultData = _T(" insert into NodeTypeDefineParent values( 1, 'Color', 1) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		strSQLDefaultData = _T(" insert into NodeTypeDefineParent values( 2, 'Gray', 0) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		strSQLDefaultData = _T(" insert into NodeTypeDefineParent values( 3, 'User(1)', 0) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		strSQLDefaultData = _T(" insert into NodeTypeDefineParent values( 4, 'User(2)', 0) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		strSQLDefaultData = _T(" insert into NodeTypeDefineParent values( 5, 'User(3)', 0) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		strSQLDefaultData = _T(" insert into NodeTypeDefineParent values( 6, 'User(4)', 0) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		strSQLDefaultData = _T(" insert into NodeTypeDefineParent values( 7, 'User(5)', 0) ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);*/
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strSQLDefaultData = _T(" insert into NodeTypeDefineParent values( 1, 'ÅÆÇÃ·¿(1)', 1) ");
			spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
			strSQLDefaultData = _T(" insert into NodeTypeDefineParent values( 2, 'ÅÆÇÃ·¿(2)', 0) ");
			spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
			strSQLDefaultData = _T(" insert into NodeTypeDefineParent values( 3, 'ÅÆÇÃ·¿(3)', 0) ");
			spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
			strSQLDefaultData = _T(" insert into NodeTypeDefineParent values( 4, 'ÅÆÇÃ·¿(4)', 0) ");
			spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
			strSQLDefaultData = _T(" insert into NodeTypeDefineParent values( 5, 'ÅÆÇÃ·¿(5)', 0) ");
			spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
			strSQLDefaultData = _T(" insert into NodeTypeDefineParent values( 6, 'ÅÆÇÃ·¿(6)', 0) ");
			spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
			strSQLDefaultData = _T(" insert into NodeTypeDefineParent values( 7, 'ÅÆÇÃ·¿(7)', 0) ");
			spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		}
		else
		{
			strSQLDefaultData = _T(" insert into NodeTypeDefineParent values( 1, 'Template(1)', 1) ");
			spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
			strSQLDefaultData = _T(" insert into NodeTypeDefineParent values( 2, 'Template(2)', 0) ");
			spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
			strSQLDefaultData = _T(" insert into NodeTypeDefineParent values( 3, 'Template(3)', 0) ");
			spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
			strSQLDefaultData = _T(" insert into NodeTypeDefineParent values( 4, 'Template(4)', 0) ");
			spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
			strSQLDefaultData = _T(" insert into NodeTypeDefineParent values( 5, 'Template(5)', 0) ");
			spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
			strSQLDefaultData = _T(" insert into NodeTypeDefineParent values( 6, 'Template(6)', 0) ");
			spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
			strSQLDefaultData = _T(" insert into NodeTypeDefineParent values( 7, 'Template(7)', 0) ");
			spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		}
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

void KDBaseVersion_P017::TableTypeDefineDefault( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T("DROP TABLE TypeDefineDefault"));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
		TxLogDebugException();
	}

	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [TypeDefineDefault] (  "));
		strSQLCreate.Append(_T( "   [Network_Type] INTEGER PRIMARY KEY DEFAULT 0,   "));
		strSQLCreate.Append(_T( "   [Symbol] Integer ,  "));
		strSQLCreate.Append(_T( "   [Color] VARCHAR2 ,  "));
		strSQLCreate.Append(_T( "   [Size] DOUBLE ,  "));
		strSQLCreate.Append(_T( "   [Desc] VARCHAR2 )   "));

		spDBaseConnection->ExecuteUpdate(strSQLCreate);

		CString strSQLDefaultData(_T(""));
		strSQLDefaultData = _T(" insert into TypeDefineDefault values( 0, 48, '217,217,217', 4.0, 'Node') ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
		strSQLDefaultData = _T(" insert into TypeDefineDefault values( 1,  0, '217,217,217', 1.0, '¸µÅ©') ");
		spDBaseConnection->ExecuteUpdate(strSQLDefaultData);
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

void KDBaseVersion_P017::TableZoneTemplate( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T("DROP TABLE ZoneTemplate"));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
		TxLogDebugException();
	}

	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [ZoneTemplate] (  "));
		strSQLCreate.Append(_T( "   [ID] Integer NOT NULL ,  "));
		strSQLCreate.Append(_T( "   [Type] Integer NOT NULL ,  "));
		strSQLCreate.Append(_T( "   [Name] VARCHAR2(255) NOT NULL,  "));
		strSQLCreate.Append(_T( "    CONSTRAINT [sqlite_autoindex_ZoneTemplate_1] PRIMARY KEY ([ID], [Type]))  "));

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

void KDBaseVersion_P017::TableZoneTemplateType( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T("DROP TABLE ZoneTemplateType"));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
		TxLogDebugException();
	}

	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [ZoneTemplateType] (  "));
		strSQLCreate.Append(_T( "   [ID] Integer NOT NULL ,  "));
		strSQLCreate.Append(_T( "   [Seq] Integer NOT NULL ,  "));
		strSQLCreate.Append(_T( "   [RGB] VARCHAR2(255) ,  "));
		strSQLCreate.Append(_T( "    CONSTRAINT [] PRIMARY KEY ([ID], [Seq]))  "));

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

void KDBaseVersion_P017::TableZoneTemplateValue( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T("DROP TABLE ZoneTemplateValue"));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
		TxLogDebugException();
	}

	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [ZoneTemplateValue] (  "));
		strSQLCreate.Append(_T( "   [ID] Integer NOT NULL ,  "));
		strSQLCreate.Append(_T( "   [Seq] Integer NOT NULL ,  "));
		strSQLCreate.Append(_T( "   [Min] VARCHAR2 ,  "));
		strSQLCreate.Append(_T( "   [Max] VARCHAR2 ,  "));
		strSQLCreate.Append(_T( "   [RGB] VARCHAR2(255) ,  "));
		strSQLCreate.Append(_T( "    CONSTRAINT [sqlite_autoindex_ZoneTemplateValue_1] PRIMARY KEY ([ID], [Seq]))  "));

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
