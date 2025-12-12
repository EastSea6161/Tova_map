#pragma once

/**
*@brief 동적메모리 삭제 및 포인터 초기화
*/
#ifndef QBicDelete
#define QBicDelete(p) {delete p; p = nullptr;}
#endif
