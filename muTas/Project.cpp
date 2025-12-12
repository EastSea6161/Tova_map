/**
 * @file Project.cpp
 * @brief KProject의 구현
 * @author 
 * @date 2010.06.15
 * @remark
 */


#include "StdAfx.h"
#include <algorithm>
#include <memory>
#include "Project.h"
//#include "CodeManager.h"
#include "kmz_common.h"
#include "AddScenarioDlg.h"
#include "ProjectUserNewDlg.h"
#include "RenameDlg.h"
#include "Scenario.h"
#include "ScenarioCopyDlg.h"
#include "Target.h"
#include "Purpose.h"
#include "Mode.h"
#include "KmzApp.h"
#include "KmzSystem.h"
#include "TProjectInfo.h"
#include "ImChampDir.h"
#include "DBaseUpgrade.h"
#include "DBaseConnector.h"
#include "../QBicCommon/QBicBinary.h"
#include "LayerDrawInfo.h"
#pragma warning(push)
#pragma warning(disable : 4996)

double KProject::m_dLinkOffest = -0.6;

/******************************************************************************
* KProject                                                                    *
******************************************************************************/


/**************************************
* Constructors and Destructor         *
**************************************/
KProject::KProject(void)
	:	KObject( KEMObjectTypeProject ), 
		m_nBaseYear(0), 
		m_pBaseScenario  (NULL),
		m_strInvestigator( _T("") ),
		m_strOrganization( _T("") ),
		m_strDescription ( _T("") )
{
    TxLogDebugVisitor();
}


