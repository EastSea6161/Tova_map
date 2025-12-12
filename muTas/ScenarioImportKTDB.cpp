#include "stdafx.h"
#include "Scenario.h"
#include "Project.h"
#include "Target.h"
#include "Purpose.h"
#include "Mode.h"
#include "ImChampDir.h"


//## start 보간 로직 작성 중 
bool KScenario::GetPurposeODColumnName( KDBaseConPtr a_spDBaseConnection, std::vector<KPurpose*> &a_vecPurpose, std::list<CString> &a_lstOldPurposeODColunName,  std::list<CString> &a_lstAddPurposeODColunName)
{
	if( a_vecPurpose.size() == 0 )
	{
		return false;
	}

	CString         strSQL (_T(""));

	KEMPurposeGroup emPurposeGroup;
	CString         strFilter(_T(""));
	CString         strId    (_T(""));

	int             nAllColumnCount(0);
	int             nOldColumnCount(0);
	int             nNewColumnCount(0);

	std::vector<KPurpose*>::iterator iter, itEnd = a_vecPurpose.end();
	for(iter= a_vecPurpose.begin(); iter != itEnd; ++iter)
	{
		KPurpose* pPurpose = *iter;

		emPurposeGroup = pPurpose->PurposeGroup();
		int nId = pPurpose->PurposeID();
		strId.Format(_T(" %d"), nId);

		if(iter != a_vecPurpose.begin())
		{
			strFilter += _T(", ");
		}

		strFilter += strId;
		nAllColumnCount++;
	}

	strSQL.Format(
		_T(" SELECT Detail_Object_ID, Purpose_OD_Column_Name FROM Distribution_Model ")
		_T(" WHERE Object_Group_Code = %d ")
		_T(" AND Detail_Object_ID IN (%s) ")
		_T(" ORDER BY Detail_Object_ID "), emPurposeGroup, strFilter
		);

	try
	{
		KResultSetPtr spResult = a_spDBaseConnection->ExecuteQuery(strSQL);

		a_lstOldPurposeODColunName.clear();
		while (spResult->Next())
		{
			int     nDetailObjectId        = spResult->GetValueInt   (0);
            CString strPurposeODColumnName = spResult->GetValueString(1);
			//TCHAR*  szValue                = spResult->GetValueString(1);
			//CString strPurposeODColumnName = szValue ? szValue : _T("");

			a_lstOldPurposeODColunName.push_back(strPurposeODColumnName);	// 기존 column은 Distribution_Model에 있지만,  사용자가 생성한 column은 Distribution_Model에 없다.

			nOldColumnCount++;
		}
	}
	catch ( ... )
	{
		a_lstOldPurposeODColunName.clear();
		return false;
	}

	nNewColumnCount = nAllColumnCount - nOldColumnCount;
	a_lstAddPurposeODColunName.clear();

	for(int i= 0; i< nNewColumnCount; i++)
	{
		a_lstAddPurposeODColunName.push_back(_T("none"));					// 사용자가 생성한 column은 여기서 생성
	}

	return true;
}


