#include "StdAfx.h"
#include "MultiAssignInfo.h"

#include "Mode.h"
#include "ModeManager.h"
#include "Target.h"

#include "DBaseDefaultParaHighwayAssign.h"

#include "ATripMatrix.h"
#include "AInputInfo.h"
#include "DefineNamesDefaultPara.h"


KMultiAssignInfo::KMultiAssignInfo(KTarget* a_pTarget, bool a_bIsBase)
	: m_pTarget(a_pTarget)
	, m_bUseOverWrite(false)
	, m_bIsBase(a_bIsBase)
	, m_bSelected(false)
	, m_bIsError(false)
{
	m_TOverWirteAResultInfo.TbUseData = false;

	if (!m_bIsBase)
		InitInputInfoList();
}


KMultiAssignInfo::~KMultiAssignInfo(void)
{
	if (!m_bIsBase)
		ClearInputInfoList();	
}

void KMultiAssignInfo::InitInputInfoList( void )
{
	KMode*						 pMode	      = nullptr;
	KModeManager*				 pModeManager = m_pTarget->ModeManager();
	int							 nModeID     (0);
	CString						 strModeName (_T(""));
	double						 dOccupancy  (0.0);
	double						 dPCE		 (0.0);

	KDBaseDefaultParaHighwayAssign::LoadDefaultOccupancyPCEMode(m_mapDefaultOccupancyPCEMode);
	KDBaseAssignment::GetModeChoiceODAtGroupingMode(m_pTarget, m_mapModeChocieColumns);

	AutoType iterDefaultMode = m_mapDefaultOccupancyPCEMode.begin();
	AutoType endDefaultMode  = m_mapDefaultOccupancyPCEMode.end();

	AutoType iterGetMode = m_mapModeChocieColumns.begin();
	AutoType endGetMode  = m_mapModeChocieColumns.end();
	size_t nModeColumnCount	 = m_mapModeChocieColumns.size();

	if((iterDefaultMode != endDefaultMode) && (iterGetMode != endGetMode))
	{
		//Default 값이 있는 경우 수행
		AutoType iterUserMode = m_mapDefaultOccupancyPCEMode.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		if(iterUserMode != endDefaultMode)//User Code가 있는 경우
		{
			for(size_t i = 0; i < nModeColumnCount; i++)
			{
				nModeID = iterGetMode->first;

				int nModeCount = pModeManager->GetModeCount();
				for(int i = 0; i < nModeCount; i++)
				{
					int nModeIDInManager = pModeManager->GetMode(i)->ModeID();
					if (nModeIDInManager == nModeID)
					{
						pMode = pModeManager->GetMode(i);

						KATripMatrix::GetDefaultModeInfo(KDefaultParameterMasterCode::USER_MASETER_CODE, m_mapGetDefaultModeInfo);

						AutoType iterDefaultMode = m_mapGetDefaultModeInfo.find(nModeID);
						AutoType  endDefaultMode = m_mapGetDefaultModeInfo.end();
						if(iterDefaultMode != endDefaultMode)
						{
							std::vector<KOccupancyPCEMode> vecDefaultModeInfo = iterDefaultMode->second;
							KOccupancyPCEMode oOccupancyPCEMode = vecDefaultModeInfo[0];
							dOccupancy							= oOccupancyPCEMode.dOccupancy;
							dPCE								= oOccupancyPCEMode.dPCE;
						}
						else
						{
							dOccupancy = 1.0;
							dPCE	   = 1.0;
						}

						KAInputInfo* pInputInfo = new KAInputInfo;
						pInputInfo->Mode(pMode);
						pInputInfo->Occupancy(dOccupancy);
						pInputInfo->PCU(dPCE);
						m_InputList.push_back(pInputInfo);

					}
				}
				iterGetMode++;
			}
		}
		else//User Code가 없고 System Code만 있는 경우
		{
			for(size_t i = 0; i < nModeColumnCount; i++)
			{
				nModeID = iterGetMode->first;

				int nModeCount = pModeManager->GetModeCount();
				for(int i = 0; i < nModeCount; i++)
				{
					int nModeIDInManager = pModeManager->GetMode(i)->ModeID();
					if (nModeIDInManager == nModeID)
					{
						pMode = pModeManager->GetMode(i);

						KATripMatrix::GetDefaultModeInfo(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE, m_mapGetDefaultModeInfo);

						AutoType iterDefaultMode = m_mapGetDefaultModeInfo.find(nModeID);
						AutoType  endDefaultMode = m_mapGetDefaultModeInfo.end();
						if(iterDefaultMode != endDefaultMode)
						{
							std::vector<KOccupancyPCEMode> vecDefaultModeInfo = iterDefaultMode->second;
							KOccupancyPCEMode oOccupancyPCEMode = vecDefaultModeInfo[0];
							dOccupancy							= oOccupancyPCEMode.dOccupancy;
							dPCE								= oOccupancyPCEMode.dPCE;
						}
						else
						{
							dOccupancy = 1.0;
							dPCE	   = 1.0;
						}
						KAInputInfo* pInputInfo = new KAInputInfo;
						pInputInfo->Mode(pMode);
						pInputInfo->Occupancy(dOccupancy);
						pInputInfo->PCU(dPCE);
						m_InputList.push_back(pInputInfo);
					}
				}
				iterGetMode++;
			}
		}
	}
	else
	{
		for(size_t i = 0; i < nModeColumnCount; i++)
		{
			nModeID = iterGetMode->first;

			int nModeCount = pModeManager->GetModeCount();
			for(int i = 0; i < nModeCount; i++)
			{
				int nModeIDInManager = pModeManager->GetMode(i)->ModeID();
				if (nModeIDInManager == nModeID)
				{
					pMode = pModeManager->GetMode(i);
				}
			}
			dOccupancy = 1.0;
			dPCE	   = 1.0;

			KAInputInfo* pInputInfo = new KAInputInfo;
			pInputInfo->Mode(pMode);
			pInputInfo->Occupancy(dOccupancy);
			pInputInfo->PCU(dPCE);
			m_InputList.push_back(pInputInfo);	

			iterGetMode++;
		}
	}
}

