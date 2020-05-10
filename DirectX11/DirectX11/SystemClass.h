#pragma once

#include "stdafx.h"
#include "InputClass.h"
#include "GraphicsClass.h"
#include "SystemClass.h"
#include "SoundClass.h"
#include "FpsClass.h"
#include "TimerClass.h"
#include "CpuClass.h"



// WinMain에서 바로 실행되는 클래스
// 윈도우로 실행된 프로그램을 전체적으로 관리
// 입력(InputClass)과 렌더링(GraphicsClass) 관련을 담당하여 실행
class SystemClass {
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputClass* m_Input;
	GraphicsClass* m_Graphics;
	SoundClass* m_Sound;

	FpsClass* m_Fps;
	CpuClass* m_Cpu;
	TimerClass* m_Timer;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static SystemClass* ApplicationHandle = 0;