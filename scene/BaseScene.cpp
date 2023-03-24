#include "BaseScene.h"

using namespace std;

BaseScene::BaseScene(DirectXCommon* dxCommon, Window* window)
{
	this->dxCommon = dxCommon;
	this->window = window;
}

void BaseScene::Application()
{
	input = Input::GetInstance();
	camera = Camera::GetInstance();
}

void BaseScene::Initialize()
{
	//Input������
	input->Initialize(window->GetHwnd());

	//�J����
	camera->Initialize(window);

	//Skydome
	skydome = make_unique<SampleObjObject>();
    skydome->Initialize("skydome");
}

void BaseScene::Update()
{
	//���͏��X�V
	input->Update();

#ifdef _DEBUG
	{
		//���W
		ImGui::SetNextWindowPos(ImVec2{0,40});
		//�T�C�Y
		ImGui::SetNextWindowSize(ImVec2{300,55});
		ImGui::Begin("Demo");
		//�f���E�B���h�E 
		ImGui::Checkbox("demoWindow", &show_demo_window);
		//�t���O�ɂ��o����
		if(show_demo_window)	ImGui::ShowDemoWindow(&show_demo_window);
		ImGui::End();
	}
#endif // _DEBUG

	//Skydome
	skydome->Update(camera);
}

void BaseScene::Draw()
{
	//Skydome
	skydome->Draw();
}

void BaseScene::Finalize()
{
	skydome->Finalize();
}

void BaseScene::NextSceneChange()
{
}

void BaseScene::EndUpdate()
{
#pragma region �ėp�@�\�X�V
	//�J����
	camera->Update();
#pragma endregion
}
