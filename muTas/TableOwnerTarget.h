#pragma once

struct TColumns 
{
	CString       strColumnName;
	KEMIODataType emDataType;
	bool          bIsPKColumn;
};

class KTarget;
class KIOTable;
class KIOColumns;
class KIOColumn;
class KIORow;
class KIORecordsetFactory;
class KIORecordset;

class KTableOwnerTarget : public KIOTableOwner
{
public:
	KTableOwnerTarget(KTarget* pTarget);
	virtual ~KTableOwnerTarget(void);
public :
	virtual KCodeManager* GetCodeManager(void);
	virtual KIOTables* Tables(void);	
	KTarget* GetTarget(void);
    virtual Integer  GetObjectID();
protected :
	KTarget* m_pTarget;

public:
    virtual KDBaseConPtr GetDBConnection(void);
public:
    virtual void BeginTransaction();
    virtual void Commit();
    virtual void Rollback();

private:
    void ExecuteUpdate(LPCTSTR strSQL);
	CString GetIndexQuery(LPCTSTR strSQL);
	void GetTableColumns(CString a_strTableName, std::vector<TColumns>& a_vecColumns);

private:
    CString MakeFieldsString(const KIOColumns* pColumns);
    CString MakeValuesString(const KIOColumns* pColumns, const KIORow* pRow);
    CString MakeSetString(const KIOColumns* pColumns, const KIORow* pRow);

public:
    virtual void AddColumnBeforeAction(KIOTable* a_pTable, KIOColumn& column, bool bDefault = false);
private:
    void AlterTableAddColumn          (KIOTable* a_pTable, KIOColumn& column, bool bDefault);
    void InsertColumnDescription      (KIOTable* a_pTable, KIOColumn &column);

public:
    virtual void RemoveColumnBeforeAction(KIOTable* a_pTable, LPCTSTR strColumnName);
private:
    void DeleteColumn(KIOTable* a_pTable, LPCTSTR a_strColumnName);
    void DeleteColumnDescription  (KIOTable* a_pTable, LPCTSTR strColumnName);
    void DeleteRelationTableColumn(KIOTable* a_pTable, LPCTSTR strColumnName);
    void DeleteAssignRelationTableColumn(KIOTable* a_pTable, LPCTSTR strColumnName);

public:
    virtual void UpdateColumnBeforeAction(KIOTable* a_pTable, KIOColumn& column);
private:
    void UpdateColumnDescription(KIOTable* a_pTable, KIOColumn& column);

public:
    virtual int TableRecordCount(KIOTable* a_pTable);
	virtual int TableRecordCount(KIOTable* a_pTable, CString strWhere);

public:
    virtual void UpdateRow( KIOTable* a_pTable, const KIOColumns* pColumns, const KIORow* pRow, LPCTSTR strFilter);
    virtual void InsertRow( KIOTable* a_pTable, const KIOColumns* pColumns, const KIORow* pRow);
    virtual void DeleteRow( KIOTable* a_pTable, LPCTSTR strFilter);
public:
    virtual KIORecordset* GetRecordset(KIOTable* a_pTable, KEMIORecordsetType emRecordsetType);
    virtual KIORecordset* GetRecordset(KIOTable* a_pTable, KEMIORecordsetType emRecordsetType, LPCTSTR strFilter, LPCTSTR strOrder);
    virtual KIORecordset* GetRecordset(KIOTable* a_pTable, KEMIORecordsetType emRecordsetType, const KIOColumns* pColumns, LPCTSTR strFilter, LPCTSTR strOrder);
    virtual KIORecordset* GetRecordset(KIOTable* a_pTable, KEMIORecordsetType emRecordsetType, const KIOColumns* pColumns, LPCTSTR strTables, LPCTSTR strFilter, LPCTSTR strOrder);
	virtual KIORecordset* GetRecordset(KIOTable* a_pTable, KEMIORecordsetType emRecordsetType, const KIOColumns* pColumns, LPCTSTR strTables, LPCTSTR strFilter, LPCTSTR strOrder, int nPageOffset, int nRecordCount);
};

