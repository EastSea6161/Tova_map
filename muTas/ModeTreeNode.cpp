/**
 * @file ModeTreeNode.cpp
 * @brief KModeTreeNode 구현 파일
 * @author 
 * @date 2011.06.03
 * @remark
 */


#include "StdAfx.h"
#include "ModeTreeNode.h"



KModeTreeNode::KModeTreeNode(KEMModeTreeNode emNodeType)
	:	m_emNodeType(emNodeType)
{
}

KModeTreeNode::KModeTreeNode(const KModeTreeNode& r)
	:	m_emNodeType(r.m_emNodeType)
{

}

KModeTreeNode::~KModeTreeNode(void)
{

}


KEMModeTreeNode KModeTreeNode::NodeType(void)
{
	return m_emNodeType;
}


KModeTreeNode& KModeTreeNode::operator=(const KModeTreeNode& r)
{
	m_emNodeType = r.m_emNodeType;
	return *this;
}

