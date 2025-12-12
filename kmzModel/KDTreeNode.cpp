#include "KDTreeNode.h"

#pragma region KDTreeNode

KDTreeNode::KDTreeNode(void) 
{
    m_resultIndexKey = 0;
    m_InternalID     = 0;
}

KDTreeNode::~KDTreeNode(void)
{
}

void KDTreeNode::SetResultIndexKey(size_t key)
{
    m_resultIndexKey = key;
}

size_t KDTreeNode::GetResultIndexKey() const
{
    return m_resultIndexKey;
}

size_t KDTreeNode::GetInternalSeqID() const
{
    return m_InternalID;
}

#pragma endregion KDTreeNode

//###########################################################################################

#pragma region KDTreeModeNode

KDTreeModeNode::KDTreeModeNode()
{
    m_strUtilityFunction = "";
}

KDTreeModeNode::KDTreeModeNode(const KDTreeModeNode& node)
{
    m_parseVariables.RemoveVariables();

    m_strUtilityFunction = node.m_strUtilityFunction;
    m_parseVariables     = node.m_parseVariables;
    //m_dDummy             = node.m_dDummy;
}


KDTreeModeNode::~KDTreeModeNode()
{
}

KDTreeModeNode& KDTreeModeNode::operator=(const KDTreeModeNode &node)
{
    if (&node == this)
    {
        return *this;
    }
    
    m_parseVariables.RemoveVariables();

    m_strUtilityFunction = node.m_strUtilityFunction;
    m_parseVariables     = node.m_parseVariables;
    //m_dDummy             = node.m_dDummy;

    return *this;
}

void KDTreeModeNode::SetUtilityFunction(CAtlString _strUtilityFunction)
{
    m_strUtilityFunction = _strUtilityFunction;
}

CAtlString KDTreeModeNode::GetUtilityFunction() const
{
    return m_strUtilityFunction;
}

size_t KDTreeModeNode::GetParseVariableCount() const
{
    return m_parseVariables.GetVariableCount();
}

KParseVariableInfo* KDTreeModeNode::AddNewVariableInfo()
{
    return m_parseVariables.AddNewVariable();
}

KParseVariableInfo* KDTreeModeNode::AddNewVariableInfo(CAtlString _strVariableName, size_t _zeroIndexReferenceKey)
{
    return m_parseVariables.AddNewVariable(_strVariableName, _zeroIndexReferenceKey);
}

KParseVariableInfo* KDTreeModeNode::GetParseVariableInfo(size_t i) const
{
   return m_parseVariables.GetVariableInfoAt(i);
}

//void KDTreeModeNode::SetDummy(KParseVariableInfo& _dummy)
//{
//    m_dDummy = _dummy;
//}


//KParseVariableInfo& KDTreeModeNode::GetParseVariableAt(size_t i)
//{
//    return m_parseVariables.GetAt(i);
//}

#pragma endregion KDTreeModeNode

//###########################################################################################

#pragma region KDTreeNestedNode

KDTreeNestedNode::KDTreeNestedNode()
{
    m_dInclusiveValue = 1.0;
}

KDTreeNestedNode::KDTreeNestedNode(double _inclusiveValue) 
{
    m_dInclusiveValue = _inclusiveValue;
}

KDTreeNestedNode::KDTreeNestedNode(const KDTreeNestedNode& node)
{
    m_dInclusiveValue = node.m_dInclusiveValue;
    m_childNodes.RemoveAll();
    m_childNodes.Copy(node.m_childNodes);
}

KDTreeNestedNode::~KDTreeNestedNode()
{
    RemoveAll();
}

KDTreeModeNode* KDTreeNestedNode::AddNewModeNode()
{
    KDTreeModeNode* pNode = new KDTreeModeNode();
    //pNode->m_InternalID = m_childNodes.GetCount();
    m_childNodes.Add(pNode);

    return pNode;
}

void KDTreeNestedNode::SetInclusiveValue(double _inclusiveValue)
{
    m_dInclusiveValue = _inclusiveValue;
}

