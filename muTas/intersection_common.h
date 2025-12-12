/**
 * @file intersection_common.h
 * @brief timeod의 공통 type 정의 파일
 * @author jyk@qbicware.com
 * @date 2012.06.08
 * @remark
 */

#pragma once

enum KEMIntersectionType
{
	KEMIntersectionTypeThree   = 3,
	KEMIntersectionTypeFour    = 4
};

#define CROSS_TYPE_A 1	// 'ㅏ'
#define CROSS_TYPE_E 2	// 'ㅓ'
#define CROSS_TYPE_O 3	// 'ㅗ'
#define CROSS_TYPE_U 4  // 'ㅜ'

struct TBoundInfo
{
	Integer nxNodeID;
	int     nLaneCnt;
	Integer nxLinkID;
};

struct TIntersectionBase
{
	Integer nxNodeID;
	int     nType;
	Integer nxFirstBNode;
	Integer nxSecondBNode;
	Integer nxThirdBNode;
	Integer nxFourthBNode;
	int     nCrossType;
};

struct TSignalIndication
{
	int     nSignalSeq;
	int     nSignalLampType;
	int     nGreenLampDuration;
	int     nYellowLampDuration;
};


struct TIntersectionInfo
{
	Integer nxNodeID;
	int     nBoundDirection;
	int     nLeftTurnCnt;
	int     nThrowCnt;
	int     nRightTurnCnt;
	int     nSignalType;
	int     nLeftTurnType;
	int     nRightTurnType;
};


enum KEMIntersectionInfoSeq
{
	KEMInterSeqLeftTurnCnt       = 0,
	KEMInterSeqThrowCnt          = 1,
	KEMInterSeqRightTurnCnt      = 2,
	KEMInterSeqSignalType        = 3,
	KEMInterSeqLeftTurnType      = 4,
	KEMInterSeqRightTurnType     = 5
};


struct TExtension
{
	Integer nxNodeID;
	int     nBoundDirection;
	int     nBusStop;
	int     nBusActivity;
	int     nBusStopLocation;
	int     nParkingPermission;
	int     nParkActivity;
	int     nPedstrian;
	int     nOffset;
};


enum KEMExtensionInfoSeq
{
	KEMExSeqBusStop           = 0,
	KEMExSeqBusActivity       = 1,
	KEMExSeqBusStopLocation   = 2,
	KEMExSeqParkingPermission = 3,
	KEMExSeqParkActivity      = 4,
	KEMExSeqPedstrian         = 5,
	KEMExSeqOffset            = 6
};

/*Bound_direction Code 정의*/
enum KEMBoundDirectionCode
{
	KEMBoundDirectionCode1B = 1,
	KEMBoundDirectionCode2B = 2,
	KEMBoundDirectionCode3B = 3,
	KEMBoundDirectionCode4B = 4
};

// enum KEMSignalLampType
// {
// 	KEMDirSeq_NONE   = -1,
// 	KEMDirSeq_EW_TH  = 1,	// 동서 양방향 직진
// 	KEMDirSeq_EW_LT  = 2,	// 동서 양방향 좌회전
// 	KEMDirSeq_NS_TH  = 3,	// 남북 양방향 직진
// 	KEMDirSeq_NS_LT  = 4,	// 남북 양방향 좌회전
// 	KEMDirSeq_E_THLT = 5,	// 동쪽 방향 직좌
// 	KEMDirSeq_W_THLT = 6,	// 서쪽 방향 직좌
// 	KEMDirSeq_N_THLT = 7,	// 북쪽 방향 직좌
// 	KEMDirSeq_S_THLT = 8,	// 남쪽 방향 직좌
// 	KEMDirSeq_E_TH   = 9,	// 동쪽 방향 직진
// 	KEMDirSeq_W_TH   = 10,	// 서쪽 방향 직진
// 	KEMDirSeq_N_TH   = 11,	// 북쪽 방향 직진
// 	KEMDirSeq_S_TH   = 12,	// 남쪽 방향 직진
// 	KEMDirSeq_E_LT   = 13,	// 동쪽 방향 좌회전
// 	KEMDirSeq_W_LT   = 14,	// 서쪽 방향 좌회전
// 	KEMDirSeq_N_LT   = 15,	// 북쪽 방향 좌회전
// 	KEMDirSeq_S_LT   = 16,	// 남쪽 방향 좌회전
// 	KEMDirSeq_EW_NoProtectLT = 17,	// 동서 양방향 비보호 좌회전
// 	KEMDirSeq_NS_NoProtectLT = 18,	// 남북 양방향 비보호 좌회전
// 	KEMDirSeq_AllRed = 19	// All Red
// };

enum KEMSignalLampType
{
	KEMDirSeq_NONE   = -1,
	KEMDirSeq_EW_TH  = 9,	// 동서 양방향 직진
	KEMDirSeq_EW_LT  = 11,	// 동서 양방향 좌회전
	KEMDirSeq_NS_TH  = 10,	// 남북 양방향 직진
	KEMDirSeq_NS_LT  = 12,	// 남북 양방향 좌회전
	KEMDirSeq_E_THLT = 13,	// 동쪽 방향 직좌
	KEMDirSeq_W_THLT = 14,	// 서쪽 방향 직좌
	KEMDirSeq_N_THLT = 15,	// 북쪽 방향 직좌
	KEMDirSeq_S_THLT = 16,	// 남쪽 방향 직좌
	KEMDirSeq_E_TH   = 1,	// 동쪽 방향 직진
	KEMDirSeq_W_TH   = 2,	// 서쪽 방향 직진
	KEMDirSeq_N_TH   = 3,	// 북쪽 방향 직진
	KEMDirSeq_S_TH   = 4,	// 남쪽 방향 직진
	KEMDirSeq_E_LT   = 5,	// 동쪽 방향 좌회전
	KEMDirSeq_W_LT   = 6,	// 서쪽 방향 좌회전
	KEMDirSeq_N_LT   = 7,	// 북쪽 방향 좌회전
	KEMDirSeq_S_LT   = 8,	// 남쪽 방향 좌회전
	KEMDirSeq_EW_NoProtectLT = 17,	// 동서 양방향 비보호 좌회전
	KEMDirSeq_NS_NoProtectLT = 18,	// 남북 양방향 비보호 좌회전
	KEMDirSeq_AllRed = 19	// All Red
};

/*현시 방향에 사용 되는 방향 이미지파일 ID 정의*/
struct TDirectionImgID
{
	UINT uiBmpResId;		// 비선택시 리소스 ID
	UINT uiBmpHotResId;		// 선택시   리소스 ID
	UINT uiBmpDsblResId;	// 비활성   리소스 ID
};