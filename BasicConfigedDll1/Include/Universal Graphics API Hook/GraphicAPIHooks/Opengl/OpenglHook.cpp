#include "OpenglHook.h"

tOpenglWglSwapBuffers OpenglHook::m_oOpenglHookRoutine;

BOOL __stdcall OpenglDraw(HDC hDC)
{
	static HGLRC s_GameContext = wglGetCurrentContext();

	static HGLRC s_MyContext;
	if (!BaseGraphicAPIHook::m_bReady)
	{
		//Create new context
		s_MyContext = wglCreateContext(hDC);

		//Make thread use our context
		wglMakeCurrent(hDC, s_MyContext);

		GLint c_ViewPort[4];
		glGetIntegerv(GL_VIEWPORT, c_ViewPort);

		//Setup our context
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, c_ViewPort[2], c_ViewPort[3], 0.0, 1.0, -1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClearColor(0, 0, 0, 1.0);
		BaseGraphicAPIHook::m_bReady = true;
	}

	wglMakeCurrent(hDC, s_MyContext);

	if (BaseGraphicAPIHook::m_bUseImGui)
	{
		ImGui_ImplWin32_NewFrame();
		ImGui_ImplOpenGL2_NewFrame();
		ImGui::NewFrame();
	}

	BaseGraphicAPIHook::m_oDrawRoutine();

	if (BaseGraphicAPIHook::m_bUseImGui)
	{
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	}

	wglMakeCurrent(hDC, s_GameContext);
	return OpenglHook::m_oOpenglHookRoutine(hDC);
}

OpenglHook::OpenglHook(tDrawRoutine _DrawRoutine)
{
	BaseGraphicAPIHook::m_oDrawRoutine = _DrawRoutine;
	HMODULE c_hModuleOpengl32 = GetModuleHandleA("opengl32.dll");
	this->m_oOpenglHookRoutine = (tOpenglWglSwapBuffers)GetProcAddress(c_hModuleOpengl32, "wglSwapBuffers");
	size_t c_HookLength = 0;
	#ifdef _WIN64
	c_HookLength = 15;
	#else
	c_HookLength = 5;
	#endif
	this->m_pGraphicAPIHook = new Hook{ (BYTE*)this->m_oOpenglHookRoutine, (BYTE*)OpenglDraw, c_HookLength };
}

DWORD OpenglHook::Enable()
{
	if (BaseGraphicAPIHook::m_bUseImGui)
	{
		ImGui::SetCurrentContext(ImGui::CreateContext());
		ImGui_ImplWin32_Init(this->m_hWindow);
		ImGui_ImplOpenGL2_Init();
		this->SetNewWindowProcedure();
	}
	this->m_oOpenglHookRoutine = (tOpenglWglSwapBuffers)this->m_pGraphicAPIHook->Init();
	return 0;
}

DWORD OpenglHook::Disable()
{
	this->m_pGraphicAPIHook->UnInit();
	if (BaseGraphicAPIHook::m_bUseImGui)
	{
		Sleep(1000); //sometimes uninit takes a little while and crashes because we shutdown imgui but hook tries to render with imgui
		ImGui_ImplWin32_Shutdown();
		ImGui_ImplOpenGL2_Shutdown();
		ImGui::DestroyContext();
		this->SetPreviousWindowProcedure();
	}
	return 0;
}