double KDTreeNestedNode::GetInclusiveValue()
{
    return m_dInclusiveValue;
}

size_t KDTreeNestedNode::GetChildNodeCount() const
{
    return m_childNodes.GetCount();
}

KDTreeModeNode* KDTreeNestedNode::GetChildNodeAt(size_t i)
{
    return m_childNodes.GetAt(i);
}

void KDTreeNestedNode::RemoveAll()
{
    size_t nCount = m_childNodes.GetCount();
    for (size_t i=0; i<nCount; i++ )
    {
        KDTreeNode* pNode = m_childNodes.GetAt(i);
        if ( pNode->GetNodeType() == KEMModeChoice::MODE_NODE_TYPE )
        {
            KDTreeModeNode* pModeNode = (KDTreeModeNode*)pNode;
            delete pModeNode;
        }
        else
        {
            delete pNode;
        }
    }
    
    m_childNodes.RemoveAll();
}

#pragma endregion KDTreeNestedNode


KDTreeNodeCollection::KDTreeNodeCollection()
{
    //m_count = 0;
}

KDTreeNodeCollection::~KDTreeNodeCollection()
{
    RemoveAll();
}

KDTreeNode* KDTreeNodeCollection::AddNode(KEMModeChoice::NODETYPE _nodeType)
{
    KDTreeNode* pNode = NULL;
    if (KEMModeChoice::MODE_NODE_TYPE == _nodeType)
    {
        pNode = new KDTreeModeNode();
    }
    else
    {
        pNode = new KDTreeNestedNode();
    }

    // original : pNode->m_InternalID = m_count;
    //pNode->m_InternalID = GetTotalNodeCount();
    // GetTotalNodeCount

    // pNode->SetResultIndexKey(m_count); -. 0으로 초기화..

    m_nodes.Add(pNode);

    //m_count++;

    return pNode;
}

void KDTreeNodeCollection::RemoveAll()
{
    size_t nCount = m_nodes.GetCount();

    for( size_t i=0; i<nCount; i++ )
    {
        KDTreeNode* pNode = m_nodes.GetAt(i);
        if ( pNode->GetNodeType() == KEMModeChoice::MODE_NODE_TYPE )
        {
            KDTreeModeNode* pModeNode = (KDTreeModeNode*)pNode;
            delete pModeNode;
        }
        else
        {
            KDTreeNestedNode* pNestedNode = (KDTreeNestedNode*)pNode;
            delete pNestedNode;
        }
    }

    m_nodes.RemoveAll();
}

size_t KDTreeNodeCollection::GetParentNodeCount() const
{
    return m_nodes.GetCount();
}

size_t KDTreeNodeCollection::GetTotalNodeCount() const
{
    size_t nCount = GetParentNodeCount();
    size_t nNodeCount = 0;

    for ( size_t i=0; i<nCount; i++ )
    {
        const KDTreeNode* pNode = GetAt(i);
        if ( NULL == pNode )
        {
            break;
        }

        // Nested Node
        nNodeCount++;

        if ( pNode->GetNodeType() == KEMModeChoice::NESTED_NODE_TYPE)
        {
            const KDTreeNestedNode* pNestedNode = (KDTreeNestedNode*) pNode;

            nNodeCount += pNestedNode->GetChildNodeCount();           
        }
    }
    
    return nNodeCount;
}

size_t KDTreeNodeCollection::GetModeNodeCount() const
{
    size_t nCount = GetParentNodeCount();
    size_t nNodeCount = 0;

    for ( size_t i=0; i<nCount; i++ )
    {
        const KDTreeNode* pNode = GetAt(i);
        if ( NULL == pNode )
        {
            break;
        }

        if ( pNode->GetNodeType() == KEMModeChoice::NESTED_NODE_TYPE)
        {
            const KDTreeNestedNode* pNestedNode = (KDTreeNestedNode*) pNode;
            nNodeCount += pNestedNode->GetChildNodeCount();           
        }
        else
        {
            nNodeCount++;
        }
    }

    return nNodeCount;
}

