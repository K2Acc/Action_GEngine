#include "SampleScene.h"

using namespace std;

SampleScene::SampleScene(DirectXCommon *dxCommon, Window *window)
		: BaseScene(
		dxCommon,
		window)
{
}

void SampleScene::Application()
{
	BaseScene::Application();
}

void SampleScene::Initialize()
{
	BaseScene::Initialize();

#pragma region _3D������
	obj = make_unique<SampleFbxObject>();
	obj->Initialize("cube");
	obj->SetPosition({0, 0, 100});
#pragma endregion _3D������
}

void SampleScene::Update()
{
	BaseScene::Update();

#pragma region _3D�X�V
	obj->Update(camera);
#pragma endregion _3D�X�V

	BaseScene::EndUpdate();
}

void SampleScene::Draw()
{
	BaseScene::Draw();

#pragma region _3D�`��
	obj->Draw();
#pragma endregion _3D�`��

	BaseScene::EndDraw();
}

void SampleScene::Finalize()
{
#pragma region _3D���
	obj->Finalize();
#pragma endregion _3D���

	BaseScene::Finalize();
}