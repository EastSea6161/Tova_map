#pragma once

struct TFieldRelationTable
{
public:
	TFieldRelationTable(CString a_strFieldName, CString a_strRelationTableName)
	{
		strFieldName         = a_strFieldName;
		strRelationTableName = a_strRelationTableName;
	};
	TFieldRelationTable(){};

	CString strFieldName;
	CString strRelationTableName;
};

class KIOAssignRelationColumn
{
public:
	KIOAssignRelationColumn(KDBaseConPtr spDBaseConnection, LPCTSTR strTableName);
	~KIOAssignRelationColumn(void);

public:
	bool IsAssignRelationTable();
	void DeleteAssignColumn(CString a_strDeleteColumnName);

private:
	void InitPassengerRelationInfo();
	void InputAssignModelTableFieldName();
	CString UpperTableName(CString a_strTableName);
	void PrepareBaseData();
	void MultiStringToArray(CString a_strMultiString, std::vector<CString> &a_vecStringValue);
	void test2();
	void DeleteAssignResultRow(int a_nSeq);
	void UpdateAssignResultRow(int a_nSeq, int a_nKey, CString a_strDeleteColumnName);
	void DeleteAssignPathInput(CString a_strDeleteColumnName);

	bool IsEmptyAllField(std::map<CString, std::map<CString, int>> &a_mapTableColumnKey);

private:
	KDBaseConPtr  m_spDBaseConnection;
	CString              m_strTableName;

	std::set< CString >  m_setRelationTable;
	std::map<int/*key*/, TFieldRelationTable> m_mapAssignTableFieldName;
	std::map<int/*Seq*/, std::map<int/*key*/, std::set<CString/*column*/>>> m_mapSeqKeyColumnset;
	std::map<int/*Seq*/, std::map<CString/*table*/, std::map<CString/*column*/, int/*key*/>>> m_mapSeqTableColumnKey;
};