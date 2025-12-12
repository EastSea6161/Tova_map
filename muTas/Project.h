
/**
 * @file Project.h
 * @brief KProject 선언
 * @author 
 * @date 2010.06.15
 * @remark
 */


#pragma once

#include "Object.h"
#include "KEMProject.h"
#include "KmzApp.h"

/* Forward declarations */
class KScenario;
class KPurpose;
class KMode;

struct TProjectInfo;

/* type definitions */
typedef std::list< CString > KTDStringList;
typedef std::list< int >     KTDIntegerList;
typedef std::map< unsigned int, KScenario* > KTDScenarioMap;


/**
 * @brief 교통정책 분석 시스템의 Project 정보를 관리
			Project는 시나리오를 관리한다.
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2010.06.15
 */

//New Version /////////////////////////////////////////////////////////////
class KSymbolInfo;

class KProject : public KObject
{
	/**************************************
	* Constructors and Destructor         *
	**************************************/
public:
	KProject(void);
	~KProject(void);

	/**************************************
	* Accessors                           *
	**************************************/
public :

	/**
	* @brief Investigator를 설정한다.
	* @param[in] strInverstigator - Investigator
	*/
	inline void SetInvestigator(CString strInvestigator)
	{
		m_strInvestigator = strInvestigator;
	}

	/**
	* @brief Investigator를 반환한다.
	* @return CString - Investigator
	*/
	inline CString getInvestigator(void)
	{
		return m_strInvestigator;
	}

	/**
	* @brief Organization을 설정한다.
	* @param[in] strOrganization - Organization
	*/
	inline void SetOrganization(CString strOrganization)
	{
		m_strOrganization = strOrganization;
	}

	/**
	* @brief Organization을 반환한다.
	* @return CString - Organization
	*/
	inline CString SetOrganization(void)
	{
		return m_strOrganization;
	}

	/**
	* @brief Summary를 설정한다.
	* @param[in] strSummary - Summary
	*/
	inline void SetDescription(CString a_strDescription)
	{
		m_strDescription = a_strDescription;
	}

	/**
	* @brief Summary를 반환한다.
	* @return CString - Summary
	*/
	inline CString GetDescription(void)
	{
		return m_strDescription;
	}

	/**
	* @brief Base year를 설정한다.
	* @param[in] nBaseYear - Base year
	*/
	inline void SetBaseYear(int nBaseYear)
	{
		m_nBaseYear = nBaseYear;
	}

	/**
	* @brief Base year를 반환한다.
	* @return int - Base year
	*/
	inline int getBaseYear(void)
	{
		return m_nBaseYear;
	}

	/**
	* @brief Target year를 설정한다.
	* @param[in] arrTargetYear - Target year 배열
	*/
	inline void SetTargetYear(std::vector<int> a_vecTargetYear)
	{
		m_vecTargetYear = a_vecTargetYear;
	}

	/**
	* @brief Target year를 반환한다.
	* @return std::vector<int> - Target year 배열
	*/
	inline std::vector<int> GetTargetYear(void)
	{
		return m_vecTargetYear;
	}

	/**
	* @brief Purpose Name을 설정한다.
	* @param[in] a_vecPurposeName - Purpose Name 배열
	*/
	inline void SetPurposeName(std::vector<CString> a_vecPurposeName)
	{
		m_vecPurposeName = a_vecPurposeName;
	}

	/**
	* @brief Purpose Name을 반환한다.
	* @return std::vector<CString> - Purpose Name 배열
	*/
	inline std::vector<CString> GetPurposeName(void)
	{
		return m_vecPurposeName;
	}

	/**
	* @brief Purpose ID를 설정한다.
	* @param[in] a_vecPurposeId - Purpose Id 배열
	*/
	inline void SetPurposeId(std::vector<int> a_vecPurposeId)
	{
		m_vecPurposeId = a_vecPurposeId;
	}

	/**
	* @brief Purpose ID을 반환한다.
	* @return std::vector<int> - Purpose ID 배열
	*/
	inline std::vector<int> GetPurposeID(void)
	{
		return m_vecPurposeId;
	}

	/**
	* @brief Purpose Group을 설정한다.
	* @param[in] a_nPurposeGroup - Purpose Group
	*/
	inline void SetPurposeGroup(int a_nPurposeGroup)
	{
		m_nPurposeGroup = a_nPurposeGroup;
	}

	/**
	* @brief Purpose Group을 반환한다.
	* @return int - Purpose Group
	*/
	inline int GetPurposeGroup(void)
	{
		return m_nPurposeGroup;
	}

	/**
	* @brief Purpose를 설정한다.
	* @param[in] a_vecPurpose - Purpose 배열
	*/
	void SetPurpose(std::vector<KPurpose*> a_vecPurpose);


	/**
	* @brief Purpose를 반환한다.
	* @return std::vector<KPurpose*> - Purpose 배열
	*/
	inline std::vector<KPurpose*> GetPurpose(void)
	{
		return m_vecPurpose;
	}

	/**
	* @brief Mode를 설정한다.
	* @param[in] a_vecMode - Mode 배열
	*/
	inline void SetModeName(std::vector<CString> a_vecMode)
	{
		m_vecModeName = a_vecMode;
	}

	/**
	* @brief Mode를 반환한다.
	* @return std::vector<CString> - Mode 배열
	*/
	inline std::vector<CString> GetModeName(void)
	{
		return m_vecModeName;
	}

	/**
	* @brief Mode ID를 설정한다.
	* @param[in] a_vecModeId - Mode Id 배열
	*/
	inline void SetModeId(std::vector<int> a_vecModeId)
	{
		m_vecModeId = a_vecModeId;
	}

