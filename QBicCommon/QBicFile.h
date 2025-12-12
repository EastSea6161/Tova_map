#pragma once
/**
*@brief File 편집 기능
*@details File의 불러오기, 삭제, 복사
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicFile
{
public:
    QBicFile()  {};
    ~QBicFile() {};

public:
	/**
	*@brief 파일 존재 확인
	*@param CString strFilePath 파일 경로
	*@return 성공이면 ture, 실패면 false 반환
	*/
    static bool FileExist (CString strFilePath);
	/**
	*@brief 파일 삭제
	*@param CString strFilePath 파일 경로
	*/
    static void DeleteFile(CString strFilePath);
	/**
	*@brief 파일 복사
	*@details 원래파일경로, 파일복사경로
	*@param CString strSrcPath	Src 경로
	*@param CString strDestPath	Dest 경로
	*@return 성공이면 ture, 실패면 false 반환
	*/
    static bool CopyFile  (CString strSrcPath, CString strDestPath);

public:
	/**
	*@brief sqlite파일경로
	*@details sqlite의 전체 경로
	*@param CString strPath	경로
	*@return 전체 경로를 리턴
	*/
    static std::vector<CString> SplitFilePath(CString strPath); // SplitFilePath : 파일을 디렉토리, 경로, 이름, 확장자로 분리
};

