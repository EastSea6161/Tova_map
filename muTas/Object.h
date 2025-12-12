/**
 * @file DataObject.h
 * @brief KDataObject ¼±¾ð
 * @author 
 * @date 2010.08.19
 * @remark
 */


#pragma once


#include "KeyGenerator.h"


/* Object type */
enum KEMObjectType
{
	KEMObjectTypeProject,
	KEMObjectTypeScenario,
	KEMObjectTypeTarget
};


class KObject
{
public:
	KObject( KEMObjectType type );
	virtual ~KObject(void);
public :
	inline bool isTypeOf( KEMObjectType type ) const
	{
		return type == m_KEMObjectType;
	}
	inline KEMObjectType GetObjectType( void ) const
	{
		return m_KEMObjectType;
	}
	inline unsigned int GetObjectID( void ) const
	{
		return m_nUniqueObjectID;
	}
	inline CString GetName( void ) const
	{
		return m_strName;
	}
	inline void SetName( const CString& strName )
	{
		m_strName = strName;
	}
	inline CString GetLocation( void ) const
	{
		return m_strLocation;
	}
	inline void SetLocation( const CString& strLocation )
	{
		m_strLocation = strLocation;
	}
	inline KObject* GetParentObject( void ) const
	{
		return m_pParentObject;
	}
	inline void SetParentObject( KObject* pObject )
	{
		m_pParentObject = pObject;
	}

public :
	virtual bool CreateObjectDirectory();
    virtual bool CreateInitialObject  () = 0;

protected :
	KEMObjectType m_KEMObjectType;
	unsigned int  m_nUniqueObjectID;

	CString       m_strName;
	CString       m_strLocation;
	KObject*      m_pParentObject;
private :
	static ZKeyGenerator< unsigned int > KS_UniqueKeyGenerator;
};

