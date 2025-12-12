#include "StdAfx.h"
#include "DBaseImpExpShapeFile.h"
#include "TableRelationInfo.h"
#include "Target.h"

KDBaseImpExpShapeFile::KDBaseImpExpShapeFile(void)
{
}


KDBaseImpExpShapeFile::~KDBaseImpExpShapeFile(void)
{
}

bool KDBaseImpExpShapeFile::IsExistNodeIDAndGetXY( KDBaseConPtr spDBaseConnection, KImportShapeNodeXYInfo& ar_NodeInfo )
{
	CString strSQL(_T(""));

	try
	{
		strSQL.Format(_T("Select x, y From %s Where node_id = %I64d "), TABLE_NODE, ar_NodeInfo.nxNodeID);
		
		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);
		if (spResultset->Next())
		{
			ar_NodeInfo.dx = spResultset->GetValueDouble(0);
			ar_NodeInfo.dy = spResultset->GetValueDouble(1);
		}
		else
		{
			return false;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}
