#pragma once

#include "KModelDefine.h"
#include "KMultiIDDoubleRecord.h"
#include "KDTreeNode.h"
#include "KParser.h"
#include "KEnumInModel.h"
#include "KMultiID.h"
#include "KModelUtilityOD.h"

class KxModeChoice
{
public:
    KxModeChoice(void);
    ~KxModeChoice(void);
public:
    KReturn::KEMReturn 
    MuExecute ( KODKeyDoubleRecords&   _paraOD, 
                KDTreeNodeCollection&  _tree, 
                KODKeyDoubleRecords&   _objectOD, int ODIndex,
                KODKeyDoubleRecords&   updatedReturnOD, 
                void (*func_ptr_msg)(CAtlString strMsg) = NULL
                );

private:
    void CalculateUtilityValue( KODKeyDoubleRecords&    _paraOD,
                                KDTreeNodeCollection&   _tree, 
                                KODKeyDoubleRecords&    utilityOD, 
                                void (*func_ptr_msg)(CAtlString strMsg) = NULL
                              );

    void CalculateModeUtilityValue( KODKeyDoubleRecords&  _paraOD,
                                    KDTreeModeNode*       _pModeNode, 
                                    KODKeyDoubleRecords&  utilityOD, 
                                    void (*func_ptr_msg)(CAtlString strMsg) = NULL
                                  );

    void CalculateNestedUtilityValue ( KODKeyDoubleRecords&  _paraOD,
                                       KDTreeNestedNode*     _pNestedNode, 
                                       KODKeyDoubleRecords&  utilityOD, 
                                       void (*func_ptr_msg)(CAtlString strMsg) = NULL
                                     );

    void CalculateRate ( KDTreeNodeCollection& _tree, 
                         KODKeyDoubleRecords&  utilityOD, 
                         void (*func_ptr_msg)(CAtlString strMsg) = NULL
                       );

    void CalculateModeOD( KODKeyDoubleRecords&  _objectOD, int ODIndex,
                          KDTreeNodeCollection& _tree,
                          KODKeyDoubleRecords&  _utilityOD, 
                          KODKeyDoubleRecords&  updatedReturnOD, 
                          void (*func_ptr_msg)(CAtlString strMsg) = NULL
                        );
};

