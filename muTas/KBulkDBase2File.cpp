#include "StdAfx.h"
#include "KBulkDBase2File.h"
#include "KBulkDBase.h"
using namespace std;

KBulkDBase2File::KBulkDBase2File(void)
{
}


KBulkDBase2File::~KBulkDBase2File(void)
{
}

bool KBulkDBase2File::ReadNodeFile2Log(CString strFileName)
{
    CString strOut;
    struct _stat results;
    size_t size;
    if (_wstat(strFileName, &results) == 0)
    {
        size = results.st_size;
    }

    strOut.Format(_T("size : %d"), size);
    TxLogDebug(strOut.AllocSysString());

    ifstream ifs( strFileName, ios::binary );    
    TFixedNode node;

    size_t nCount = size / sizeof(TFixedNode);
    //eof    
    if ( ifs ) 
    {      
        for (size_t i=0; i<nCount; i++)
        {
            ifs.read( reinterpret_cast<char*>( &node ), sizeof(TFixedNode) );
            strOut.Format(_T("%I64d, %d, %f, %f, %I64d"), node.node_id, node.node_type, node.x, node.y, node.zone_id);
            TxLogDebug(strOut.AllocSysString());
        }
    }

    return true;
}


bool KBulkDBase2File::ExportDB2NodeFile(KDBaseConPtr a_spDBaseConnection, CString strFileName)
{
    TxLogDebugStartMsg();

    ofstream ofs(strFileName, ios::binary);
    if (! ofs)
    {
        TxLogDebugException();
        return false;
    }

    CString strSQL = _T(" Select node_id, node_type, x, y, zone_id From Node Order By node_id ");

	KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

	try
	{
		TFixedNode node;
		while(spResultSet->Next())
		{
			//ofs.write(reinterpret_cast<char*>)
			node.node_id   = spResultSet->GetValueInt64 (0);
			node.node_type = spResultSet->GetValueInt   (1);
			node.x         = spResultSet->GetValueDouble(2);
			node.y         = spResultSet->GetValueDouble(3);
			node.zone_id   = spResultSet->GetValueInt64 (4);

			ofs.write(reinterpret_cast<char*>( &node   ), sizeof(TFixedNode) );  
		}
	}
	catch (...)
	{
		TxLogDebugException();
		ofs.close();
		return false;
	}
    
    ofs.close();

    TxLogDebugEndMsg();
    return true;
}


