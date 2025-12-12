#include "StdAfx.h"
#include "IOAssignRelationColumn.h"
#include "KmzSystem.h"
//^#include "DBaseConnector.h"


KIOAssignRelationColumn::KIOAssignRelationColumn(KDBaseConPtr spDBaseConnection, LPCTSTR strTableName)
{
	m_spDBaseConnection = spDBaseConnection;
	m_strTableName = strTableName;

	InitPassengerRelationInfo();

	InputAssignModelTableFieldName();
}


KIOAssignRelationColumn::~KIOAssignRelationColumn(void)
{
}


bool KIOAssignRelationColumn::IsAssignRelationTable()
{
	CString strUpperName = m_strTableName.MakeUpper();
	AutoType  iter = m_setRelationTable.find(strUpperName);
	if (iter != m_setRelationTable.end())
	{
		return true;
	}

	return false;
}


void KIOAssignRelationColumn::InitPassengerRelationInfo()
{
	m_setRelationTable.clear();

	CString strTableName(_T(""));

	strTableName = TABLE_LINK;
	m_setRelationTable.insert(strTableName.MakeUpper());

	strTableName = TABLE_PARAMETER_OD;
	m_setRelationTable.insert(strTableName.MakeUpper());

	strTableName = TABLE_TRANSIT;
	m_setRelationTable.insert(strTableName.MakeUpper());

	strTableName = TABLE_TRANSITNODES;
	m_setRelationTable.insert(strTableName.MakeUpper());

	strTableName = TABLE_MODE_OD;
	m_setRelationTable.insert(strTableName.MakeUpper());

	strTableName = TABLE_NODE;
	m_setRelationTable.insert(strTableName.MakeUpper());
}