void KMultiAssignInfo::ClearInputInfoList( void )
{
	std::list<KAInputInfo*>::iterator itInput,itEnd = m_InputList.end();
	for(itInput = m_InputList.begin(); itInput != itEnd; ++itInput)
	{
		KAInputInfo* pInput = *itInput;
		QBicDelete(pInput);
	}
	m_InputList.clear();
}

std::list<KAInputInfo*> KMultiAssignInfo::GetInputInfo( void )
{
	return m_InputList;
}



bool KMultiAssignInfo::IsBase()
{
	return m_bIsBase;
}


void KMultiAssignInfo::SetSelected(bool bSelected)
{
	m_bSelected = bSelected;
}


bool KMultiAssignInfo::GetSelected()
{
	return m_bSelected;
}


void KMultiAssignInfo::SetError(bool bError)
{
	m_bIsError = bError;
}


bool KMultiAssignInfo::GetError()
{
	return m_bIsError;
}

void KMultiAssignInfo::SetStatusString(CString& str)
{
	m_strStatus = str;
}


CString KMultiAssignInfo::GetStatusString()
{
	return m_strStatus;
}



KMultiAssignInfoSOBA::KMultiAssignInfoSOBA(KTarget* pTarget, bool bIsBase)
	: KMultiAssignInfo(pTarget, bIsBase)
{

}


KMultiAssignInfoSOBA::~KMultiAssignInfoSOBA(void)
{

}


KARunningOptionInfo& KMultiAssignInfoSOBA::GetRunningOption()
{
	return m_runnginOption;
}


void KMultiAssignInfoSOBA::SetRunningOption(KARunningOptionInfo& ro)
{
	m_runnginOption = ro;
}