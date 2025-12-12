#include "StdAfx.h"
#include "ModeInfoCollection.h"

#include "Target.h"
#include "Mode.h"
#include "model_common.h"

KModeInfoCollection::KModeInfoCollection(KTarget* a_pTarget)
	: m_pTarget(a_pTarget)
{
	TxLogDebugStartMsg();
	RemoveAll();

	KEMModeGroup eumGroup = KEMModeGroupODBase;

	if (KmzSystem::GetSystemType() != KEMPASSENGER_SYSTEM)
	{
		eumGroup = KEMModeGroupFreightOD;
	}

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQL(_T(""));
	strSQL.Format( 
		_T(" SELECT detail_mode_id, mode_name ")
		_T(" FROM detail_mode ")
		_T(" WHERE mode_group_code = %d order by detail_mode_id "),
		eumGroup);

	try
	{
		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResult->Next())			
		{
			int     nModeID = spResult->GetValueInt(0);
			CString strName = spResult->GetValueString(1);

			KMode* pMode = new KMode(KEMModeGroupODBase, nModeID, strName);
			m_lstMode.push_back(pMode);
		}
	}
	catch (...)
	{		
		TxLogDebugException();
	}

	TxLogDebugEndMsg();
}


KModeInfoCollection::~KModeInfoCollection(void)
{
	TxLogDebugStartMsg();
	RemoveAll();
	TxLogDebugEndMsg();
}

KModeInfoCollection& KModeInfoCollection::operator=(KModeInfoCollection& a_right)
{
	if (&a_right == this)
	{
		return *this;
	}

	this->RemoveAll();

	std::list<KMode*>::iterator iter = a_right.begin();
	std::list<KMode*>::iterator end  = a_right.end();

	while ( iter != end )
	{
		KMode* pRightMode = *iter;
		KMode* pMode      = new KMode(pRightMode->ModeGroup(), pRightMode->ModeID(), pRightMode->ModeName());

		m_lstMode.push_back(pMode);

		++iter;
	}

	return *this;
}

std::list<KMode*>::iterator KModeInfoCollection::begin()
{
	return m_lstMode.begin();
}


std::list<KMode*>::iterator KModeInfoCollection::end()
{
	return m_lstMode.end();
}


void KModeInfoCollection::RemoveAll()
{
	std::list<KMode*>::iterator iter = m_lstMode.begin();
	std::list<KMode*>::iterator end  = m_lstMode.end();

	while(iter != end)
	{
		KMode* pMode = *iter;		
		delete pMode;

		++iter;
	}

	m_lstMode.clear();
}