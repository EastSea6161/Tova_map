#include "StdAfx.h"
#include "DBaseGenerationParaModel.h"
#include "Target.h"
#include "PurposeInfoCollection.h"
#include "PurposeManager.h"
#include "PAPurpose.h"
#include "Purpose.h"
#include "../QBicExtStaticCppLib/tinyXmlLib/tinyxml.h"
#include "../QBicExtStaticCppLib/tinyXmlLib/tinystr.h"
#include "XmlManager.h"
#include "GDefaultSimpleFuntion.h"
#include "GDefaultZoneGroupFunction.h"

#include "ImChampDir.h"
#include "Project.h"
#include "ImChampFrameWindow.h"
#include "DefineNamesDefaultPara.h"
#include "KXmlDefaultGeneration.h"

KDBaseGenerationParaModel::KDBaseGenerationParaModel(void)
{
}


KDBaseGenerationParaModel::~KDBaseGenerationParaModel(void)
{
}

void KDBaseGenerationParaModel::InitDefaultGenerationData( KTarget* a_pTarget, std::map<KIntKey, std::map<KIntKey, KGenerationInfo*>>& a_mapGenerationInfo )
{
	try
	{
		KPurposeManager* pPurposeMgr = a_pTarget->PurposeManager();
		int nCount = pPurposeMgr->GetPurposeCount();
		for (int i = 0; i < nCount; i++)
		{
			KPAPurpose* pPAPurpse = pPurposeMgr->GetPurpose(i); 

			// PurposeID & 1:Production 2:Attection
			KIntKey oMainKey(pPAPurpse->PurposeID(), pPAPurpse->PAType());

			std::map<KIntKey, KGenerationInfo*> mapGenerationInfo;

			//기본 데이터 구축
			//ZoneGroup : No , Model : Unit
			KIntKey oNZUnitIntKey(0, 1);
			KGenerationInfo* pNZUnitInifo = new KGenerationInfo;
			pNZUnitInifo->Purpose(pPAPurpse);
			pNZUnitInifo->GenerationModel(KEMGenerationUnit);
			pNZUnitInifo->ZoneGroup(false);
			mapGenerationInfo.insert(std::make_pair(oNZUnitIntKey, pNZUnitInifo));

			//ZoneGroup : No , Model : Regression
			KIntKey oNZRegressionIntKey(0, 2);
			KGenerationInfo* pNZRegressionInifo = new KGenerationInfo;
			pNZRegressionInifo->Purpose(pPAPurpse);
			pNZRegressionInifo->GenerationModel(KEMGenerationRegression);
			pNZRegressionInifo->ZoneGroup(false);
			mapGenerationInfo.insert(std::make_pair(oNZRegressionIntKey, pNZRegressionInifo));

			//ZoneGroup : No , Model : CrossClassfication
			KIntKey oNZCrossIntKey(0, 3);
			KGenerationInfo* pNZCrossInifo = new KGenerationInfo;
			pNZCrossInifo->Purpose(pPAPurpse);
			pNZCrossInifo->GenerationModel(KEMGenerationCrossClassification);
			pNZCrossInifo->ZoneGroup(false);
			mapGenerationInfo.insert(std::make_pair(oNZCrossIntKey, pNZCrossInifo));

			//ZoneGroup : Yes , Model : Unit
			KIntKey oYZUnitIntKey(1, 1);
			KGenerationInfo* pYZUnitInifo = new KGenerationInfo;
			pYZUnitInifo->Purpose(pPAPurpse);
			pYZUnitInifo->GenerationModel(KEMGenerationUnit);
			pYZUnitInifo->ZoneGroup(true);
			mapGenerationInfo.insert(std::make_pair(oYZUnitIntKey, pYZUnitInifo));

			//ZoneGroup : Yes , Model : Regression
			KIntKey oYZRegressionIntKey(1, 2);
			KGenerationInfo* pYZRegressionInifo = new KGenerationInfo;
			pYZRegressionInifo->Purpose(pPAPurpse);
			pYZRegressionInifo->GenerationModel(KEMGenerationRegression);
			pYZRegressionInifo->ZoneGroup(true);
			mapGenerationInfo.insert(std::make_pair(oYZRegressionIntKey, pYZRegressionInifo));

			//ZoneGroup : Yes , Model : CrossClassfication
			KIntKey oYZCrossIntKey(1, 3);
			KGenerationInfo* pYZCrossInifo = new KGenerationInfo;
			pYZCrossInifo->Purpose(pPAPurpse);
			pYZCrossInifo->GenerationModel(KEMGenerationCrossClassification);
			pYZCrossInifo->ZoneGroup(true);
			mapGenerationInfo.insert(std::make_pair(oYZCrossIntKey, pYZCrossInifo));

			KXmlDefaultGeneration oDefaultGenertion(a_pTarget);
			oDefaultGenertion.GetDefaultData(mapGenerationInfo);

			KIntKey oIntKey(pPAPurpse->PurposeID(), pPAPurpse->PAType());
			a_mapGenerationInfo.insert(std::make_pair(oIntKey,mapGenerationInfo));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}
