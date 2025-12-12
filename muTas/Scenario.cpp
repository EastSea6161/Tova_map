/**
 * @file Scenario.cpp
 * @brief KScenario 구현
 * @author 
 * @date 2010.08.10
 * @remark
 */


#include "StdAfx.h"
#include <memory>
#include "Scenario.h"
#include "Project.h"
#include "Target.h"
#include "Purpose.h"
#include "Mode.h"
#include "MainFrameWnd.h"
#include "ProjectExplorerWnd.h"
#include "KmzSystem.h"
#include "model_common.h"
#include "ScenarioAddYearDlg.h"
#include "ImChampDir.h"

#include "KBulkDBase.h"
#include "ImChampFrameWindow.h"
#pragma warning(push)
#pragma warning(disable:4996)

KScenario::KScenario(void)
	:	KObject( KEMObjectTypeScenario ),
		m_hKSX( NULL ),
		m_bOpen(false)
{
}

KScenario::~KScenario(void)
{
    TxLogDebugStartMsg();
	if(NULL != m_hKSX)
	{
		fclose(m_hKSX);
		m_hKSX = NULL;
	}

	ClearTarget();

    TxLogDebugEndMsg();
}

bool KScenario::CreateInitialObject(void)
{
	if( CreateObjectDirectory() == false )
	{
		return false;
	}

	// create Scenario property file
	CString strFile;
	strFile.Format( _T("%s\\%s.ksx"), m_strLocation, m_strName );
	if(NULL != m_hKSX)
	{
		fclose(m_hKSX);
		m_hKSX = NULL;
	}
	m_hKSX = _tfopen( strFile, _T("w") );

	return true;
}


KTarget* KScenario::CreateTarget(int nTargetYear, CString strTargetName)
{
	KTarget* pNewTarget = NULL;
	try
	{
		pNewTarget = new KTarget;
		pNewTarget->SetTargetYear(nTargetYear);
		pNewTarget->SetName(strTargetName);

		CString strTargetLocation = m_strLocation + _T("\\");
		strTargetLocation += strTargetName;

		pNewTarget->SetLocation( strTargetLocation );
		pNewTarget->SetParentObject( this );
		m_mapTarget.insert( std::make_pair( nTargetYear, pNewTarget ) );
	}
	catch(...)
	{
		return NULL;
	}
	return pNewTarget;
}

