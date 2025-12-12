#pragma once

class TNodeSelectionInfo
{
public:
	TNodeSelectionInfo()
	{
        TNodeID       = -1;
        TType           = 0;
        TCharacterIndex = 74;
        TSize           = 15;
        TColorRGB       = NODE_POINT_COLOR;
        TLabel          = _T("");
	}
	~TNodeSelectionInfo() {}

public:
	Integer  TNodeID;
	int      TCharacterIndex;
	double   TSize;
	COLORREF TColorRGB;    
	CString  TLabel;
	int      TType; 
};