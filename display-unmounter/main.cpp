#define NDEBUG

#include <Windows.h>
#include <string>
#include <assert.h>

//전방 선언
//VOID CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
//전역 선언
LPCTSTR g_lpszClass = TEXT("DisplayPowerOff");//Windows 클래스 정의
LPSTR g_lpszCmdParam;
CHAR g_szCmdParam[32] = { 0 };
INT g_nWaitingSeconds = 0;

INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {//인스턴스 식별, 도스 호환성, 파라메터, 윈도우 표시 옵션
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass = { 0 };//구조체 
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = g_lpszClass;
	RegisterClass(&WndClass);//윈도우 클래스 등록
	g_lpszCmdParam = lpszCmdParam;
	hWnd = CreateWindow(g_lpszClass, g_lpszClass, NULL, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, (HMENU)NULL, hInstance, NULL);
	//메모리에 윈도우 할당: 등록한 WNDCLASS 구조체의 lpszClassName 입력, 타이틀 바 제목, 스타일, 픽셀 단위 x, y, 너비, 높이, 부모 윈도우 핸들(NULL시 데스크탑이 부모, 현재 윈도우에서만 사용되는 메뉴(WndClass, CreateWindow 모두 NULL 이므로 메뉴는 없다., 소속 인스턴트, CREATESTRUCT
	//ShowWindow(hWnd, nCmdShow);

	//메시지 루프
	while (GetMessage(&Message, NULL, 0, 0)) {//메시지 큐에서 메시지를 읽어 MSG 구조체에 저장. 메시지가 WM_QUIT일 경우 FALSE else TRUE.
		//TranslateMessage(&Message);//키보드 관련 메시지. WM_KEYDOWN 메시지 발생 시 코드값이 문자라면 WM_CHAR 생성.
								   //KEYDOWN -> KEYCHAR -> KEYUP
		DispatchMessage(&Message);//프로시저로 메시지 전송.
								  //다시 GetMessage로 돌아가 다음 메시지 수신 대기.
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {//메시지 처리
	//HDC hdc;
	//PAINTSTRUCT ps;
	switch (iMessage) {
	case WM_CREATE:
		if (strnicmp(g_lpszCmdParam, "/t ", 3) == 0 && strlen(g_lpszCmdParam) >= 4) {
			try {
				strcpy(g_szCmdParam, g_lpszCmdParam + 3);
				g_nWaitingSeconds = atoi(g_szCmdParam);
				if (g_nWaitingSeconds == 2147483647 || g_nWaitingSeconds == 0) {
					throw g_nWaitingSeconds;
				}
			}
			catch (int e) {
				MessageBeep(MB_ICONERROR);
				DestroyWindow(hWnd);
				return 0;
			}
		}
		assert(1 != 1 && "test");
		SetTimer(hWnd, 1, g_nWaitingSeconds, NULL);
		return 0;
		//case WM_PAINT:
		//	hdc = BeginPaint(hWnd, &ps);
		//	EndPaint(hWnd, &ps);
		//	return 0;
	case WM_DESTROY://시스템 메뉴 더블 클릭, Alt F4 입력시 발생, DestroyWindow를 통해 호출 됨
		PostQuitMessage(0);
		return 0;
	case WM_TIMER:
		KillTimer(hWnd, 1);
		//MessageBeep(MB_ICONASTERISK);
		SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, (LPARAM)2);
		DestroyWindow(hWnd);
		return 0;
	}
	//SYSKEY... 들은 리턴값을 DefWIndowProc으로 보내줘야 한다. p.90
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));//나머지 메시지들에 대한 디폴트 처리: 윈도우 이동, 크기 변경
}

//VOID CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime){
//	KillTimer(hWnd, 1);
//	//MessageBeep(MB_ICONASTERISK);
//	SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, (LPARAM) 2);
//	DestroyWindow(hWnd);
//}