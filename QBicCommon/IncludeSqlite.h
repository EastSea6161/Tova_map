#pragma once

#include "../QBicExtStaticCLib/SQLite3Lib/sqlite3.h"


#define Sqlite              sqlite3
#define SqliteStatement     sqlite3_stmt

#if defined( _UNICODE )
#ifndef SqliteOpen
#define SqliteOpen		    sqlite3_open_v2/*sqlite3_open16*/
#endif
#ifndef SqlitePrepare
#define SqlitePrepare		sqlite3_prepare16
#endif
#ifndef SqlitePrepareV2
#define SqlitePrepareV2		sqlite3_prepare16_v2
#endif
#ifndef SqliteColumnText
#define SqliteColumnText	(wchar_t*)sqlite3_column_text16
#endif
#ifndef SqliteErrorMsg
#define SqliteErrorMsg	    (wchar_t*)sqlite3_errmsg16
#endif
#ifndef SqliteTail
#define SqliteTail		    const void*
#endif
#ifndef SqliteBindText
#define SqliteBindText      sqlite3_bind_text16
#endif
#else
#ifndef SqliteOpen
#define SqliteOpen		    sqlite3_open
#endif
#ifndef SqlitePrepare
#define SqlitePrepare		sqlite3_prepare
#endif
#ifndef SqlitePrepareV2
#define SqlitePrepareV2		sqlite3_prepare_v2
#endif
#ifndef SqliteColumnText
#define SqliteColumnText	sqlite3_column_text
#endif
#ifndef SqliteErrorMsg
#define SqliteErrorMsg		sqlite3_errmsg
#endif
#ifndef SqliteTail
#define SqliteTail		    const char* 
#endif
#ifndef SqliteBindText
#define SqliteBindText      sqlite3_bind_text
#endif
#endif

#ifndef SqliteBindBlob
#define SqliteBindBlob sqlite3_bind_blob
#endif

#ifndef SqliteBindNull
#define SqliteBindNull sqlite3_bind_null
#endif

#ifndef SqliteColumnInt
#define SqliteColumnInt     sqlite3_column_int
#endif
#ifndef SqliteColumnInt64
#define SqliteColumnInt64   sqlite3_column_int64
#endif
#ifndef SqliteColumnDouble
#define SqliteColumnDouble  sqlite3_column_double
#endif
#ifndef SqliteBindInt
#define SqliteBindInt       sqlite3_bind_int
#endif
#ifndef SqliteBindInt64
#define SqliteBindInt64     sqlite3_bind_int64
#endif
#ifndef SqliteBindDouble
#define SqliteBindDouble    sqlite3_bind_double
#endif

#ifndef SqliteColumnBytes
#define SqliteColumnBytes sqlite3_column_bytes
#endif

#ifndef SqliteColumnBlob
#define SqliteColumnBlob sqlite3_column_blob
#endif

#define SqliteFinalize      sqlite3_finalize
#define SqliteClose         sqlite3_close

#ifndef SqliteStep
#define SqliteStep          sqlite3_step
#endif
#ifndef SqliteReset
#define SqliteReset         sqlite3_reset
#endif
#ifndef SqliteClearBindings
#define SqliteClearBindings sqlite3_clear_bindings
#endif
#ifndef SqliteColumnCount
#define SqliteColumnCount   sqlite3_column_count
#endif
#ifndef SqliteBindParameterCount
#define SqliteBindParameterCount sqlite3_bind_parameter_count
#endif
#ifndef SqliteErrorCode
#define SqliteErrorCode sqlite3_errcode
#endif