bool KBulkDBase2File::ExportDB2NodeFileExcludeType( KDBaseConPtr a_spDBaseConnection, std::set<Integer> a_setInCludeNode, CString strFileName /*=_T("node.dat")*/ )
{
	TxLogDebugStartMsg();

	ofstream ofs(strFileName, ios::binary);
	if (! ofs)
	{
		TxLogDebugException();
		return false;
	}

	CString strSQL = _T(" Select node_id, node_type, x, y, zone_id From Node Order By node_id ");

	KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

	try
	{
		TFixedNode node;
		while(spResultSet->Next())
		{
			//ofs.write(reinterpret_cast<char*>)
			node.node_id   = spResultSet->GetValueInt64 (0);
			node.node_type = spResultSet->GetValueInt   (1);
			node.x         = spResultSet->GetValueDouble(2);
			node.y         = spResultSet->GetValueDouble(3);
			node.zone_id   = spResultSet->GetValueInt64 (4);

			AutoType iterfind = a_setInCludeNode.find(node.node_id);
			AutoType iterend  = a_setInCludeNode.end();

			if (iterfind != iterend)
			{
				ofs.write(reinterpret_cast<char*>( &node   ), sizeof(TFixedNode) );  
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
		ofs.close();
		return false;
	}

	ofs.close();

	TxLogDebugEndMsg();
	return true;
}



bool KBulkDBase2File::ExportDB2AccessLineStationFile(KDBaseConPtr a_spDBaseConnection, CString strFileName)
{
	TxLogDebugStartMsg();

	ofstream ofs(strFileName, ios::binary);
	if (! ofs)
	{ 
		TxLogDebugException();
		return false;
	}

	CString strSQL(_T(""));
	strSQL.Format(_T(" SELECT zone_id, line_id, type, no_station, station_access FROM %s order by zone_id, line_id "), TABLE_ACCESS_LINE_STATION);

	KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

	try
	{
		TFixedAccessLineStation oTFixedAccessLineStation;
		while(spResultSet->Next())
		{

			oTFixedAccessLineStation.zone_id    = spResultSet->GetValueInt64 (0);
			oTFixedAccessLineStation.line_id    = spResultSet->GetValueInt64 (1);
			oTFixedAccessLineStation.type       = spResultSet->GetValueInt   (2);
			oTFixedAccessLineStation.no_station = spResultSet->GetValueInt   (3);
			CString strStationAccess            = spResultSet->GetValueString(4);

			if(strStationAccess.CompareNoCase(_T("")) == 0)
			{
				continue;
			}

			ofs.write(reinterpret_cast<char*>( &oTFixedAccessLineStation), sizeof(TFixedAccessLineStation) );  

			int     nCount  = 0;
			int     nPos    = 0;
			CString strTemp = strStationAccess;
			strTemp.Trim();
			CString strPart = strTemp.Tokenize( COLUMN_STATION_ACCESS_SEPARATOR, nPos );
			while( strPart != _T(""))
			{
				CString  strStationNodeID = strPart.Mid(0,                          strPart.Find(_T(':')) );
				CString  strClassName     = strPart.Mid(strPart.Find(_T(':'))+1,    strPart.GetLength()   );

				Integer  nxStationID = _ttoi64(strStationNodeID);
				char*    cClassName = LPSTR(LPCTSTR(strClassName));
				char cName = cClassName[0];

				ofs.write(reinterpret_cast<char*>( &nxStationID),     sizeof(Integer));  
				ofs.write(reinterpret_cast<char*>( &(cName)), sizeof(char));

				strPart = strStationAccess.Tokenize( COLUMN_STATION_ACCESS_SEPARATOR, nPos );

				++nCount;

				if(oTFixedAccessLineStation.no_station <= nCount)
					break;
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
		ofs.close();
		return false;
	}

	ofs.close();

	TxLogDebugEndMsg();
	return true;
}


KIOColumns KBulkDBase2File::GetFixedLinkColumns()
{
    KIOColumns columnCollection;

    KIOColumn* pColumn = NULL;                      
    // link_id
    pColumn = columnCollection.AddNewColumn();    
    pColumn->Name(_T("link_id"));

    pColumn = columnCollection.AddNewColumn();    
    pColumn->Name(_T("fnode_id"));

    pColumn = columnCollection.AddNewColumn();    
    pColumn->Name(_T("tnode_id"));

    pColumn = columnCollection.AddNewColumn();    
    pColumn->Name(_T("link_type"));

    pColumn = columnCollection.AddNewColumn();    
    pColumn->Name(_T("length"));

    pColumn = columnCollection.AddNewColumn();    
    pColumn->Name(_T("lane"));

    pColumn = columnCollection.AddNewColumn();    
    pColumn->Name(_T("capa"));

    pColumn = columnCollection.AddNewColumn();    
    pColumn->Name(_T("speed"));
       
    return columnCollection;
}


bool KBulkDBase2File::ExportDB2LinkFile(KDBaseConPtr a_spDBaseConnection, KIOColumns& userColumnCollection, CString strFileName/* =_T */)
{
    TxLogDebugStartMsg();
    
    CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed ") ;

    KIOColumn* pColumn = NULL;
    int nColumnCount = userColumnCollection.ColumnCount();
    for (int i=0; i<nColumnCount; i++)
    {
        pColumn = userColumnCollection.GetColumn(i);
        strSQL  = strSQL + _T(", ") + pColumn->Name() + _T(" ");
    }

    strSQL = strSQL + _T(" From Link Order By link_id ");

    ofstream ofs(strFileName, ios::binary);
    if (! ofs)
    {
        TxLogDebugException();
        return false;
    }

	KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

	try
	{
		TFixedLink fixedLink;
		while(spResultSet->Next())
		{
			//ofs.write(reinterpret_cast<char*>)
			fixedLink.link_id   = spResultSet->GetValueInt64 (0);
			fixedLink.fnode_id  = spResultSet->GetValueInt64 (1);
			fixedLink.tnode_id  = spResultSet->GetValueInt64 (2);
			fixedLink.link_type = spResultSet->GetValueInt   (3);
			fixedLink.length    = spResultSet->GetValueDouble(4);
			fixedLink.lane      = spResultSet->GetValueInt   (5);
			fixedLink.capa      = spResultSet->GetValueDouble(6);
			fixedLink.speed     = spResultSet->GetValueDouble(7);

			ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TFixedLink) );  

			for (int i=0; i<nColumnCount; i++)
			{
				double dValue = spResultSet->GetValueDouble(8+i);
				ofs.write(reinterpret_cast<char*>( &dValue ), sizeof(double) ); 
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
		ofs.close();
		return false;
	}

    ofs.close();

    TxLogDebugEndMsg();
    return true;
}


bool KBulkDBase2File::ExportDB2LinkFileB(KDBaseConPtr a_spDBaseConnection, KIOColumns& userColumnCollection, CString strFileName/* =_T */)
{
	TxLogDebugStartMsg();

	CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed, function_id ") ;

	KIOColumn* pColumn = NULL;
	int nColumnCount = userColumnCollection.ColumnCount();
	for (int i=0; i<nColumnCount; i++)
	{
		pColumn = userColumnCollection.GetColumn(i);
		strSQL  = strSQL + _T(", ") + pColumn->Name() + _T(" ");
	}

	strSQL = strSQL + _T(" From Link Order By link_id ");

	ofstream ofs(strFileName, ios::binary);
	if (! ofs)
	{
		TxLogDebugException();
		return false;
	}

	KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

	try
	{
		TFixedLink_B fixedLink;
		while(spResultSet->Next())
		{
			//ofs.write(reinterpret_cast<char*>)
			fixedLink.link_id     = spResultSet->GetValueInt64 (0);
			fixedLink.fnode_id    = spResultSet->GetValueInt64 (1);
			fixedLink.tnode_id    = spResultSet->GetValueInt64 (2);
			fixedLink.link_type   = spResultSet->GetValueInt   (3);
			fixedLink.length      = spResultSet->GetValueDouble(4);
			fixedLink.lane        = spResultSet->GetValueInt   (5);
			fixedLink.capa        = spResultSet->GetValueDouble(6);
			fixedLink.speed       = spResultSet->GetValueDouble(7);
			fixedLink.function_id = spResultSet->GetValueInt   (8);

			ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TFixedLink_B) );  

			for (int i=0; i<nColumnCount; i++)
			{
				double dValue = spResultSet->GetValueDouble(9+i);
				ofs.write(reinterpret_cast<char*>( &dValue ), sizeof(double) ); 
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
		ofs.close();
		return false;
	}

	ofs.close();

	TxLogDebugEndMsg();
	return true;
}



bool KBulkDBase2File::ExportDB2LinkFileExcludeType( KDBaseConPtr a_spDBaseConnection, KIOColumns& userColumnCollection, std::set<Integer> a_setIncldueLink, CString strFileName/*=_T("link.dat")*/ )
{
	TxLogDebugStartMsg();

	CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed, function_id ") ;

	KIOColumn* pColumn = NULL;
	int nColumnCount = userColumnCollection.ColumnCount();
	for (int i=0; i<nColumnCount; i++)
	{
		pColumn = userColumnCollection.GetColumn(i);
		strSQL  = strSQL + _T(", ") + pColumn->Name() + _T(" ");
	}

	strSQL = strSQL + _T(" From Link Order By link_id ");

	ofstream ofs(strFileName, ios::binary);
	if (! ofs)
	{
		TxLogDebugException();
		return false;
	}

	KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

	try
	{
		TFixedLink_B fixedLink;
		while(spResultSet->Next())
		{
			//ofs.write(reinterpret_cast<char*>)
			fixedLink.link_id     = spResultSet->GetValueInt64 (0);
			fixedLink.fnode_id    = spResultSet->GetValueInt64 (1);
			fixedLink.tnode_id    = spResultSet->GetValueInt64 (2);
			fixedLink.link_type   = spResultSet->GetValueInt   (3);
			fixedLink.length      = spResultSet->GetValueDouble(4);
			fixedLink.lane        = spResultSet->GetValueInt   (5);
			fixedLink.capa        = spResultSet->GetValueDouble(6);
			fixedLink.speed       = spResultSet->GetValueDouble(7);
			fixedLink.function_id = spResultSet->GetValueInt   (8);

			AutoType iterfind = a_setIncldueLink.find(fixedLink.link_id);
			AutoType iterend  = a_setIncldueLink.end();

			if (iterfind != iterend)
			{
				ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TFixedLink_B) );  

				for (int i=0; i<nColumnCount; i++)
				{
					double dValue = spResultSet->GetValueDouble(9+i);
					ofs.write(reinterpret_cast<char*>( &dValue ), sizeof(double) ); 
				}
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
		ofs.close();
		return false;
	}

	ofs.close();

	TxLogDebugEndMsg();
	return true;
}


bool KBulkDBase2File::ExportDB2LinkfileExcludeTypeByPathFind( KDBaseConPtr a_spDBaseConnection, KIOColumns& userColumnCollection, std::set<Integer> a_setIncldueLink, CString strFileName/*=_T("link.dat")*/ )
{
	TxLogDebugStartMsg();

	CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed ") ;

	KIOColumn* pColumn = NULL;
	int nColumnCount = userColumnCollection.ColumnCount();
	for (int i=0; i<nColumnCount; i++)
	{
		pColumn = userColumnCollection.GetColumn(i);
		strSQL  = strSQL + _T(", ") + pColumn->Name() + _T(" ");
	}

	strSQL = strSQL + _T(" From Link Order By link_id ");

	ofstream ofs(strFileName, ios::binary);
	if (! ofs)
	{
		TxLogDebugException();
		return false;
	}

	KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

	try
	{
		TFixedLink fixedLink;
		while(spResultSet->Next())
		{
			//ofs.write(reinterpret_cast<char*>)
			fixedLink.link_id   = spResultSet->GetValueInt64 (0);
			fixedLink.fnode_id  = spResultSet->GetValueInt64 (1);
			fixedLink.tnode_id  = spResultSet->GetValueInt64 (2);
			fixedLink.link_type = spResultSet->GetValueInt   (3);
			fixedLink.length    = spResultSet->GetValueDouble(4);
			fixedLink.lane      = spResultSet->GetValueInt   (5);
			fixedLink.capa      = spResultSet->GetValueDouble(6);
			fixedLink.speed     = spResultSet->GetValueDouble(7);

			AutoType iterfind = a_setIncldueLink.find(fixedLink.link_id);
			AutoType iterend  = a_setIncldueLink.end();

			if (iterfind != iterend)
			{
				ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TFixedLink) );  

				for (int i=0; i<nColumnCount; i++)
				{
					double dValue = spResultSet->GetValueDouble(8+i);
					ofs.write(reinterpret_cast<char*>( &dValue ), sizeof(double) ); 
				}
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
		ofs.close();
		return false;
	}

	ofs.close();

	TxLogDebugEndMsg();
	return true;
}


bool KBulkDBase2File::ExportDB2LinkFileC(KDBaseConPtr a_spDBaseConnection, KIOColumns& userColumnCollection, CString strFileName/* =_T */)
{
	TxLogDebugStartMsg();

	CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length ") ;

	KIOColumn* pColumn = NULL;
	int nColumnCount = userColumnCollection.ColumnCount();
	for (int i=0; i<nColumnCount; i++)
	{
		pColumn = userColumnCollection.GetColumn(i);
		strSQL  = strSQL + _T(", ") + pColumn->Name() + _T(" ");
	}

	strSQL = strSQL + _T(" From Link Order By link_id ");

	ofstream ofs(strFileName, ios::binary);
	if (! ofs)
	{
		TxLogDebugException();
		return false;
	}

	KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

	try
	{
		TFixedLink_C fixedLink;
		while (spResultSet->Next())
		{
			//ofs.write(reinterpret_cast<char*>)
			fixedLink.link_id   = spResultSet->GetValueInt64 (0);
			fixedLink.fnode_id  = spResultSet->GetValueInt64 (1);
			fixedLink.tnode_id  = spResultSet->GetValueInt64 (2);
			fixedLink.link_type = spResultSet->GetValueInt   (3);
			fixedLink.length    = spResultSet->GetValueDouble(4);

			ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TFixedLink_C) );  

			for (int i=0; i<nColumnCount; i++)
			{
				double dValue = spResultSet->GetValueDouble(5+i);
				ofs.write(reinterpret_cast<char*>( &dValue ), sizeof(double) ); 
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
		ofs.close();
		return false;
	}

	ofs.close();

	TxLogDebugEndMsg();
	return true;
}


bool KBulkDBase2File::ExportDB2LinkFileForShortestPathTransit(KDBaseConPtr a_spDBaseConnection, KIOColumns& userColumnCollection, CString strFileName/* =_T */)
{
	TxLogDebugStartMsg();

	CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length ") ;

	KIOColumn* pColumn = NULL;
	int nColumnCount = userColumnCollection.ColumnCount();
	for (int i=0; i<nColumnCount; i++)
	{
		pColumn = userColumnCollection.GetColumn(i);
		strSQL  = strSQL + _T(", ") + pColumn->Name() + _T(" ");
	}

	strSQL = strSQL + _T(" From Link Order By link_id ");

	ofstream ofs(strFileName, ios::binary);
	if (! ofs)
	{
		TxLogDebugException();
		return false;
	}

	KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

	try
	{
		TFixedLink_C fixedLink;
		while(spResultSet->Next())
		{
			//ofs.write(reinterpret_cast<char*>)
			fixedLink.link_id   = spResultSet->GetValueInt64 (0);
			fixedLink.fnode_id  = spResultSet->GetValueInt64 (1);
			fixedLink.tnode_id  = spResultSet->GetValueInt64 (2);
			fixedLink.link_type = spResultSet->GetValueInt   (3);
			fixedLink.length    = spResultSet->GetValueDouble(4);

			ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TFixedLink_C) );  

			for (int i=0; i<nColumnCount; i++)
			{
				double dValue = spResultSet->GetValueDouble(5+i);
				ofs.write(reinterpret_cast<char*>( &dValue ), sizeof(double) ); 
			}

			if (1 == nColumnCount) // Transit_ShortestPath_arg_파일프로토콜 참조
			{
				double dValueZero = 0.0;
				ofs.write(reinterpret_cast<char*>( &dValueZero ), sizeof(double) ); 
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
		ofs.close();
		return false;
	}

	ofs.close();

	TxLogDebugEndMsg();
	return true;
}


bool KBulkDBase2File::ExportDB2LinkFileGeneratorPath( KDBaseConPtr a_spDBaseConnection, KIOColumn* a_pColumn, CString strFileName/*=_T("link.dat")*/ )
{
	TxLogDebugStartMsg();

	CString strSQL        = _T(" Select link_id, fnode_id, tnode_id, link_type, length, speed ") ;
	CString strColumnName = a_pColumn->Name();

	if(strColumnName.CompareNoCase(COLUMN_LINK_SPEED) != 0)
	{
		strSQL  = strSQL + _T(", ") + a_pColumn->Name() + _T(" ");
	}

	strSQL = strSQL + _T(" From Link Order By link_id ");

	ofstream ofs(strFileName, ios::binary);
	if (! ofs)
	{
		TxLogDebugException();
		return false;
	}

	KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

	try
	{
		TFixedLink_Generator fixedLink;
		while(spResultSet->Next())
		{
			//ofs.write(reinterpret_cast<char*>)
			fixedLink.link_id   = spResultSet->GetValueInt64 (0);
			fixedLink.fnode_id  = spResultSet->GetValueInt64 (1);
			fixedLink.tnode_id  = spResultSet->GetValueInt64 (2);
			fixedLink.link_type = spResultSet->GetValueInt   (3);
			fixedLink.length    = spResultSet->GetValueDouble(4);
			fixedLink.speed		= spResultSet->GetValueDouble(5);

			ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TFixedLink_Generator) );  

			if ( strColumnName.CompareNoCase(COLUMN_LINK_SPEED) != 0)
			{

				double dValue = spResultSet->GetValueDouble(6);
				ofs.write(reinterpret_cast<char*>( &dValue ), sizeof(double) ); 
			}
			else
			{
				double dValue = 0;
				ofs.write(reinterpret_cast<char*>( &dValue ), sizeof(double) );
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
		ofs.close();
		return false;
	}

	ofs.close();

	TxLogDebugEndMsg();
	return true;
}


bool KBulkDBase2File::ExportDB2LinkFileD(KDBaseConPtr a_spDBaseConnection, KIOColumns& userColumnCollection, CString strFileName/* =_T */)
{
	TxLogDebugStartMsg();

	CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed, function_id, alpha, beta, a, b ") ;

	KIOColumn* pColumn = NULL;
	int nColumnCount = userColumnCollection.ColumnCount();
	for (int i=0; i<nColumnCount; i++)
	{
		pColumn = userColumnCollection.GetColumn(i);
		strSQL  = strSQL + _T(", ") + pColumn->Name() + _T(" ");
	}

	strSQL = strSQL + _T(" From Link Order By link_id ");

	ofstream ofs(strFileName, ios::binary);
	if (! ofs)
	{
		TxLogDebugException();
		return false;
	}

	KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

	try
	{
		TFixedLink_D fixedLink;
		while(spResultSet->Next())
		{
			//ofs.write(reinterpret_cast<char*>)
			fixedLink.link_id       = spResultSet->GetValueInt64 (0);
			fixedLink.fnode_id      = spResultSet->GetValueInt64 (1);
			fixedLink.tnode_id      = spResultSet->GetValueInt64 (2);
			fixedLink.link_type     = spResultSet->GetValueInt   (3);
			fixedLink.length        = spResultSet->GetValueDouble(4);
			fixedLink.lane          = spResultSet->GetValueInt   (5);
			fixedLink.capa          = spResultSet->GetValueDouble(6);
			fixedLink.speed         = spResultSet->GetValueDouble(7);
			fixedLink.function_id   = spResultSet->GetValueInt   (8);
			fixedLink.alpha         = spResultSet->GetValueDouble(9);
			fixedLink.beta          = spResultSet->GetValueDouble(10);
			fixedLink.a             = spResultSet->GetValueDouble(11);
			fixedLink.b             = spResultSet->GetValueDouble(12);

			ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TFixedLink_D) );  

			for (int i=0; i<nColumnCount; i++)
			{
				double dValue = spResultSet->GetValueDouble(13+i);
				ofs.write(reinterpret_cast<char*>( &dValue ), sizeof(double) ); 
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
		ofs.close();
		return false;
	}

	ofs.close();

	TxLogDebugEndMsg();
	return true;
}


bool KBulkDBase2File::ExportDB2ODFile(KDBaseConPtr a_spDBaseConnection, CString strTableName, KIOColumns& userColumnCollection, CString strFileName)
{
    TxLogDebugStartMsg();

    CString strSQL = _T(" Select ozone_id, dzone_id ") ;

    KIOColumn* pColumn = NULL;
    int nColumnCount = userColumnCollection.ColumnCount();
    for (int i=0; i<nColumnCount; i++)
    {
        pColumn = userColumnCollection.GetColumn(i);
        strSQL  = strSQL + _T(", ") + pColumn->Name() + _T(" ");
    }

    strSQL = strSQL + _T(" From ") + strTableName;
    strSQL = strSQL + _T(" Order By ozone_id, dzone_id");

    ofstream ofs(strFileName, ios::binary);
    if (! ofs)
    {
        TxLogDebugException();
        return false;
    }

	KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

	try
	{
		TFixedOD fixedOD;
		while(spResultSet->Next())
		{
			//ofs.write(reinterpret_cast<char*>)
			fixedOD.ozone_id  = spResultSet->GetValueInt64 (0);
			fixedOD.dzone_id  = spResultSet->GetValueInt64 (1);

			ofs.write(reinterpret_cast<char*>( &fixedOD ), sizeof(TFixedOD) );  

			for (int i=0; i<nColumnCount; i++)
			{
				double dValue = spResultSet->GetValueDouble(2+i);
				ofs.write(reinterpret_cast<char*>( &dValue ), sizeof(double) ); 
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
		ofs.close();
		return false;
	}

	ofs.close();

    TxLogDebugEndMsg();
    return true;
}


bool KBulkDBase2File::ExportDB2TurnFile(KDBaseConPtr a_spDBaseConnection, CString strFileName/* =_T */)
{
    TxLogDebugStartMsg();

    std::map<Integer, TFTNode> linkFTNodeRecords;
    if ( ! KBulkDBase::BulkLinkFTNodeSelect(a_spDBaseConnection, linkFTNodeRecords))
    {
        TxLogDebugException();
        return false;
    }

    std::map<Integer, TFTNode>::iterator iterLink;
    std::map<Integer, TFTNode>::iterator endLink  = linkFTNodeRecords.end();

    std::map<Integer, TFixedTurnDB> turnRecords;
    if ( ! KBulkDBase::BulkTurnSelect(a_spDBaseConnection, turnRecords) )
    {
        TxLogDebugException();
        return false;
    }

    std::map<Integer, TFixedTurnDB>::iterator iterTurn = turnRecords.begin();
    std::map<Integer, TFixedTurnDB>::iterator endTurn  = turnRecords.end();

    TFixedTurn fixedTurnOut;
    bool isWriteDB = true;
    
    ofstream ofs(strFileName, ios::binary);
    if (! ofs)
    {
        TxLogDebugException();
        return false;
    }

    while ( iterTurn != endTurn )
    {
        Integer       turnID      = iterTurn->first;
        TFixedTurnDB& fixedTurnDB = iterTurn->second;
                
        fixedTurnOut.turn_id = turnID;
        iterLink = linkFTNodeRecords.find(fixedTurnDB.inlink_id);
        if ( iterLink != endLink )
        {
            TFTNode& FTNode = iterLink->second;
            fixedTurnOut.snode_id = FTNode.fnode_id;
            fixedTurnOut.vnode_id = FTNode.tnode_id;

            iterLink = linkFTNodeRecords.find(fixedTurnDB.outlink_id);
            if ( iterLink != endLink )
            {
                TFTNode& FTNode = iterLink->second;
                fixedTurnOut.enode_id = FTNode.tnode_id;
            }
            else
            {
                CString strOut;
                strOut.Format(_T("Out Link : %I64d"), fixedTurnDB.outlink_id);
                TxLogDebug(strOut.AllocSysString());
                isWriteDB = false;
            }
        }
        else
        {
            CString strOut;
            strOut.Format(_T("In Link : %I64d"), fixedTurnDB.inlink_id);
            TxLogDebug(strOut.AllocSysString());
            isWriteDB = false;
        }
        
        fixedTurnOut.turn_type = fixedTurnDB.turn_type;
        //fixedTurnOut.turn_time = fixedTurnDB.turn_time;

        if (isWriteDB)
        {
            ofs.write(reinterpret_cast<char*>( &fixedTurnOut ), sizeof(TFixedTurn) );  
        }
        else
        {
            TxLogDebugException();
        }

        ++iterTurn;
    }

    ofs.close();

    TxLogDebugEndMsg();
    return true;
}



bool KBulkDBase2File::ExportDB2TurnFileExcludeType( KDBaseConPtr a_spDBaseConnection, std::set<Integer> a_setIncludeTurn, CString strFileName/*=_T("turn.dat")*/ )
{
	TxLogDebugStartMsg();

	std::map<Integer, TFTNode> linkFTNodeRecords;
	if ( ! KBulkDBase::BulkLinkFTNodeSelect(a_spDBaseConnection, linkFTNodeRecords))
	{
		TxLogDebugException();
		return false;
	}

	std::map<Integer, TFTNode>::iterator iterLink;
	std::map<Integer, TFTNode>::iterator endLink  = linkFTNodeRecords.end();

	std::map<Integer, TFixedTurnDB> turnRecords;
	if ( ! KBulkDBase::BulkTurnSelect(a_spDBaseConnection, turnRecords) )
	{
		TxLogDebugException();
		return false;
	}

	std::map<Integer, TFixedTurnDB>::iterator iterTurn = turnRecords.begin();
	std::map<Integer, TFixedTurnDB>::iterator endTurn  = turnRecords.end();

	TFixedTurn fixedTurnOut;
	bool isWriteDB = true;

	ofstream ofs(strFileName, ios::binary);
	if (! ofs)
	{
		TxLogDebugException();
		return false;
	}

	while ( iterTurn != endTurn )
	{
		Integer       turnID      = iterTurn->first;
		TFixedTurnDB& fixedTurnDB = iterTurn->second;

		fixedTurnOut.turn_id = turnID;
		iterLink = linkFTNodeRecords.find(fixedTurnDB.inlink_id);
		if ( iterLink != endLink )
		{
			TFTNode& FTNode = iterLink->second;
			fixedTurnOut.snode_id = FTNode.fnode_id;
			fixedTurnOut.vnode_id = FTNode.tnode_id;

			iterLink = linkFTNodeRecords.find(fixedTurnDB.outlink_id);
			if ( iterLink != endLink )
			{
				TFTNode& FTNode = iterLink->second;
				fixedTurnOut.enode_id = FTNode.tnode_id;
			}
			else
			{
				CString strOut;
				strOut.Format(_T("Out Link : %I64d"), fixedTurnDB.outlink_id);
				TxLogDebug(strOut.AllocSysString());
				isWriteDB = false;
			}
		}
		else
		{
			CString strOut;
			strOut.Format(_T("In Link : %I64d"), fixedTurnDB.inlink_id);
			TxLogDebug(strOut.AllocSysString());
			isWriteDB = false;
		}

		fixedTurnOut.turn_type = fixedTurnDB.turn_type;
		//fixedTurnOut.turn_time = fixedTurnDB.turn_time;

		AutoType iterfind = a_setIncludeTurn.find(turnID);
		AutoType iterend  = a_setIncludeTurn.end();

		if (iterfind != iterend)
		{
			if (isWriteDB)
			{
				ofs.write(reinterpret_cast<char*>( &fixedTurnOut ), sizeof(TFixedTurn) );  
			}
			else
			{
				TxLogDebugException();
			}
		}

		++iterTurn;
	}

	ofs.close();

	TxLogDebugEndMsg();
	return true;
}



bool KBulkDBase2File::ExportDB2LineFile(KDBaseConPtr a_spDBaseConnection, CString strLineInfoFileName, CString strLineFileName)
{
    TxLogDebugStartMsg();

    std::vector<TFixedTransitFile> records;
    if ( ! KBulkDBase::BulkTransitSelect(a_spDBaseConnection, records) )
    {
        TxLogDebugException();
        return false;
    }

    size_t nCount = records.size();
    if (nCount < 1)
    {
        return true;
    }

    std::map<Integer, TFTNode> linkRecords;
    if ( ! KBulkDBase::BulkLinkFTNodeSelect(a_spDBaseConnection, linkRecords) )
    {
        TxLogDebugException();
        return false;
    }

    if ( linkRecords.size() < 1)
    {
        return true;
    }

    ofstream ofsLineInfo(strLineInfoFileName, ios::binary);
    if (! ofsLineInfo)
    {
        TxLogDebugException();
        return false;
    }

    ofstream ofsLineData(strLineFileName, ios::binary);
    if (! ofsLineData)
    {
        TxLogDebugException();
        return false;
    }

    for (size_t i=0; i<nCount; i++)
    {
        TFixedTransitFile record = records[i];
        int nLineCount = ExportLineDataFile(a_spDBaseConnection, ofsLineData, record.transit_id, linkRecords);
        if ( nLineCount > 0 )
        {
            record.lineCount = nLineCount;
            ofsLineInfo.write(reinterpret_cast<char*>( &record ), sizeof(TFixedTransitFile) );  
        }        
    }

    ofsLineInfo.close();
    ofsLineData.close();

    TxLogDebugEndMsg();
    return true;
}

int KBulkDBase2File::ExportLineDataFile(KDBaseConPtr a_spDBaseConnection, ofstream& ofs, Integer transitID, std::map<Integer, TFTNode>& linkRecords)
{   
    std::map<Integer, TFTNode>::iterator iter;
    std::map<Integer, TFTNode>::iterator end  = linkRecords.end();

    std::vector<TFixedTransitLink> transitLinkRecords;
    if ( ! KBulkDBase::BulkTransitLinksSelect(a_spDBaseConnection, transitLinkRecords, transitID) )
    {
        TxLogDebugException();
        return -1;
    }

    size_t nTransitLinksCount = transitLinkRecords.size();

    if (nTransitLinksCount < 1)
    {
        return -1;
    }

    int nLineCount = 0;
    for (size_t i=0; i<nTransitLinksCount; i++)
    {
        TFixedTransitLink& transitLink = transitLinkRecords[i];
        iter = linkRecords.find(transitLink.link_id);
        int     nSeq        = transitLink.seq;
        Integer fNode       = iter->second.fnode_id;
        Integer tNode       = iter->second.tnode_id;
        int     stationFlag = transitLink.station_yn;

        if (i == 0)
        {
            ofs.write(reinterpret_cast<char*>( &transitID  ), sizeof(Integer) ); 
            ofs.write(reinterpret_cast<char*>( &nSeq       ), sizeof(int)     ); 
            ofs.write(reinterpret_cast<char*>( &fNode      ), sizeof(Integer) ); 
			int nTemp = 1;
            ofs.write(reinterpret_cast<char*>( &nTemp/*시작=1*/ ), sizeof(int)     ); 
            nLineCount++;
        }

        ++nSeq;
        ofs.write(reinterpret_cast<char*>( &transitID   ), sizeof(Integer) ); 
        ofs.write(reinterpret_cast<char*>( &nSeq        ), sizeof(int)     ); 
        ofs.write(reinterpret_cast<char*>( &tNode       ), sizeof(Integer) ); 
        ofs.write(reinterpret_cast<char*>( &stationFlag ), sizeof(int)     );    
        nLineCount++;
    }   

    return nLineCount;
}

bool KBulkDBase2File::ReadLineOutFile(std::vector<TFixedTransitFileRead>& transitRecords, CString strFileName/* =_T */)
{
    struct _stat results;
    size_t size;
    if (_wstat(strFileName, &results) == 0)
    {
        size = results.st_size;
    }

    ifstream ifs( strFileName, ios::binary );    
    TFixedTransitFileRead record;

    size_t nCount = size / sizeof(TFixedTransitFileRead);
    //eof    
    if ( ifs ) 
    {      
        for (size_t i=0; i<nCount; i++)
        {            
            ifs.read( reinterpret_cast<char*>( &record ), sizeof(TFixedTransitFileRead) );
            transitRecords.push_back(record);
        }
    }

    ifs.close();
    return true;
}


bool KBulkDBase2File::ReadLineDataOutFile(std::vector<TFixedTransitLinkFileRead>& transitLineRecords, CString strFileName/* =_T */)
{
    struct _stat results;
    size_t size;
    if (_wstat(strFileName, &results) == 0)
    {
        size = results.st_size;
    }

    ifstream ifs( strFileName, ios::binary );    
    TFixedTransitLinkFileRead record;

    size_t nCount = size / sizeof(TFixedTransitLinkFileRead);
    //eof    
    if ( ifs ) 
    {      
        for (size_t i=0; i<nCount; i++)
        {            
            ifs.read( reinterpret_cast<char*>( &record ), sizeof(TFixedTransitLinkFileRead) );
            transitLineRecords.push_back(record);
        }
    }

    ifs.close();

    return true;
}


bool KBulkDBase2File::ExportDB2SedVecFile(KDBaseConPtr a_spDBaseConnection, KIOColumns& userColumnCollection, CString strFileName)
{
	TxLogDebugStartMsg();

	CString strSQL = _T(" Select Zone_ID ") ;

	KIOColumn* pColumn = NULL;
	int nColumnCount = userColumnCollection.ColumnCount();
	for (int i=0; i<nColumnCount; i++)
	{
		pColumn = userColumnCollection.GetColumn(i);
		strSQL  = strSQL + _T(", ") + pColumn->Name() + _T(" ");
	}

	strSQL = strSQL + _T(" From Sed_Vector ");

	ofstream ofs(strFileName, ios::binary);
	if (! ofs)
	{
		TxLogDebugException();
		return false;
	}

	KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

	try
	{
		while(spResultSet->Next())
		{
			Integer nxZoneID    = spResultSet->GetValueInt64 (0);
			ofs.write(reinterpret_cast<char*>( &nxZoneID ), sizeof(Integer) );  

			for (int i=0; i<nColumnCount; i++)
			{
				double dValue   = spResultSet->GetValueDouble(1+i);
				ofs.write(reinterpret_cast<char*>( &dValue ), sizeof(double) ); 
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
		ofs.close();
		return false;
	}

	ofs.close();

	TxLogDebugEndMsg();
	return true;
}


bool KBulkDBase2File::ExportDB2ZoneFile(KDBaseConPtr a_spDBaseConnection, KIOColumns& userColumnCollection, CString strFileName)
{
	TxLogDebugStartMsg();

	CString strSQL = _T(" Select Zone_ID ") ;

	KIOColumn* pColumn = NULL;
	int nColumnCount = userColumnCollection.ColumnCount();
	for (int i=0; i<nColumnCount; i++)
	{
		pColumn = userColumnCollection.GetColumn(i);
		strSQL  = strSQL + _T(", ") + pColumn->Name() + _T(" ");
	}

	strSQL = strSQL + _T(" From zone ");

	ofstream ofs(strFileName, ios::binary);
	if (! ofs)
	{
		TxLogDebugException();
		return false;
	}

	KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

	try
	{
		while (spResultSet->Next())
		{
			Integer nxZoneID    = spResultSet->GetValueInt64 (0);
			ofs.write(reinterpret_cast<char*>( &nxZoneID ), sizeof(Integer) );  

			for (int i=0; i<nColumnCount; i++)
			{
				double dValue   = spResultSet->GetValueDouble(1+i);
				ofs.write(reinterpret_cast<char*>( &dValue ), sizeof(double) ); 
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
		ofs.close();
		return false;
	}

	ofs.close();

	TxLogDebugEndMsg();
	return true;
}


bool KBulkDBase2File::ExportDB2AccessStopNodeFile( KDBaseConPtr a_spDBaseConnection, CString strFileName/*=_T("AccBusSubwayStopAtZone.dat")*/ )
{
	TxLogDebugStartMsg();

	ofstream ofs(strFileName, ios::binary);
	if (! ofs)
	{ 
		TxLogDebugException();
		return false;
	}

	CString strSQL(_T(""));
	strSQL.Format(_T(" SELECT zone_id, node_id, mode, class_in_zone, class_in_network, no_of_line FROM %s order by zone_id, node_id "), TABLE_ACCESS_STOP_NODE);

	Integer nxZoneID(0);
	Integer nxNodeID(0);
	int		nMode(0);
	CString strClassInZone(_T(""));
	CString strClassInNetWork(_T(""));
	int		nNoofLine(0);

	KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

	try
	{
		while(spResultSet->Next())
		{
			nxZoneID           = spResultSet->GetValueInt64 (0);
			ofs.write(reinterpret_cast<char*>( &nxZoneID),     sizeof(Integer));

			nxNodeID           = spResultSet->GetValueInt64 (1);
			ofs.write(reinterpret_cast<char*>( &nxNodeID),     sizeof(Integer));

			nMode              = spResultSet->GetValueInt   (2);
			ofs.write(reinterpret_cast<char*>( &nMode),     sizeof(int));

			strClassInZone	   = spResultSet->GetValueString(3);
			char*    cClassName = LPSTR(LPCTSTR(strClassInZone));
			char cName = cClassName[0];
			ofs.write(reinterpret_cast<char*>( &(cName)), sizeof(char));

			strClassInNetWork  = spResultSet->GetValueString(4);
			cClassName = LPSTR(LPCTSTR(strClassInNetWork));
			cName = cClassName[0];
			ofs.write(reinterpret_cast<char*>( &(cName)), sizeof(char));

			nNoofLine          = spResultSet->GetValueInt   (5);
			ofs.write(reinterpret_cast<char*>( &nNoofLine),     sizeof(int));
		}
	}
	catch (...)
	{
		TxLogDebugException();
		ofs.close();
		return false;
	}

	ofs.close();

	TxLogDebugEndMsg();
	return true;
}

bool KBulkDBase2File::EXportDB2LinkFileInterGeneratorPath( KDBaseConPtr a_spDBaseConnection, KIOColumn* a_pColumn, CString strFileName/*=_T("link.dat")*/ )
{
	TxLogDebugStartMsg();

	CString strSQL        = _T(" Select link_id, fnode_id, tnode_id, link_type, length, speed ") ;
	CString strColumnName = a_pColumn->Name();

	if(strColumnName.CompareNoCase(COLUMN_LINK_SPEED) != 0)
	{
		strSQL = strSQL + _T(", ") + a_pColumn->Name() + _T(" ");
	}
	else
	{
		//선택한 필드 값이 없을 경우에는 0을 Binary에써주고 DLL에서 Time을 계산함
		strSQL = strSQL + _T(", 0 as Tmp ");
	}

	strSQL = strSQL + _T(", alpha, beta, a,  b From Link Order By link_id ");

	ofstream ofs(strFileName, ios::binary);
	if (! ofs)
	{
		TxLogDebugException();
		return false;
	}

	KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

	try
	{
		TFixedLink_Generator fixedLink;
		while(spResultSet->Next())
		{
			//ofs.write(reinterpret_cast<char*>)
			fixedLink.link_id   = spResultSet->GetValueInt64 (0);
			fixedLink.fnode_id  = spResultSet->GetValueInt64 (1);
			fixedLink.tnode_id  = spResultSet->GetValueInt64 (2);
			fixedLink.link_type = spResultSet->GetValueInt   (3);
			fixedLink.length    = spResultSet->GetValueDouble(4);
			fixedLink.speed		= spResultSet->GetValueDouble(5);
			ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TFixedLink_Generator) );  

			int nIndex(6);
			if ( strColumnName.CompareNoCase(COLUMN_LINK_SPEED) != 0)
			{

				double dValue = spResultSet->GetValueDouble(6);
				nIndex++;
				ofs.write(reinterpret_cast<char*>( &dValue ), sizeof(double) ); 
			}
			else
			{
				double dValue = 0;
				ofs.write(reinterpret_cast<char*>( &dValue ), sizeof(double) );
			}
			double dAhpla(0), dbeta(0), da(0), db(0);
			dAhpla = spResultSet->GetValueDouble(7);
			nIndex++;
			dbeta  = spResultSet->GetValueDouble(8);
			nIndex++;
			da     = spResultSet->GetValueDouble(9);
			nIndex++;
			db     = spResultSet->GetValueDouble(10);
			nIndex++;
			ofs.write(reinterpret_cast<char*>( &dAhpla ), sizeof(double));
			ofs.write(reinterpret_cast<char*>( &dbeta ), sizeof(double));
			ofs.write(reinterpret_cast<char*>( &da ), sizeof(double));
			ofs.write(reinterpret_cast<char*>( &db ), sizeof(double));
		}
	}
	catch (...)
	{
		TxLogDebugException();
		ofs.close();
		return false;
	}

	ofs.close();

	TxLogDebugEndMsg();
	return true;
}

bool KBulkDBase2File::ExportDB2ZoneIDFile( KDBaseConPtr a_spDBaseConnection,CString strFileName /*= _T("zone.dat")*/ )
{
	TxLogDebugStartMsg();

	CString strSQL = _T(" Select Zone_ID  From zone Ord") ;

	ofstream ofs(strFileName, ios::binary);
	if (! ofs)
	{
		TxLogDebugException();
		return false;
	}

	KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

	try
	{
		while (spResultSet->Next())
		{
			Integer nxZoneID    = spResultSet->GetValueInt64 (0);
			ofs.write(reinterpret_cast<char*>( &nxZoneID ), sizeof(Integer) ); 
		}
	}
	catch (...)
	{
		TxLogDebugException();
		ofs.close();
		return false;
	}

	ofs.close();

	TxLogDebugEndMsg();
	return true;
}