bool KScenario::GetCoefficientColumnName( KDBaseConPtr a_spDBaseConnection, std::vector<KPurpose*> &a_vecPurpose, std::list<CString> &lstOldCoefficientColunName,  std::list<CString> &a_lstAddCoefficientColunName)
{
	if( a_vecPurpose.size() == 0 )
	{
		return false;
	}

	CString strSQL (_T(""));

	KEMPurposeGroup emPurposeGroup;
	CString         strFilter(_T(""));
	CString         strId    (_T(""));

	int nAllColumnCount(0);
	int nOldColumnCount(0);
	int nNewColumnCount(0);

	std::vector<KPurpose*>::iterator iter, itEnd = a_vecPurpose.end();
	for(iter= a_vecPurpose.begin(); iter != itEnd; ++iter)
	{
		KPurpose* pPurpose = *iter;

		emPurposeGroup = pPurpose->PurposeGroup();
		int nId = pPurpose->PurposeID();
		strId.Format(_T(" %d"), nId);

		if(iter != a_vecPurpose.begin())
		{
			strFilter += _T(", ");
		}

		strFilter += strId;
		nAllColumnCount++;
	}

	strSQL.Format(
		_T(" SELECT Detail_Object_ID, Input_Coefficient_Column_Name FROM %s ")
		_T(" WHERE Object_Group_Code = %d ")
		_T(" AND Detail_Object_ID IN (%s) ")
		_T(" ORDER BY Detail_Object_ID "), TABLE_INPUT_COEFFICIENT_MODEL, emPurposeGroup, strFilter
		);

	try
	{
		KResultSetPtr spResult = a_spDBaseConnection->ExecuteQuery(strSQL);

		lstOldCoefficientColunName.clear();
		while(spResult->Next())
		{
			int    nDetailObjectId           = spResult->GetValueInt   (0);
            CString strCoefficientColumnName = spResult->GetValueString(1);
			//TCHAR* szValue                   = spResult->GetValueString(1);
			//CString strCoefficientColumnName = szValue ? szValue : _T("");

			lstOldCoefficientColunName.push_back(strCoefficientColumnName);	// 기존 column은 InputCoefficient_Model에 있지만,  사용자가 생성한 column은 InputCoefficient_Model에 없다.

			nOldColumnCount++;
		}
	}
	catch ( ... )
	{
		lstOldCoefficientColunName.clear();
		return false;
	}

	nNewColumnCount = nAllColumnCount - nOldColumnCount;
	a_lstAddCoefficientColunName.clear();

	for(int i= 0; i< nNewColumnCount; i++)
	{
		a_lstAddCoefficientColunName.push_back(_T("none"));					// 사용자가 생성한 column은 여기서 생성
	}

	return true;
}


bool KScenario::GetModeODColumnName( KDBaseConPtr a_spDBaseConnection, std::vector<KMode*> &a_vecMode, std::list<CString> &a_lstOldModeODColunName,  std::list<CString> &a_lstAddModeODColunName)
{
	if( a_vecMode.size() == 0 )
	{
		return false;
	}

	CString      strSQL (_T(""));

	KEMModeGroup emModeGroup;
	CString      strFilter(_T(""));
	CString      strId    (_T(""));

	int          nAllColumnCount(0);
	int          nOldColumnCount(0);
	int          nNewColumnCount(0);

	std::vector<KMode*>::iterator iter, itEnd = a_vecMode.end();
	for(iter= a_vecMode.begin(); iter != itEnd; ++iter)
	{
		KMode* pMode = *iter;

		emModeGroup = pMode->ModeGroup();
		int nId = pMode->ModeID();
		strId.Format(_T(" %d"), nId);

		if(iter != a_vecMode.begin())
		{
			strFilter += _T(", ");
		}

		strFilter += strId;
		nAllColumnCount++;
	}

	strSQL.Format(
		_T(" SELECT Detail_Mode_ID, Mode_OD_Column_Name FROM ModeChoice_Model ")
		_T(" WHERE Mode_Group_Code = %d ")
		_T(" AND Detail_Mode_ID IN (%s) ")
		_T(" ORDER BY Detail_Mode_ID "), emModeGroup, strFilter
		);

	try
	{
		KResultSetPtr spResult = a_spDBaseConnection->ExecuteQuery(strSQL);

		a_lstOldModeODColunName.clear();
		while(spResult->Next())
		{
			int    nDetailModeId        = spResult->GetValueInt(0);
            CString strModeODColumnName = spResult->GetValueString(1);
			//TCHAR* szValue              = spResult->GetValueString(1);
			//CString strModeODColumnName = szValue ? szValue : _T("");

			a_lstOldModeODColunName.push_back(strModeODColumnName);	// 기존 column은 modechoice_Model에 있지만,  사용자가 생성한 column은 modechoice_Model에 없다.

			nOldColumnCount++;
		}
	}
	catch ( ... )
	{
		a_lstOldModeODColunName.clear();
		return false;
	}

	nNewColumnCount = nAllColumnCount - nOldColumnCount;
	a_lstAddModeODColunName.clear();

	for(int i= 0; i< nNewColumnCount; i++)
	{
		a_lstAddModeODColunName.push_back(_T("none"));					// 사용자가 생성한 column은 여기서 생성
	}

	return true;
}


