#pragma once
#include "ImChampMapDefineInfo.h"

// 시스템 전역
class ImChampMapDefineLabel
{
public:
    ImChampMapDefineLabel() {}
    ~ImChampMapDefineLabel(){}
public:
    static double   DefaultLabelSize;
    static COLORREF DefaultLabelColor;
    static CString  DefaultLabelFontName;

    static double   NodeLabelSize;
    static COLORREF NodeLabelColor;
    static CString  NodeLabelFontName;

    static double   LinkLabelSize;
    static COLORREF LinkLabelColor;
    static CString  LinkLabelFontName;
public:
    static void LoadPreferenceLabel();
    static void SavePreferenceLabel();
private:
    static void SavePreferenceLabel(int a_nLabelType, double a_dLabelSize, COLORREF a_rgbLabelColor, CString a_strLabelFontName);
};

class ImChampMapDefineSymbol
{
public:
	ImChampMapDefineSymbol(){}
	~ImChampMapDefineSymbol(){}
public:
	static TPreferenceSymbol DefaultSymbol;
	static TPreferenceSymbol NodeSymbol;
	static TPreferenceSymbol ZoneSymbol;

public:
	static void LoadPreferenceSymbol();
	static void SavePreferenceSymbol();
private:
	static void SavePreferenceSymbol(int a_nSymbolType, TPreferenceSymbol &a_oTPreferenceSymbol);
};