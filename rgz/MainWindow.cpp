#include "MainWindow.h"

Point q_init;
Point q_goal;
vector<Circle> obstacles = {};
vector<Point> points;
vector<Point> result;
Graph g;

int WINAPI WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInst, _In_ LPSTR args, _In_ int ncmdshow) {
	WNDCLASS SoftwareMainClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)), L"MainWndClass", SoftwareMainProcedure);

	if (!RegisterClassW(&SoftwareMainClass)) { return -1; }

	MSG SoftwareMainMessage = { 0 };

	CreateWindow(L"MainWndClass", L"RPM", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 800, 600, NULL, NULL, NULL, NULL);

	while (GetMessage(&SoftwareMainMessage, NULL, NULL, NULL)) {
		TranslateMessage(&SoftwareMainMessage);
		DispatchMessage(&SoftwareMainMessage);
	}

	return 0;
}

WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure) {
	WNDCLASS NWC = { 0 };

	NWC.hIcon = Icon;
	NWC.hCursor = Cursor;
	NWC.hInstance = hInst;
	NWC.lpszClassName = Name;
	NWC.hbrBackground = BGColor;
	NWC.lpfnWndProc = Procedure;

	return NWC;
}

LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	static HINSTANCE hInst;
	switch (msg) {
	case WM_COMMAND:
		switch (wp) {
		case OnMenuSave:
			if (obstacles.empty() || q_init.x == INT_MAX || q_init.y == INT_MAX ||
				q_goal.x == INT_MAX || q_goal.y == INT_MAX) {
				MessageBoxA(hWnd, "Not enough data. Try again", "Error", MB_OK);
			}
			else {
				SaveData();
			}
			break;

		case OnMenuRead:
			if (!result.empty() || !obstacles.empty() || q_init.x != INT_MAX || q_init.y != INT_MAX ||
				q_goal.x != INT_MAX || q_goal.y != INT_MAX) {
				g.clear();
				points.clear();
				obstacles.clear();
				result.clear();
				startPositionX = INT_MAX;
				startPositionY = INT_MAX;
				finishPositionX = INT_MAX;
				finishPositionY = INT_MAX;
				q_init.x = INT_MAX;
				q_init.y = INT_MAX;
				q_goal.x = INT_MAX;
				q_goal.y = INT_MAX;
				InvalidateRect(hWnd, &OBS, TRUE);
			}
			ReadData();
			InvalidateRect(hWnd, &OBS, TRUE);
			break;

		case OnExitSoftware:
			PostQuitMessage(0);
			break;

		case OnButtonStartClicked:
			if (/*obstacles.empty() ||*/ q_init.x == INT_MAX || q_init.y == INT_MAX ||
				q_goal.x == INT_MAX || q_goal.y == INT_MAX || startPositionX == INT_MAX || startPositionY == INT_MAX
				|| finishPositionX == INT_MAX || finishPositionY == INT_MAX) {
				MessageBoxA(hWnd, "Not enough data. Try again", "Error", MB_OK);
			} 
			else {
				g.clear();
				result = rpm(g, q_init, q_goal, obstacles);
				points = g.points;
				if (result.empty()) {
					MessageBoxA(hWnd, "Solution not found. Try again", "Result", MB_OK);
				}
			}
			InvalidateRect(hWnd, &OBS, TRUE);
			break;

		case OnButtonCleanClicked:
			g.clear();
			points.clear();
			obstacles.clear();
			result.clear();
			startPositionX = INT_MAX;
			startPositionY = INT_MAX;
			finishPositionX = INT_MAX;
			finishPositionY = INT_MAX;
			q_init.x = INT_MAX;
			q_init.y = INT_MAX;
			q_goal.x = INT_MAX;
			q_goal.y = INT_MAX;
			InvalidateRect(hWnd, &OBS, TRUE);
			break;

		case OnButtonSetPointsPointClicked:
			DialogBox(hInst, (LPWSTR)IDD_DIALOG1, hWnd, (DLGPROC)AddPoint);
			if (q_init.x < 0 || q_init.x > 570 || q_init.y < 0 || q_init.y > 470 ||
				q_init.x < 0 || q_init.x > 570 || q_init.y < 0 || q_init.y > 470 ||
				q_init.x == q_goal.x || q_init.y == q_goal.y) {
				MessageBoxA(hWnd, "Incorrect entry of the starting/finishing position of the robot. Try again", "Error", MB_OK);
				startPositionX = INT_MAX;
				startPositionY = INT_MAX;
				finishPositionX = INT_MAX;
				finishPositionY = INT_MAX;
				q_init.x = INT_MAX;
				q_init.y = INT_MAX;
				q_goal.x = INT_MAX;
				q_goal.y = INT_MAX;
			}
			else {
				g.clear();
				points.clear();
				result.clear();
			}
			InvalidateRect(hWnd, &OBS, TRUE);
			break;

 		case OnButtonSetCircleClicked:
			DialogBox(hInst, (LPWSTR)IDD_DIALOG2, hWnd, (DLGPROC)AddCircle);
			g.clear();
			points.clear();
			result.clear();
			/*if (!obstacles.empty() && (190 + obstacles[obstacles.size() - 1].c.x - obstacles[obstacles.size() - 1].r < 190 ||
				-obstacles[obstacles.size() - 1].c.y - obstacles[obstacles.size() - 1].r + 480 < 10 ||
				190 + obstacles[obstacles.size() - 1].c.x + obstacles[obstacles.size() - 1].r > 760 ||
				- obstacles[obstacles.size() - 1].c.y + obstacles[obstacles.size() - 1].r + 480 > 480)) {
				MessageBoxA(hWnd, "The circle is outside the area. Try again", "Error", MB_OK);
				obstacles.erase(obstacles.end() - 1);
			}
			else {
				g.clear();
				points.clear();
				result.clear();
			}*/
			InvalidateRect(hWnd, &OBS, TRUE);
			break;

		case OnButtonDeleteCircleClicked:
			if (obstacles.empty()) {
				MessageBoxA(hWnd, "All circles are removed", "Error", MB_OK);
			}
			else {
				g.clear();
				points.clear();
				result.clear();
				obstacles.erase(obstacles.end() - 1);
			}
			InvalidateRect(hWnd, &OBS, TRUE);
			break;

		default:
			break;
		}

	case WM_PAINT:
		PAINTSTRUCT ps;
		HPEN hp;

		BeginPaint(hWnd, &ps);
		Rectangle(ps.hdc, 188, 8, 762, 482);
		if (!points.empty()) {
			for (int i = 0; i < points.size(); ++i) {
				hp = CreatePen(PS_ALTERNATE, 1, RGB(128, 128, 128));
				SelectObject(ps.hdc, hp);
				SelectObject(ps.hdc, CreateSolidBrush(RGB(128, 128, 128)));
				Ellipse(ps.hdc, 190 + points[i].x - 1, -points[i].y - 1 + 480, 190 + points[i].x + 1, -points[i].y + 1 + 480);
				DeleteObject(hp);
			}
		}

		if (!obstacles.empty()) {
			for (int i = 0; i < obstacles.size(); ++i) {
				if (obstacles[i].c.x != INT_MAX && obstacles[i].c.y != INT_MAX && obstacles[i].r != INT_MAX) {
					hp = CreatePen(PS_ALTERNATE, 1, RGB(235, 82, 132));
					SelectObject(ps.hdc, hp);
					SelectObject(ps.hdc, CreateSolidBrush(RGB(235, 82, 132)));
					Ellipse(ps.hdc, 190 + obstacles[i].c.x - obstacles[i].r,
						-obstacles[i].c.y - obstacles[i].r + 480,
						190 + obstacles[i].c.x + obstacles[i].r,
						-obstacles[i].c.y + obstacles[i].r + 480);
					DeleteObject(hp);
				}
			}
		}

		if (!points.empty()) {
			for (int i = 0; i < points.size() - 1; ++i) {
				hp = CreatePen(PS_ALTERNATE, 1, RGB(128, 128, 128));
				SelectObject(ps.hdc, hp);
				SelectObject(ps.hdc, CreateSolidBrush(RGB(128, 128, 128)));
				vector<int> adjacent_vertices = g.get_adjacent_vertices(i);
				for (int j = 0; j < adjacent_vertices.size(); ++j) {
					DrawLine(ps.hdc, 190 + points[i].x, -points[i].y + 480, 190 + points[adjacent_vertices[j]].x, -points[adjacent_vertices[j]].y + 480);
					DeleteObject(hp);
				}
				DeleteObject(hp);
			}
		}

		if (!(startPositionX == INT_MAX || startPositionY == INT_MAX 
			|| finishPositionX == INT_MAX || finishPositionY == INT_MAX 
			|| startPositionX == finishPositionX || startPositionY == finishPositionY)) {
			hp = CreatePen(PS_ALTERNATE, 3, RGB(255, 0, 0));
			SelectObject(ps.hdc, hp);
			SelectObject(ps.hdc, CreateSolidBrush(RGB(255, 0, 0)));
			Ellipse(ps.hdc, 190 + q_init.x - 3, -q_init.y - 3 + 480, 190 + q_init.x + 3, -q_init.y + 3 + 480);
			Ellipse(ps.hdc, 190 + q_goal.x - 3, -q_goal.y - 3 + 480, 190 + q_goal.x + 3, -q_goal.y + 3 + 480);
			DeleteObject(hp);
		}

		if (!result.empty()) {
			for (int i = 0; i < result.size() - 1; ++i) {
				hp = CreatePen(PS_ALTERNATE, 2, RGB(215, 24, 104));
				SelectObject(ps.hdc, hp);
				SelectObject(ps.hdc, CreateSolidBrush(RGB(215, 24, 104)));
				DrawLine(ps.hdc, 190 + result[i].x, -result[i].y + 480, 190 + result[i + 1].x, -result[i + 1].y + 480);
				DeleteObject(hp);
			}

			for (int i = 0; i < result.size(); ++i) {
				hp = CreatePen(PS_ALTERNATE, 2, RGB(0, 0, 0));
				SelectObject(ps.hdc, hp);
				SelectObject(ps.hdc, CreateSolidBrush(RGB(0, 0, 0)));
				if (result[i] != q_init && result[i] != q_goal) {
					Ellipse(ps.hdc, 190 + result[i].x - 2, -result[i].y - 2 + 480, 190 + result[i].x + 2, -result[i].y + 2 + 480);
				}
				DeleteObject(hp);
			}
		}

		EndPaint(hWnd, &ps);
		break;

	case WM_CREATE:
		MainWndAddMenus(hWnd);
		MainWndAddWidgets(hWnd);
		SetOpenFileParams(hWnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default: return DefWindowProc(hWnd, msg, wp, lp);
	}
}