void KIOAssignRelationColumn::InputAssignModelTableFieldName()
{
	m_mapAssignTableFieldName.clear();

	m_mapAssignTableFieldName.insert(std::make_pair(0,  TFieldRelationTable(COLUMN_ASSIGN_AVERAGE_TIME,         UpperTableName(TABLE_LINK))));
	m_mapAssignTableFieldName.insert(std::make_pair(1,  TFieldRelationTable(COLUMN_ASSIGN_VC,                   UpperTableName(TABLE_LINK))));
	m_mapAssignTableFieldName.insert(std::make_pair(2,  TFieldRelationTable(COLUMN_ASSIGN_SUM_VOLUME,           UpperTableName(TABLE_LINK))));
	m_mapAssignTableFieldName.insert(std::make_pair(3,  TFieldRelationTable(COLUMN_ASSIGN_TRIP,                 UpperTableName(TABLE_LINK))));
	m_mapAssignTableFieldName.insert(std::make_pair(4,  TFieldRelationTable(COLUMN_ASSIGN_MODE_VOLUME,          UpperTableName(TABLE_LINK))));
	m_mapAssignTableFieldName.insert(std::make_pair(5,  TFieldRelationTable(COLUMN_ASSIGN_VDF_TIME,             UpperTableName(TABLE_LINK))));
	m_mapAssignTableFieldName.insert(std::make_pair(6,  TFieldRelationTable(COLUMN_ASSIGN_AVE_TIME_TOLL,        UpperTableName(TABLE_LINK))));
	m_mapAssignTableFieldName.insert(std::make_pair(7,  TFieldRelationTable(COLUMN_ASSIGN_VDF_TOLL_TIME,        UpperTableName(TABLE_LINK))));
	m_mapAssignTableFieldName.insert(std::make_pair(8,  TFieldRelationTable(COLUMN_ASSIGN_BUS_INITIAL_VOLUME,   UpperTableName(TABLE_LINK))));
	m_mapAssignTableFieldName.insert(std::make_pair(9,  TFieldRelationTable(COLUMN_ASSIGN_OD_AVERAGE_TIME,      UpperTableName(TABLE_PARAMETER_OD))));
	m_mapAssignTableFieldName.insert(std::make_pair(10, TFieldRelationTable(COLUMN_ASSIGN_OD_VDF_TIME,          UpperTableName(TABLE_PARAMETER_OD))));
	m_mapAssignTableFieldName.insert(std::make_pair(11, TFieldRelationTable(COLUMN_ASSIGN_OD_AVERAGE_BUS,       UpperTableName(TABLE_PARAMETER_OD))));
	m_mapAssignTableFieldName.insert(std::make_pair(12, TFieldRelationTable(COLUMN_ASSIGN_OD_AVERAGE_SUBWAY,    UpperTableName(TABLE_PARAMETER_OD))));
	m_mapAssignTableFieldName.insert(std::make_pair(13, TFieldRelationTable(COLUMN_ASSIGN_OD_AVERAGE_BUSSUBWAY, UpperTableName(TABLE_PARAMETER_OD))));
	m_mapAssignTableFieldName.insert(std::make_pair(14, TFieldRelationTable(COLUMN_ASSIGN_OD_BUS_DIRECT,        UpperTableName(TABLE_PARAMETER_OD))));
	m_mapAssignTableFieldName.insert(std::make_pair(15, TFieldRelationTable(COLUMN_ASSIGN_OD_BUS_ONE_TRANSFER,  UpperTableName(TABLE_PARAMETER_OD))));
	m_mapAssignTableFieldName.insert(std::make_pair(16, TFieldRelationTable(COLUMN_ASSIGN_OD_BUS_TWO_TRANSFER,  UpperTableName(TABLE_PARAMETER_OD))));
	m_mapAssignTableFieldName.insert(std::make_pair(17, TFieldRelationTable(COLUMN_ASSIGN_LINFO_NUM_PASSENGERS, UpperTableName(TABLE_TRANSIT))));
	m_mapAssignTableFieldName.insert(std::make_pair(18, TFieldRelationTable(COLUMN_ASSIGN_OD_BUS,               UpperTableName(TABLE_MODE_OD))));
	m_mapAssignTableFieldName.insert(std::make_pair(19, TFieldRelationTable(COLUMN_ASSIGN_OD_RAIL,              UpperTableName(TABLE_MODE_OD))));
	m_mapAssignTableFieldName.insert(std::make_pair(20, TFieldRelationTable(COLUMN_ASSIGN_OD_BUSRAIL,           UpperTableName(TABLE_MODE_OD))));
	m_mapAssignTableFieldName.insert(std::make_pair(21, TFieldRelationTable(COLUMN_ASSIGN_LINE_BOARD_NET,       UpperTableName(TABLE_TRANSITNODES))));
	m_mapAssignTableFieldName.insert(std::make_pair(22, TFieldRelationTable(COLUMN_ASSIGN_LINE_BOARD_TRANSFER,  UpperTableName(TABLE_TRANSITNODES))));
	m_mapAssignTableFieldName.insert(std::make_pair(23, TFieldRelationTable(COLUMN_ASSIGN_LINE_BOARD_TOTAL,     UpperTableName(TABLE_TRANSITNODES))));
	m_mapAssignTableFieldName.insert(std::make_pair(24, TFieldRelationTable(COLUMN_ASSIGN_LINE_ALIGHT_NET,      UpperTableName(TABLE_TRANSITNODES))));
	m_mapAssignTableFieldName.insert(std::make_pair(25, TFieldRelationTable(COLUMN_ASSIGN_LINE_ALIGHT_TRANSFER, UpperTableName(TABLE_TRANSITNODES))));
	m_mapAssignTableFieldName.insert(std::make_pair(26, TFieldRelationTable(COLUMN_ASSIGN_LINE_ALIGHT_TOTAL,    UpperTableName(TABLE_TRANSITNODES))));
	m_mapAssignTableFieldName.insert(std::make_pair(27, TFieldRelationTable(COLUMN_ASSIGN_LINE_PASSENGER,       UpperTableName(TABLE_TRANSITNODES))));
	m_mapAssignTableFieldName.insert(std::make_pair(28, TFieldRelationTable(COLUMN_ASSIGN_NODE_BUS_NET_BOARD,      UpperTableName(TABLE_NODE))));
	m_mapAssignTableFieldName.insert(std::make_pair(29, TFieldRelationTable(COLUMN_ASSIGN_NODE_BUS_TRANSFER_BOARD, UpperTableName(TABLE_NODE))));
	m_mapAssignTableFieldName.insert(std::make_pair(30, TFieldRelationTable(COLUMN_ASSIGN_NODE_BUS_SUM_BOARD,      UpperTableName(TABLE_NODE))));
	m_mapAssignTableFieldName.insert(std::make_pair(31, TFieldRelationTable(COLUMN_ASSIGN_NODE_BUS_NET_ALIGHT,     UpperTableName(TABLE_NODE))));
	m_mapAssignTableFieldName.insert(std::make_pair(32, TFieldRelationTable(COLUMN_ASSIGN_NODE_BUS_TRANSFER_ALIGHT,UpperTableName(TABLE_NODE))));
	m_mapAssignTableFieldName.insert(std::make_pair(33, TFieldRelationTable(COLUMN_ASSIGN_NODE_BUS_SUM_ALIGHT,     UpperTableName(TABLE_NODE))));
}


