#pragma once

#include <list>
#include "IMapEditObserver.h"


class KMapEditMaster
{
private:
	KMapEditMaster(void);
public:
	~KMapEditMaster(void);

public:
	static KMapEditMaster* Instance();
	static void Regist   (IMapEditObserver* a_pMapEditObserver, bool a_bEditMaster = false);
	static void UnRegist (IMapEditObserver* a_pMapEditObserver);

private:
	static 
		KMapEditMaster*              m_pMapEditMaster;
	static 
		std::list<IMapEditObserver*> m_lstIMapEditObserver;
};

typedef std::shared_ptr<KMapEditMaster> KMapEditMasterPtr;