#pragma once

#ifndef DeleteNull
#define DeleteNull(p) {delete p; p = nullptr;}
#endif

typedef float BinDouble;