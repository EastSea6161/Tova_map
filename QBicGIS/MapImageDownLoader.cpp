#include "stdafx.h"
#include "MapImageDownLoader.h"

MapImageDownloader::MapImageDownloader()
{
    m_hManualWork = CreateEvent(NULL, /*수동-Reset*/TRUE, FALSE, NULL); //★ 종료는 모든 Worker가 받아야 한다.
}

MapImageDownloader::~MapImageDownloader()
{
    if (m_hManualWork != nullptr) {
        ::CloseHandle(m_hManualWork);
    }        
}

void MapImageDownloader::SetDownloadData( std::vector<MapDownloadData>& vecDownLoad )
{
    Lock.Lock();

    if (TMapImageTile.size() >= 1000) {
        TMapImageTile.clear();
    }

    m_mapDownLoadData.clear();

    for (size_t i=0; i<vecDownLoad.size(); i++) {
        int nID = (int)i;
        MapDownloadData& oData = vecDownLoad[i];

        int& nLoad = TMapImageTile[TxTileKey(oData.Tx, oData.Ty, oData.Tz)];
        if (nLoad == 0/*최초*/ || nLoad == 5/*실패*/) {
            m_mapDownLoadData.insert(std::make_pair(nID, oData));
            nLoad = 1 /*요청*/;
        } else if (nLoad == 100/*성공*/) {
            // 완벽하게 체크 하자(외부에서 삭제한 경우 대비)
            struct _stat statFile;
            int nError = _tstat(oData.TLocalPath, &statFile);
            if (nError != 0) {
                m_mapDownLoadData.insert(std::make_pair(nID, oData));
                nLoad = 1/*요청*/;
            }
        } else {
            nLoad++  /*요청*/;
        }
    }

    Lock.Unlock();
}

void MapImageDownloader::Start()
{    
    if (m_mapDownLoadData.size() == 0)
        return;
    
    Lock.Lock();
    int nDevide(50); //★ 50개 단위로 다운로딩 ** MAXIMUM_WAIT_OBJECTS(64개) **
    int nCount = (int)m_mapDownLoadData.size();
    
    if (nCount <= nDevide) {
        StartDevide64(m_mapDownLoadData);
        Lock.Unlock();
        return;
    }
    
    int nLoop(0);
    std::map<int, MapDownloadData> mapDownLoadData;
    for (auto iter = m_mapDownLoadData.begin(); iter != m_mapDownLoadData.end(); ++iter) 
    {
        nLoop++;
        mapDownLoadData.insert(std::make_pair(iter->first, iter->second));

        if (nLoop >= nDevide) 
        {
            StartDevide64(mapDownLoadData);
            nLoop = 0;
            mapDownLoadData.clear();            
        }
    }

    if (mapDownLoadData.size() > 0) {
        StartDevide64(mapDownLoadData);
    }

    Lock.Unlock();
}

void MapImageDownloader::StartDevide64( std::map<int, MapDownloadData> mapDownLoadData )
{
    try
    {
        int nCount = (int)mapDownLoadData.size();
        if (nCount == 0)
            return;

        HANDLE* hEvents = new HANDLE[nCount];

        int i(0);
        for (auto iter = mapDownLoadData.begin(); iter != mapDownLoadData.end(); ++iter) 
        {
            int nIndex = iter->first;
            MapDownloadData& oData = iter->second;
            unsigned int nThreadID = 0;

            TParam* pParam = new TParam();    
            pParam->ItsMe = this;
            pParam->Tx = oData.Tx;
            pParam->Ty = oData.Ty;
            pParam->Tz = oData.Tz;

            _tcscpy(pParam->TServerPath, oData.TServerPath.GetBuffer(0));
            oData.TServerPath.ReleaseBuffer();

            _tcscpy(pParam->TLocalPath, oData.TLocalPath.GetBuffer(0));
            oData.TLocalPath.ReleaseBuffer();

            //★ Thread를 n개를 실행함. -> 각 Thread는 대기 상태로 있음.
            hEvents[i] = (HANDLE)::_beginthreadex(NULL, 0, DownloadThreadProcess, pParam, 0, &nThreadID);
            i++;
        }

        //★ 대기 상태의 Thread를 모두 깨운다(Send DownLoad Event)
        SetEvent(m_hManualWork);
        //★ 기다린다. Timeout - 2초정도 그리고 MAXIMUM_WAIT_OBJECTS(64개) ** 64개가 초과한 경우 I don't know
        DWORD dwState = ::WaitForMultipleObjects(nCount, hEvents, TRUE, 2 * 1000/*INFINITE*/);               
        if(dwState == WAIT_TIMEOUT) {
            TxLogDebugVisitor();
        }

        for (int i=0; i<nCount; i++) {
            //★ Thread는 아직 살아있을 수 있다.(TerminateThread)
            ::CloseHandle(hEvents[i]);
        }

        //★ Event 해제 : 수동 Reset
        ResetEvent(m_hManualWork);
        delete[] hEvents;
    }
    catch (...)
    {
        TxLogDebugException();
    }
}

unsigned __stdcall MapImageDownloader::DownloadThreadProcess( void *pParam )
{
    if (pParam == nullptr)
        return 1;

    TParam* pParameter = (TParam*)(pParam);        
    MapImageDownloader* pCaller = pParameter->ItsMe;
    return pCaller->DownloadProcess(pParameter);
}

unsigned MapImageDownloader::DownloadProcess( TParam* pParameter )
{
    try
    {
        ::WaitForSingleObject(m_hManualWork, INFINITE);               
          
        bool bDownload(false);
        struct _stat statFile;
        int nError = _tstat(pParameter->TLocalPath, &statFile);

        if (nError != 0) {
            bDownload = true;
        } else {
            struct tm tmNow; {
                time_t ttNow = time(nullptr);
                localtime_s(&tmNow, &ttNow); 
            }

            struct tm tmFile; {
                localtime_s(&tmFile, &statFile.st_mtime); 
            }

            if (tmNow.tm_mon != tmFile.tm_mon) {
                bDownload = true;
            } 
        }

        int& nLoad = MapImageDownloader::TMapImageTile[TxTileKey(pParameter->Tx, pParameter->Ty, pParameter->Tz)];

        if (bDownload == true) {
            HRESULT hr = ::URLDownloadToFile(nullptr, pParameter->TServerPath, pParameter->TLocalPath, 0, nullptr);
            if (FAILED(hr)) {
                nLoad = 5; /*실패*/
                TxLogDebugException();
            } else {
                nLoad = 100/*성공*/;
            }
        } else {
            nLoad = 100/*성공*/;
        }

        try {
            DeleteNull(pParameter);
        } catch (...) {
            TxLogDebugException();
        }
    } catch(...)  {
        TxLogDebugException();
    }

    return 1;
}

std::map<TxTileKey, int> MapImageDownloader::TMapImageTile;