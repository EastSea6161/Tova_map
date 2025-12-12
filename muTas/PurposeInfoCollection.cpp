#include "StdAfx.h"
#include "PurposeInfoCollection.h"

#include "Target.h"
#include "Purpose.h"
#include "model_common.h"

KPurposeInfoCollection::KPurposeInfoCollection(KTarget* a_pTarget)
	:	m_pTarget(a_pTarget)
{
	TxLogDebugStartMsg();
	RemoveAll();

	KEMPurposeGroup eumGroup = KEMPurposeGroupODBase;

	if (KmzSystem::GetSystemType() != KEMPASSENGER_SYSTEM)
	{
		eumGroup = KEMPurposeGroupFreightOD;
	}

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	
	CString strSQL(_T(""));
	strSQL.Format( 
		_T(" SELECT detail_object_id, object_name ")
		_T(" FROM detail_object ")
		_T(" WHERE object_group_code = %d order by detail_object_id "),
		eumGroup);

	try
	{
		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResult->Next())			
		{
			int nObjectID   = spResult->GetValueInt(0);
			CString strName = spResult->GetValueString(1);
			
			KPurpose* pPurpose = new KPurpose(KEMPurposeGroupODBase, nObjectID, strName);
			m_lstPurpose.push_back(pPurpose);
		}
	}
	catch (...)
	{		
		TxLogDebugException();
	}

	TxLogDebugEndMsg();
}


KPurposeInfoCollection::~KPurposeInfoCollection(void)
{
	TxLogDebugStartMsg();
	RemoveAll();
	TxLogDebugEndMsg();
}

KPurposeInfoCollection& KPurposeInfoCollection::operator=(KPurposeInfoCollection& a_right)
{
	if (&a_right == this)
	{
		return *this;
	}

	this->RemoveAll();
	
	std::list<KPurpose*>::iterator iter = a_right.begin();
	std::list<KPurpose*>::iterator end  = a_right.end();

	while ( iter != end )
	{
		KPurpose* pRightPurpose = *iter;
		KPurpose* pPurpose = new KPurpose(pRightPurpose->PurposeGroup(), pRightPurpose->PurposeID(), pRightPurpose->PurposeName());

		m_lstPurpose.push_back(pPurpose);

		++iter;
	}
	
	return *this;
}

std::list<KPurpose*>::iterator KPurposeInfoCollection::begin()
{
	return m_lstPurpose.begin();
}


std::list<KPurpose*>::iterator KPurposeInfoCollection::end()
{
	return m_lstPurpose.end();
}


void KPurposeInfoCollection::RemoveAll()
{
	std::list<KPurpose*>::iterator iter = m_lstPurpose.begin();
	std::list<KPurpose*>::iterator end = m_lstPurpose.end();

	while(iter != end)
	{
		KPurpose* pPurpose = *iter;		
		delete pPurpose;

		++iter;
	}

	m_lstPurpose.clear();
}