#pragma once
#include "afxwin.h"
/**
*@brief 사진 편집기능
*@details 사진 편집 기능
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicPictureEx : public CStatic
{
public:

    struct TFrame    // structure that keeps a single frame info
    {
        IPicture *m_pPicture;  // pointer to the interface used for drawing
        SIZE     m_frameSize;
        SIZE     m_frameOffset;
        UINT     m_nDelay;     // delay (in 1/100s of a second)
        UINT     m_nDisposal;  // disposal method
    };

#pragma pack(1)   // turn byte alignment on

    enum GIFBlockTypes
    {
        BLOCK_UNKNOWN,
        BLOCK_APPEXT,
        BLOCK_COMMEXT,
        BLOCK_CONTROLEXT,
        BLOCK_PLAINTEXT,
        BLOCK_IMAGE,
        BLOCK_TRAILER
    };

    enum ControlExtValues // graphic control extension packed field values
    {
        GCX_PACKED_DISPOSAL,  // disposal method
        GCX_PACKED_USERINPUT,
        GCX_PACKED_TRANSPCOLOR
    };

    enum LSDPackedValues  // logical screen descriptor packed field values
    {
        LSD_PACKED_GLOBALCT,
        LSD_PACKED_CRESOLUTION,
        LSD_PACKED_SORT,
        LSD_PACKED_GLOBALCTSIZE
    };

    enum IDPackedValues   // image descriptor packed field values
    {
        ID_PACKED_LOCALCT,
        ID_PACKED_INTERLACE,
        ID_PACKED_SORT,
        ID_PACKED_LOCALCTSIZE
    };

    struct TGIFHeader       // GIF header  
    {
        char m_cSignature[3]; // Signature - Identifies the GIF Data Stream
        // This field contains the fixed value 'GIF'
        char m_cVersion[3];	// Version number. May be one of the following:
        // "87a" or "89a"
    };

    struct TGIFLSDescriptor // Logical Screen Descriptor
    {
        WORD m_wWidth;	// 2 bytes. Logical screen width
        WORD m_wHeight; // 2 bytes. Logical screen height

        unsigned char m_cPacked;      // packed field	

        unsigned char m_cBkIndex;     // 1 byte. Background color index
        unsigned char m_cPixelAspect; // 1 byte. Pixel aspect ratio
        inline int GetPackedValue(enum LSDPackedValues Value);
    };

    struct TGIFAppExtension // application extension block
    {
        unsigned char m_cExtIntroducer; // extension introducer (0x21)
        unsigned char m_cExtLabel; // app. extension label (0xFF)
        unsigned char m_cBlockSize; // fixed value of 11
        char m_cAppIdentifier[8];   // application identifier
        char m_cAppAuth[3];  // application authentication code
    };

    struct TGIFControlExt // graphic control extension block
    {
        unsigned char m_cExtIntroducer; // extension introducer (0x21)
        unsigned char m_cControlLabel;  // control extension label (0xF9)
        unsigned char m_cBlockSize; // fixed value of 4
        unsigned char m_cPacked;    // packed field
        WORD m_wDelayTime;	// delay time
        unsigned char m_cTColorIndex; // transparent color index
        unsigned char m_cBlockTerm;   // block terminator (0x00)
    public:
        inline int GetPackedValue(enum ControlExtValues Value);
    };

    struct TGIFCommentExt  // comment extension block
    {
        unsigned char m_cExtIntroducer; // extension introducer (0x21)
        unsigned char m_cCommentLabel;  // comment extension label (0xFE)
    };

    struct TGIFPlainTextExt // plain text extension block
    {
        unsigned char m_cExtIntroducer;  // extension introducer (0x21)
        unsigned char m_cPlainTextLabel; // text extension label (0x01)
        unsigned char m_cBlockSize; // fixed value of 12
        WORD m_wLeftPos;    // text grid left position
        WORD m_wTopPos;     // text grid top position
        WORD m_wGridWidth;  // text grid width
        WORD m_wGridHeight; // text grid height
        unsigned char m_cCellWidth;  // character cell width
        unsigned char m_cCellHeight; // character cell height
        unsigned char m_cFgColor; // text foreground color index
        unsigned char m_cBkColor; // text background color index
    };

    struct TGIFImageDescriptor // image descriptor block
    {
        unsigned char m_cImageSeparator; // image separator byte (0x2C)
        WORD m_wLeftPos; // image left position
        WORD m_wTopPos;  // image top position
        WORD m_wWidth;   // image width
        WORD m_wHeight;  // image height
        unsigned char m_cPacked; // packed field
        inline int GetPackedValue(enum IDPackedValues Value);
    };

#pragma pack() // turn byte alignment off

public:
	/**
	*@brief 그림 추출
	*@param RECT *lpRect
	*@return 그림의 추출 결과 반환
	*/
    BOOL GetPaintRect(RECT *lpRect);
	/**
	*@brief 그림 설정
	*@param const RECT *lpRect
	*@return 그림의 설정 결과 반환
	*/
    BOOL SetPaintRect(const RECT *lpRect);
    QBicPictureEx();
    virtual ~QBicPictureEx();
	/**
	*@brief 움직이는 에니메이션 정지
	*/
    void Stop();   // stops animation
	/**
	*@brief 움직이는 에니메이션 정지, 다른 모든것들 초기화
	*/
    void UnLoad(); // stops animation plus releases all resources
	/**
	*@brief GIF파일인지 확인
	*@return 참 거짓 반환
	*/
    BOOL IsGIF() const;
	/**
	*@brief 재생중인지 확인
	*@return 참 거짓 반환
	*/
    BOOL IsPlaying() const;
	/**
	*@brief 에니메이션GIF파일인지 확인
	*@return 참 거짓 반환
	*/
    BOOL IsAnimatedGIF() const;
	/**
	*@brief 사이즈 추출
	*@return 사이즈크기 반환
	*/
    SIZE GetSize() const;
	/**
	*@brief 애니메이션GIF의 크기
	*@return 크기 반환
	*/
    int GetFrameCount() const;
	/**
	*@brief 배경색 추출
	*@return 배경색 반환
	*/
    COLORREF GetBkColor() const;
	/**
	*@brief 배경색 설정
	*@param COLORREF clr 배경색
	*/
    void SetBkColor(COLORREF clr);

    // draws the picture (starts an animation thread if needed)
    // if an animation was previously stopped by Stop(),
    // continues it from the last displayed frame
	/**
	*@brief 에니메이션을 실행
	*@return 실행시 TURE, 실패시 FALSE를 반환
	*/
    BOOL Draw();

    // loads a picture from a file
    // i.e. Load(_T("mypic.gif"));
	/**
	*@brief 사진 불러오기
	*@return 사진을 Load하는 것으로 반환98
	*@param LPCTSTR szFileName 파일 명
	*/
    BOOL Load(LPCTSTR szFileName);

    // loads a picture from a global memory block (allocated by GlobalAlloc)
    // Warning: this function DOES NOT free the global memory, pointed to by hGlobal
    BOOL Load(HGLOBAL hGlobal, DWORD dwSize);

    // loads a picture from a program resource
    // i.e. Load(MAKEINTRESOURCE(IDR_MYPIC),_T("GIFTYPE"));
    BOOL Load(LPCTSTR szResourceName,LPCTSTR szResourceType);

