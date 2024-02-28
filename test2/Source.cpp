#pragma once
#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include <fstream>
using namespace std;

BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow)
{
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL,DlgProc);
}

DWORD WINAPI Write(LPVOID lp)
{
	HWND hWnd = HWND(lp);
	// получим дескриптор существующего события
	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, 0, TEXT("{2BA7C99B-D9F7-4485-BB3F-E4735FFEF139}"));
	if (WaitForSingleObject(hEvent, INFINITE) == WAIT_OBJECT_0)
	{
		MessageBox(0, L"Write", NULL, MB_OK);

		wofstream myfile("Test.txt", ios::out);
		myfile << "Hello world\n";

		myfile.close();

	}
	ResetEvent(hEvent);

	return 0;
}
DWORD WINAPI Read(LPVOID lp)
{
	HWND hWnd = HWND(lp);
	// получим дескриптор существующего события
	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, 0, TEXT("{2BA7C99B-D9F7-4485-BB3F-E4735FFEF139}"));
	TCHAR buff[100];

	if (WaitForSingleObject(hEvent, INFINITE) == WAIT_OBJECT_0)
	{
		wifstream myfile("Test.txt", ios::in);
		wstring res;
		while (!myfile.eof())
		{
			myfile >> buff;
			res += buff;
			res += L" ";
		}
		myfile.close();
		MessageBox(0, res.c_str(), NULL, MB_OK);

	}
	ResetEvent(hEvent);

	return 0;
}

DWORD WINAPI WriteToEdit(LPVOID Lp) 
{
	HWND hWnd = HWND(Lp);	
	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, 0, TEXT("{2BA7C99B-D9F7-4485-BB3F-E4735FFEF139}"));		
	HWND hEdit = GetDlgItem(hWnd, IDC_EDIT1);
	TCHAR buff[100];	

	if (WaitForSingleObject(hEvent, INFINITE) == WAIT_OBJECT_0) 
	{
		wifstream myfile("Test.txt", ios::in);	
		wstring res;	
		while (!myfile.eof()) {
			myfile >> buff;
			res += buff;	
			res += L"";
		}
		myfile.close();	
		SetWindowText(hEdit, res.c_str());		
	}
	ResetEvent(hEvent);		


	return 0;	
}

DWORD WINAPI AddTime(LPVOID lp)	
{
	HWND hEdit = HWND(lp);
	while (true){
		SYSTEMTIME time;
		GetLocalTime(&time);

		TCHAR buff[50];
		_stprintf_s(buff, _T("%02d:%02d:%02d"), time.wHour, time.wMinute, time.wSecond);	
		SetWindowText(hEdit, buff);
		Sleep(1000);
	}
	return 0;
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp)
{
	switch (mes)
	{
	case WM_INITDIALOG:
	{
	CreateEvent(NULL, TRUE /* ручной сброс события */, FALSE /* несигнальное состояние */, TEXT("{2BA7C99B-D9F7-4485-BB3F-E4735FFEF139}"));
	    HANDLE h = CreateThread(NULL, 0, Write, 0, 0, NULL);
		CloseHandle(h);
		h = CreateThread(NULL, 0, Read, 0, 0, NULL);
		CloseHandle(h);
		h = CreateThread(NULL, 0, WriteToEdit, hWnd, 0, NULL);	
		CloseHandle(h);
		h = CreateThread(NULL, 0, AddTime, GetDlgItem(hWnd, IDC_EDIT2), 0, NULL);	
		CloseHandle(h);	
	 }
		break;
	case WM_COMMAND:
	{
		HANDLE h = OpenEvent(EVENT_ALL_ACCESS, 0, TEXT("{2BA7C99B-D9F7-4485-BB3F-E4735FFEF139}"));
		SetEvent(h); // перевод события в сигнальное состояние
	}
		break;
	case WM_CLOSE:
		EndDialog(hWnd, 0);
		break;

	}
	return FALSE;
}