bool KScenario::DefinePurposeAndMode( CString& a_strIODataFile )
{
	KDBaseConPtr spDBaseConnection = KDBaseConPtr(new KSQLiteConnection(a_strIODataFile));

	if (nullptr == spDBaseConnection)
	{
		return false;
	}

	DefinePurpose( spDBaseConnection );
	DefineMode( spDBaseConnection );

	return true;
}


bool KScenario::DefinePurpose(KDBaseConPtr a_spDBaseConnection)
{
	TxLogDebugStartMsg();
	KProject*              pProject   = (KProject*)this->GetParentObject();
	std::vector<KPurpose*> vecPurpose = pProject->GetPurpose();

	if (vecPurpose.size() < 1) {
		return false;
	}

	std::vector<KPurpose*>::iterator iterP = vecPurpose.begin();
	KPurpose* pPurpose = *iterP;

	KEMPurposeGroup emPurposeGroup = pPurpose->PurposeGroup();
	CString strGroupName = ::GetPurposeGroupName(emPurposeGroup);

	CString strSQL_Group;
	strSQL_Group.Format( 
		_T(" INSERT Into %s (%s, %s) ") 
		_T(" Values (%d, '%s') "), 
		TABLE_OBJECT_GROUP, COLUMN_OBJECT_GROUP_CODE, COLUMN_OBJECT_GROUP_NAME,
		emPurposeGroup, strGroupName );

	CString strSQL_Detail;
	strSQL_Detail.Format( 
		_T(" INSERT Into %s (%s, %s, %s) ") 
		_T(" Values (?, ?, ?) "), 
		TABLE_DETAIL_OBJECT, COLUMN_OBJECT_GROUP_CODE, COLUMN_DETAIL_OBJECT_ID, COLUMN_DETAIL_OBJECT_NAME);

	try
	{
		a_spDBaseConnection->BeginTransaction();

		if (SQLITE_ERROR == a_spDBaseConnection->ExecuteUpdate(strSQL_Group))
		{
			TxLogDebug(strSQL_Group.AllocSysString());
			throw 1;
		}

		KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQL_Detail);

		while( vecPurpose.end() != iterP)
		{
			KPurpose* pPurpose = *iterP;

			spPrepareStatement->BindInt        ( 1, pPurpose->PurposeGroup()         );
			spPrepareStatement->BindInt        ( 2, pPurpose->PurposeID()            );
			spPrepareStatement->BindText       ( 3, (LPCTSTR)pPurpose->PurposeName() );

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			++iterP;
		}

		a_spDBaseConnection->Commit(); 
	}
	catch(...)
	{
		TxLogDebugException();
		a_spDBaseConnection->RollBack();
		return false;
	}

	TxLogDebugEndMsg();
	return true;
}


