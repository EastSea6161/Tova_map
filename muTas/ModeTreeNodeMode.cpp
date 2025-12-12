/**
 * @file ModeTreeNodeMode.cpp
 * @brief KModeTreeNodeMode 구현 파일
 * @author 
 * @date 2011.06.08
 * @remark
 */


#include "StdAfx.h"
#include "ModeTreeNodeMode.h"
#include "Mode.h"



KModeTreeNodeMode::KModeTreeNodeMode(void)
	:	KModeTreeNode(KEMModeTreeNodeMode)
{
}


KModeTreeNodeMode::KModeTreeNodeMode(const KModeTreeNodeMode& r)
	:	KModeTreeNode(r)
{
	m_pMode = r.m_pMode;

	std::list<KModeTreeNode*>::const_iterator itNode, itEnd = r.m_NodeList.end();
	for(itNode = r.m_NodeList.begin(); itNode != itEnd; ++itNode)
	{
		KModeTreeNode* pNode = *itNode;
		if(pNode->NodeType() == KEMModeTreeNodeMode)
		{
			KModeTreeNodeMode* pModeNode = (KModeTreeNodeMode*)pNode;
			KModeTreeNodeMode* pLNode = new KModeTreeNodeMode(*pModeNode);
			m_NodeList.push_back(pLNode);
		}
	}
}


KModeTreeNodeMode::~KModeTreeNodeMode(void)
{
	RemoveAllNode();
}


KMode* KModeTreeNodeMode::Mode(void)
{
	return m_pMode;
}


void KModeTreeNodeMode::Mode(KMode* pMode)
{
	m_pMode = pMode;
}


LPCTSTR KModeTreeNodeMode::ModeName(void)
{
	ASSERT(NULL != m_pMode);
	return m_pMode->ModeName();
}


bool KModeTreeNodeMode::HasChild(void)
{
	return m_NodeList.empty() == false;
}


int KModeTreeNodeMode::ModeID(void)
{
	ASSERT(NULL != m_pMode);
	return m_pMode->ModeID();
}


KModeTreeNodeMode* KModeTreeNodeMode::AddModeNode(void)
{
	KModeTreeNodeMode* pNewNode = new KModeTreeNodeMode;
	m_NodeList.push_back(pNewNode);
	return pNewNode;
}


void KModeTreeNodeMode::RemoveNode(int nIndex)
{
	if((0 > nIndex) || (nIndex >= (int)m_NodeList.size()))
	{
		return;
	}

	std::list<KModeTreeNode*>::iterator itNode, itEnd = m_NodeList.end();
	for(itNode = m_NodeList.begin(); itNode != itEnd; ++itNode)
	{
		if(0 == nIndex)
		{
			KModeTreeNode* pNode = *itNode;
			delete pNode;
			m_NodeList.erase(itNode);
			break;
		}
		else
		{
			--nIndex;
		}
	}
}


void KModeTreeNodeMode::RemoveNode(KModeTreeNode* pNode)
{
	std::list<KModeTreeNode*>::iterator itNode, itEnd = m_NodeList.end();
	for(itNode = m_NodeList.begin(); itNode != itEnd; ++itNode)
	{
		if(pNode == *itNode)
		{
			delete *itNode;
			m_NodeList.erase(itNode);
			break;
		}
	}
}


void KModeTreeNodeMode::RemoveAllNode(void)
{
	std::list<KModeTreeNode*>::iterator itNode, itEnd = m_NodeList.end();
	for(itNode = m_NodeList.begin(); itNode != itEnd; ++itNode)
	{
		KModeTreeNode* pNode = *itNode;
		delete pNode;
	}

	m_NodeList.clear();
}


int KModeTreeNodeMode::NodeCount(void)
{
	return m_NodeList.size();
}


KModeTreeNode* KModeTreeNodeMode::Node(int nIndex)
{
	KModeTreeNode* pRet = NULL;
	if((0 > nIndex) || (nIndex >= (int)m_NodeList.size()))
	{
		return pRet;
	}

	std::list<KModeTreeNode*>::iterator itNode, itEnd = m_NodeList.end();
	for(itNode = m_NodeList.begin(); itNode != itEnd; ++itNode)
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


KModeTreeNodeMode* KModeTreeNodeMode::ParentNode(void)
{
	return m_pParent;
}


void KModeTreeNodeMode::ParentNode(KModeTreeNodeMode* pParent)
{
	m_pParent = pParent;
}


KModeTreeNodeMode& KModeTreeNodeMode::operator=(const KModeTreeNodeMode& r)
{
	KModeTreeNode::operator=(r);

	m_pMode = r.m_pMode;

	std::list<KModeTreeNode*>::const_iterator itNode, itEnd = r.m_NodeList.end();
	for(itNode = r.m_NodeList.begin(); itNode != itEnd; ++itNode)
	{
		KModeTreeNode* pNode = *itNode;
		if(pNode->NodeType() == KEMModeTreeNodeMode)
		{
			KModeTreeNodeMode* pModeNode = (KModeTreeNodeMode*)pNode;
			KModeTreeNodeMode* pLNode = new KModeTreeNodeMode(*pModeNode);
			m_NodeList.push_back(pLNode);
		}
	}
	return *this;
}
