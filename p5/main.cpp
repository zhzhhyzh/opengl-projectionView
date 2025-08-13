
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "OpenGL Window"

float tx = 0, ty = 0, tz = 0;
float tSpeed = 1;
bool isOrtho = true;
LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		else if (wParam == VK_UP) {
			tz -= tSpeed;
			ty -= 0.1;

		}
		else if (wParam == VK_DOWN) {
			tz += tSpeed;
			ty += 0.1;
		}
		else if (wParam == '1') {
			isOrtho = true;
			tz = 0;
			ty = 0;
		}
		else if (wParam == '2') {
			isOrtho = false;
			tz = 0;
			ty = 0;
		}
		break;
		

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------
void drawSphere(double r) {	
	GLUquadricObj* sphere = NULL; //create the quadric obj pointer
	sphere = gluNewQuadric(); // create the quadric ob in the memory
	//gluQuadricDrawStyle(sphere, GLU_LINE); // make it line to view

	gluSphere(sphere, r, 30, 30);//draw sphere
	gluDeleteQuadric(sphere); // Delete object and free memory

}

void projection() {
	

	glMatrixMode(GL_PROJECTION); // Refer to the prjection matrix
	glLoadIdentity(); // Load the identity matrix, reset the projection matrix


	if (isOrtho) {
		glOrtho(-10, 10, -10, 10, -6, 6); // The ortho view
		
	}
	else {
		glFrustum(-10, 10, -10, 10, 5, 10); // Perspective projection in Furstum
		//gluPerspective(45, 1, 1, 10);// Perspective view (One slice)
	}
}
void display()
{
	//--------------------------------
	//	OpenGL drawing
	//--------------------------------
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);


	glMatrixMode(GL_MODELVIEW); //Refer to the model view matrix
	glLoadIdentity();                 // make sure we start clean

	if (!isOrtho) glTranslatef(0, 0, -10); // put it between near(5) and far(10)
	glColor3f(1, 1, 0);
	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex3f(-0.5f, 0.5f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);

	glVertex3f(-0.5f, 0, 0.0f);
	glVertex3f(0.5f, 0, 0.0f);

	glVertex3f(-0.5f, -0.5f, 0.0f);
	glVertex3f(0.5f, -0.5f, 0.0f);
		glEnd();

		glColor3f(1, 0, 0);
	glLoadIdentity();//Reset the model view matrix
	glTranslatef(0, ty, tz);
	drawSphere(5);
	
	//--------------------------------
	//	End of OpenGL drawing
	//--------------------------------

}


//--------------------------------------------------------------------



int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		900,10,300,300,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	projection();


	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------