#include "Application.h"
#include "GameScene01.h"

Application* Application::app = nullptr;

using namespace std;

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

	delete objectFbx;
	delete modelFbx;
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
#pragma region �ėp�@�\������
	//Window����
	window->Create("GiliraEngine", 1280, 720);

	//DirectXCommon
	dxCommon->Initialize(window);

	//Input������
	input->Initialize(window->GetHwnd());

	////�e�N�X�`��
	TextureManager::GetInstance()->Initialize(dxCommon);
	TextureManager::Load(0, "Texture.jpg");

	//�J����
	camera = Camera::GetInstance();
	camera->Initialize();

	//FBX�}�l�[�W���[
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());

#pragma endregion

#pragma region �X�v���C�g������
	//�X�v���C�g�ÓI������
	Sprite::StaticInitialize(dxCommon, window->GetWindowWidth(), window->GetWindowHeight());
	//����
	sprite = Sprite::Create(0, Vector2(100.f,100.f));
	sprite->SetSize(Vector2(100,100));
#pragma endregion

#pragma region �I�u�W�F�N�g������

	////�􉽊w�I�u�W�F�N�g�ÓI������
	//GeometryObjectManager::GetInstance()->CreateBuffer();
	////����
	//worldTransform.Initialize();
	//GeometryObject::StaticInitialize(dxCommon);
	//object = GeometryObject::Create(0);

	//Fbx
	modelFbx = FbxLoader::GetInstance()->LoadModelFromFile("cube");
	FbxModelObject::SetDevice(dxCommon->GetDevice());
	FbxModelObject::SetCamera(camera);
	FbxModelObject::CreateGraphicsPipeline();

	objectFbx = new FbxModelObject();
	objectFbx->Initialize();
	objectFbx->SetModel(modelFbx);

#pragma endregion

#ifdef _DEBUG
	//�ꎞ��~
	sceneStopper = SceneStopper::GetInstance();

#endif // _DEBUG
}

void Application::Update()
{
#pragma region �ėp�@�\�X�V
	//���͏��X�V
	input->Update();
	//�J����
	camera->Update();
#pragma endregion

#ifdef _DEBUG

#pragma region �ꎞ��~
	//����
	if(input->Trigger(DIK_F1)){
		if(!sceneStopper->GetIsSceneStop()){
			sceneStopper->SetIsSceneStop(true);
		}
		else if(sceneStopper->GetInstance()){
			sceneStopper->SetIsSceneStop(false);
		}
	}
	//��~
	if(sceneStopper->GetIsSceneStop()) return;
#pragma endregion

#endif // _DEBUG

#pragma region ���͏���
	/*{
		if(input->Push(DIK_SPACE)){
			sprite->SetColor({1,0,0,1});
		}
		else{
			sprite->SetColor({1,1,1,1});
		}
	}
	{
		if(input->Push(DIK_RIGHT)){
			worldTransform.rotation.y += XMConvertToRadians(1.f);
		}
		else if(input->Push(DIK_LEFT)){
			worldTransform.rotation.y += XMConvertToRadians(-1.f);
		}
		worldTransform.UpdateMatrix();
	}*/
#pragma endregion

#pragma region �X�v���C�g�X�V


#pragma endregion

#pragma region �I�u�W�F�N�g�X�V
	//object->Update(worldTransform, camera);
	objectFbx->Update();
#pragma endregion

}

void Application::Draw()
{
	//�`��O����
	dxCommon->BeginDraw();
	//object->Draw();
	objectFbx->Draw(dxCommon->GetCommandList());

	Sprite::SetPipelineState();
	sprite->Draw();

	//�`��㏈��
	dxCommon->EndDraw();
}

void Application::Finalize()
{
	FbxLoader::GetInstance()->Finalize();
	GeometryObject::StaticFinalize();
	Sprite::StaticFinalize();
	window->Finalize();
}