BOOL DrawLine(HDC hdc, int x1, int y1, int x2, int y2) {
	POINT pt;
	MoveToEx(hdc, x1, y1, &pt);
	return LineTo(hdc, x2, y2);
}

void MainWndAddMenus(HWND hWnd) {
	HMENU RootMenu = CreateMenu();
	HMENU SubMenu = CreateMenu();

	AppendMenu(SubMenu, MF_STRING, OnMenuSave, L"Save configuration");
	AppendMenu(SubMenu, MF_STRING, OnMenuRead, L"Read configuration");
	AppendMenu(SubMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(SubMenu, MF_STRING, OnExitSoftware, L"Exit");
	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu, L"File");


	SetMenu(hWnd, RootMenu);
}

void MainWndAddWidgets(HWND hWnd) {
	CreateWindowA("button", "Set points", WS_VISIBLE | WS_CHILD, 10, 10, 160, 30, hWnd, (HMENU)OnButtonSetPointsPointClicked, NULL, NULL);
	CreateWindowA("button", "Set a circle", WS_VISIBLE | WS_CHILD, 10, 50, 160, 30, hWnd, (HMENU)OnButtonSetCircleClicked, NULL, NULL);
	CreateWindowA("button", "Delete the last circle", WS_VISIBLE | WS_CHILD, 10, 90, 160, 30, hWnd, (HMENU)OnButtonDeleteCircleClicked, NULL, NULL);
	CreateWindowA("button", "Start", WS_VISIBLE | WS_CHILD, 640, 490, 120, 30, hWnd, (HMENU)OnButtonStartClicked, NULL, NULL);
	CreateWindowA("button", "Clean", WS_VISIBLE | WS_CHILD, 500, 490, 120, 30, hWnd, (HMENU)OnButtonCleanClicked, NULL, NULL);
}