protected:

#ifdef GIF_TRACING
    void EnumGIFBlocks();
    void WriteDataOnDisk(CString szFileName, HGLOBAL hData, DWORD dwSize);
#endif // GIF_TRACING

    RECT m_PaintRect;
    SIZE m_PictureSize;
    COLORREF m_clrBackground;
    UINT m_nCurrFrame;
    UINT m_nDataSize;
    UINT m_nCurrOffset;
    UINT m_nGlobalCTSize;
    BOOL m_bIsGIF;
    BOOL m_bIsPlaying;
    volatile BOOL m_bExitThread;
    BOOL m_bIsInitialized;
    HDC m_hMemDC;

    HDC m_hDispMemDC;
    HBITMAP m_hDispMemBM;
    HBITMAP m_hDispOldBM;

    HBITMAP m_hBitmap;
    HBITMAP m_hOldBitmap;
    HANDLE m_hThread;
    HANDLE m_hExitEvent;
    IPicture * m_pPicture;
    TGIFHeader * m_pGIFHeader;
    unsigned char * m_pRawData;
    TGIFLSDescriptor * m_pGIFLSDescriptor;
    std::vector<TFrame> m_arrFrames;

	/**
	*@brief 애니메이션 재생
	*/
    void ThreadAnimation();
	/**
	*@brief 애니메이션 재생확인
	*@param LPVOID pParam 
	*@return 종료시 0을 반환
	*/
    static UINT WINAPI _ThreadAnimation(LPVOID pParam);

    int GetNextBlockLen() const;
    BOOL SkipNextBlock();
    BOOL SkipNextGraphicBlock();
    BOOL PrepareDC(int nWidth, int nHeight);
	/**
	*@brief 데이터 초기화
	*/
    void ResetDataPointer();
	/**
	*@brief 다음 영역 확인
	*@return 블럭 상태 반환
	*/
    enum GIFBlockTypes GetNextBlock() const;
	/**
	*@brief 
	*@param UINT nStartingOffset
	*@return UINT의 값 반환
	*/
    UINT GetSubBlocksLen(UINT nStartingOffset) const;
    HGLOBAL GetNextGraphicBlock(UINT *pBlockLen, UINT *pDelay, 
        SIZE *pBlockSize, SIZE *pBlockOffset, UINT *pDisposal);

    // Generated message map functions
    //{{AFX_MSG(CPictureEx)
	/**
	*@brief 종료시 해제작업 및 닫기
	*/
    afx_msg void OnDestroy();
	/**
	*@brief 시작
	*/
    afx_msg void OnPaint();
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

