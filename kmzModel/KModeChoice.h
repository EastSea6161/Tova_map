#pragma once

#include "KModelDefine.h"
#include "KMultiIDDoubleRecord.h"
#include "KDTreeNode.h"
#include "KParser.h"
#include "KEnumInModel.h"
#include "KMultiID.h"
#include "KModelUtilityOD.h"

class KModeChoice
{
public:
    KModeChoice(void);
    ~KModeChoice(void);

private:
  
public:
    KReturn::KEMReturn 
    MuExecute ( KMultiIDDoubleRecord&   _paraOD, 
                KDTreeNodeCollection&  _tree, 
                KMultiIDDoubleRecord&   _objectOD, int ODIndex,
                KMultiIDDoubleRecord&   updatedReturnOD, 
                void (*func_ptr_msg)(CAtlString strMsg) = NULL
              );

private:
    void CalculateUtilityValue( KMultiIDDoubleRecord&    _paraOD,
                                KDTreeNodeCollection&   _tree, 
                                KMultiIDDoubleRecord&    utilityOD, 
                                void (*func_ptr_msg)(CAtlString strMsg) = NULL
                              );

    void CalculateModeUtilityValue( KMultiIDDoubleRecord&  _paraOD,
                                    KDTreeModeNode*       _pModeNode, 
                                    KMultiIDDoubleRecord&  utilityOD, 
                                    void (*func_ptr_msg)(CAtlString strMsg) = NULL
                                  );

    void CalculateNestedUtilityValue ( KMultiIDDoubleRecord&  _paraOD,
                                       KDTreeNestedNode*     _pNestedNode, 
                                       KMultiIDDoubleRecord&  utilityOD, 
                                       void (*func_ptr_msg)(CAtlString strMsg) = NULL
                                     );

    void CalculateRate ( KDTreeNodeCollection& _tree, 
                         KMultiIDDoubleRecord&  utilityOD, 
                         void (*func_ptr_msg)(CAtlString strMsg) = NULL);

    void CalculateModeOD( KMultiIDDoubleRecord&  _objectOD, int ODIndex,
                          KDTreeNodeCollection& _tree,
                          KMultiIDDoubleRecord&  _utilityOD, 
                          KMultiIDDoubleRecord&  updatedReturnOD, 
                          void (*func_ptr_msg)(CAtlString strMsg) = NULL
                        );
};
