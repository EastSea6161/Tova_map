/**
 * @file TargetYear.cpp
 * @brief KTarget 
 * @author 
 * @date 2010.08.04
 * @remark
 */


#include "StdAfx.h"
#include "KmzApp.h"
#include "Target.h"
#include "ChildFrm.h"

#include "IOTableController.h"
#include "MainFrameWnd.h"
#include "PurposeManager.h"
#include "ModeManager.h"
#include "Scenario.h"
#include "TableOwnerTarget.h"
#include "KmzSystem.h"

#include "DBaseConnector.h"
#include "DBaseUpgrade.h"

KTarget::KTarget(void)
	:	KObject               (KEMObjectTypeTarget), 
		m_bEmptyData          (true), 
		m_nTargetYear         (0   ),
		m_strIODataFile       ( _T("") ), 
        m_pTableOwner         ( NULL ),
		m_pTables             ( NULL ),          
        m_pCodeManager        ( NULL ),
        m_pPurposeMgr         ( NULL ),
        m_pModeManager        ( NULL )
{
	// 2019.05.27 : Free 버전 관련 수정
	m_bUseFreeVersion = true;
}

KTarget::~KTarget(void)
{
    TxLogDebugStartMsg();
    TxLogDebug((LPCTSTR)m_strIODataFile);
	Clear();
    TxLogDebugEndMsg();
}


void KTarget::Clear()
{
    try
    {   
        QBicDelete (m_pTables);
        QBicDelete (m_pCodeManager);         
        QBicDelete (m_pPurposeMgr) ;         
        QBicDelete (m_pModeManager);         
        QBicDelete (m_pTableOwner) ;       

        ClearController();
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }
}

bool KTarget::CreateInitialObject( void )
{
    TxLogDebug((LPCTSTR)m_strIODataFile);

    if( m_strIODataFile.IsEmpty() )
    {
        return false;
    }
    
    Clear();	

    m_spDBaseConnection = KDBaseConPtr(new KSQLiteConnection(m_strIODataFile));

    // DB File Upgrade
    UpgradeDataBase();

    m_pCodeManager = new KCodeManager(m_spDBaseConnection);
    //m_pCodeManager->CodeGroupFile(m_strIODataFile);
    m_pCodeManager->LoadCodeGroups();

    m_pTableOwner  = new KTableOwnerTarget(this);
    OpenIOTables(m_pTableOwner);
    
	m_pPurposeMgr = new KPurposeManager();
	m_pPurposeMgr->LoadPurpose(m_spDBaseConnection);
    
	m_pModeManager = new KModeManager();
	m_pModeManager->LoadMode(m_spDBaseConnection);
        
	m_bEmptyData = false;
	return true;
}


bool KTarget::CreateOtherObject()
{
	if( m_strIODataFile.IsEmpty() )
	{
		return false;
	}

	Clear();	

	m_spDBaseConnection = KDBaseConPtr(new KSQLiteConnection(m_strIODataFile));

	m_pCodeManager = new KCodeManager(m_spDBaseConnection);
	//m_pCodeManager->CodeGroupFile(m_strIODataFile);
	m_pCodeManager->LoadCodeGroups();

	m_pTableOwner  = new KTableOwnerTarget(this);
	OpenIOTables(m_pTableOwner);

	m_pPurposeMgr = new KPurposeManager();
	m_pPurposeMgr->LoadPurpose(m_spDBaseConnection);

	m_pModeManager = new KModeManager();
	m_pModeManager->LoadMode(m_spDBaseConnection);

	m_bEmptyData = false;
	return true;
}