CString KIOAssignRelationColumn::UpperTableName(CString a_strTableName)
{
	return a_strTableName.MakeUpper();
}


void KIOAssignRelationColumn::DeleteAssignColumn(CString a_strDeleteColumnName)
{
	try
	{
		PrepareBaseData();

		DeleteAssignPathInput(a_strDeleteColumnName);

		CString  strTableName = m_strTableName.MakeUpper();

		AutoType iter  = m_mapSeqTableColumnKey.begin();
		AutoType itEnd = m_mapSeqTableColumnKey.end();

		while(iter != itEnd)
		{
			int nSeq = iter->first;
			std::map<CString, std::map<CString, int>> &mapTableColumnKey = iter->second;
			
			AutoType iterB  = mapTableColumnKey.find(strTableName);
			AutoType itEndB = mapTableColumnKey.end();

			if (iterB == itEndB)
			{
				++iter;
				continue;
			}

			std::map<CString, int> &mapColumnKey = iterB->second;

			AutoType iterC  = mapColumnKey.find(a_strDeleteColumnName);
			AutoType itEndC = mapColumnKey.end();

			if(iterC == itEndC)
			{
				++iter;
				continue;
			}

			int nKey = iterC->second;

			mapColumnKey.erase(iterC);
			if (IsEmptyAllField(mapTableColumnKey) == true)
			{
				DeleteAssignResultRow(nSeq);
			}
			else
			{
				UpdateAssignResultRow(nSeq, nKey, a_strDeleteColumnName);
			}

			++iter;
		}
	}
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
        TxLogDebugException();
        throw -1;
    }
}


bool KIOAssignRelationColumn::IsEmptyAllField(std::map<CString, std::map<CString, int>> &a_mapTableColumnKey)
{

	AutoType iter  = a_mapTableColumnKey.begin();
	AutoType itEnd = a_mapTableColumnKey.end();

	while(iter != itEnd)
	{
		std::map<CString, int> &mapColumnKey = iter->second;

		if (mapColumnKey.size() > 0)
		{
			return false;
		}
		++iter;
	}

	return true;
}


void KIOAssignRelationColumn::PrepareBaseData()
{	
	CString strQuery(_T(" SELECT run_seq "));

	AutoType iter  = m_mapAssignTableFieldName.begin();
	AutoType itEnd = m_mapAssignTableFieldName.end();

	int nIdx = 0;
	while(iter != itEnd)
	{
		TFieldRelationTable &oTFieldRelationTable = iter->second;

		strQuery.Append(_T(", "));
		strQuery.Append(oTFieldRelationTable.strFieldName);

		++nIdx;
		++iter;
	}
	strQuery.AppendFormat(_T(" FROM %s "), TABLE_ASSIGN_MODEL);

	try
	{  
		m_mapSeqKeyColumnset.clear();
		m_mapSeqTableColumnKey.clear();

		KResultSetPtr spResultSet  = m_spDBaseConnection->ExecuteQuery(strQuery);
		while ( spResultSet->Next() )
		{
			int      nRunSeq = spResultSet->GetValueInt(0);
		
			AutoType iter    = m_mapAssignTableFieldName.begin();
			AutoType itEnd   = m_mapAssignTableFieldName.end();

			std::map<int, std::set<CString>>          mapKeyColumnset;
			std::map<CString, std::map<CString, int>> mapTableColumnKey;
			int nIdx = 0;
			while(iter != itEnd)
			{
				int                  nKey                 = iter->first;
				TFieldRelationTable &oTFieldRelationTable = iter->second;
				CString             &strRelationTableName = oTFieldRelationTable.strRelationTableName;

				CString strColumns = spResultSet->GetValueString(1+nIdx);
				std::vector<CString> vecColumn;
				std::set<CString>    setColumn;

				MultiStringToArray(strColumns, vecColumn);

				for (size_t i= 0; i< vecColumn.size(); i++)
				{
					CString &strColumn = vecColumn[i];
					setColumn.insert(strColumn);

					AutoType iterB  = mapTableColumnKey.find(strRelationTableName);
					AutoType itEndB = mapTableColumnKey.end();

					if (iterB == itEndB)
					{
						std::map<CString, int> mapColumnKey;
						mapColumnKey.insert(std::make_pair(strColumn, nKey));

						mapTableColumnKey.insert(std::make_pair(strRelationTableName, mapColumnKey));
					}
					else
					{
						std::map<CString, int> &mapColumnKey = iterB->second;
						mapColumnKey.insert(std::make_pair(strColumn, nKey));
					}
				}

				mapKeyColumnset.insert(std::make_pair(nKey, setColumn));
				
				++nIdx;
				++iter;
			}

			m_mapSeqKeyColumnset.insert(std::make_pair(nRunSeq, mapKeyColumnset));
			m_mapSeqTableColumnKey.insert(std::make_pair(nRunSeq, mapTableColumnKey));
		}
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	} 
}


