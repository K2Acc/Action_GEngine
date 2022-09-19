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
	window = Window::GetInstance();
	dxCommon = DirectXCommon::GetInstance();
	input = Input::GetInstance();
}

Application::~Application()
{
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
	window->Create("GiliraEngine", 1280, 720);

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

	//�􉽊w�}�l�[�W���[
	GeometryObjectManager::GetInstance()->CreateBuffer();

	//�􉽊w�I�u�W�F�N�g

	worldTransform.Initialize();

	GeometryObject::StaticInitialize(dxCommon);
	object = GeometryObject::Create(0);
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

	object->Update(worldTransform);
}

void Application::Draw()
{
	//�`��O����
	dxCommon->BeginDraw();
	Sprite::SetPipelineState();

	sprite->Draw();
	object->Draw();

	//�`��㏈��
	dxCommon->EndDraw();
}

void Application::Finalize()
{
	GeometryObject::StaticFinalize();
	Sprite::StaticFinalize();
	window->Finalize();
}
