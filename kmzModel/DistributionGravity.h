#pragma once
#include "KModelInclude.h"
namespace KDistributionGravity_ZonStructure
{
    const int P  = 0;
    const int A  = 1;
    const int Bj = 2;
    const int Ai = 3;
    const int As = 4;
    const int Bs = 5;
}


class KDistributionGravity
{
public:
    KDistributionGravity(void);
    ~KDistributionGravity(void);

public:
    static int MuDistributionGravity
        ( 
            KIDKeyDoubleRecords& a_ixDrZonePAInfo,   size_t a_nxZonePIndex, 
            KODKeyDoubleRecords& a_oxDrRijInfo,      size_t a_nxRijIndex, 
            int a_functionType,  double a_dAlpha,    double a_dBeta, double a_dTheta, 
            int a_nIteration,    double a_dTolerance, 
            KODKeyDoubleRecords& a_oxDrResultRecord, size_t a_nxResultIndex,
            void (*func_ptr_msg)(CAtlString) = NULL
        );                      

private:    
    static int  IterationAiBj(KIDKeyDoubleRecords& a_ixDrZone_P_A_Bj_Ai, std::map<KODKey, Double>& a_mapFnCij, double a_dTolerance);
    static void CalculateTij(KIDKeyDoubleRecords& a_ixDrZone_P_A_Bj_Ai, std::map<KODKey, Double>& a_mapFnCij, KODKeyDoubleRecords& a_oxDrResultRecord, size_t a_nxResultIndex);
};

