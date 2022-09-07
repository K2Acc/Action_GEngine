#include "Application.h"

Application* Application::app = nullptr;

Application *Application::GetInstance()
{
	if(!app)
	{
		app = new Application();
	}
	return app;
}

void Application::Delete()
{
	if(app){
		delete app;
		app = nullptr;
	}
}

Application::Application()
{
	window = new Window("�S�����G���W��", 1280, 720);
	dxCommon = new DirectXCommon();
}

Application::~Application()
{
	delete window;
	delete dxCommon;
}

void Application::Run()
{
	MSG msg{};//���b�Z�[�W
	while (true)
	{
		if(PeekMessage(&msg, nullptr,0, 0,PM_REMOVE)){
			TranslateMessage(&msg);	//�L�[���̓��b�Z�[�W�̏���
			DispatchMessage(&msg);	//�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
		}
		if(msg.message == WM_QUIT) {
			break;
		}

		Update();
		Draw();
	}
}

void Application::Initialize()
{
	//Window����
	window->Create();

	//DirectXCommon
	dxCommon->Initialize(window);
}

void Application::Update()
{

}

void Application::Draw()
{
	dxCommon->BeginDraw();

	dxCommon->EndDraw();
}

void Application::Finalize()
{
	window->Finalize();
}