bool KScenario::DefineMode(KDBaseConPtr a_spDBaseConnection)
{
	TxLogDebugStartMsg();

	if (m_vecMode.size() < 1) {
		return false;
	}

	std::vector<KMode*>::iterator iterM = m_vecMode.begin();
	KMode* pMode = *iterM;

	KEMModeGroup emModeGroup = pMode->ModeGroup();
	CString strGroupName = ::GetModeGroupName(emModeGroup);

	CString strSQL_Group;
	strSQL_Group.Format( 
		_T(" INSERT Into %s (%s, %s) ") 
		_T(" Values (%d, '%s') "), 
		TABLE_MODE_GROUP, COLUMN_MODE_GROUP_CODE, COLUMN_MODE_GROUP_NAME,
		emModeGroup, strGroupName );

	CString strSQL_Detail;
	strSQL_Detail.Format( 
		_T(" INSERT Into %s (%s, %s, %s) ") 
		_T(" Values (?, ?, ?) "), 
		TABLE_DETAIL_MODE, COLUMN_MODE_GROUP_CODE, COLUMN_DETAIL_MODE_ID, COLUMN_DETAIL_MODE_NAME);

	try
	{
		a_spDBaseConnection->BeginTransaction();

		if (SQLITE_ERROR == a_spDBaseConnection->ExecuteUpdate(strSQL_Group))
		{
			TxLogDebug(strSQL_Group.AllocSysString());
			throw 1;
		}

		KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQL_Detail);

		while( m_vecMode.end() != iterM )
		{
			KMode* pMode = *iterM;

			spPrepareStatement->BindInt  ( 1, pMode->ModeGroup()         );
			spPrepareStatement->BindInt  ( 2, pMode->ModeID()            );
			spPrepareStatement->BindText ( 3, (LPCTSTR)pMode->ModeName() );

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			++iterM;
		}

		a_spDBaseConnection->Commit(); 
	}
	catch(...)
	{
		a_spDBaseConnection->RollBack();
		return false;
	}

	TxLogDebugEndMsg();
	return true;
}


bool KScenario::CreateEmptyTargetData( CString&	a_strNetworkDataFile, CString& a_strIODataFile, KTarget* pTarget )
{
	if( pTarget->CreateObjectDirectory() == false )
	{
		return false;
	}

	CString strBlankFile = _T(""); {
		CString strApplicationDirectory = ImChampDir::GetAppDirectory();
        if (KmzSystem::GetLanguage() == KEMKorea) {
            strBlankFile.Format(_T("%s\\Template\\%s"), strApplicationDirectory, DEFAULT_IODATAFILE_NAME);
        }
        else {
            strBlankFile.Format(_T("%s\\Template\\%s"), strApplicationDirectory, DEFAULT_IODATAFILE_NAMEENG);
        }		
	}

	a_strIODataFile = _T(""); {
		CString strLocation  = pTarget->GetLocation();
		a_strIODataFile		 = strLocation + _T("\\") + IODATAFILE_NAME;
	}

	if( ::CopyFile( strBlankFile, a_strIODataFile, FALSE ) == FALSE ) {
		return false;
	}

	KProject*              pProject   = (KProject*)this->GetParentObject();
	std::vector<KPurpose*> vecPurpose = pProject->GetPurpose();

	if( false == InsertNewUserPurposeOD(a_strIODataFile, vecPurpose) ||
		false == InsertNewUserModeOD   (a_strIODataFile, m_vecMode)) {
		return false;
	}
    	
	return true;
}


void KScenario::GetVectorDataYear(std::map<int, int> &a_mapDataYear, std::vector<int> &a_vecDataYear)
{
	std::map<int, int>::iterator itData, itEnd = a_mapDataYear.end();

	for(itData=a_mapDataYear.begin(); itData != itEnd; ++itData)
	{
		int nYear = itData->second;	
		a_vecDataYear.push_back(nYear);
	}
}


bool KScenario::CopyNetworkData(CString &a_strSysBaseDomain, CString &a_strBaseCopyYear, CString &a_strNetworkDataFile)
{
	
	return true;
}