BOOL CALLBACK AddPoint(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case WM_INITDIALOG:
		return TRUE;
		break;

	case WM_COMMAND:
		switch (LOWORD(wp)) {
		case IDOK:
			startPositionX = GetDlgItemInt(hWnd, IDC_EDIT1, NULL, false);
			startPositionY = GetDlgItemInt(hWnd, IDC_EDIT2, NULL, false);
			finishPositionX = GetDlgItemInt(hWnd, IDC_EDIT3, NULL, false);
			finishPositionY = GetDlgItemInt(hWnd, IDC_EDIT4, NULL, false);
			EndDialog(hWnd, LOWORD(wp));
			q_init = Point(startPositionX, startPositionY);
			q_goal = Point(finishPositionX, finishPositionY);
			break;
			return FALSE;

		case IDCANCEL:
			EndDialog(hWnd, LOWORD(wp));
			break;
			return FALSE;

		}
	}
	return FALSE;
}

BOOL CALLBACK AddCircle(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case WM_INITDIALOG:
		return TRUE;
		break;

	case WM_COMMAND:
		switch (LOWORD(wp)) {
		case IDOK_2:
			circleCenterX = GetDlgItemInt(hWnd, IDC_EDIT1_2, NULL, false);
			circleCenterY = GetDlgItemInt(hWnd, IDC_EDIT2_2, NULL, false);
			circleRadius = GetDlgItemInt(hWnd, IDC_EDIT3_2, NULL, false);
			EndDialog(hWnd, LOWORD(wp));
			obstacles.push_back(Circle(Point(circleCenterX, circleCenterY), circleRadius));
			break;
			return FALSE;

		case IDCANCEL_2:
			EndDialog(hWnd, LOWORD(wp));
			break;
			return FALSE;
		}
	/*case WM_CLOSE:
		EndDialog(hWnd, LOWORD(wp));
		break;
		return FALSE;*/

	}

	return FALSE;
}

