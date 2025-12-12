#pragma once

#include "KModelDefine.h"
#include "KEnumInModel.h"
#include "KParseVariableInfo.h"


class KDTreeNode
{
public:
    KDTreeNode(void) ;
    ~KDTreeNode(void); 
protected:   
    size_t m_resultIndexKey;
public:
    virtual KEMModeChoice::NODETYPE GetNodeType() const {return KEMModeChoice::MODE_NODE_TYPE;}    
    void   SetResultIndexKey(size_t key);
    size_t GetResultIndexKey() const;
    size_t GetInternalSeqID() const;
    
private:
    size_t m_InternalID;
    friend class KDTreeNodeCollection;
};
    
//###########################################################################################

class KDTreeModeNode : public KDTreeNode
{
public:
    KDTreeModeNode(void);
    KDTreeModeNode(const KDTreeModeNode& node);
    //KDTreeModeNode(CAtlString _strUtilityFunction, KParseVariables& _variables, double _dummy);
    ~KDTreeModeNode();

    KDTreeModeNode& operator=(const KDTreeModeNode& node);

private:
    CAtlString           m_strUtilityFunction;    
    KParseVariables      m_parseVariables;
    //KParseVariableInfo   m_dDummy;

public:
    virtual KEMModeChoice::NODETYPE GetNodeType() const {return KEMModeChoice::MODE_NODE_TYPE;}

    void SetUtilityFunction(CAtlString _strUtilityFunction);
    CAtlString GetUtilityFunction() const;

    size_t GetParseVariableCount() const;
    KParseVariableInfo* AddNewVariableInfo();
    KParseVariableInfo* AddNewVariableInfo(CAtlString _strVariableName, size_t _zeroIndexReferenceKey);
    KParseVariableInfo* GetParseVariableInfo(size_t i) const;

    //void SetDummy(KParseVariableInfo& _dummy);
};

//###########################################################################################

class KDTreeNestedNode : public KDTreeNode
{
public:
    KDTreeNestedNode();
    KDTreeNestedNode(const KDTreeNestedNode& node);
    KDTreeNestedNode(double _inclusiveValue);
    ~KDTreeNestedNode();

private:    
    double                      m_dInclusiveValue;
    CAtlArray<KDTreeModeNode*>  m_childNodes; 

public:
    virtual KEMModeChoice::NODETYPE  GetNodeType() const {return KEMModeChoice::NESTED_NODE_TYPE;}

    void   SetInclusiveValue(double _inclusiveValue);
    double GetInclusiveValue();

    KDTreeModeNode* AddNewModeNode();
   
    size_t GetChildNodeCount() const;
    KDTreeModeNode* GetChildNodeAt(size_t i);    
    void RemoveAll();
};

class KDTreeNodeCollection
{
public:
    KDTreeNodeCollection();
    ~KDTreeNodeCollection();

private:
    CAtlArray<KDTreeNode*> m_nodes;
    //size_t m_count;

public:
    KDTreeNode* AddNode(KEMModeChoice::NODETYPE _nodeType = KEMModeChoice::MODE_NODE_TYPE);
    void RemoveAll();

    size_t GetParentNodeCount() const;
    size_t GetTotalNodeCount() const;
    size_t GetModeNodeCount() const;

    size_t MakeInternalNodeId();

    KDTreeNode* GetAt(size_t i);
    const KDTreeNode* GetAt(size_t i) const;

    size_t GetMaxResultIndexKey() const;
    size_t GetMaxInputIndexKey()  const;
};