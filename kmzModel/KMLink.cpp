#include "KMLink.h"
#include "KModelDefine.h"
#include "KMNode.h"
#include <queue>
#include <vector>
#include "Klog.h"


KMLink::KMLink(void)
{
}


KMLink::~KMLink(void)
{
}

typedef std::deque<KMNode, std::allocator<KMNode> > INTDQU;

void KMLink::xTest()
{
    
    std::priority_queue <KMNode, std::vector<KMNode>, KMNodeCompare >  minQue; 
    
    KMNode node;
    node.SetCostValue(10);

    int nCount = 10000;
   
    LOG4CXX_DEBUG(KLog::getLogger(), "시작(1)");	
    for (int i=0; i<nCount; i++)
    {
        node.SetCostValue(i);
        minQue.push(node);
    }

    while ( ! minQue.empty() )
    {
        minQue.top();
        minQue.pop();
    }
    LOG4CXX_DEBUG(KLog::getLogger(), "종료(1)");	

    /*std::priority_queue <KMNode, INTDQU, KMNodeCompare >  minQue2; 
    
    for (int i=0; i<nCount; i++)
    {
        node.SetCostValue(i);
        minQue2.push(node);
    }
    while ( ! minQue2.empty() )
    {
        minQue2.top();
        minQue2.pop();
    }*/

    LOG4CXX_DEBUG(KLog::getLogger(), "시작(2)");	
    std::priority_queue<int, std::vector<int>, std::greater<int> > tQue;
    for (int i=0; i<nCount; i++)
    {
        tQue.push(i);
    }

     while ( ! tQue.empty() )
    {
        tQue.top();
        tQue.pop();
    }
    LOG4CXX_DEBUG(KLog::getLogger(), "종료(2)");	
    
}