bool KScenario::InsertNewUserPurposeOD( CString& a_strIODataFile, std::vector<KPurpose*>& a_vecPurpose )
{
	KDBaseConPtr spDBaseConnection = KDBaseConPtr(new KSQLiteConnection(a_strIODataFile));
	if (nullptr == spDBaseConnection)
	{
		return false;
	}

	// add purpose_od column_name
	std::list<CString> lstNewPurposeODColumnName;
	std::list<CString> lstNewPurposeODDisplayName;

	std::vector<KPurpose*>::iterator iter =  a_vecPurpose.begin();
	std::vector<KPurpose*>::iterator end  =  a_vecPurpose.end();

	CString strColName = _T("");
	int     nIdx       = 1;
	while( iter != end )
	{
		KPurpose *pPurpose = *iter;

		strColName.Format(_T("p%d"), nIdx );

		if(false == InsertNewODColumn( spDBaseConnection, TABLE_PURPOSE_OD, strColName ))
		{
			return false;
		}
		lstNewPurposeODColumnName.push_back(strColName);
		lstNewPurposeODDisplayName.push_back(pPurpose->PurposeName());

		++nIdx;
		++iter;
	}

	// add Column_Description
	if( false == InsertNewColumnDescription(spDBaseConnection, TABLE_PURPOSE_OD, lstNewPurposeODColumnName, lstNewPurposeODDisplayName) )
	{
		return false;
	}

	// add Distribution_Model
	if( false == InsertNewDistributionModel(spDBaseConnection, lstNewPurposeODColumnName, a_vecPurpose) )
	{
		return false;
	}

	return true;
}

bool KScenario::InsertNewUserModeOD( CString& a_strIODataFile, std::vector<KMode*>& a_vecMode )
{
	KDBaseConPtr spDBaseConnection = KDBaseConPtr(new KSQLiteConnection(a_strIODataFile));
	if (nullptr == spDBaseConnection)
	{
		return false;
	}

	// add mode_od column_name
	std::list<CString> lstNewModeODColumnName;
	std::list<CString> lstNewModeODDisplayName;

	std::vector<KMode*>::iterator iter =  a_vecMode.begin();
	std::vector<KMode*>::iterator end  =  a_vecMode.end();

	CString strColName = _T("");                           
	int     nIdx       = 1;
	while( iter != end )
	{
		KMode* pMode = *iter;

		strColName.Format(_T("m%d"), nIdx );

		if(false == InsertNewODColumn( spDBaseConnection, TABLE_MODE_OD, strColName ))
		{
			return false;
		}
		lstNewModeODColumnName.push_back(strColName);
		lstNewModeODDisplayName.push_back(pMode->ModeName());

		++nIdx;
		++iter;
	}

	// add Column_Description
	if( false == InsertNewColumnDescription(spDBaseConnection, TABLE_MODE_OD, lstNewModeODColumnName, lstNewModeODDisplayName) )
	{
		return false;
	}

	// add Modechoice_Model
	if( false == InsertNewModeChoiceModel(spDBaseConnection, lstNewModeODColumnName, a_vecMode) )
	{
		return false;
	}

	return true;
}


bool KScenario::InsertNewDistributionModel( KDBaseConPtr a_spDBaseConnection, std::list<CString>& a_lstPurposeODColumnName, std::vector<KPurpose*>& a_vecPurpose )
{
	CString strSQL(_T(""));

	strSQL = _T(" Insert Into Distribution_Model ");
	strSQL = strSQL + _T(" ( Object_Group_Code, Detail_Object_ID, Purpose_OD_Column_Name, Start_Date, End_Date )");
	strSQL = strSQL + _T(" Values ( ?, ?, ?, ?, ? )");

	try
	{        
		a_spDBaseConnection->BeginTransaction();

		KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQL);

		// 입력 레코드
		std::list<CString>::iterator iterName = a_lstPurposeODColumnName.begin();	
		
		std::vector<KPurpose*>::iterator iter = a_vecPurpose.begin();
		std::vector<KPurpose*>::iterator end  = a_vecPurpose.end();

		while(iter != end)
		{
			KPurpose* pPurpose = *iter;

			CString strColName		= *iterName;

			int nObjectGroupCode    = pPurpose->PurposeGroup();
			int nDetailObjectId     = pPurpose->PurposeID();

			CTime timeStart = CTime::GetCurrentTime();
			CTime timeEnd   = CTime::GetCurrentTime();

			CString strStart = timeStart.Format(_T("%Y-%m-%d %H:%M:%S"));
			CString strEnd   = timeEnd.Format(_T("%Y-%m-%d %H:%M:%S"));

			spPrepareStatement->BindInt (1, nObjectGroupCode);
			spPrepareStatement->BindInt (2, nDetailObjectId);
			spPrepareStatement->BindText(3, (LPCTSTR)strColName);
			spPrepareStatement->BindText(4, (LPCTSTR)strStart);
			spPrepareStatement->BindText(5, (LPCTSTR)strEnd);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			++iterName;
			++iter;
		}    

		a_spDBaseConnection->Commit();
	}
	catch(...)
	{
		a_spDBaseConnection->RollBack();
		return false;
	}

	return true;
}


