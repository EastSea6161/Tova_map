#pragma once

//#include "Klog.h"
#include "KMultiID.h"
#include "KModelDefine.h"
#include "KConstModel.h"
#include "KEnumInModel.h"
#include "KDistributionData.h"
#include "KModelUtilityOD.h"
#include "KModelUtilityZone.h"

class KxDistributionFratar_Old
{
public:
    KxDistributionFratar_Old(void);
    ~KxDistributionFratar_Old(void);

private:
    void GetRefBaseODAdjustmentByAverage( std::vector<Integer>& _targetZoneID,            
                                          std::map<Integer, Double>& _baseOD2Production, std::map<Integer, Double>& _baseOD2Attraction, 
                                          std::map<Integer, Double>& _targetProduction,  std::map<Integer, Double>& _targetAttraction, 
                                          std::map<KODKey, Double>& rBaseOD, 
                                          void (*func_ptr_msg)(CAtlString) = NULL
                                        );



    void FratarModel( std::vector<Integer>&       _caTargetZoneID, 
                      std::map<Integer, Double>&  _cmTargetProduction,
                      std::map<Integer, Double>&  _cmTargetAttraction,
                      /*std::map<KODKey,  Double>&  _cmBaseOD, */
                      int           _iterationCount, 
                      double        _fratarGap, 
                      std::map<KODKey,  Double>&    cmReturnOD, 
                      void (*func_ptr_msg)(CAtlString)     
                    );

    void BaseODInitialize( std::vector<Integer>&       _targetZoneID, 
                           std::map<Integer, Double>&  _targetProduction, 
                           std::map<Integer, Double>&  _targetAttraction,
                           std::map<KODKey, Double>&   _baseOD, 
                           std::map<KODKey, Double>&   rBaseOD,
                           void (*func_ptr_msg)( CAtlString ) = NULL
                          );

    void GetRefGrowthFactor( std::map<KODKey, Double>&   _cmBaseOD, 
                             std::vector<Integer>& _caTargetZoneID,
                             std::map<Integer, Double>&   _cmTargetProduction,
                             std::map<Integer, Double>&   _cmTargetAttraction, 
                             std::map<Integer, Double>&   cmREn, 
                             std::map<Integer, Double>&   cmRFn,
                             void (*func_ptr_msg)(CAtlString) = NULL
                           );

    void GetRefAdjustFactor( std::map<KODKey, Double> &  _cmBaseOD, 
                             std::vector<Integer>     &  _caTargetZoneID, 
                             std::map<Integer, Double>&  _cmEn,
                             std::map<Integer, Double>&  _cmFn,
                             std::map<Integer, Double>&  cmRLn, 
                             std::map<Integer, Double>&  cmRMn,
                             void (*func_ptr_msg)(CAtlString) = NULL
                           );

    KReturn::KEMReturn GetRefPAAdjustmentByAverage( std::map<Integer, Double>& _rProduction, 
                                                    std::map<Integer, Double>& _rAttraction,
                                                    void (*func_ptr_msg)(CAtlString ) = NULL
                                                  );   

    void   GetRefZonePADataFromOD( std::map<KODKey, Double>&  _inOD, 
                                   std::map<Integer, Double>& _outProduction, 
                                   std::map<Integer, Double>& _outAttraction,
                                   void (*func_ptr_msg)(CAtlString ) = NULL
                                 );

public:	
    KReturn::KEMReturn MuDistributionFratarExecute( 
                                  KDistributionZoneDataCollection&   _caTargetZonePA, 
                                  std::map<KODKey, Double>&          _cmOriginBaseOD, 
                                  int                                _iterationCount,  
                                  double                             _fratarGap,
                                  std::map<KODKey, Double>&          cmReturnOD,
                                  void (*func_ptr_msg)(CAtlString) = NULL
                                );
};


