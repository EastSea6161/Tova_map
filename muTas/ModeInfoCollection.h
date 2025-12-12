#pragma once

class KTarget;
class KMode;

class KModeInfoCollection
{
public:
	KModeInfoCollection(KTarget* a_pTarget);
	~KModeInfoCollection(void);

	KModeInfoCollection& operator=(KModeInfoCollection& a_right);

protected:
	KTarget* m_pTarget;
	std::list<KMode*> m_lstMode;

public:
	std::list<KMode*>::iterator begin();
	std::list<KMode*>::iterator end();

	std::list<KMode*>::const_iterator cbegin() 
	{ 
		return m_lstMode.cbegin(); 
	}

	std::list<KMode*>::const_iterator cend() 
	{ 
		return m_lstMode.cend(); 
	}


	void RemoveAll();
};

