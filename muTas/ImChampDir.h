#pragma once
class KTarget;

class ImChampDir
{
public:
	ImChampDir(void);
	~ImChampDir(void);
public:
	static CString SetCurrentDirectoryToApp();
	static CString GetAppDirectory();
public:
    static CString SetCurDirToTarget       (KTarget* a_pTarget);
    static CString SetCurDirToTargetSubDir (KTarget* a_pTarget, CString a_strFolder);
    static CString SetCurDirToTargetModel  (KTarget* a_pTarget);
    static CString GetCurDir();
public:
    static CString GetScenarioDirectory(KTarget* a_pTarget);
    static CString GetProjectDirectory (KTarget* a_pTarget);

public:
    static void    CreateDirectory(CString a_strPath);
    static void    RemoveDirectory(CString a_strPath, QBicSimpleProgressPtr spSimpleProgress);
    static void    CopyDirectory  (CString a_strSourcePath, CString a_strDestPath, bool a_bSubDirCopy, QBicSimpleProgressPtr spSimpleProgress);
	static void    CopyDirectoryWinSimpleProgress(CString a_strFromDir, CString a_strToDir);
};