void KTarget::OpenIOTables( KIOTableOwner* pTableOwner )
{
    CString strSQL(_T(""));
    try
    {
        strSQL.Append(_T(" SELECT table_name, display_name, table_info, table_type, display_type "));
        strSQL.Append(_T(" FROM table_master "));
       // strSQL.Append(_T(" WHERE table_type != -9  AND display_type >= 0 "));
        strSQL.Append(_T(" ORDER BY display_order "));	        
        
        QBicDelete(m_pTables);
        m_pTables = new KIOTables(pTableOwner);


        
        KResultSetPtr spResultSet = m_spDBaseConnection->ExecuteQuery(strSQL);
        // spResultSet이 nullptr인지 확인
        if (spResultSet == nullptr) {
            // 쿼리 실행 실패 처리
            return;
        }
        while (spResultSet->Next())
        {
            CString strName    = spResultSet->GetValueString(0);
            CString strCaption = spResultSet->GetValueString(1);
            CString strInfo    = spResultSet->GetValueString(2);
            int   nTableType   = spResultSet->GetValueInt   (3);
            int   nDisplayType = spResultSet->GetValueInt   (4);

            KEMIOTableType emTableType;
            switch(nTableType)
            {
            case -9 : emTableType = KEMIOTableTypeManager; break;
            case -1 : emTableType = KEMIOTableTypeSystem;  break;
            case  2	: emTableType = KEMIOTableTypeODData;  break;
            case  1	:
            default : emTableType = KEMIOTableTypeUser;    break;
            }
                        
            KEMIOTableDisplayType emTableDisplayType;
            switch(nDisplayType)
            {
            case -1 : emTableDisplayType = KEMIOTableDisplayTypeSystemManager; break;
            case  0 : emTableDisplayType = KEMIOTableDisplayTypeNetwork;    break;
            case  1 : emTableDisplayType = KEMIOTableDisplayTypeMatrix;     break;
            case  2 : emTableDisplayType = KEMIOTableDisplayTypeParameter;  break;
            case  3 : emTableDisplayType = KEMIOTableDisplayTypeSED;        break;
            case  9 : emTableDisplayType = KEMIOTableDisplayTypeEtc;        break;
            default : emTableDisplayType = KEMIOTableDisplayTypeEtc;        break;
            }
                        
            KIOTable* pIOTable = m_pTables->CreateTable(strName);
            if(NULL != pIOTable)
            {
                pIOTable->Name            (strName);
                pIOTable->Caption         (strCaption);
                pIOTable->TableType       (emTableType);
                pIOTable->TableDisplayType(emTableDisplayType);
                OpenIOColumns(pIOTable);
            }
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
        throw 1;
    }
}

void KTarget::OpenIOColumns(KIOTable* pIOTable)
{
    try
    { 
        CString strSQL = _T("");
        strSQL.Append(_T("Select column_name, data_type, column_type, code_type, display_yn, freeze_yn, "));
        strSQL.Append(_T("       display_name, column_info, display_order, base_id, pk_yn, kcode_group_key "));
        strSQL.Append(_T("From   column_description "));
        strSQL.Append(_T("Where LOWER(table_name) = LOWER('"));
        strSQL.AppendFormat(_T("%s"), pIOTable->Name());
        strSQL.Append(_T("') "));
        strSQL.Append(_T(" And Use_Flag = 1 ORDER BY display_order "));

        KResultSetPtr spResultSet = m_spDBaseConnection->ExecuteQuery(strSQL);
        while(spResultSet->Next())
        {
            CString strColumnName = spResultSet->GetValueString(0);

            KEMIODataType   emDataType   = (KEMIODataType)  (spResultSet->GetValueInt(1));
            KEMIOColumnType emColumnType = (KEMIOColumnType)(spResultSet->GetValueInt(2));				    
            KEMIOCodeType   emCodeType   = (KEMIOCodeType)  (spResultSet->GetValueInt(3));		

            CString strDisplayYN = spResultSet->GetValueString(4);             
            bool    bDisplay     = false;
            if (_tcsicmp(strDisplayYN, _T("Y")) == 0) bDisplay = true;

            CString strFreezeYN = spResultSet->GetValueString(5);       
            bool    bFreeze = false;
            if (_tcsicmp(strFreezeYN, _T("Y")) == 0) bFreeze = true;

            CString strDisplayName = spResultSet->GetValueString(6);   
            CString strColumnInfo  = spResultSet->GetValueString(7); 

            int nDisplayOrder = spResultSet->GetValueInt(8);
            int nBaseID       = spResultSet->GetValueInt(9);

            CString strPKYN   = spResultSet->GetValueString(10);
            bool bPK  = false;
            if (_tcsicmp(strPKYN, _T("Y")) == 0) bPK = true;

            int nCodeGroup = spResultSet->GetValueInt(11);

            KIOColumn* pColumn = pIOTable->Columns()->AddNewColumn();
            pColumn->Name        (strColumnName );
            pColumn->DataType    (emDataType    );
            pColumn->ColumnType  (emColumnType  );
            pColumn->CodeType    (emCodeType    );
            pColumn->Visible     (bDisplay      );
            pColumn->FreezeColumn(bFreeze       );
            pColumn->Caption     (strDisplayName);
            pColumn->ColumnInfo  (strColumnInfo );
            pColumn->DisplayOrder(nDisplayOrder );
            pColumn->BaseID      (nBaseID       );
            pColumn->PrimaryKey  (bPK           );
            pColumn->CodeGroup   (nCodeGroup    );
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
        throw 1;
    }
}

bool KTarget::CopyFrom( const KTarget* pSource )
{
	Clear();
	m_spDBaseConnection = nullptr;

	if( CreateObjectDirectory() == false )
	{
		return false;
	}

	CString strIODataSource  = pSource->GetIODataFilePath();
	CString sTemp = pSource->GetLocation();
	
    m_strIODataFile =  m_strLocation + _T("\\");
	m_strIODataFile += IODATAFILE_NAME;
	
	if( ::CopyFile( strIODataSource, m_strIODataFile, FALSE ) == FALSE )
	{
		m_strIODataFile = _T("");
		return false;
	}

	//Include Model Result logic
	if (true)
	{
		//Target폴더 밑에 있는 MODEL 폴더를 복사함
		CString strFromPath;
		strFromPath.Format(_T("%s\\%s"), pSource->GetLocation(), DLL_MODEL_FOLDER_NAME);
		CString strToPath   = this->GetLocation();

		//Model 폴더가 존재할시에만 복사진행
// 		if (GetFileAttributes(strFromPath) != 0xFFFFFFFF)
// 			ImChampDir::CopyDirectoryWinSimpleProgress(strFromPath, strToPath);
	}

	if (theApp.IsFreeVersion() && (CheckCanOpenFreeVersion() == false))
	{
		m_bUseFreeVersion = false;
		return false;
	}

	m_bUseFreeVersion = true;
    return CreateInitialObject();
}


bool KTarget::OpenTarget( const CString& strFile )
{
	bool bOpen (false);
	bool bEmtpy(false);

	TCHAR   szValue[256] = {0};
	CString strValue;

	m_nTargetYear = ::GetPrivateProfileInt( m_strName, _T("TargetYear"), 0, strFile );
	memset( szValue, 0, sizeof(szValue) );
	::GetPrivateProfileString( m_strName, _T("IOData"), _T(""), szValue, sizeof(szValue), strFile );
	if( _tcslen( szValue ) > 0 )
	{
		m_strIODataFile =  m_strLocation + _T("\\");
		m_strIODataFile += IODATAFILE_NAME;
	}
	else
	{
		bEmtpy = true;
	}

	if(true == bEmtpy)
	{
		return false;
	}

	if (theApp.IsFreeVersion() && (CheckCanOpenFreeVersion() == false))
	{
		m_bUseFreeVersion = false;
		return false;
	}

	m_bUseFreeVersion = true;
    return CreateInitialObject();
}


void KTarget::SaveTargetInfo( const CString& strFile )
{
	CString strValue;

	strValue.Format( _T("%d"), m_nTargetYear );
	::WritePrivateProfileString( m_strName, _T("TargetYear"), strValue, strFile );
    	
	if( m_strIODataFile.IsEmpty() == false )
	{
		::WritePrivateProfileString( m_strName, _T("IOData"), IODATAFILE_NAME, strFile );
	}
}


void KTarget::RegisterController(KIOTableController* pController)
{
    std::list<KIOTableController*>::iterator iter = m_lstIOControllers.begin();
    std::list<KIOTableController*>::iterator end  = m_lstIOControllers.end();

    bool bExist(false);
    while (iter != end)
    {
        KIOTableController* pTemp = *iter;
        if (pTemp == pController)
        {
            bExist = true;
            break;
        }

        ++iter;
    }

    if (bExist == false)
    {
        CString strName = pController->Name();
        UnregisterController(strName);

        m_lstIOControllers.push_back(pController);
    }    
}


void KTarget::UnregisterController(CString a_strName)
{
    std::list<KIOTableController*>::iterator iter = m_lstIOControllers.begin();
    std::list<KIOTableController*>::iterator end  = m_lstIOControllers.end();

    size_t nxCount = m_lstIOControllers.size();

    while (iter != end)
    {
        KIOTableController* pTemp = *iter;
        if (_tcsicmp(pTemp->Name(), a_strName) == 0)
        {
            delete pTemp;
            m_lstIOControllers.erase(iter);
            break;
        }
        
        ++iter;
    }
}

void KTarget::ClearController()
{
    std::list<KIOTableController*>::iterator iter = m_lstIOControllers.begin();
    std::list<KIOTableController*>::iterator end  = m_lstIOControllers.end();

    while (iter != end)
    {
        KIOTableController* pTemp = *iter;
        delete pTemp;
        ++iter;
    }

    m_lstIOControllers.clear();
}

CString KTarget::Caption() const
{
    CString strScenaioName(_T("Unknown"));
    CString strCaption(_T(""));

    KObject* pScenario = GetParentObject();
    if ( pScenario != NULL )
    {
        strScenaioName = pScenario->GetName();
    }

    strCaption.Format( _T("%s - %s"), strScenaioName, GetName() );
    return strCaption;
}

CString KTarget::CaptionScenario()
{
    CString strScenarioName(_T("Unknown"));
    CString strCaption(_T(""));

    KObject* pScenario = GetParentObject();
    if ( pScenario != NULL )
    {
        strScenarioName = pScenario->GetName();
    }

    return strScenarioName;
}

CString KTarget::CaptionTargetYear()
{
    CString strTargetYear(GetName());

    return strTargetYear;
}

void KTarget::UpgradeDataBase()
{
    if (m_spDBaseConnection == nullptr)
        return;

    try
    {
		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}
    
		QBicSimpleProgressThread::ExecuteThread(UpgradeDataBaseThreadCaller, this, false, nLang);
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
    	TxLogDebugException();
        throw 1;
    }    
}

unsigned __stdcall KTarget::UpgradeDataBaseThreadCaller( void* p )
{
    try
    {
        QBicSimpleProgressParameter*   pParameter      = (QBicSimpleProgressParameter*)p;
        QBicSimpleProgress*            pProgressWindow = pParameter->GetProgressWindow();
        

		if (KmzSystem::GetLanguage() == KEMKorea) {
			pProgressWindow->SetStatus(_T("DB 버전 확인 중..."));
		}
		else {
			pProgressWindow->SetStatus(_T("Database Version Check"));
		}
        
    	KTarget* pTarget = (KTarget*)pParameter->GetParameter();
        pTarget->UpgradeDataBaseProcess();        
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
    	TxLogDebugException();
        throw 1;
    }

    return 0;
}

void KTarget::Vaccum()
{
    if (m_bEmptyData)
        return;

    if (m_spDBaseConnection == nullptr)
        return;

    try
    {
        KSQLiteConnection* pSQLiteConnection = (KSQLiteConnection*)m_spDBaseConnection.get();
        pSQLiteConnection->Vaccum();
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }    
}


void KTarget::UpgradeDataBaseProcess()
{
    if (m_spDBaseConnection == nullptr)
    {
        TxLogDebugException();
        return;
    }

	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();  
	KDBaseUpgrade::UpgradeIolocalProcess(m_spDBaseConnection, spProgressWindow);
    
    TxLogDebugVisitor();
}

void KTarget::CompactGISDB()
{	
}


//2019.05.27 : Free 버전 관련 수정
// 노드의 개수를 확인후 초과시 실패를 반환
bool KTarget::CheckCanOpenFreeVersion()
{
	if (m_strIODataFile.IsEmpty())
	{
		return false;
	}

	KDBaseConPtr spDBConn = KDBaseConPtr(new KSQLiteConnection(m_strIODataFile));

	int nNodeCount = 0;
	CString strSQL(_T(""));
	try
	{
		strSQL = _T("SELECT count(*) FROM node");
		KResultSetPtr spResultSet = spDBConn->ExecuteQuery(strSQL);
		if (spResultSet->Next())
		{
			nNodeCount = spResultSet->GetValueInt(0);
		}

		spDBConn->CloseConnection();
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}

	if (nNodeCount > CKmzApp::MAX_NODE_OF_FREEVERSION)
	{
		return false;
	}

	return true;
}


bool KTarget::IsUseFreeVersion()
{
	return m_bUseFreeVersion;
}


void KTarget::ResetUseFreeVersion()
{
	m_bUseFreeVersion = true;
}
