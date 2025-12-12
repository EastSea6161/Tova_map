#include "StdAfx.h"
#include "ModeTree.h"
#include "ModeTreeNodeMode.h"


KModeTree::KModeTree(void)
{
}


KModeTree::KModeTree(const KModeTree& r)
{
	std::list<KModeTreeNodeMode*>::const_iterator itNode, itEnd = r.m_ModeNodeList.end();
	for(itNode = r.m_ModeNodeList.begin(); itNode != itEnd; ++itNode)
	{
		KModeTreeNodeMode* pModeNode = *itNode;
		KModeTreeNodeMode* pL = new KModeTreeNodeMode(*pModeNode);
		m_ModeNodeList.push_back(pL);
	}
}


KModeTree::~KModeTree(void)
{
	RemoveAllModeNode();
}


KModeTreeNodeMode* KModeTree::AddModeNode(void)
{
	KModeTreeNodeMode* pNode = new KModeTreeNodeMode;
	m_ModeNodeList.push_back(pNode);
	return pNode;
}


void KModeTree::RemoveModeNode(size_t nIndex)
{
	if((0 > nIndex) || (nIndex >= m_ModeNodeList.size()))
	{
		return;
	}

	std::list<KModeTreeNodeMode*>::iterator itNode, itEnd = m_ModeNodeList.end();
	for(itNode = m_ModeNodeList.begin(); itNode != itEnd; ++itNode)
	{
		if(0 == nIndex)
		{
			KModeTreeNodeMode* pNode = *itNode;
			delete pNode;
			m_ModeNodeList.erase(itNode);
			break;
		}
		else
		{
			--nIndex;
		}
	}
}


void KModeTree::RemoveModeNode(KModeTreeNodeMode* pNode)
{
	std::list<KModeTreeNodeMode*>::iterator itNode, itEnd = m_ModeNodeList.end();
	for(itNode = m_ModeNodeList.begin(); itNode != itEnd; ++itNode)
	{
		if(pNode == *itNode)
		{
			delete *itNode;
			m_ModeNodeList.erase(itNode);
			break;
		}
	}
}


void KModeTree::RemoveAllModeNode(void)
{
	std::list<KModeTreeNodeMode*>::iterator itNode, itEnd = m_ModeNodeList.end();
	for(itNode = m_ModeNodeList.begin(); itNode != itEnd; ++itNode)
	{
		KModeTreeNodeMode* pNode = *itNode;
		delete pNode;
	}
	m_ModeNodeList.clear();
}


size_t KModeTree::ModeNodeCount(void)
{
	return m_ModeNodeList.size();
}


KModeTreeNodeMode* KModeTree::ModeNode(size_t nIndex)
{
	KModeTreeNodeMode* pRet = NULL;
	if((0 > nIndex) || (nIndex >= m_ModeNodeList.size()))
	{
		return pRet;
	}

	std::list<KModeTreeNodeMode*>::iterator itNode, itEnd = m_ModeNodeList.end();
	for(itNode = m_ModeNodeList.begin(); itNode != itEnd; ++itNode)
	{
		if(0 == nIndex)
		{
			pRet = *itNode;
			break;
		}
		else
		{
			--nIndex;
		}
	}

	return pRet;
}


KModeTree& KModeTree::operator=(const KModeTree& r)
{
	RemoveAllModeNode();

	std::list<KModeTreeNodeMode*>::const_iterator itNode, itEnd = r.m_ModeNodeList.end();
	for(itNode = r.m_ModeNodeList.begin(); itNode != itEnd; ++itNode)
	{
		KModeTreeNodeMode* pModeNode = *itNode;
		KModeTreeNodeMode* pL = new KModeTreeNodeMode(*pModeNode);
		m_ModeNodeList.push_back(pL);
	}
	return *this;
}

