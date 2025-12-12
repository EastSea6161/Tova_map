#pragma once

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include "model_common.h"

struct TAOBMultiVDF
{
	bool TbUseVDf;
	std::map<int, int> TmapVDFModeMating;
	int TnStandardVDF;
	std::map<int, double> TmapVDFValue;
	bool TbPamAlBeta;
	bool TbPamabToll;
	bool TbpamIntialTime;
	KEMAssignmentBPRFunctionType TemEachFunctionType;
	std::vector<std::vector<CString>> TvecFucntionType;
	std::vector<CString> TvecEachLinkColumn;
	CString TstrAlpha;
	CString TstrBeta;
	CString TA;
	CString TB;
};

struct TAOBSingleVDF
{
	bool TbUseVDf;
	double TdValueofTime;
	KEMAssignmentBPRFunctionType TemFunctionType;
	double TdAllAlpha;
	double TdAllBeta;
	std::map<int, KSDoubleRecord> TmapEachFunction;
	std::vector<std::vector<CString>> TvecFucntionType;
	CString TstrAlpha;
	CString TstrBeta;
	CString TA;
	CString TB;
	CString TstrCaptionAlpha;
	CString TstrCaptionBeta;
	CString TstrCaptionA;
	CString TstrCaptionB;
	int     TnEachFnCombo;
	std::map<int, std::vector<double>> TmapEachFN;
};