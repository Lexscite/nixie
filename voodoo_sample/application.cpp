#include "stdafx.h"
#include "application.h"

#include <voodoo/logger.h>
#include <voodoo/camera.h>
#include <voodoo/model.h>
#include <voodoo/text.h>

namespace voodoo
{
	HWND Application::window_ = nullptr;
	LPCWSTR Application::window_caption_ = nullptr;
	Time* Application::time_ = nullptr;
	DirectXManager* Application::directx_ = nullptr;
	std::shared_ptr<Scene> Application::scene_ = nullptr;
	unsigned int Application::screen_width_ = 0;
	unsigned int Application::screen_height_ = 0;
	bool Application::vsync_enabled_ = 0;
	bool Application::fullscreen_enabled_ = 0;
	bool Application::is_paused_ = 0;

	bool Application::Init(HINSTANCE instance)
	{
		directx_ = DirectXManager::Get();
		time_ = Time::Get();

		InitSettings();

		if (!InitWindow(instance)) {
			MessageBoxA(window_, "Failed to create window", "Error", MB_OK | MB_ICONERROR);
			Logger::Write("Failed to initialize window");
			return false;
		}

		if (!directx_->Init(window_, screen_width_, screen_height_, vsync_enabled_, fullscreen_enabled_)) {
			MessageBoxA(window_, "DirectX initialization failed", "Error", MB_OK | MB_ICONERROR);
			Logger::Write("Failed to initialize DirectX");
			return false;
		}

		//if (!CConnection::GetSingleton()->Establish("127.0.0.1", 1111)) {
		//	MessageBox(m_hwnd, "Failed to conenct to server", "Network Error", MB_OK | MB_ICONERROR);
		//	return false;
		//} else {
		//	if (CConnection::GetSingleton()->SendPacketType(PacketType::HelloMessage))
		//		CConnection::GetSingleton()->SendString(std::string("Hi Server!"));
		//}

		if (!LoadScene()) {
			Logger::Write("Failed to load scene");
			return false;
		}
		else {
			if (!scene_->Init()) {
				Logger::Write("Failed to initialize scene");
				return false;
			}
		}

		return true;
	}

	void Application::InitSettings()
	{
		vsync_enabled_ = true;
		fullscreen_enabled_ = false;

		if (fullscreen_enabled_) {
			screen_width_ = GetSystemMetrics(SM_CXSCREEN);
			screen_height_ = GetSystemMetrics(SM_CYSCREEN);
		}
		else {
			screen_width_ = 800;
			screen_height_ = 600;
		}
	}

	bool Application::InitWindow(HINSTANCE instance)
	{
		WNDCLASSEX wc;
		LPCWSTR class_name = L"MainWindowClass";
		DWORD style = WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU;

		window_caption_ = L"Voodoo";

		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbClsExtra = NULL;
		wc.cbWndExtra = NULL;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.hInstance = instance;
		wc.lpfnWndProc = MessageProcessor;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = class_name;

		if (!RegisterClassEx(&wc))
			return false;

		int window_pos_x, window_pos_y;
		if (fullscreen_enabled_) {
			DEVMODE dmScreenSettings;

			memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
			dmScreenSettings.dmSize = sizeof(dmScreenSettings);
			dmScreenSettings.dmPelsWidth = static_cast<ULONG>(screen_width_);
			dmScreenSettings.dmPelsHeight = static_cast<ULONG>(screen_height_);
			dmScreenSettings.dmBitsPerPel = 32;
			dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

			window_pos_x = 0;
			window_pos_y = 0;
		}
		else {
			window_pos_x = GetSystemMetrics(SM_CXSCREEN) / 2 - screen_width_ / 2;
			window_pos_y = GetSystemMetrics(SM_CYSCREEN) / 2 - screen_height_ / 2;
		}

		window_ = CreateWindowEx(WS_EX_APPWINDOW, class_name, window_caption_, style,
			window_pos_x, window_pos_y, screen_width_, screen_height_, NULL, NULL, instance, NULL);
		if (!window_) {
			Logger::Write("WINAPI CreateWindowEx failed");
			return false;
		}

		ShowWindow(window_, SW_SHOW);

		return true;
	}

	LRESULT Application::MessageProcessor(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
	{
		switch (message) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_MOVE:
			if (LOWORD(w_param) == WM_MOVING) {
				is_paused_ = true;
				time_->Stop();
			} else {
				is_paused_ = false;
				time_->Start();
			}
			return 0;
		case WM_MENUCHAR:
			return MAKELRESULT(0, MNC_CLOSE);
		case WM_GETMINMAXINFO:
			((MINMAXINFO*)l_param)->ptMinTrackSize.x = 200;
			((MINMAXINFO*)l_param)->ptMinTrackSize.y = 200;
			return 0;
		case WM_ACTIVATEAPP:
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
		default:
			return DefWindowProc(window, message, w_param, l_param);
		}
	}

	int Application::Run()
	{
		time_->Reset();

		MSG msg;
		ZeroMemory(&msg, sizeof(msg));
		while (msg.message != WM_QUIT) {
			if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			} else {
				time_->Tick();
				if (!is_paused_) {
#ifdef _DEBUG
					CalculateFrameStats();
#endif
					if (!Update(0.0f)) {
						// Temporary exit code
						return 404;
					}
				} else {
					Sleep(100);
				}
			}
		}

		return static_cast<int>(msg.wParam);
	}

	bool Application::Update(float delta_time)
	{
		directx_->BeginScene(scene_->GetClearColor());

		if (!scene_->Update()) {
			Logger::Write("Failed to update scene");
			return false;
		}

		directx_->EndScene();

		return true;
	}

	void Application::CalculateFrameStats()
	{
		static int frame_count = 0;
		static float time_elapsed = 0;

		frame_count++;

		if ((time_->GetTime() - time_elapsed) >= 1) {
			float fps = static_cast<float>(frame_count);
			float ms_per_frame = 1000 / fps;

			std::wostringstream window_caption;
			window_caption.precision(6);
			window_caption << window_caption_ << " | FPS: " << fps << " Frame time: " << ms_per_frame << "ms";
			SetWindowTextW(window_, static_cast<LPCWSTR>(window_caption.str().c_str()));
			frame_count = 0;
			time_elapsed++;
		}
	}

	bool Application::LoadScene()
	{
		auto scene = std::make_shared<Scene>();

		auto camera = std::make_shared<GameObject>("Camera");
		camera->AddComponent(std::make_shared<Camera>());
		camera->GetTransform()->SetPosition(0, 0, -5);
		camera->GetTransform()->SetRotationByDegrees(0, 180, 0);
		scene->AddGameObject(camera);

		auto text = std::make_shared<GameObject>("Text");
		text->AddComponent(std::make_shared<Text>(
			"Hello World",
			"../assets/shaders/font_vs.cso",
			"../assets/shaders/font_ps.cso",
			"../assets/textures/fonts/consolas.png"));
		text->GetTransform()->SetPosition(1, 1, 0);
		text->GetTransform()->SetScale(0.005f);
		scene->AddGameObject(text);

		auto cube = std::make_shared<GameObject>("Cube");
		cube->AddComponent(std::make_shared<Model>(
			"../assets/meshes/cube.mesh",
			"../assets/shaders/default_vs.cso",
			"../assets/shaders/default_ps.cso",
			"../assets/textures/placeholder.png"));
		cube->GetTransform()->SetPosition(0, 0, 0);
		scene->AddGameObject(cube);

		scene->SetCamera(std::static_pointer_cast<Camera>(camera->GetComponent("Camera")));

		scene_ = scene;

		return true;
	}
}