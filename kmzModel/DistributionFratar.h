#pragma once
#include "KModelInclude.h"

namespace KDistributionFratar_ZonStructure
{
    const int P  = 0;
    const int A  = 1;
    const int Pb = 2;
    const int Ab = 3;
    const int Ei = 4;
    const int Fj = 5;
    const int Li = 6;
    const int Mj = 7;
}


class KDistributionFratar
{
public:
    KDistributionFratar(void);
    ~KDistributionFratar(void);

public:
    static int MuDistributionFratar
        ( 
            KIDKeyDoubleRecords& a_ixDrZonePAInfo,   size_t a_nxZonePIndex, 
            KODKeyDoubleRecords& a_oxDrBaseODInfo,   size_t a_nxBaseODIndex,  
            int a_nIteration,    double a_dTolerance, 
            KODKeyDoubleRecords& a_oxDrResultRecord, size_t a_nxResultIndex,
            void (*func_ptr_msg)(CAtlString)
        );   
private:
    static int CalculateTij
        (
            KIDKeyDoubleRecords& a_ixDrZone_P_A_Pb_Ab_Ei_Fj_Li_Mj, std::map<KODKey, Double>& a_mapTij,
            double a_dTolerance
        );
};