bool KScenario::InsertNewModeChoiceModel( KDBaseConPtr a_spDBaseConnection, std::list<CString>& a_lstModeODColumnName, std::vector<KMode*>& a_vecMode )
{
	CString strSQL = _T("");

	strSQL = _T(" Insert Into modechoice_Model ");
	strSQL = strSQL + _T(" ( Mode_Group_Code, Detail_Mode_ID, Mode_OD_Column_Name, Start_Date, End_Date )");
	strSQL = strSQL + _T(" Values ( ?, ?, ?, ?, ? )");

	try
	{        
		a_spDBaseConnection->BeginTransaction();

		KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQL);

		// 입력 레코드
		std::list<CString>::iterator iterName = a_lstModeODColumnName.begin();	

		std::vector<KMode*>::iterator iter = a_vecMode.begin();
		std::vector<KMode*>::iterator end  = a_vecMode.end();

		while(iter != end)
		{
			KMode* pMode = *iter;

			CString strColName		= *iterName;

			int nModeGroupCode		= pMode->ModeGroup();
			int nDetailModeId		= pMode->ModeID();

			CTime timeStart = CTime::GetCurrentTime();
			CTime timeEnd   = CTime::GetCurrentTime();

			CString strStart = timeStart.Format(_T("%Y-%m-%d %H:%M:%S"));
			CString strEnd   = timeEnd.Format(_T("%Y-%m-%d %H:%M:%S"));

			spPrepareStatement->BindInt (1,  nModeGroupCode);
			spPrepareStatement->BindInt (2,  nDetailModeId);
			spPrepareStatement->BindText(3,  (LPCTSTR)strColName);
			spPrepareStatement->BindText(4,  (LPCTSTR)strStart);
			spPrepareStatement->BindText(5,  (LPCTSTR)strEnd);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			++iterName;
			++iter;
		}
		a_spDBaseConnection->Commit();
	}
	catch(...)
	{
		a_spDBaseConnection->RollBack();
		return false;
	}

	return true;
}


bool KScenario::InsertNewCoefficientModel( KDBaseConPtr a_spDBaseConnection, std::list<CString>& a_lstCoefficientColumnName, std::vector<KPurpose*>& a_vecPurpose )
{
	// DB Insert
	CString strSQL (_T(""));

	strSQL = _T(" Insert Into InputCoefficient_Model ");
	strSQL = strSQL + _T(" ( Object_Group_Code, Detail_Object_ID, Input_Coefficient_Column_Name )");
	strSQL = strSQL + _T(" Values ( ?, ?, ? )");

	try
	{        
		a_spDBaseConnection->BeginTransaction();

		KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQL);

		// 입력 레코드
		std::list<CString>::iterator iterName = a_lstCoefficientColumnName.begin();	

		std::vector<KPurpose*>::iterator iter = a_vecPurpose.begin();
		std::vector<KPurpose*>::iterator end  = a_vecPurpose.end();

		while(iter != end)
		{
			KPurpose* pPurpose = *iter;

			CString strColName		= *iterName;

			int nObjectGroupCode    = pPurpose->PurposeGroup();
			int nDetailObjectId     = pPurpose->PurposeID();

			spPrepareStatement->BindInt (1, nObjectGroupCode);
			spPrepareStatement->BindInt (2, nDetailObjectId);
			spPrepareStatement->BindText(3, (LPCTSTR)strColName);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			++iterName;
			++iter;
		}    
		
		a_spDBaseConnection->Commit();
	}
	catch(...)
	{
		a_spDBaseConnection->RollBack();
		return false;
	}

	return true;
}