size_t KDTreeNodeCollection::MakeInternalNodeId()
{
    size_t nCount     = GetParentNodeCount();
    size_t nTotalNodeCount = 0;

    for ( size_t i=0; i<nCount; i++ )
    {
        KDTreeNode* pNode = GetAt(i);
        if ( NULL == pNode )
        {
            break;
        }

        pNode->m_InternalID = nTotalNodeCount;
        nTotalNodeCount++;
        
        if ( pNode->GetNodeType() == KEMModeChoice::NESTED_NODE_TYPE)
        {
            KDTreeNestedNode* pNestedNode   = (KDTreeNestedNode*) pNode;
            size_t            nSubNodeCount = pNestedNode->GetChildNodeCount();

            for ( size_t k=0; k<nSubNodeCount; k++ )
            {
                KDTreeNode* pSubModeNode = pNestedNode->GetChildNodeAt(k);
                pSubModeNode->m_InternalID = nTotalNodeCount;
                nTotalNodeCount++;
            }      
        }
    }

    return nTotalNodeCount;
}

KDTreeNode* KDTreeNodeCollection::GetAt(size_t i)
{
    if ( i >= GetParentNodeCount() )
    {
        return NULL;
    }

    return m_nodes.GetAt(i);
}

const KDTreeNode* KDTreeNodeCollection::GetAt(size_t i) const
{
    if ( i >= GetParentNodeCount() )
    {
        return NULL;
    }

    return m_nodes.GetAt(i);
}

size_t KDTreeNodeCollection::GetMaxResultIndexKey() const
{
    size_t nCount = GetParentNodeCount();

    size_t nMaxIndex = 0;

    for ( size_t i=0; i<nCount; i++ )
    {
        const KDTreeNode* pNode = GetAt(i);
        if ( NULL == pNode )
        {
            break;
        }

        size_t indexKey = pNode->GetResultIndexKey();

        if ( nMaxIndex < indexKey )
        {
            nMaxIndex = indexKey;
        }
    }

    return nMaxIndex;
}

size_t KDTreeNodeCollection::GetMaxInputIndexKey() const
{
    size_t nCount = GetParentNodeCount();

    size_t nMaxIndex = 0;

    for ( size_t i=0; i<nCount; i++ )
    {
        const KDTreeNode* pNode = GetAt(i);
        if ( NULL == pNode )
        {
            break;
        }

        if ( pNode->GetNodeType() == KEMModeChoice::MODE_NODE_TYPE)
        {
            KDTreeModeNode* pModeNode = (KDTreeModeNode*) pNode;
            size_t nVariablesCount = pModeNode->GetParseVariableCount();
            for ( size_t j=0; j<nVariablesCount; j++ )
            {
                KParseVariableInfo* pVariable = pModeNode->GetParseVariableInfo(j);
                size_t indexKey = pVariable->GetZeroIndexReferenceKey();

                if ( nMaxIndex < indexKey )
                {
                    nMaxIndex = indexKey;
                }
            }
        }  
        else
        {
            KDTreeNestedNode* pNestedNode = (KDTreeNestedNode*) pNode;
            size_t nChildNodeCount = pNestedNode->GetChildNodeCount();
            for ( size_t m=0; m<nChildNodeCount; m++ )
            {
                KDTreeModeNode* pModeNode = pNestedNode->GetChildNodeAt(m);
                size_t nVariablesCount = pModeNode->GetParseVariableCount();
                for ( size_t j=0; j<nVariablesCount; j++ )
                {
                    KParseVariableInfo* pVariable = pModeNode->GetParseVariableInfo(j);
                    size_t indexKey = pVariable->GetZeroIndexReferenceKey();

                    if ( nMaxIndex < indexKey )
                    {
                        nMaxIndex = indexKey;
                    }
                }
            }
        }
    }

    return nMaxIndex;
}