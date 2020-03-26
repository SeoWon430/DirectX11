

#include "stdafx.h"
#include "SystemClass.h"


// 윈도우 프로그램의 시작점
// 콘솔용 main, 윈도우용 WinMain, 유니코드 사용 가능한 윈도우용 wWinMain

/* ★ APIENTRY
	#define CALLBACK __stdcall 
	#define WINAPI __stdcall
	#define WINAPIV     __cdecl
	#define APIENTRY    WINAPI

	APIENTRY와 CALLBACK은 동일한 기능
*/

/* ★ CALLBACK
	함수 호출 방식

	◆ 일반 함수 : 작성자의 의도에 따라 호출함

	◆ CALLBACK 함수 : 호출 되면 작성자, OS에 알려줌
		이벤트에 사용 되어 특정 상황에 반응함
		EX) 키입력 이벤트 -> 윈도우에서 반응하여 처리

	* 일반 함수는 주기적으로 매번 이벤트를 확인 한 후 처리하고
		CALLBACK함수는 이벤트가 발생하면 바로 처리함

	* 윈도우가 이벤트(입력 같은 것)에 대해 반응하기 위해 
		수시로 확인하지 않고 CALLBACK으로 이벤트 발생시에만 처리함

	# https://smok95.tistory.com/193
	# https://rockdrumy.tistory.com/898
*/

/* ★ __stdcall와 __cdecl
	함수 호출 규약 : 코드 내에서 함수 호출 시 현재 위치를 저장(stack  push) 하고
		해당 함수가 끝날 때 저장 했던 데이터를 삭제하는데 이에 대한 방식을 정의

	◆ __stdcall : 이 방식으로 정의된 함수는 자기 자신이 메모리 관리
		받아오는 매개변수의 갯수가 일정 한 함수에 사용되어 메모리를 함수 자기 자신이 처리

	◆ __cdecl : 이 방식으로 정의된 함수는 호출 한 곳에서 메모리 관리
		받아오는 매개변수의 갯수가 일정 하지 않은 함수에 사용되어 메모리를 호출 한 곳에서 처리

	# http://www.tipssoft.com/bulletin/board.php?bo_table=FAQ&wr_id=625
*/

/* ★ SAL (Source Code Annotation Language)
	컴파일러를 위한 매개변수의 방식, 데이터의 안정성 확보

	◆ _In_ : 읽기 전용
	◆ _Out_ : 쓰기 (ref)

	# https://docs.microsoft.com/en-us/cpp/code-quality/understanding-sal?view=vs-2019
*/

/* ★ 프로그램, 윈도우 핸들
	◆ HINSTANCE : 프로그램 객체 식별자 
		메모리에 올라온 프로그램 실행 코드로 WinMain에 넘겨 명령을 실행
	◆ HWND : 프로그램에 의해 생긴 윈도우 식별자
		한 프로그램 내 여러 윈도우가 있는 경우 구분하기 위한 객체로 OS에 의해 화면에 띄워짐

	# http://www.sosori.com/2009/09/api-hinstance-hwnd%EC%9D%98-%EC%B0%A8%EC%9D%B4.html
*/

/* ★ 문자열 자료형
	LP : long pointer
	C : constant
	STR : string (=char배열)
	W : wide (유니코드 문자)
	T : 컴파일시 형식 구분

	LPWSTR : long pointer char*를 의미
	LPCWSTR : long pointer constant char*를 의미

	LPCWSTR : long pointer constant wide string를 의미 ( = const w_char * )
	LPWSTR : long pointer wide string를 의미 ( = w_char * )

	LPCTSTR : long pointer constant t_string를 의미 ( = const t_char * )
		컴파일 시 유니코드 사용 여부에 따른 호환성 문제를 해결하기 위한 방식
		유니코드 사용 시 LPCWSTR로, 비사용 시 LPCWSTR로 처리

	# https://engkimbs.tistory.com/110
*/
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {


	// System 객체 생성
	SystemClass* System = new SystemClass;
	if (!System) {
		return -1;
	}

	// System 객체 초기화 및 실행
	if (System->Initialize()) {
		System->Run();
	}

	// System 객체 종료 및 메모리 반환
	System->Shutdown();
	delete System;
	System = nullptr;

	return 0;
}