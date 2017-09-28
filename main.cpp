/*WinAPI 2.3. Написать приложение, обладающее следующей функциональностью:
1) при нажатии клавиши <Enter> главное окно позиционируется в левый
верхний угол экрана с размерами (300×300) и начинает перемещаться по периметру
экрана с определённой скоростью;
2) при нажатии клавиши <Esc> перемещение окна прекращается.*/
#include <Windows.h>
#include <tchar.h>

#define WNDWIDTH 300
#define WNDHEIGHT 300

TCHAR szClassName[] = TEXT("Simple Window");
TCHAR szWindowName[] = TEXT("Smart Program");
int posX = 0;
int posY = 0;

VOID CALLBACK TimerProc(HWND, UINT, UINT_PTR, DWORD);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
	LPSTR lpszCmdLine, INT nCmdShow) {
	WNDCLASSEX wclex;
	wclex.cbClsExtra = NULL;
	wclex.cbSize = sizeof(wclex);
	wclex.cbWndExtra = NULL;
	wclex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wclex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wclex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wclex.hIconSm = NULL;
	wclex.hInstance = hInst;
	wclex.lpfnWndProc = WndProc;
	wclex.lpszClassName = szClassName;
	wclex.lpszMenuName = NULL;
	wclex.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&wclex))
	{
		return 0;
	}
	HWND hWnd = CreateWindowEx(NULL, szClassName, szWindowName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInst, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG Msg;
	while (GetMessage(&Msg, NULL, NULL, NULL))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

VOID CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {

	TCHAR str[50];
	HDC hDCScreen = GetDC(NULL);
	int Horres = GetDeviceCaps(hDCScreen, HORZRES);//Разрешение экрана по горизонтали
	int Vertres = GetDeviceCaps(hDCScreen, VERTRES);//Разрешение экрана по вертикали
	ReleaseDC(NULL, hDCScreen);

	if (posX < Horres - WNDWIDTH && posY == 0) posX++;
	else
		if (posX >= Horres - WNDWIDTH && posY <= Vertres - WNDHEIGHT) posY++;
		else
			if (posX != 0 && posY >= Vertres - WNDHEIGHT) posX--;
			else
				if (posY <= Vertres - 300 && posX == 0) posY--;

	//Устанавливаем новую позицию кона
	SetWindowPos(hWnd, HWND_TOP, posX, posY, WNDWIDTH, WNDHEIGHT, SWP_SHOWWINDOW);

	//Выводим в заголовок окна позицию по X и Y
	wsprintf(str, TEXT("%i, %i, PosX: %i, PosY: %i"), Horres - WNDWIDTH, Vertres - WNDHEIGHT, posX, posY);
	SetWindowText(hWnd, str);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	switch (Msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_RETURN)
			SetTimer(hWnd, 1, 0, TimerProc);
		else if (wParam == VK_ESCAPE)
			KillTimer(hWnd, 1);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	return EXIT_SUCCESS;
}