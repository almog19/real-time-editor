#include <iostream>
#include <Windows.h>
#include <conio.h> 

#define CTRL_KEY(k) ((k) & 0x1f)

HANDLE hStdin;
HANDLE hStdout;
DWORD fdwOldMode;
DWORD fdwNewMode;
CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
int counter = 0;

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

void ascii(char c) {
	int of = 0;
	//function that will translate the text into ascii and print it one line under the text
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdout == INVALID_HANDLE_VALUE)
		ErrorExit();

	if (!GetConsoleScreenBufferInfo(hStdout, &csbiInfo))
		ErrorExit();


	csbiInfo.dwCursorPosition.X = 70+ 5*(counter%5);
	csbiInfo.dwCursorPosition.Y = 2 + (counter/5);//going 1 line down

	if (!SetConsoleCursorPosition(hStdout, csbiInfo.dwCursorPosition))
		ErrorExit();

	std::cout << (int)c;//printing the ascii value

	csbiInfo.dwCursorPosition.X = (counter%50);//after 10 character going 1 line below
	csbiInfo.dwCursorPosition.Y = 2 + (counter / 50);//going 1 line up

	if(!SetConsoleCursorPosition(hStdout, csbiInfo.dwCursorPosition))
		ErrorExit();

	counter += 1;
}
int main()
{
	enableRowMode();
	std::cout << "\tpress control q to exit:\n";
	char c;
	while (true) {
		std::cin >> c;
		switch (c){
			case CTRL_KEY('q'):
				std::cout << "\n\texit\n";
				goto finish;
				break;
			default:
				ascii(c);
				std::cout << c;
				break;
		}
		Sleep(10);
	}

finish:
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
