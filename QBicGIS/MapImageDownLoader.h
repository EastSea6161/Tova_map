#pragma once

class TParam;
/**
*@brief TileKey
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class TxTileKey
{
public:
    TxTileKey(int x, int y, int z) :Tx(x), Ty(y), Tz(z) {}
    ~TxTileKey() {}
public:
    int Tx;
    int Ty;
    int Tz;
public:
    bool operator <  (const TxTileKey& right) const {
        if (Tz == right.Tz) {
            if (Tx == right.Tx) {
                return Ty < right.Ty;
            } else {
                return Tx < right.Tx;
            }
        } else {
            return Tz < right.Tz;
        }
    }
};
/**
*@brief 다운로드 데이타
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class MapDownloadData
{
public:
    MapDownloadData() : Tx(0), Ty(0), Tz(0) {};
    virtual ~MapDownloadData(){};
public:
    CString TServerPath;
    CString TLocalPath;
public:
    int Tx;
    int Ty;
    int Tz;
};

/**
*@brief 맵 이미지 다운로더
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class MapImageDownloader
{
public:
    MapImageDownloader();
    virtual ~MapImageDownloader();
private:
    CCriticalSection Lock;
private:
    HANDLE m_hManualWork;
private:
	/**
	*@brief 다운로드 스레드 포르세스
	*@param void *pParam
	*@return 다운로드 반환
	*/
    static unsigned __stdcall DownloadThreadProcess(void *pParam); 
private:
	/**
	*@brief 다운로드 프로세스
	*@param TParam* pParam
	*@return 
	*@throw 디버그
	*/
    unsigned DownloadProcess(TParam* pParam);
private:
    std::map<int, MapDownloadData> m_mapDownLoadData;
public:
	/**
	*@brief 다운로드 데이타 설정
	*@param std::vector<MapDownloadData>& vecDownLoad 데이타 설정
	*/
    void SetDownloadData(std::vector<MapDownloadData>& vecDownLoad);
	/**
	*@brief 스레드 시작
	*/
    void Start();
    void StartDevide64(std::map<int, MapDownloadData> mapDownLoadData);
public:
    static std::map<TxTileKey, int> TMapImageTile;
};
/**
*@brief TParam
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class TParam
{
public:
    TParam() : Tx(0), Ty(0), Tz(0) {
        ::ZeroMemory(TServerPath, sizeof(TServerPath));
        ::ZeroMemory(TLocalPath,  sizeof(TLocalPath));
        ItsMe = nullptr;
    }
    ~TParam(){}
public:
    TCHAR TServerPath[512];
    TCHAR TLocalPath [512];
public:
    int Tx;
    int Ty;
    int Tz;
public:
    MapImageDownloader* ItsMe;
};