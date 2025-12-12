#pragma once

/**
 * Import KTDB ÀÇ status
 */
enum KEMImportKTDB
{
	KEMImportNone = 0,
	KEMImportCopy,
	KEMImportInterpolation,
	KEMImportExtrapolation
};


enum KEMProjectType
{
	KEMKTDBProject = 1,
	KEMUserDefineProject
};