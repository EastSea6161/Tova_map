#pragma once
//^^ #include "IOColumns.h"

struct TDrawLabelInfo
{   
    Integer  TFeatureID;    
    CString  TLabel;
};

enum KEMLabelDisplayStatus
{
	KEMLabelDisplayStatusNone  = -1,
	KEMLabelDisplayStatusOff   = 0,
	KEMLabelDisplayStatusOn    = 1
};

struct TNodeLabelInfo 
{
	KEMLabelDisplayStatus emLabelDisplay;	/*라벨 표출 유무*/
	KIOColumns oNodeLabelColumns;
};

struct TLinkLabelInfo 
{
	KEMLabelDisplayStatus emLabelDisplay;	/*라벨 표출 유무*/
	KIOColumns oLinkLabelColumns;
};