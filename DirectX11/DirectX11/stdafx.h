#pragma once

//자주 사용하는 헤더를 모두 담음

// 윈도우 최신버전 유지
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.

// Windows 헤더 파일
#include <windows.h>

// C관련 헤더 파일
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// DirectX관련 헤더 파일
#include "DX11def.h"