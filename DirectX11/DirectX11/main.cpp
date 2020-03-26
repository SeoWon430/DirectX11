

#include "stdafx.h"
#include "SystemClass.h"


// ������ ���α׷��� ������
// �ֿܼ� main, ������� WinMain, �����ڵ� ��� ������ ������� wWinMain

/* �� APIENTRY
	#define CALLBACK __stdcall 
	#define WINAPI __stdcall
	#define WINAPIV     __cdecl
	#define APIENTRY    WINAPI

	APIENTRY�� CALLBACK�� ������ ���
*/

/* �� CALLBACK
	�Լ� ȣ�� ���

	�� �Ϲ� �Լ� : �ۼ����� �ǵ��� ���� ȣ����

	�� CALLBACK �Լ� : ȣ�� �Ǹ� �ۼ���, OS�� �˷���
		�̺�Ʈ�� ��� �Ǿ� Ư�� ��Ȳ�� ������
		EX) Ű�Է� �̺�Ʈ -> �����쿡�� �����Ͽ� ó��

	* �Ϲ� �Լ��� �ֱ������� �Ź� �̺�Ʈ�� Ȯ�� �� �� ó���ϰ�
		CALLBACK�Լ��� �̺�Ʈ�� �߻��ϸ� �ٷ� ó����

	* �����찡 �̺�Ʈ(�Է� ���� ��)�� ���� �����ϱ� ���� 
		���÷� Ȯ������ �ʰ� CALLBACK���� �̺�Ʈ �߻��ÿ��� ó����

	# https://smok95.tistory.com/193
	# https://rockdrumy.tistory.com/898
*/

/* �� __stdcall�� __cdecl
	�Լ� ȣ�� �Ծ� : �ڵ� ������ �Լ� ȣ�� �� ���� ��ġ�� ����(stack  push) �ϰ�
		�ش� �Լ��� ���� �� ���� �ߴ� �����͸� �����ϴµ� �̿� ���� ����� ����

	�� __stdcall : �� ������� ���ǵ� �Լ��� �ڱ� �ڽ��� �޸� ����
		�޾ƿ��� �Ű������� ������ ���� �� �Լ��� ���Ǿ� �޸𸮸� �Լ� �ڱ� �ڽ��� ó��

	�� __cdecl : �� ������� ���ǵ� �Լ��� ȣ�� �� ������ �޸� ����
		�޾ƿ��� �Ű������� ������ ���� ���� ���� �Լ��� ���Ǿ� �޸𸮸� ȣ�� �� ������ ó��

	# http://www.tipssoft.com/bulletin/board.php?bo_table=FAQ&wr_id=625
*/

/* �� SAL (Source Code Annotation Language)
	�����Ϸ��� ���� �Ű������� ���, �������� ������ Ȯ��

	�� _In_ : �б� ����
	�� _Out_ : ���� (ref)

	# https://docs.microsoft.com/en-us/cpp/code-quality/understanding-sal?view=vs-2019
*/

/* �� ���α׷�, ������ �ڵ�
	�� HINSTANCE : ���α׷� ��ü �ĺ��� 
		�޸𸮿� �ö�� ���α׷� ���� �ڵ�� WinMain�� �Ѱ� ����� ����
	�� HWND : ���α׷��� ���� ���� ������ �ĺ���
		�� ���α׷� �� ���� �����찡 �ִ� ��� �����ϱ� ���� ��ü�� OS�� ���� ȭ�鿡 �����

	# http://www.sosori.com/2009/09/api-hinstance-hwnd%EC%9D%98-%EC%B0%A8%EC%9D%B4.html
*/

/* �� ���ڿ� �ڷ���
	LP : long pointer
	C : constant
	STR : string (=char�迭)
	W : wide (�����ڵ� ����)
	T : �����Ͻ� ���� ����

	LPWSTR : long pointer char*�� �ǹ�
	LPCWSTR : long pointer constant char*�� �ǹ�

	LPCWSTR : long pointer constant wide string�� �ǹ� ( = const w_char * )
	LPWSTR : long pointer wide string�� �ǹ� ( = w_char * )

	LPCTSTR : long pointer constant t_string�� �ǹ� ( = const t_char * )
		������ �� �����ڵ� ��� ���ο� ���� ȣȯ�� ������ �ذ��ϱ� ���� ���
		�����ڵ� ��� �� LPCWSTR��, ���� �� LPCWSTR�� ó��

	# https://engkimbs.tistory.com/110
*/
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {


	// System ��ü ����
	SystemClass* System = new SystemClass;
	if (!System) {
		return -1;
	}

	// System ��ü �ʱ�ȭ �� ����
	if (System->Initialize()) {
		System->Run();
	}

	// System ��ü ���� �� �޸� ��ȯ
	System->Shutdown();
	delete System;
	System = nullptr;

	return 0;
}