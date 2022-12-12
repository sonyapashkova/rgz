#pragma once
#ifndef H_MAINWINDOW
#define H_MAINWINDOW
#include "rpm.h"
#endif
#include <Windows.h>
#include <fstream>
#include "resource.h"
using namespace std;

#define OnMenuSave							1
#define OnMenuRead							2
#define OnExitSoftware						3

#define OnButtonStartClicked				4
#define OnButtonCleanClicked				5
#define OnButtonSetPointsPointClicked		6
#define OnButtonSetCircleClicked			7
#define OnButtonDeleteCircleClicked			8

#define OnReadStartPositionX				9
#define OnReadStartPositionY				10
#define OnReadFinishPositionX				11
#define OnReadFinishPositionY				12

RECT OBS = { 188, 8, 762, 482 };

LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);

void MainWndAddMenus(HWND hWnd);
void MainWndAddWidgets(HWND hWnd);
void SaveData();
void ReadData();
void SetOpenFileParams(HWND hWnd);

OPENFILENAME ofn; 
char szFile[260]; 

BOOL CALLBACK AddPoint(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
BOOL CALLBACK AddCircle(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
BOOL DrawLine(HDC hdc, int x1, int y1, int x2, int y2);

HWND hEditStartPositionX;
HWND hEditStartPositionY;
HWND hEditFinishPositionX;
HWND hEditFinishPositionY;

int startPositionX = INT_MAX;
int startPositionY = INT_MAX;
int finishPositionX = INT_MAX;
int finishPositionY = INT_MAX;

int circleCenterX = INT_MAX;
int circleCenterY = INT_MAX;
int circleRadius = INT_MAX;