#pragma once

/**
*@brief 디렉터리 편집기능
*@details 디렉터리 생성, 복사, 삭제 작업
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicDir
{
public:
    QBicDir() {};
    ~QBicDir(){};

public:
	/**
	*@brief 디렉터리 생성
	*@param CString a_strPath	경로
	*@throws 작업 실패시 디버그 익셉션 실행
	*/
    static void  CreateDirectory(CString a_strPath);
	/**
	*@brief 디렉터리 제거
	*@details 경로에 있는 디렉터리를 제거
	*@param CString a_strPath	경로
	*@throws 작업 실패시 디버그 익셉션 실행
	*/
    static void  RemoveDirectory(CString a_strPath);
	/**
	*@brief 디렉터리 복사
	*@details 디렉터리를 복사 한 후 파일의 속성을 검사한다.
	*@param CString a_strSourcePath	소스 경로
	*@param CString a_strDestPath	Dest 경로
	*@param bool a_bSubDirCopy		디렉토리 복사 여부
	*@throws 작업 실패시 디버그 익셉션 실행 
	*/
    static void  CopyDirectory  (CString a_strSourcePath, CString a_strDestPath, bool a_bSubDirCopy);

public:
	/**
	*@brief 이전버전 특별 폴더 조회
	*@details 윈도우에서 관리하는 특별한 폴더를 조회한다(윈도우 Vista 이전 버전)
	*@param HWND hwndOwner	윈도우
	*@param int CSIDL		버전
	*@return 경로 전체를 반환 한다.
	*/
    static CString WindowsSpecialFolderPath(HWND hwndOwner, int CSIDL);
	/**
	*@brief 이전버전 특별 폴더 조회
	*@details 윈도우에서 관리하는 특별한 폴더를 조회한다(윈도우 Vista 이후 버전)
	*@param GUID ID	Gu ID
	*@return 경로 전체를 반환 한다.
	*/
    static CString WindoswsGetKnownFolderPath (GUID ID);

public:
	/**
	*@brief 파일들의 경로정보
	*@details 현재 폴더에 있는 파일들의 경로 정보, 서브 폴더의 정보는 반환하지 않는다.
	*@param CString strPath	경로
	*@return 파일들의 경로정보 반환
	*@throws 디버그 익셉션
	*/
    static std::vector<CString> DirFiles(CString strPath);

public:
	/**
	*@brief 디렉터리 생성
	*@details 2015년 8월 6일 이후 디렉터리 생성
	*@param CString strPath 경로
	*@return 결과값을 반환한다.
	*@throws 디버그 익셉션 실행
	*/
    static int CreateDir(CString strPath);
};