	/**
	* @brief Mode Group을 설정한다.
	* @param[in] a_nModeGroup - Mode Group
	*/
	inline void SetModeGroup(int a_nModeGroup)
	{
		m_nModeGroup = a_nModeGroup;
	}

	/**
	* @brief Mode를 설정한다.
	* @param[in] a_vecMode - Mode 배열
	*/
	inline void SetMode(std::vector<KMode*> a_vecMode)
	{
		m_vecMode = a_vecMode;
	}

	/**
	* @brief Mode를 반환한다.
	* @return std::vector<KMode*> - Mode 배열
	*/
	inline std::vector<KMode*> GetMode(void)
	{
		return m_vecMode;
	}

	/**
	* @brief Base Scenario를 반환한다.
	* @return KScenario* - Base Scenario
	*/
	inline KScenario* GetBaseScenario(void)
	{
		return m_pBaseScenario;
	}

	/**
	* @brief Scenario 항목을 반환한다.
	* @return KTDScenarioMap& - Scenario 항목
	*/
	inline const KTDScenarioMap& GetScenarioMap( void ) const 
	{
		return m_mapScenario; 
	}


	/**
	* @brief 사용 Data 정의 주체를 설정한다.
	* @param[in] emDataUse - KEMDataUse 열거체 정보	
	*/
	inline void SetProjectType(const KEMProjectType emProjectType)
	{
		m_emProjectType = emProjectType;
	}

	/**
	* @brief 프로젝트의 사용 Data 정의 주체 정보를 반환 한다.
	* @return KEMDataUse& - Data 정의 주체 
	*/
	inline const KEMProjectType& GetProjectType( void )
	{
		return m_emProjectType;
	}

	void SetSubJectAreaInfo(int a_nAreaId, CString a_strAreaName, CString a_strFolderName);

	inline void SetAnalysisAreaId( int a_nAnalysisAreaId )
	{
		m_nAnalysisAreaId = a_nAnalysisAreaId;
	}

	inline int GetAnalysisAreaId()
	{
		return m_nAnalysisAreaId;
	}

	inline void SetAnalysisAreaName( CString a_strAnalysisAreaName )
	{
		m_strAnalysisAreaName = a_strAnalysisAreaName;
	}

	inline CString GetAnalysisAreaName()
	{
		return m_strAnalysisAreaName;
	}

	inline void SetAnalysisFolderName( CString a_strAnalysisFolderName )
	{
		m_strAnalysisFolderName = a_strAnalysisFolderName;
	}

	inline CString GetAnalysisFolderName()
	{
		return m_strAnalysisFolderName;
	}

	/**************************************
	* Operations                          *
	**************************************/
public :
	bool CreateInitialObject( void );
	bool modifyProject(TProjectInfo& a_projectInfo);
	bool OpenProject( const CString& strFile );
	void SaveProject( const CString& strFile );
	
	int addScenario( void );
	KScenario* CreateScenario( CString a_strScenarioName );
	KScenario* findScenario( unsigned int nID ) const;
	void removeScenario( unsigned int nID );
	void clearScenario( void );
	bool CopyScenario( KScenario* pSoruce );
public:
    void Vaccum();
	void CompactGISDB();
private:
    static unsigned __stdcall VaccumThreadCaller(void* p);
    void VaccumProcess();
public:
	//CString m_strProjectPath;
	void UpgradeProjectDB();
private:
	static unsigned __stdcall UpgradeProjectDBThreadCaller(void* p);
	void UpgradeProjectDBProcess();

protected :
	BOOL CopyProjectDBase ( bool a_bModePassenger );
	bool OpenScenario( const CString &a_strFile );
	bool MakePurposeArr( std::vector<CString> &a_vecPurposeName, std::vector<int> &a_vecPurposeId, int a_nPurposeGroup, std::vector<KPurpose*> &a_vecPurpose );
	bool MakeModeArr( std::vector<CString> &a_vecModeName, std::vector<int> &a_vecModeId, int a_nModeGroup, std::vector<KMode*> &a_vecMode );

	bool ModifyPassengerUserProject(TProjectInfo& a_projectInfo);

protected :
	int						m_nBaseYear;
	CString					m_strInvestigator;
	CString					m_strOrganization;
	CString					m_strDescription;
	std::vector<int>		m_vecTargetYear;
	
	std::vector<CString>	m_vecPurposeName;
	std::vector<int>		m_vecPurposeId;
	int						m_nPurposeGroup;
	std::vector<KPurpose*>  m_vecPurpose;

	std::vector<CString>	m_vecModeName;
	std::vector<int>		m_vecModeId;
	int						m_nModeGroup;
	std::vector<KMode*>		m_vecMode;
	
	KScenario*				m_pBaseScenario;
	
	KTDScenarioMap			m_mapScenario;

	KEMProjectType		    m_emProjectType;

	int		m_nAnalysisAreaId;
	CString m_strAnalysisAreaName;
	CString m_strAnalysisFolderName;

private:
	static double m_dLinkOffest;
public:
    static double GetLinkOffset();
    static void   SetLinkOffset(double offset);
    static double GetLinkOffset(double dWidth);

public:
	static KProject* ImProject() {
		 KProject* pProject = ((CKmzApp*)AfxGetApp())->GetProject();
		 return pProject;
	}


private:
    std::map<int, std::map<int, std::shared_ptr<KSymbolInfo>>> m_mapKeyTypeSymbol;
public:
    std::map<int, std::shared_ptr<KSymbolInfo>> TypeSymbol(int nKey) 
    {
        auto iFind = m_mapKeyTypeSymbol.find(nKey);
        if (iFind != m_mapKeyTypeSymbol.end()) {
            return iFind->second;
        }

        std::map<int, std::shared_ptr<KSymbolInfo>> mapBlank;
        return mapBlank;
    }
public:
    void LoadTypeSymbol();
};
