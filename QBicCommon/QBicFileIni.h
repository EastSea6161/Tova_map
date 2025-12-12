#pragma once
#include "QBicFlashIni.h"
/**
*@brief Ini 파일 작업
*@details Ini 파일의 저장, 불러오기 작업
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicFileIni : public QBicIni
{
public:
    QBicFileIni(CString strPath);
    virtual ~QBicFileIni();
private:
    CString m_strPath;
private:
	/**
	*@brief Ini 정보 불러오기
	*@details TFileIni의 테이블에서 키값, 벨류값을 불러온다.
	*/
    void Load();
	/**
	*@brief Ini 정보 입력
	*@details TFileIni의 테이블에서 키값, 벨류값을 입력한다.
	*/
    void Save();

private:
	/**
	*@brief Ini파일 관련 작업
	*@details Ini 파일 관련 테이블 생성(키, 벨류)
	*@param CString strPath	경로
	*@return 데이터베이스 정보를 반환
	*@throws 데이터베이스의 초기화를 반환 
	*/
    KDBaseConnectionPtr IniConnect(CString strPath);    
};

typedef std::shared_ptr<QBicFileIni> QBicFileIniPtr;
/**
*@brief grp 파일 작업
*@details grp 파일의 저장, 불러오기 작업
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicFileGrpIni : public QBicGrpIni
{
public:
    QBicFileGrpIni(CString strPath);
    virtual ~QBicFileGrpIni();
private:
    CString m_strPath;
private:
	/**
	*@brief grpIni 정보 불러오기
	*@details TFileIni의 테이블에서 타입, 키값, 벨류값을 불러온다.
	*/
    void Load();
	/**
	*@brief grpIni 정보 입력
	*@details TFileIni의 테이블에서 타입, 키값, 벨류값을 입력한다.
	*/
    void Save();

private:
	/**
	*@brief Ini파일 관련 작업
	*@details Ini 파일 관련 테이블 생성(타입, 키, 벨류)
	*@param CString strPath 경로
	*@return 데이터베이스 정보를 반환
	*@throws 데이터베이스의 초기화를 반환 
	*/
    KDBaseConnectionPtr IniConnect(CString strPath);    
};

typedef std::shared_ptr<QBicFileGrpIni> QBicFileGrpIniPtr;