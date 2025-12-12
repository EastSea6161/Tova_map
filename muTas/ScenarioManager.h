/**
 * @file : ScenarioManager.h
 * @brief : KScenarioManager 急攫
 * @auther : teegee@zaolsoft.com
 * @date : 2010.08.19
 * @remark :
 */


#pragma once


/* Forward declarations */
class KProject;
class KScenario;


/* Type definations */
typedef std::map< unsigned int, KScenario* > KTDScenarioMap;


/**
 * @class : KScenarioManager 
 * @brief : Scenario甫 积己/昏力/包府
 * @remark :
 * @warning :
 * @version : 1.0
 * @auther : teegee@zaolsoft.com
 * @date : 2010.08.19
 */
class KScenarioManager
{
	/**************************************
	* Constructors / Destructor           *
	**************************************/
public:
	KScenarioManager(void);
	~KScenarioManager(void);


	/**************************************
	* Accessors                           *
	**************************************/
public :
	inline const KTDScenarioMap& getScenarioMap( void ) const 
	{
		return m_ScenarioMap;
	}

	/**************************************
	* Operations                          *
	**************************************/
public :
	KScenario* createScenario( void );
	KScenario* createScenario( const KProject* pProject, const CString& strName );
	KScenario* findScenario( unsigned int nID ) const;
	void renameScenario( unsigned int nID );
	//void renameScenario( unsigned int nID, const CString& strNewName = _T("") );
	void removeScenario( unsigned int nID );
	void clearScenario( void );


	/**************************************
	* Properties / Variables              *
	**************************************/
protected :
	KTDScenarioMap m_ScenarioMap;

};