KProject::~KProject(void)
{
    try
    {
    	QBicDelete(m_pBaseScenario);

        AutoType iter = m_mapScenario.begin();
        AutoType end  = m_mapScenario.end();
        
        while(iter != end)
        {
            KScenario* pScenario = iter->second;
                       QBicDelete(pScenario);
            ++iter;
        }

        m_mapScenario.clear();
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


/**************************************
* Operations                          *
**************************************/
bool KProject::CreateInitialObject( void )
{
    TxLogDebugStartMsg();

	if( CreateObjectDirectory() == false )
	{
		return false;
	}
	
	bool bModePassenger = KmzSystem::IsPassengerSystem();
	
    // Step1.
    CopyProjectDBase(bModePassenger);

	std::sort(m_vecTargetYear.begin(), m_vecTargetYear.end());
	
    TxLogDebugEndMsg();
	return true;
}


BOOL KProject::CopyProjectDBase( bool a_bModePassenger )
{
	CString strSysDomain(_T(""));
	CString strOrgProjectDbPath(_T(""));

	CString strApplicationDirectory = ImChampDir::GetAppDirectory();
    if (KmzSystem::GetLanguage() == KEMKorea) {
        strOrgProjectDbPath.Format(_T("%s\\Template\\%s"), strApplicationDirectory, DEFAULT_PROJECTFILE_NAME);
    }
    else {
        strOrgProjectDbPath.Format(_T("%s\\Template\\%s"), strApplicationDirectory, DEFAULT_PROJECTFILE_NAMEENG);
    }
	
	CString strDestProjectDbPath =  m_strLocation;
            strDestProjectDbPath.Format(_T("%s\\%s"), m_strLocation, PROJECTFILE_NAME);

	if( ::CopyFile( strOrgProjectDbPath, strDestProjectDbPath, FALSE ) == FALSE )
	{
		return FALSE;
	}

	return TRUE;
}

bool KProject::ModifyPassengerUserProject( TProjectInfo& a_projectInfo )
{
	KProjectUserNewDlg dlg;

	/// editing 모드로 dlg open
	dlg.m_bEdit = true;
	///////////////////////////// 
	/// Project 기본정보 입력
	dlg.m_strName = m_strName;
	dlg.m_strInvestigator = m_strInvestigator;
	dlg.m_strOrganization = m_strOrganization;
	dlg.m_strDescription = m_strDescription;
	dlg.m_strLocation = m_strLocation;

	/// base 시나리오 입력
	const KTDTargetMap& targetMap = m_pBaseScenario->GetTargetMap();
	if( targetMap.empty() == false )
	{
		dlg.m_nBaseYear = targetMap.begin()->second->GetTargetYear();
	}

	/// target year 입력
	if( m_vecTargetYear.empty() == false )
	{
		CString strTarget;
		std::vector< int >::iterator itTargetYear = m_vecTargetYear.begin();
		while( m_vecTargetYear.end() != itTargetYear )
		{
			strTarget.Format( _T("%d"), (*itTargetYear) );
			dlg.m_strTargetYear += strTarget;
			dlg.m_strTargetYear += _T(", ");
			++ itTargetYear;
		}
		dlg.m_strTargetYear.TrimRight( _T(", ") );
	}

	// Purpose 입력
	dlg.SetPurpose(m_vecPurpose);
	//Mode 입력
	dlg.SetMode(m_vecMode);

	if( dlg.DoModal() == IDOK )
	{
		a_projectInfo.strProjectName  = dlg.m_strName;
		a_projectInfo.strInvestigator = dlg.m_strInvestigator;
		a_projectInfo.strOrganization = dlg.m_strOrganization;
		a_projectInfo.strDescription  = dlg.m_strDescription;
		a_projectInfo.nBaseYear       = dlg.m_nBaseYear;
		a_projectInfo.strTargetYear   = dlg.m_strTargetYear;
		
		//Purpose
		a_projectInfo.bEditedPurpose  = dlg.m_bEditedPurpose;
		a_projectInfo.strPurposeName  = dlg.m_strPurposeName;
		a_projectInfo.strPurposeID    = dlg.m_strPurposeId;
		a_projectInfo.strPurposeGroup = dlg.m_strPurposeGroup;
		a_projectInfo.vecPurpose      = dlg.m_vecPurpose;

		//Mode
		a_projectInfo.bEditedMode  = dlg.m_bEditedMode;
		a_projectInfo.strModeName  = dlg.m_strModeName;
		a_projectInfo.strPurposeID = dlg.m_strModeId;
		a_projectInfo.strModeGroup = dlg.m_strModeGroup;
		a_projectInfo.vecMode      = dlg.m_vecMode;
		
		return true;
	}
	return false;
}

bool KProject::modifyProject( TProjectInfo& a_projectInfo )
{
	return ModifyPassengerUserProject(a_projectInfo);
}

bool KProject::OpenProject( const CString& strFile )
{
    TxLogDebugStartMsg();	

	TCHAR   szValue[2048] = {0};	
	CString pszApp = _T("Project");

	// 프로젝트 이름 :
	GetPrivateProfileString( pszApp, _T("Name"), _T(""), szValue, sizeof(szValue), strFile );
	SetName( szValue );

    // 조사자
	GetPrivateProfileString( pszApp, _T("Investigator"), _T(""), szValue, sizeof(szValue), strFile );
	SetInvestigator( szValue );

    // 조직
	GetPrivateProfileString( pszApp, _T("Organization"), _T(""), szValue, sizeof(szValue), strFile );
	SetOrganization( szValue );

    // 프로젝트 설명
	GetPrivateProfileString( pszApp, _T("Description"), _T(""), szValue, sizeof(szValue), strFile );
	SetDescription( szValue );

	// 기준년도
	m_nBaseYear = GetPrivateProfileInt( pszApp, _T("BaseYear"), 0, strFile );

    // Target Year
	GetPrivateProfileString( pszApp, _T("Targets"), _T(""), szValue, sizeof(szValue), strFile );
	std::vector< int > vecTarget;
	int nTargetCount = ::ParseTargetYear( szValue, vecTarget );
	SetTargetYear( vecTarget );

	// Purpose Name
	GetPrivateProfileString( pszApp, _T("Purpose"), _T(""), szValue, sizeof(szValue), strFile );
	std::vector<CString> vecPurposeName;
	int nPurposeCount = ::ParsePurposeName( szValue, vecPurposeName );
	SetPurposeName( vecPurposeName );

	// Purpose Id
	GetPrivateProfileString( pszApp, _T("PurposeId"), _T(""), szValue, sizeof(szValue), strFile);
	std::vector<int> vecPurposeId;
	int nPIdCount = ::ParsePurposeId( szValue, vecPurposeId );
	SetPurposeId( vecPurposeId );

	// Purpose Group
	int nPurposeGroup = GetPrivateProfileInt( pszApp, _T("PurposeGroup"), 0, strFile );
	SetPurposeGroup( nPurposeGroup );

	// Purpose
	std::vector<KPurpose*> vecPurpose;
	MakePurposeArr( vecPurposeName, vecPurposeId, nPurposeGroup, vecPurpose);
	SetPurpose( vecPurpose );

	// Mode Name
	GetPrivateProfileString( pszApp, _T("Mode"), _T(""), szValue, sizeof(szValue), strFile );
	std::vector<CString> vecModeName;
	int nModeCount = ::ParseModeName( szValue, vecModeName);
	SetModeName( vecModeName );

	// ModeId
	GetPrivateProfileString( pszApp, _T("ModeId"), _T(""), szValue, sizeof(szValue), strFile );
	std::vector<int> vecModeId;
	int nMIdCount = ::ParseModeId( szValue, vecModeId);
	SetModeId( vecModeId );

	//Mode Group
	int nModeGroup = GetPrivateProfileInt( pszApp, _T("ModeGroup"), 0, strFile);
	SetModeGroup( nModeGroup );

	// Mode
	std::vector<KMode*> vecMode;
	MakeModeArr( vecModeName, vecModeId, nModeGroup, vecMode);
	SetMode( vecMode );

	// Analysis Info - Id
	m_nAnalysisAreaId = GetPrivateProfileInt( pszApp, _T("AnalysisId"), 0, strFile );

	// Analysis Info - Name
	GetPrivateProfileString( pszApp, _T("AnalysisName"), _T(""), szValue, sizeof(szValue), strFile );
	SetAnalysisAreaName( szValue );

	// Analysis Info - folderName
	GetPrivateProfileString( pszApp, _T("AnalysisFolderName"), _T(""), szValue, sizeof(szValue), strFile );
	SetAnalysisFolderName( szValue );
    
	//-KEMDataUse
	GetPrivateProfileString( pszApp, _T("ProjectDataUse"), _T(""), szValue, sizeof(szValue), strFile );
	CString strDataUseKey = szValue;
	SetProjectType(KEMUserDefineProject);

	// Data Year
	bool bModePassenger = KmzSystem::IsPassengerSystem();
    
	/// create base scenario
	m_pBaseScenario = new KScenario;
	m_pBaseScenario->SetName( _T("BASE") );
	m_pBaseScenario->SetLocation( m_strLocation + _T("\\BASE") );
	m_pBaseScenario->SetParentObject( this );

	m_pBaseScenario->SetMode(vecMode);

	memset( szValue, 0, sizeof(szValue) );
	::GetPrivateProfileString( pszApp, _T("Scenario"), _T(""), szValue, sizeof(szValue), strFile );

	CString strValue = szValue;

	int nPos = 0;
	CString strScenario = strValue.Tokenize( _T(";"), nPos );
	while( strScenario != _T("") )
	{
		KScenario* pScenario = new KScenario;
		pScenario->SetName( strScenario );

		CString strKey;
		strKey.Format( _T("Scenario_%s"), strScenario );
		memset( szValue, 0, sizeof(szValue) );
		::GetPrivateProfileString( pszApp, strKey, _T(""), szValue, sizeof(szValue), strFile );

		CString strLocation = m_strLocation + _T("\\");
		if( _tcslen(szValue) > 0 )
		{
			strLocation += szValue;
		}
		else
		{
			strLocation += strScenario;
		}
		pScenario->SetLocation( strLocation );
		pScenario->SetParentObject( this );

		pScenario->SetMode(vecMode);

		m_mapScenario.insert( std::make_pair( pScenario->GetObjectID(), pScenario ) );

		strScenario = strValue.Tokenize( _T(";"), nPos );
	}
    
	bool bOpen = OpenScenario( strFile );

	UpgradeProjectDB();

    TxLogDebugEndMsg();
	return bOpen;
}


bool KProject::MakePurposeArr( std::vector<CString> &a_vecPurposeName, std::vector<int> &a_vecPurposeId, int a_nPurposeGroup, std::vector<KPurpose*> &a_vecPurpose)
{

	if( a_vecPurposeName.size() == 0 || a_vecPurposeId.size() == 0 )
	{
		return false;
	}

	if( a_vecPurposeName.size() != a_vecPurposeId.size() )
	{
		return false;
	}

	KEMPurposeGroup emPurposeGroup;
	KPurpose oPurpose;
	if( false == oPurpose.MatchPurposeGroup(a_nPurposeGroup, emPurposeGroup) )
	{
		return false;
	}

	std::vector<CString>::iterator iterName = a_vecPurposeName.begin();
	std::vector<int>::iterator	   iterId	= a_vecPurposeId.begin();
	while( a_vecPurposeName.end() != iterName)
	{

		KPurpose* pPurpose = new KPurpose(emPurposeGroup, *iterId, *iterName);
		
		a_vecPurpose.push_back(pPurpose);

		++iterName;
		++iterId;
	}

	return true;
}

bool KProject::MakeModeArr( std::vector<CString> &a_vecModeName, std::vector<int> &a_vecModeId, int a_nModeGroup, std::vector<KMode*> &a_vecMode )
{

	if( a_vecModeName.size() == 0 || a_vecModeId.size() == 0 )
	{
		return false;
	}

	if( a_vecModeName.size() != a_vecModeId.size() )
	{
		return false;
	}

	KEMModeGroup emModeGroup;
	KMode oMode;
	if( false == oMode.MatchModeGroup(a_nModeGroup, emModeGroup) )
	{
		return false;
	}

	std::vector<CString>::iterator iterName = a_vecModeName.begin();
	std::vector<int>::iterator	   iterId	= a_vecModeId.begin();
	while( a_vecModeName.end() != iterName )
	{
		KMode* pMode = new KMode(emModeGroup, *iterId, *iterName);

		a_vecMode.push_back(pMode);

		++iterName;
		++iterId;
	}

	return true;
}

bool KProject::OpenScenario( const CString& a_strFile )
{
    TxLogDebugStartMsg();
	bool bOpen = false;

	TCHAR szValue[256] = {0};

	/// Load base scenario
	if( NULL != m_pBaseScenario )
	{
		m_pBaseScenario->OpenScenario();
// 		const KTDTargetMap& targetMap = m_pBaseScenario->getTargetMap();
// 		KTDTargetMap::const_iterator it = targetMap.begin();
// 		if(it != targetMap.end())
// 		{
// 			g_pTarget = it->second;
// 		}
	}

	KTDScenarioMap::iterator itScenarioMap = m_mapScenario.begin();
	while( m_mapScenario.end() != itScenarioMap )
	{
		KScenario* pScenario = itScenarioMap->second;
		pScenario->OpenScenario( );
		++itScenarioMap;
	}

    TxLogDebugEndMsg();
	return true;
}

void KProject::SaveProject( const CString& strFile )
{
	CString pszApp = _T("Project");
	CString strValue;

	::WritePrivateProfileString( pszApp, _T("Name"), m_strName, strFile );
	::WritePrivateProfileString( pszApp, _T("Investigator"), m_strInvestigator, strFile );
	::WritePrivateProfileString( pszApp, _T("Organization"), m_strOrganization, strFile );
	::WritePrivateProfileString( pszApp, _T("Description"), m_strDescription, strFile );

	//- base year
	if(m_nBaseYear >0)
	{
		strValue.Format( _T("%d"), m_nBaseYear );
	}
	else 
	{
		strValue = _T("");
	}
	::WritePrivateProfileString( pszApp, _T("BaseYear"), strValue, strFile );

	//- target year
	strValue = _T("");
	std::vector< int >::iterator itTargets = m_vecTargetYear.begin();
	while( m_vecTargetYear.end() != itTargets )
	{
		CString strYear;
		strYear.Format( _T("%d"), *itTargets );
		strValue += strYear;
		strValue += _T(", ");
		++itTargets;
	}

	if( strValue.IsEmpty() == false )
	{
		strValue.Truncate( strValue.GetLength() - 2 );
		::WritePrivateProfileString( pszApp, _T("Targets"), strValue, strFile );
	}

	//- purpose name
	strValue = _T("");
	std::vector<CString>::iterator itPurposeName = m_vecPurposeName.begin();
	while( m_vecPurposeName.end() != itPurposeName )
	{
		CString strPurposeName;
		strPurposeName = *itPurposeName;
		strValue += strPurposeName;
		strValue += _T(", ");
		++itPurposeName;
	}

	if( strValue.IsEmpty() == false )
	{
		strValue.Truncate( strValue.GetLength() -2);
		::WritePrivateProfileString( pszApp, _T("Purpose"), strValue, strFile );
	}

	strValue			= _T("");
	CString strValue2	= _T("");
	CString strValue3	= _T("");
	std::vector<KPurpose*>::iterator itPurpose = m_vecPurpose.begin();
	while( m_vecPurpose.end() != itPurpose)
	{
		KPurpose* pPurpose = *itPurpose;
		CString strPurposeId;
		CString strPurposeGroup;

		strPurposeId.Format(_T("%d"),	 pPurpose->PurposeID());
		strPurposeGroup.Format(_T("%d"), pPurpose->PurposeGroup());

		strValue2 += strPurposeId;
		strValue2 += _T(", ");

		strValue3 = strPurposeGroup;

		++itPurpose;
	}

	if( strValue2.IsEmpty() == false )
	{
		strValue2.Truncate( strValue2.GetLength() -2);
		::WritePrivateProfileString( pszApp, _T("PurposeId"), strValue2, strFile);
	}

	if( strValue3.IsEmpty() == false )
	{
		::WritePrivateProfileString( pszApp, _T("PurposeGroup"), strValue3, strFile);
	}
	
	//- mode 
	strValue = _T("");
	std::vector<CString>::iterator itModeName = m_vecModeName.begin();
	while( m_vecModeName.end() != itModeName )
	{
		CString strMode;
		strMode = *itModeName;
		strValue += strMode;
		strValue += _T(", ");
		++itModeName;
	}

	if( strValue.IsEmpty() == false )
	{
		strValue.Truncate( strValue.GetLength() -2);
		::WritePrivateProfileString( pszApp, _T("Mode"), strValue, strFile );
	}

	strValue = _T("");
	strValue2 = _T("");
	strValue3 = _T("");
	std::vector<KMode*>::iterator itMode = m_vecMode.begin();
	while( m_vecMode.end() != itMode)
	{
		KMode* pMode = *itMode;
		CString strModeId;
		CString strModeGroup;

		strModeId.Format(_T("%d"),	  pMode->ModeID());
		strModeGroup.Format(_T("%d"), pMode->ModeGroup());

		strValue2 += strModeId;
		strValue2 += _T(", ");

		strValue3 = strModeGroup;

		++itMode;
	}

	if( strValue2.IsEmpty() == false )
	{
		strValue2.Truncate( strValue2.GetLength() -2);
		::WritePrivateProfileString( pszApp, _T("ModeId"), strValue2, strFile);
	}

	if( strValue3.IsEmpty() == false )
	{
		::WritePrivateProfileString( pszApp, _T("ModeGroup"), strValue3, strFile);
	}

	//- Analysis Info
	strValue = _T("");
	strValue.Format(_T("%d"), m_nAnalysisAreaId);
	::WritePrivateProfileString( pszApp, _T("AnalysisId"), strValue, strFile );
	::WritePrivateProfileString( pszApp, _T("AnalysisName"), m_strAnalysisAreaName, strFile );
	::WritePrivateProfileString( pszApp, _T("AnalysisFolderName"), m_strAnalysisFolderName, strFile );

	//-KEMDataUse
	CString strDataUse = _T("");
// 	if( m_emProjectType == KEMKTDBProject)
// 	{
// 		strDataUse = _T("1");
// 	}
// 	else
// 	{
		strDataUse = _T("2");
//	}
	::WritePrivateProfileString( pszApp, _T("ProjectDataUse"), strDataUse, strFile );
	
	strValue = _T("");
	const KTDTargetMap& targetMap = m_pBaseScenario->GetTargetMap();
	if( targetMap.empty() == false )
	{
		strValue.Format( _T("%d"), targetMap.begin()->second->GetTargetYear() );

		// 상단의 BaseYear 저장은, 프로젝트 생성 단계에서 설정 한 Year에 대한 저장이고, 여기 BaseYear 저장은 시나리오 설정 단계에서, addYear 한 Year에 대한 저장 이다.
		::WritePrivateProfileString( pszApp, _T("BaseYear"), strValue, strFile );
	}
	m_pBaseScenario->saveScenario();

	strValue = _T("");
	KTDScenarioMap::iterator itScenarioMap = m_mapScenario.begin();
	while( m_mapScenario.end() != itScenarioMap )
	{
		KScenario* pScenario = itScenarioMap->second;
		pScenario->saveScenario();

		CString strScenarioPath = pScenario->GetLocation();
		int nIndex = strScenarioPath.ReverseFind( _T('\\') );
		if( -1 != nIndex )
		{
			strScenarioPath = strScenarioPath.Right( strScenarioPath.GetLength() - nIndex );
			strScenarioPath.Trim( _T("\\") );
			CString strKey;
			strKey.Format( _T("Scenario_%s"), pScenario->GetName() );
			::WritePrivateProfileString( pszApp, strKey, strScenarioPath, strFile );
		}

		strValue += pScenario->GetName();
		strValue += _T(";");
		++itScenarioMap;
	}

	if( strValue.IsEmpty() == false )
	{
		strValue.TrimRight(_T(";"));
		::WritePrivateProfileString( pszApp, _T("Scenario"), strValue, strFile );
	}
}

int KProject::addScenario( void )
{
	int nCount = 0;
	KAddScenarioDlg dlg;
	dlg.m_strProjectName = m_strName;
	
	CString strYear;
	std::vector<int>::iterator itTargetYear = m_vecTargetYear.begin();
	while( m_vecTargetYear.end() != itTargetYear )
	{
		strYear.Format( _T("%d"), *itTargetYear );
		dlg.m_strTargetYear += strYear;
		
		++itTargetYear;
		if( m_vecTargetYear.end() != itTargetYear )
		{
			dlg.m_strTargetYear += _T(", ");
		}
	}

	KTDScenarioMap::iterator itScenarioMap = m_mapScenario.begin();
	std::map<CString, KScenario*> scenarioMap;
	
    size_t nMaxIndex = 0;

	while( m_mapScenario.end() != itScenarioMap )
	{
        nMaxIndex++;
		CString strName = itScenarioMap->second->GetName();
		dlg.m_ExistsNameSet.insert( strName );
		++itScenarioMap;
	}
	dlg.m_ExistsNameSet.insert(m_pBaseScenario->GetName());

    dlg.SetNameIndex(nMaxIndex + 1);

	if( dlg.DoModal() != IDOK )
	{
		//AfxMessageBox( _T("Cancel add scenario!") );
		return nCount;
	}
	
	std::vector< CString >::iterator it = dlg.m_arrScenario.begin();
	while( dlg.m_arrScenario.end() != it )
	{
		KScenario* pScenario = CreateScenario( *it );
		std::vector< int >::iterator itTargetYear = m_vecTargetYear.begin();
		while( m_vecTargetYear.end() != itTargetYear )
		{
			CString strTargetName;
			strTargetName.Format( _T("%d"), *itTargetYear );
						
			pScenario->SetMode   (m_vecMode);

			pScenario->CreateTarget( *itTargetYear, strTargetName );
			++itTargetYear;
		}

		++nCount;
		++it;
	}

	return nCount;
}

KScenario* KProject::CreateScenario( CString a_strScenarioName )
{
	CString strLocation;
	        strLocation =  m_strLocation + _T("\\");
	        strLocation += a_strScenarioName;

	KScenario* pNewScenario = new KScenario();
	           pNewScenario->SetName    ( a_strScenarioName );
	           pNewScenario->SetLocation( strLocation );
	  /*^^*/   pNewScenario->SetParentObject( this );

	if( pNewScenario->CreateInitialObject() == false )
	{
		delete pNewScenario;
		return nullptr;
	}
    	
    //Base 시나리오 구분방법:황당하게도 이름이 BASE
	if( pNewScenario->IsBaseScenario() == true )
	{
		m_pBaseScenario = pNewScenario;
	}
	else
	{
		m_mapScenario.insert( std::make_pair( pNewScenario->GetObjectID(), pNewScenario ) );
	}

	return pNewScenario;
}

KScenario* KProject::findScenario( unsigned int nID ) const
{
	KTDScenarioMap::const_iterator citScenarioMap = m_mapScenario.find( nID );
	if( m_mapScenario.end() != citScenarioMap )
	{
		return citScenarioMap->second;
	}
	return NULL;
}

void KProject::removeScenario( unsigned int nID )
{
	KTDScenarioMap::iterator itScenarioMap = m_mapScenario.find( nID );
	if( m_mapScenario.end() != itScenarioMap )
	{
		delete itScenarioMap->second;
		m_mapScenario.erase( itScenarioMap );
	}
}

void KProject::clearScenario( void )
{
	KTDScenarioMap::iterator itScenarioMap = m_mapScenario.begin();
	while( m_mapScenario.end() != itScenarioMap )
	{
		delete itScenarioMap->second;
		++itScenarioMap;
	}
	m_mapScenario.clear();
}

bool KProject::CopyScenario( KScenario* pSource )
{
	KScenarioCopyDlg dlg;
	dlg.m_pSourceScenario = pSource;

	KTDScenarioMap::iterator itScenarioMap = m_mapScenario.begin();
	while( m_mapScenario.end() != itScenarioMap )
	{
		if( itScenarioMap->second != pSource )
		{
			dlg.m_DestScenarioArray.push_back( itScenarioMap->second );
		}
		++itScenarioMap;
	}

	if( dlg.DoModal() != IDOK )
	{
		return false;
	}

	std::vector< KScenario* >::iterator itScenarioArray = dlg.m_DestScenarioArray.begin();
	if( dlg.m_DestScenarioArray.end() == itScenarioArray )
	{
		return false;
	}

	bool bDupl = false;

	itScenarioArray = dlg.m_DestScenarioArray.begin();

	while (!bDupl && dlg.m_DestScenarioArray.end() != itScenarioArray)
	{
		const KTDTargetMap& mapDestTarget = (*itScenarioArray)->GetTargetMap();

		KTDTargetMap::const_iterator bDestIter = mapDestTarget.begin();
		KTDTargetMap::const_iterator eDestIter = mapDestTarget.end();

		while (!bDupl && bDestIter != eDestIter)
		{
			KTarget* pTarget = bDestIter->second;
			int      nYear   = pTarget->GetTargetYear();

			const KTDTargetMap& mapSourceTarget = pSource->GetTargetMap();

			KTDTargetMap::const_iterator bSourceIter = mapSourceTarget.begin();
			KTDTargetMap::const_iterator eSourceIter = mapSourceTarget.end();

			while (!bDupl && bSourceIter != eSourceIter)
			{
				KTarget* pSourceTarget = bSourceIter->second;

				if (nYear == pSourceTarget->GetTargetYear())
					bDupl = true;

				++bSourceIter;
			}

			++bDestIter;
		}

		++itScenarioArray;
	}

	if (bDupl)
	{
		CString strMsg  = _T("해당 시나리오에 중복된 분석 년도가 존재합니다.\n시나리오 복사 실행 후 기존 데이터는 삭제됩니다.\n계속하시겠습니까?");
		int     nResult = AfxMessageBox(strMsg, MB_YESNO);

		if (nResult == IDNO)
		{
			AfxMessageBox(_T("시나리오 복사가 취소되었습니다."));
			return false;
		}
	}
		

	CWaitCursor wait;
	itScenarioArray = dlg.m_DestScenarioArray.begin();
	while( dlg.m_DestScenarioArray.end() != itScenarioArray )
	{
		(*itScenarioArray)->CopyFrom( pSource, false, dlg.m_bCopyResult );
		++itScenarioArray;
	}

	AfxMessageBox(_T("시나리오 복사를 완료하였습니다."));

	return true;
}

double KProject::GetLinkOffset()
{
    return m_dLinkOffest;
}

double KProject::GetLinkOffset(double dWidth)
{
    double offset = 0;

    if ( m_dLinkOffest <= 0)
    {        
        offset = m_dLinkOffest + -0.5 * dWidth;
    }
    else
    {
        offset = m_dLinkOffest + 0.5 * dWidth;
    }

    return offset;
}

void KProject::LoadTypeSymbol()
{
    CString strSQL(_T("")); {
        strSQL.Append(_T(" Select "));
        strSQL.Append(_T("  TLayerID, TTypeID, TTypeName, TTypeInfo "));
        strSQL.Append(_T(" From "));
        strSQL.Append(_T(" TLayerTemplate "));
    }

    KDBaseConPtr spDBase = KDBase::GetProjectDBConnection();
    if (spDBase == nullptr)
        return;

    try 
    {
        KResultSetPtr spResult = spDBase->ExecuteQuery(strSQL);
        while(spResult->Next()) {
            int TLayerID = spResult->GetValueInt(0);
            int TTypeID  = spResult->GetValueInt(1);
            CString TTypeName(_T("")); {
                TTypeName = spResult->GetValueString(2);
            }

            int nSize(0);
            const void * pValue  = spResult->GetValueBin(3, nSize);
            QBicBinary oBinary(nSize);  
            oBinary.PushBack(pValue, (int)nSize);

            if (TLayerID == 1) {
                KSymbolInfoPtr spSymbol = KSymbolInfoPtr(new KPointSymbolInfo());
                if (oBinary.Size() != 0) {
                    spSymbol->Read(oBinary);
                }

                std::map<int, std::shared_ptr<KSymbolInfo>>& mapTypeSymbol = m_mapKeyTypeSymbol[TLayerID];
                mapTypeSymbol[TTypeID] = spSymbol;
            }

            if (TLayerID == 2) {
                KSymbolInfoPtr spSymbol = KSymbolInfoPtr(new KLineSymbolInfo());
                if (oBinary.Size() != 0) {
                    spSymbol->Read(oBinary);
                }

                std::map<int, std::shared_ptr<KSymbolInfo>>& mapTypeSymbol = m_mapKeyTypeSymbol[TLayerID];
                mapTypeSymbol[TTypeID] = spSymbol;
            }

            if (TLayerID == 3) {
                KSymbolInfoPtr spSymbol = KSymbolInfoPtr(new KPolySymbolInfo());
                if (oBinary.Size() != 0) {
                    spSymbol->Read(oBinary);
                }

                std::map<int, std::shared_ptr<KSymbolInfo>>& mapTypeSymbol = m_mapKeyTypeSymbol[TLayerID];
                mapTypeSymbol[TTypeID] = spSymbol;
            }
        }
    } 
    catch (int& ex) {
    	CString strError(_T(""));
        strError.Format(_T("Error : %d"), ex);
        TxLogDebug((LPCTSTR)strError);
    } catch (KExceptionPtr ex) {
        TxExceptionPrint(ex);
    } catch (...) {
        TxLogDebugException();
    }
}

void KProject::SetLinkOffset(double offset)
{
    m_dLinkOffest = offset;
}

void KProject::SetSubJectAreaInfo(int a_nAreaId, CString a_strAreaName, CString a_strFolderName)
{	
}

void KProject::SetPurpose(std::vector<KPurpose*> a_vecPurpose)
{
	//기존 목적삭제
	for (size_t i = 0; i < m_vecPurpose.size(); ++i)
	{
		KPurpose* pPurpose = m_vecPurpose[i];

		delete pPurpose;
		pPurpose = nullptr;
	}
	m_vecPurpose.clear();

	m_vecPurpose = a_vecPurpose;
}

void KProject::UpgradeProjectDB()
{
	try
	{
		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}
		QBicSimpleProgressThread::ExecuteThread(UpgradeProjectDBThreadCaller, this, false, nLang);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

unsigned __stdcall KProject::UpgradeProjectDBThreadCaller( void* p )
{
	QBicSimpleProgressParameter*   pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*            pProgressWindow = pParameter->GetProgressWindow();
	
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("DB 버전 확인 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Database Version Check"));
	}
	KProject* pDlg = (KProject*)pParameter->GetParameter();
	pDlg->UpgradeProjectDBProcess();

	return 0;
}

void KProject::UpgradeProjectDBProcess()
{
	CString strProjectPath(_T(""));
	strProjectPath.Format(_T("%s\\project.db"),m_strLocation);

	KDBaseConPtr spDBaseConnection = KDBaseConPtr(new KSQLiteConnection(strProjectPath));
	if (nullptr == spDBaseConnection)
	{
		return;
	}



	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();  
	KDBaseUpgrade::UpgradeProjectProcess(this, spDBaseConnection, spProgressWindow);

	

	TxLogDebugVisitor();

}

void KProject::Vaccum()
{
    try
    {
		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		//IoLocal Data
        QBicSimpleProgressThread::ExecuteThread(VaccumThreadCaller, this, false, nLang);

		//Gis Data
		QBicDonkeyProgress oDonKeyProgress(NULL, _T("Compact GIS DataBase"), TRUE, FALSE, FALSE, nLang);
		CompactGISDB();

    }
    catch (KExceptionPtr ex)
    {
        TxLogDebug(ex->GetErrorMessage());
    }
    catch(...)
    {
        TxLogDebugException();
    }
}

unsigned __stdcall KProject::VaccumThreadCaller( void* p )
{
    QBicSimpleProgressParameter*   pParameter      = (QBicSimpleProgressParameter*)p;
    QBicSimpleProgress*            pProgressWindow = pParameter->GetProgressWindow();
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("DB 공간 회수 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Database Vaccum"));
	}
    KProject* pProject = (KProject*)pParameter->GetParameter();
    pProject->VaccumProcess();

    return 0;
}