bool KScenario::ImportSingleTargetBLANK( KTarget* a_pTarget )
{
	try
	{
		CWaitCursor wait;

		// 2019.05.28 : Free 버전 관련 수정
		// 빈 데이터로 만들기 전에 Free 버전 사용여부를 초기화
		a_pTarget->ResetUseFreeVersion();

		// Done copy or interpolation or extrapolation 
		CString strNetworkDataFile = _T("");					
		CString strIODataFile	   = _T("");	 
		if( true == CreateEmptyTargetData( strNetworkDataFile, strIODataFile, a_pTarget ) )
		{
			DefinePurposeAndMode(strIODataFile);

			//a_pTarget->SetNetrorkDataFilePath(strNetworkDataFile);
			a_pTarget->SetIODataFilePath(strIODataFile);
			a_pTarget->CreateInitialObject();
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		return false;
	}

	m_bOpen = true;
	return true;
}


bool KScenario::ImportBLANK( void )
{
	CWaitCursor wait;
	
	KTDTargetMap::iterator itTargetMap = m_mapTarget.begin();

	//시나리오 내 DB가 있는 Target이 있는지 여부
	bool bIsEmpty = true;

	while (itTargetMap != m_mapTarget.end())
	{
		KTarget* pTarget= itTargetMap->second;

		if (!pTarget->IsEmptyData())
		{
			bIsEmpty = false;
			break;
		}

		++itTargetMap;
	}

	if (!bIsEmpty)
	{
		CString strMsg = _T("해당 선택 대상에 데이터가 존재합니다.\r\n빈 주머니 만들기 실행 후 기존 데이터는 삭제됩니다.\r\n계속하시겠습니까?");

		if (AfxMessageBox(strMsg, MB_YESNO) == IDNO)
		{
			return false;
		}
	}


	itTargetMap = m_mapTarget.begin();

	while( m_mapTarget.end() != itTargetMap )
	{
		KTarget* pTarget = itTargetMap->second;

		// Done copy or interpolation or extrapolation 
		CString strNetworkDataFile = _T("");					
		CString strIODataFile	   = _T("");	 
		if( true == CreateEmptyTargetData( strNetworkDataFile, strIODataFile, pTarget ) )
		{
			DefinePurposeAndMode(strIODataFile);

			//pTarget->SetNetrorkDataFilePath(strNetworkDataFile);
			pTarget->SetIODataFilePath(strIODataFile);
			pTarget->CreateInitialObject();
		}
        
		++itTargetMap;
	}

	m_bOpen = true;
	return true;
} 

bool KScenario::DeletePurposeODData( CString a_strFileName )
{
	KDBaseConPtr spDBaseConnection = KDBaseConPtr(new KSQLiteConnection(a_strFileName));
	if (nullptr == spDBaseConnection)
	{
		return false;
	}

	try
	{
		CString strSQL;
		strSQL.Format(_T(" DELETE FROM Purpose_od "));

		spDBaseConnection->ExecuteUpdate(strSQL);
	}
	catch (...)
	{
		TxLogDebugException();	
		return false;
	}

	return true;
}


bool KScenario::CopyFrom( const KScenario* pSource, bool bOverwrite, bool bCopyModelResults /*= false*/ )
{
	ASSERT( pSource );

	try
	{
		std::map< CString, KTarget* > sourceMap;
		std::map< CString, KTarget* >::iterator itSourceMap;

		const KTDTargetMap& sourceTargetMap = pSource->GetTargetMap();
		KTDTargetMap::const_iterator citSourceTargetMap = sourceTargetMap.begin();
		while( sourceTargetMap.end() != citSourceTargetMap )
		{
			KTarget* pSourceTarget = citSourceTargetMap->second;
			CString strSourceName = pSourceTarget->GetName();
			sourceMap.insert( std::make_pair( strSourceName, pSourceTarget ) );
			++citSourceTargetMap;
		}

		if( sourceMap.empty() == false )
		{
			CMainFrameWnd* pMainFrame   = (CMainFrameWnd*)(AfxGetMainWnd());

			const KTDTargetMap&          oTargetMap = this->GetTargetMap();
			KTDTargetMap::const_iterator bIter      = oTargetMap.begin();
			KTDTargetMap::const_iterator eIter      = oTargetMap.end();

			while (bIter != eIter)
			{
				KTarget*     pTarget      = bIter->second;
				CChildFrame* pChildeFrame = pMainFrame->GetChildFrame(pTarget->GetObjectID());

				if (pChildeFrame != NULL)
					pChildeFrame->SendMessage(WM_CLOSE);

				++bIter;
			}

			itSourceMap = sourceMap.begin();
			while( sourceMap.end() != itSourceMap )
			{
				int nTargetYear;
				CString strName;
				KTarget* pTarget;

				nTargetYear = itSourceMap->second->GetTargetYear();
				strName = itSourceMap->second->GetName();
				pTarget = findTarget( strName );

				if( NULL == pTarget )
				{
					pTarget = CreateTarget( nTargetYear, strName );
				}

				if (pTarget->CopyFrom( itSourceMap->second ) && bCopyModelResults)
				{
					CString strFromPath = itSourceMap->second->GetLocation();
					CString strToPath   = pTarget->GetLocation();

					strFromPath.AppendFormat(_T("\\%s"), DLL_MODEL_FOLDER_NAME);

					if (GetFileAttributes(strFromPath) != 0xFFFFFFFF)
					{
						ImChampDir::CopyDirectoryWinSimpleProgress(strFromPath, strToPath);
					}					
				}
				++itSourceMap;
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}

void KScenario::removeTarget( KTarget* pTarget )
{
	KTDTargetMap::iterator itTargetMap = m_mapTarget.begin();
	while( m_mapTarget.end() != itTargetMap )
	{
		if( itTargetMap->second == pTarget )
		{
			m_mapDeleteYear[pTarget->GetTargetYear()] = pTarget->GetTargetYear();

			delete itTargetMap->second;
			m_mapTarget.erase( itTargetMap );
			break;
		}
		++itTargetMap;
	}
}


void KScenario::ClearTarget( void )
{
	KTDTargetMap::iterator itTargetMap = m_mapTarget.begin();
	while( m_mapTarget.end() != itTargetMap )
	{
		delete itTargetMap->second;
		++itTargetMap;
	}

	m_mapTarget.clear();
}


bool KScenario::addNewTarget( void )
{
	kScenarioAddYearDlg dlg;
	
	KTDTargetMap::iterator itTargetMap = m_mapTarget.begin();
	while( m_mapTarget.end() != itTargetMap )
	{
		dlg.m_YearArray.push_back( itTargetMap->second->GetTargetYear() );
		++itTargetMap;
	}

	if( dlg.DoModal() != IDOK )
	{
		return false;
	}

	if(dlg.m_nTargetYear >0)
	{
		CString strYear;
		strYear.Format( _T("%d"), dlg.m_nTargetYear	);
		KTarget* pNewTarget = CreateTarget( dlg.m_nTargetYear, strYear );

	}

	return true;	
}


KTarget* KScenario::findTarget( CString strName )
{
	KTDTargetMap::iterator itTargetMap = m_mapTarget.begin();

	while( m_mapTarget.end() != itTargetMap )
	{
		if( itTargetMap->second->GetName() == strName )
		{
			return itTargetMap->second;
		}
		++itTargetMap;
	}
	return NULL;
}


bool KScenario::OpenScenario( void )
{
	TCHAR szValue[256] = {0};
	CString strValue;
	//TCHAR* pszApp = _T("Scenario");
    CString pszApp = _T("Scenario");
	CString strFile = m_strLocation + _T("\\");
	strFile += m_strName;
	strFile += _T(".ksx");

	::GetPrivateProfileString( pszApp, _T("Target"), _T(""), szValue, sizeof(szValue), strFile );
	strValue = szValue;
	
	int nPos = 0;
	CString strTarget = strValue.Tokenize( _T(";"), nPos );
	while( strTarget != _T("") )
	{
		KTarget* pTarget = new KTarget;
		pTarget->SetName( strTarget );

		CString strKey;
		strKey.Format( _T("Target_%s"), strTarget );
		memset( szValue, 0, sizeof(szValue) );
		::GetPrivateProfileString( pszApp, strKey, _T(""), szValue, sizeof(szValue), strFile );
		
		CString strLocation = m_strLocation + _T("\\");
		if( _tcslen(szValue) > 0 )
		{
			strLocation += szValue;
		}
		else
		{
			strLocation += strTarget;
		}
		pTarget->SetLocation( strLocation );
		pTarget->SetParentObject( this );

		m_mapTarget.insert( std::make_pair( pTarget->GetObjectID(), pTarget ) );
		
		strTarget = strValue.Tokenize( _T(";"), nPos );
	}
	
	OpenTarget( strFile );
	m_bOpen = true;

	return true;
}


bool KScenario::OpenTarget( const CString& strFile )
{
	bool bOpen = false;
	
	KTDTargetMap::iterator itTargetMap = m_mapTarget.begin();
	while( m_mapTarget.end() != itTargetMap )
	{
		KTarget* pTarget = itTargetMap->second;
		pTarget->OpenTarget( strFile );
		++itTargetMap;
	}

	return bOpen;
}


void KScenario::saveScenario( void )
{
	CString pszApp = _T("Scenario");
	CString strValue;
	CString strFile = m_strLocation + _T("\\");
	strFile += m_strName;
	strFile += _T(".ksx");

	ResetScenario(strFile);

	::WritePrivateProfileString( pszApp, _T("Name"), m_strName, strFile );

	KTDTargetMap::iterator itTargetMap = m_mapTarget.begin();
	while( m_mapTarget.end() != itTargetMap )
	{
		KTarget* pTarget = itTargetMap->second;
		pTarget->SaveTargetInfo( strFile );

		CString strTargetPath = pTarget->GetLocation();
		int nIndex = strTargetPath.ReverseFind( _T('\\') );
		if( -1 != nIndex )
		{
			strTargetPath = strTargetPath.Right( strTargetPath.GetLength() - nIndex );
			strTargetPath.Trim( _T("\\") );
			CString strKey;
			strKey.Format( _T("Target_%s"), pTarget->GetName() );
			::WritePrivateProfileString( pszApp, strKey, strTargetPath, strFile ); 
		}

		strValue += pTarget->GetName();
		strValue += _T(";");

		++itTargetMap;
	}

	if( strValue.IsEmpty() == false )
	{
		strValue.TrimRight( _T(";") );
		::WritePrivateProfileString( pszApp, _T("Target"), strValue, strFile );
	}
}


void KScenario::ResetScenario(CString a_strFile)
{
	CString pszApp = _T("Scenario");

	// .ksx 파일내 기존 [Scenario] 내용 삭제 
	::WritePrivateProfileString( pszApp, NULL, NULL, a_strFile );

	// .ksx 파일내 기존 [year] 내용 삭제 
	CString strYear = _T("");
	std::map<int,int>::iterator itDelYear = m_mapDeleteYear.begin();
	while( m_mapDeleteYear.end() != itDelYear)
	{
		strYear.Format(_T("%d"), itDelYear->first);
		::WritePrivateProfileString( strYear, NULL, NULL, a_strFile );
		++itDelYear;
	}
	m_mapDeleteYear.clear();
}


bool KScenario::IsOpened(void)
{
	return m_bOpen;
}


bool KScenario::IsBaseScenario(void)
{
	if( m_strName.CompareNoCase(_T("BASE")) == 0 )
	{
		return true;
	}
	else 
	{
		return false;
	}
}

void KScenario::Vaccum()
{
    KTDTargetMap::iterator itTargetMap = m_mapTarget.begin();
    while( m_mapTarget.end() != itTargetMap )
    {
        KTarget* pTarget = itTargetMap->second;
        pTarget->Vaccum();

        ++itTargetMap;
    }
}

void KScenario::CompactGISDB()
{
	KTDTargetMap::iterator itTargetMap = m_mapTarget.begin();
	while( m_mapTarget.end() != itTargetMap )
	{
		KTarget* pTarget = itTargetMap->second;
		pTarget->CompactGISDB();

		++itTargetMap;
	}
}

#pragma warning(pop)
