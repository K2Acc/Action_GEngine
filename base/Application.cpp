#include "Application.h"
#include "GameScene01.h"

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
	window = new Window("GiliraEngine", 1280, 720);
	dxCommon = new DirectXCommon();
	input = Input::GetInstance();
}

Application::~Application()
{
	delete window;
	delete dxCommon;
	delete sprite;
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

	//Input������
	input->Initialize(window->GetHwnd());

	////�e�N�X�`��
	TextureManager::GetInstance()->Initialize(dxCommon);
	TextureManager::Load(0, "white1x1.png");

	////�X�v���C�g
	Sprite::StaticInitialize(dxCommon, window->GetWindowWidth(), window->GetWindowHeight());

	sprite = Sprite::Create(0, Vector2(100.f,100.f));
	sprite->SetSize(Vector2(100,100));
}

void Application::Update()
{
	//���͏��X�V
	input->Update();

	if(input->Push(DIK_SPACE)){
		sprite->SetColor({1,0,0,1});
	}
	else{
		sprite->SetColor({1,1,1,1});
	}
}

void Application::Draw()
{
	//�`��O����
	dxCommon->BeginDraw();
	Sprite::SetPipelineState();

	sprite->Draw();

	//�`��㏈��
	dxCommon->EndDraw();
}

void Application::Finalize()
{
	Sprite::StaticFinalize();
	window->Finalize();
}
