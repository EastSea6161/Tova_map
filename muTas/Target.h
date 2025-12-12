/**
 * @file Target.h
 * @brief KTarget 선언
 * @author 
 * @date 2010.06.22
 * @remark
 */

#pragma once

#include "Object.h"
#include "kmz_common.h"

/* forward declarations */
//class KScenario;
class KCodeManager;
class KIOTable;
class KIOTableController;
class KIOTables;
class KModeManager;
class KPurposeManager;
class KIOTableOwner;


class KTarget : public KObject
{
public:
	KTarget (void);
	~KTarget(void);
private:
    void Clear();
private:
    int m_nTargetYear;
public :
	int  GetTargetYear(void) const { return m_nTargetYear;  }
	void SetTargetYear(int nYear)  { m_nTargetYear = nYear; }

private:
    bool m_bEmptyData;
public:
	bool IsEmptyData(void) const   { return m_bEmptyData;   }

private: 
    CString m_strIODataFile;
public:
	CString GetIODataFilePath( void ) const   { return m_strIODataFile;   }
    void    SetIODataFilePath(LPCTSTR strFile){ m_strIODataFile = strFile;}

private:
    KDBaseConPtr m_spDBaseConnection;
public:
    KDBaseConPtr GetDBaseConnection() { return m_spDBaseConnection; }

private:   
    KIOTables* m_pTables;
public:
	KIOTables* Tables(void) const { return m_pTables; }

private:
    std::map<int, int>	m_mapNetworkDataYear;
public:
	void SetNetworkDataYear( std::map<int, int> a_mapNetworkDataYear ) { m_mapNetworkDataYear = a_mapNetworkDataYear; }

private:
    std::map<int, int>	m_mapODDataYear;
public:
	void SetODDataYear( std::map<int, int> a_mapODDataYear ) { m_mapODDataYear = a_mapODDataYear; }

private:
    KEMImportKTDB m_emNetworkDataStatus;
    KEMImportKTDB m_emODDataStatus;
public:
	inline void SetNetworkStatus(KEMImportKTDB a_emNetworkDataStatus)
	{
		m_emNetworkDataStatus = a_emNetworkDataStatus;
	}

	inline void SetODStatus(KEMImportKTDB a_emODDataStatus)
	{
		m_emODDataStatus = a_emODDataStatus;
	}

private:
    CString	m_strAnalysisArea;	// folder name
public:
	inline void SetAnalysisArea(CString a_strAnalysisArea)
	{
		m_strAnalysisArea = a_strAnalysisArea;
	}

public :
	bool CreateInitialObject();
	bool CreateOtherObject();

private:
	void OpenIOTables  (KIOTableOwner*);
    void OpenIOColumns (KIOTable* );
public:
	bool CopyFrom      ( const KTarget* pSource );
	bool OpenTarget    ( const CString& strFile );
	void SaveTargetInfo( const CString& strFile );	

private:
    KCodeManager*	 m_pCodeManager;
    KPurposeManager* m_pPurposeMgr;
    KModeManager*    m_pModeManager;
public:
    KCodeManager*    CodeManager   () { return m_pCodeManager; }
    KPurposeManager* PurposeManager() { return m_pPurposeMgr;  }
    KModeManager*    ModeManager   () { return m_pModeManager; }
    
public:
    CString Caption() const;
	CString CaptionScenario();
	CString CaptionTargetYear();

protected :
	KIOTableOwner*		m_pTableOwner;
	
private:
    std::list<KIOTableController*> m_lstIOControllers;
public:
    void RegisterController  (KIOTableController* pController);
    void UnregisterController(CString a_strName);
private:
    void ClearController();
public:
    void Vaccum();
	void CompactGISDB();


// DataBase Upgrade
private:
    void UpgradeDataBase();
        static unsigned __stdcall UpgradeDataBaseThreadCaller(void* p);
               void UpgradeDataBaseProcess();


// 2019.05.27 : Free 버전 관련 수정
private:
	bool m_bUseFreeVersion;	// Free 버전에서 사용가능 여부
	bool CheckCanOpenFreeVersion();

public:
	void ResetUseFreeVersion();
	bool IsUseFreeVersion();
};