void SaveData() {
	if (GetOpenFileName(&ofn) == TRUE) {
		ofstream scene;
		scene.open(ofn.lpstrFile/*, ios_base::binary*/);

		scene << q_init.x << endl;
		scene << q_init.y << endl;
		scene << q_goal.x << endl;
		scene << q_goal.y << endl;

		for (int i = 0; i < obstacles.size(); ++i) {
			scene << obstacles[i].c.x << endl;
			scene << obstacles[i].c.y << endl;
			scene << obstacles[i].r << endl;

		}

		/*scene.write((char*)&q_init.x, sizeof(int));
		scene.write((char*)&q_init.y, sizeof(int));
		scene.write((char*)&q_goal.x, sizeof(int));
		scene.write((char*)&q_goal.y, sizeof(int));

		for (int i = 0; i < obstacles.size(); ++i) {
			scene.write((char*)&obstacles[i].c.x, sizeof(int));
			scene.write((char*)&obstacles[i].c.y, sizeof(int));
			scene.write((char*)&obstacles[i].r, sizeof(int));
		}*/

		scene.close();
	}
}

void ReadData() {
	if (GetOpenFileName(&ofn) == TRUE) {
		ifstream scene;
		scene.open(ofn.lpstrFile/*, ios_base::binary*/);

		scene >> startPositionX;
		scene >> startPositionY;
		scene >> finishPositionX;
		scene >> finishPositionY;

		q_init = Point(startPositionX, startPositionY);
		q_goal = Point(finishPositionX, finishPositionY);

		while (!scene.eof()) {
			Circle circle;
			scene >> circle.c.x;
			scene >> circle.c.y;
			scene >> circle.r;
			obstacles.push_back(circle);
		}

		/*scene.read((char*)&startPositionX, sizeof(int));
		scene.read((char*)&startPositionY, sizeof(int));
		scene.read((char*)&finishPositionX, sizeof(int));
		scene.read((char*)&finishPositionY, sizeof(int));

		q_init = Point(startPositionX, startPositionY);
		q_goal = Point(finishPositionX, finishPositionY);

		while (!scene.eof()) {
			Circle circle;
			scene.read((char*)&circle.c.x, sizeof(int));
			scene.read((char*)&circle.c.y, sizeof(int));
			scene.read((char*)&circle.r, sizeof(int));
			obstacles.push_back(circle);
		}*/

		scene.close();
	}
}

void SetOpenFileParams(HWND hWnd) {
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = (LPWSTR)szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L".txt";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
}