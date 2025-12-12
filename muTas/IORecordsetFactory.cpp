#include "StdAfx.h"
#include "IORecordsetFactory.h"
//^^ #include "IODynamicrecordset.h"
//^^ #include "IOStaticRecordset.h"

KIORecordsetFactory::KIORecordsetFactory(void)
{
}

KIORecordsetFactory::~KIORecordsetFactory(void)
{
}

KIORecordset* KIORecordsetFactory::CreateRecordset(KIOTable* pTable, KEMIORecordsetType emRecordsetType)
{
	KIORecordset* pRecordset = NULL;

	switch(emRecordsetType)
	{
	case KEMIORecordsetTypeDynamic : pRecordset = new KIODynamicRecordset(pTable); break;
	case KEMIORecordsetTypeStatic  : pRecordset = new KIOStaticRecordset (pTable); break;
	}

	return pRecordset;
}