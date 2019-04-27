#define NDEBUG

#include <Windows.h>
#include <string>
#include <assert.h>

//���� ����
//VOID CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
//���� ����
LPCTSTR g_lpszClass = TEXT("DisplayPowerOff");//Windows Ŭ���� ����
LPSTR g_lpszCmdParam;
CHAR g_szCmdParam[32] = { 0 };
INT g_nWaitingSeconds = 0;

INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {//�ν��Ͻ� �ĺ�, ���� ȣȯ��, �Ķ����, ������ ǥ�� �ɼ�
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass = { 0 };//����ü 
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = g_lpszClass;
	RegisterClass(&WndClass);//������ Ŭ���� ���
	g_lpszCmdParam = lpszCmdParam;
	hWnd = CreateWindow(g_lpszClass, g_lpszClass, NULL, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, (HMENU)NULL, hInstance, NULL);
	//�޸𸮿� ������ �Ҵ�: ����� WNDCLASS ����ü�� lpszClassName �Է�, Ÿ��Ʋ �� ����, ��Ÿ��, �ȼ� ���� x, y, �ʺ�, ����, �θ� ������ �ڵ�(NULL�� ����ũž�� �θ�, ���� �����쿡���� ���Ǵ� �޴�(WndClass, CreateWindow ��� NULL �̹Ƿ� �޴��� ����., �Ҽ� �ν���Ʈ, CREATESTRUCT
	//ShowWindow(hWnd, nCmdShow);

	//�޽��� ����
	while (GetMessage(&Message, NULL, 0, 0)) {//�޽��� ť���� �޽����� �о� MSG ����ü�� ����. �޽����� WM_QUIT�� ��� FALSE else TRUE.
		//TranslateMessage(&Message);//Ű���� ���� �޽���. WM_KEYDOWN �޽��� �߻� �� �ڵ尪�� ���ڶ�� WM_CHAR ����.
								   //KEYDOWN -> KEYCHAR -> KEYUP
		DispatchMessage(&Message);//���ν����� �޽��� ����.
								  //�ٽ� GetMessage�� ���ư� ���� �޽��� ���� ���.
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {//�޽��� ó��
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
	case WM_DESTROY://�ý��� �޴� ���� Ŭ��, Alt F4 �Է½� �߻�, DestroyWindow�� ���� ȣ�� ��
		PostQuitMessage(0);
		return 0;
	case WM_TIMER:
		KillTimer(hWnd, 1);
		//MessageBeep(MB_ICONASTERISK);
		SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, (LPARAM)2);
		DestroyWindow(hWnd);
		return 0;
	}
	//SYSKEY... ���� ���ϰ��� DefWIndowProc���� ������� �Ѵ�. p.90
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));//������ �޽����鿡 ���� ����Ʈ ó��: ������ �̵�, ũ�� ����
}

//VOID CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime){
//	KillTimer(hWnd, 1);
//	//MessageBeep(MB_ICONASTERISK);
//	SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, (LPARAM) 2);
//	DestroyWindow(hWnd);
//}