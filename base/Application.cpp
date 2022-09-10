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
	window = new Window("�S�����G���W��", 1280, 720);
	dxCommon = new DirectXCommon();
	sceneManager = new SceneManager();
}

Application::~Application()
{
	delete window;
	delete dxCommon;
	delete sceneManager;
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
		if(sceneManager->GetIsAlive()) break;

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

	//�V�[���ǉ�
	sceneManager->Add("Area01", new GameScene01(window));

	//�V�[���w��
	sceneManager->Change("Area01");

	//�e�N�X�`���}�l�[�W���[
	TextureManager::GetInstance()->Initialize(dxCommon);
	TextureManager::Load(0,"white1x1.png");

	//�X�v���C�g
	Sprite::StaticInitialize(dxCommon, window->GetWindowWifth(), window->GetWindowHeight());
}

void Application::Update()
{
	sceneManager->Update();
}

void Application::Draw()
{
	dxCommon->BeginDraw();
	Sprite::PreDraw();

	sceneManager->Draw();

	dxCommon->EndDraw();
}

void Application::Finalize()
{
	window->Finalize();
}
