#pragma once

class KTarget;

class ImChampFileExist
{
public:
    ImChampFileExist () {}
    ~ImChampFileExist() {}

public:
    static bool IsExistBushFileMessage(KTarget* a_pTarget);
    static bool IsExistMultiModalFileMessage(KTarget* pTarget);
	static bool IsExistInterModalGeneraterFileMessage(KTarget* a_pTarget);

public:
    static bool BushFileCheckMessage(KTarget* pTarget);
    static bool MultiModalFileCheckMessage(KTarget* pTarget);
    static bool InterModalFileCheckMessage(KTarget* pTarget);
};