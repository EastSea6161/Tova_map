#pragma once

//#include "Klog.h"
#include "KMultiID.h"
#include "KModelDefine.h"
#include "KConstModel.h"
#include "KEnumInModel.h"
#include "KDistributionData.h"
#include "KModelUtilityOD.h"
#include "KModelUtilityZone.h"

class KDistribution_Fratar_Old
{
public:
	KDistribution_Fratar_Old(void);
	~KDistribution_Fratar_Old(void);

private:
    void GetRefBaseODAdjustmentByAverage( CAtlZnArray& _caZoneId,            
                                          CAtlZnMap& _cmProductionInOD,         CAtlZnMap& _cmAttractionInOD, 
                                          CAtlZnMap& _cmProductionInTargetZone, CAtlZnMap& _cmAttractionInTargetZone, 
                                          CAtlODValueMap& _cmRBaseOD, 
                                          void (*func_ptr_msg)(CAtlString strMsg) = NULL
                                        );



	void FratarModel( CAtlZnArray&     _caTargetZoneID,  
                      CAtlZnMap&       _cmTargetProduction, 
                      CAtlZnMap&       _cmTargetAttraction,
		              CAtlODValueMap&  _cmBaseOD, 
                      int              _iterationCount, 
                      double           _fratarGap, 
                      CAtlODValueMap&  cmReturnOD, 
                      void (*func_ptr_msg)(CAtlString) = NULL
					);

    void BaseODInitialize( CAtlZnArray&       _caTargetZoneID, 
                           CAtlZnMap&         _cmTargetProduction, 
                           CAtlZnMap&         _cmTargetAttraction,
                           CAtlODValueMap&    _cmOriginBaseOD, 
                           CAtlODValueMap&    cmRBaseOD,
                           void (*func_ptr_msg)( CAtlString ) = NULL
                         );

    void GetRefGrowthFactor( CAtlODValueMap&  _cmBaseOD, 
                             CAtlZnArray&     _caTargetZoneID, 
                             CAtlZnMap&       _cmTargetProduction,
                             CAtlZnMap&       _cmTargetAttraction, 
                             CAtlZnMap&       cmREn, 
                             CAtlZnMap&       cmRFn,
                             void (*func_ptr_msg)( CAtlString ) = NULL
                           );

    void GetRefAdjustFactor( CAtlODValueMap&  _cmBaseOD,
                             CAtlZnArray&     _caTargetZoneID,
                             CAtlZnMap&       _cmEn,
                             CAtlZnMap&       _cmFn,
                             CAtlZnMap&       cmRLn, 
                             CAtlZnMap&       cmRMn,
                             void (*func_ptr_msg)(CAtlString ) = NULL
                           );

    KReturn::KEMReturn GetRefPAAdjustmentByAverage( CAtlZnMap& _cmRProduction, CAtlZnMap& _cmRAttraction,
                                                     void (*func_ptr_msg)(CAtlString ) = NULL);
    void   GetRefZonePADataFromOD( CAtlODValueMap& _cmOD, 
                                   CAtlZnMap& _cmRZoneProductionFromOD, 
                                   CAtlZnMap& _cmRZoneAttractionFromOD,
                                   void (*func_ptr_msg)(CAtlString ) = NULL
                                 );
    

public:	
	KReturn::KEMReturn MuExecute( CAtlArray<KDistributionZoneData*>&    _caTargetZonePA, 
                                  CAtlODValueMap&                       _cmOriginBaseOD, 
                                  int                                   _iterationCount, 
                                  double                                _fratarGap,
                                  CAtlODValueMap&                       cmReturnOD, 
                                  void (*func_ptr_msg)(CAtlString ) = NULL
                                );
};


