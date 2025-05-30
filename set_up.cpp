#include <iostream>
#include <Windows.h>
#include <conio.h> 

HANDLE hStdin;
DWORD fdwOldMode;
DWORD fdwNewMode;

void ErrorExit()
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	DWORD dw = GetLastError();

	if (FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL) == 0) {
		MessageBox(NULL, TEXT("FormatMessage failed"), TEXT("Error"), MB_OK);
		ExitProcess(dw);
	}

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	ExitProcess(dw);
}

void enableRowMode() {
	DWORD fdwEchoOff;
	hStdin = GetStdHandle(STD_INPUT_HANDLE);//geting the input handle
	if (hStdin == INVALID_HANDLE_VALUE)
		ErrorExit();

	if (!GetConsoleMode(hStdin, &fdwOldMode))
		ErrorExit();

	fdwOldMode &= ~(ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT | ENABLE_LINE_INPUT); //disable echo input(0x0004), processes(ctr+c) and line_input(enter for user input)
	if (!SetConsoleMode(hStdin, fdwOldMode))
		ErrorExit();
	std::cout << "\tRow mode has been enable\n";
}
void disableRowMode() {
	if (!GetConsoleMode(hStdin, &fdwNewMode))
		ErrorExit();

	fdwNewMode |= (ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT | ENABLE_LINE_INPUT); //enable echo input(0x0004), processes(ctr+c) and line_input(enter for user input)
	if (!SetConsoleMode(hStdin, fdwNewMode))
		ErrorExit();
	std::cout << "\tRow mode has been disable\n";

}

int main()
{
	enableRowMode();
	std::cout << "\tpress control q to exit:\n";
	char c;
	while (true) {
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000 && GetAsyncKeyState('Q') & 0x8000) {
			std::cout << "\n\tcontrol q has been pressed, exiting\n";
			break;
		}
		if (_kbhit()){//check if no key is getting pressed
			std::cin >> c;
			std::cout << c;
		}
		Sleep(10);
	}
	disableRowMode();
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