void KProject::VaccumProcess()
{
    QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
    
    CString strMsg(_T(""));
    if (m_pBaseScenario)
    {
        strMsg.Format(_T("Vaccum - %s"), m_pBaseScenario->GetName());
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strMsg.Format(_T("회수 - %s"), m_pBaseScenario->GetName());
		}
        if(spProgressWindow)
        {
            spProgressWindow->SetStatus(strMsg);
        }

        m_pBaseScenario->Vaccum();
    }

    KTDScenarioMap::iterator itScenarioMap = m_mapScenario.begin();
    while( m_mapScenario.end() != itScenarioMap )
    {
        KScenario* pScenario = itScenarioMap->second;
        strMsg.Format(_T("Vaccum - %s"), pScenario->GetName());
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strMsg.Format(_T("회수 - %s"), pScenario->GetName());
		}
        if(spProgressWindow)
        {
            spProgressWindow->SetStatus(strMsg);
        }
        pScenario->Vaccum();
        ++itScenarioMap;
    }
}

void KProject::CompactGISDB()
{
	if (m_pBaseScenario != nullptr)
	{
		m_pBaseScenario->CompactGISDB();
	}

	KTDScenarioMap::iterator itScenarioMap = m_mapScenario.begin();
	while( m_mapScenario.end() != itScenarioMap )
	{
		KScenario* pScenario = itScenarioMap->second;
		pScenario->CompactGISDB();
		++itScenarioMap;
	}
}

#pragma warning(pop)