void KIOAssignRelationColumn::MultiStringToArray(CString a_strMultiString, std::vector<CString> &a_vecStringValue)
{
	a_vecStringValue.clear();

	int     nPos(0);
	CString strTemp = a_strMultiString;
	strTemp.Trim();
	CString strValue = strTemp.Tokenize( MULTI_COLUMN_SEPARATOR, nPos );
	while( strValue != _T(""))
	{
		a_vecStringValue.push_back(strValue);

		strValue = a_strMultiString.Tokenize( MULTI_COLUMN_SEPARATOR, nPos );
	}
}


void KIOAssignRelationColumn::DeleteAssignResultRow(int a_nSeq)
{
	try
	{
		CString strSQL(_T(""));

		strSQL.Format(_T(" Delete From %s Where run_seq = %d "), TABLE_ASSIGN_MODEL, a_nSeq);
		m_spDBaseConnection->ExecuteUpdate(strSQL);		
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}


void KIOAssignRelationColumn::DeleteAssignPathInput(CString a_strDeleteColumnName)
{
	if (m_strTableName.CompareNoCase(TABLE_MODE_OD) != 0)
	{
		return;
	}

	try
	{
		CString strSQL(_T(""));

		strSQL.Format(_T(" Delete From %s Where InputColumnName = '%s' "), TABLE_ASSIGN_PATH_INPUT, a_strDeleteColumnName);
		m_spDBaseConnection->ExecuteUpdate(strSQL);		
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KIOAssignRelationColumn::UpdateAssignResultRow(int a_nSeq, int a_nKey, CString a_strDeleteColumnName)
{
	try
	{
		AutoType iter  = m_mapSeqKeyColumnset.find(a_nSeq);
		if (iter == m_mapSeqKeyColumnset.end())
		{
			return;
		}

		std::map<int, std::set<CString>> &mapKeyColumnset = iter->second;
		AutoType iterB  = mapKeyColumnset.find(a_nKey);
		if(iterB == mapKeyColumnset.end())
		{
			return;
		}

		std::set<CString> &setColumns = iterB->second;
		AutoType iterC  = setColumns.find(a_strDeleteColumnName);
		if (iterC == setColumns.end())
		{
			return;
		}
		setColumns.erase(iterC); //해당 컬럼 메모리 삭제

		int nFieldCnt(0);
		CString strFieldNames(_T(""));
		AutoType iterD  = mapKeyColumnset.begin();
		AutoType itEndD = mapKeyColumnset.end();
		while (iterD != itEndD)
		{
			int nKey                     = iterD->first;
			std::set<CString> setColumns = iterD->second;
			CString strColumns(_T(""));

			AutoType iterF  = setColumns.begin();
			AutoType itEndF = setColumns.end();
			
			int nIdx = 0;
			while (iterF != itEndF)
			{
				if( 0 != nIdx )
					strColumns.Append( MULTI_COLUMN_SEPARATOR );

				strColumns.Append(*iterF);

				++nIdx;
				++iterF;
			}

			AutoType iterG  = m_mapAssignTableFieldName.find(nKey);
			if (iterG != m_mapAssignTableFieldName.end())
			{
				if (nFieldCnt > 0)
				{
					strFieldNames.Append(_T(", "));
				}

				TFieldRelationTable &oTFieldRelationTable = iterG->second;
				CString &strFieldName = oTFieldRelationTable.strFieldName;

				if (strColumns.Compare(_T("")) == 0)
				{
					strFieldNames.AppendFormat(_T(" %s = '' "), strFieldName);
				}
				else
				{
					strFieldNames.AppendFormat(_T(" %s = '%s' "), strFieldName,  strColumns);
				}

				++nFieldCnt;
			}
			++iterD;
		}

		CString strSQL(_T(""));
		strSQL.Append(_T(" Update "));
		strSQL.Append(TABLE_ASSIGN_MODEL);
		strSQL.Append(_T(" Set "));
		strSQL.Append(strFieldNames);
		strSQL.AppendFormat(_T(" Where run_seq = %d "), a_nSeq);

		m_spDBaseConnection->ExecuteUpdate(strSQL);
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}