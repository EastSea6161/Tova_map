#pragma once

/*forward declare*/
class KTarget;
class KPurpose;

class KPurposeInfoCollection
{
public:
	KPurposeInfoCollection(KTarget* a_pTarget);
	~KPurposeInfoCollection(void);

	KPurposeInfoCollection& operator=(KPurposeInfoCollection& a_right);
protected:
	KTarget* m_pTarget;
	std::list<KPurpose*> m_lstPurpose;

public:
	std::list<KPurpose*>::iterator begin();
	std::list<KPurpose*>::iterator end();

	std::list<KPurpose*>::const_iterator cbegin() 
	{ 
		return m_lstPurpose.cbegin(); 
	}

	std::list<KPurpose*>::const_iterator cend() 
	{ 
		return m_lstPurpose.cend(); 
	}


	void RemoveAll();

	//size_t 
};

