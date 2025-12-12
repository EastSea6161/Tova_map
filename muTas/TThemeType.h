#pragma once

struct TNodeTypeRecord
{
	int      TCharacterIndex;
	double   TSize;
	COLORREF TColorRGB;    
	CString  TLabel;
};

struct TThemeResultSettingInfo
{
	double dHighValue;
	double dLowValue;
	double dLowSize;
	double dHighSize;

	COLORREF clrDefault;
	COLORREF clrOther;

	double dOutSize;
};

enum KEMThemeRangeIndex
{
	 KEMThemeRangeIndexFirst  = 0,
	 KEMThemeRangeIndexLast   = 1,
	 KEMThemeRangeIndexNormal = 2
};