bool KScenario::InsertNewColumnDescription( KDBaseConPtr a_spDBaseConnection, CString a_strTableName, std::list<CString>& a_lstODColumnName, std::list<CString>& a_lstODDisplayName )
{
	CString strSQL(_T(""));

	strSQL = _T(" Insert Into Column_Description ");
	strSQL = strSQL + _T(" ( table_name, column_name, data_type, column_type, code_type, display_yn, freeze_yn, display_name, column_info, display_order, pk_yn )");
	strSQL = strSQL + _T(" Values ( ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ? )");

	try
	{  
		a_spDBaseConnection->BeginTransaction();

		KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQL);

		// 입력 레코드
		std::list<CString>::iterator iterName = a_lstODColumnName.begin();	
		
		std::list<CString>::iterator iter = a_lstODDisplayName.begin();
		std::list<CString>::iterator end  = a_lstODDisplayName.end();

		int nIdx = 3;
		while(iter != end)
		{
			CString strColName		= *iterName;
			CString strDisplayName  = *iter;

			spPrepareStatement->BindText(1,  (LPCTSTR)a_strTableName);
			spPrepareStatement->BindText(2,  (LPCTSTR)strColName);
			spPrepareStatement->BindInt (3,  2);
			spPrepareStatement->BindInt (4,  0);
			spPrepareStatement->BindInt (5, -1);
			spPrepareStatement->BindText(6,  _T("Y"));
			spPrepareStatement->BindText(7,  _T("N"));
			spPrepareStatement->BindText(8,  (LPCTSTR)strDisplayName);
			spPrepareStatement->BindText(9,  (LPCTSTR)strDisplayName);
			spPrepareStatement->BindInt (10, nIdx++);
			spPrepareStatement->BindText(11, _T("N"));

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			++iterName;
			++iter;
		}    
		
		a_spDBaseConnection->Commit();
	}
	catch(...)
	{
		a_spDBaseConnection->RollBack();
		return false;
	}

	return true;
}


bool KScenario::InsertNewODColumn( KDBaseConPtr a_spDBaseConnection, CString a_strTableName, CString& a_strColumnName )
{
	CString strSQL = _T(" ALTER TABLE ");
	strSQL += a_strTableName;
	strSQL += _T(" ADD COLUMN ");

	strSQL += ( _T(" \"")+a_strColumnName +_T("\" ") );
	strSQL += DATATYPESTRING_DOUBLE;

	try
	{
		a_spDBaseConnection->ExecuteUpdate(strSQL);
	}
	catch(...)
	{
		return false;
	}

	return true;
}


Double KScenario::GetTargetRecord( Double a_dFirstYear, Double a_dFirstRecord, Double a_dSecondYear, Double a_dSecondRecord, Double a_dTargetYear )
{
	Double dSlope		 = 0.0;
	Double dIntercept	 = 0.0;
	Double dTargetRecord = 0.0;

	double dValue = a_dSecondYear - a_dFirstYear;
	if (dValue == 0)
	{
		return a_dFirstRecord;
	}
	
	try
	{
		dSlope		  = (a_dSecondRecord - a_dFirstRecord) /
			            (a_dSecondYear   - a_dFirstYear);
		dTargetRecord = a_dFirstRecord + dSlope * (a_dTargetYear-a_dFirstYear);	
	}
	catch(...)
	{	
		TxLogDebugException();
	}	

	return dTargetRecord;
}


