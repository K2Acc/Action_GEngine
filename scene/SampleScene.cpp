#include "SampleScene.h"

#include "../Engine/math//Easing/Easing.h"

//���C�g�������悤
#include <sstream>
#include <iomanip>

using namespace std;
using namespace DirectX;

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

#pragma region �ėp������
	lightGroup = LightGroup::Create();
	//�F�ݒ�
	lightGroup->SetAmbientColor({1,1,1});
	//3D�I�u�W�F�N�g(.obj)�ɃZ�b�g
	ObjModelObject::SetLight(lightGroup);

	lightGroup->SetDirLightActive(0, false);
	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);
	lightGroup->SetPointLightActive(0, true);
	pointLightPos[0] = 0.5f;
	pointLightPos[1] = 1.0f;
	pointLightPos[2] = 0.0f;
#pragma endregion �ėp������

#pragma region _3D������
	//obj = make_unique<SampleFbxObject>();
	//obj->Initialize("cube");
	//obj->SetPosition({0, -50, 200});

	//obj2 = make_unique<SampleParticleObject>();
	//obj2->Initialize(1);

	obj3_1 = make_unique<SampleObjObject>();
	obj3_1->Initialize("chr_sword");
	obj3_1->SetPosition({1,0,0});

	obj3_2 = make_unique<SampleObjObject>();
	obj3_2->Initialize("ground");
	obj3_2->SetPosition({0,-1,0});

	obj3_3 = make_unique<SampleObjObject>();
	obj3_3->Initialize("sphere", true);
	obj3_3->SetPosition({-1,0,0});


	obj3_4 = make_unique<SampleObjObject>();
	obj3_4->Initialize("skydome");

#pragma endregion _3D������

#pragma region _2D������
	//sp = make_unique<SampleSprite>();
	//sp->Initialize(0);
#pragma endregion _2D������
}

void SampleScene::Update()
{
	BaseScene::Update();

#pragma region ���͏���

	if(input->Push(DIK_A)){
		camera->RotVector({0.f, XMConvertToRadians(3.f), 0.f});
	}
	else if(input->Push(DIK_D)){
		camera->RotVector({0.f,XMConvertToRadians(-3.f), 0.f});
	}

#pragma endregion ���͏���

#pragma region _3D�X�V
	/*Vector3 pos = Easing_Linear_Point2({0, 0, 300}, {0, -100, 300}, Time_OneWay(time, 2.f));
	Vector3 pos = Easing_Linear_Point3({0, 0, 300}, {0, -50, 500}, {0, -100, 300}, Time_Loop(time, 2.f));
	obj->SetPosition(pos);*/

	//obj->Update(camera);

	//obj2->Update(camera);

	Vector3 rot1 = obj3_1->GetRotation();
	rot1.y += XMConvertToRadians(1.f);
	obj3_1->SetRotation(rot1);
	obj3_1->Update(camera);

	obj3_2->Update(camera);

	Vector3 rot2 = obj3_3->GetRotation();
	rot2.y += XMConvertToRadians(1.f);
	obj3_3->SetRotation(rot2);
	obj3_3->Update(camera);

	obj3_4->Update(camera);
#pragma endregion _3D�X�V

#pragma region _2D�X�V
	//sp->Update();
#pragma endregion _2D�X�V

#pragma region �ėp�X�V
	{
		/*lightGroup->SetAmbientColor(Vector3{1,1,1});
		lightGroup->SetDirLightDir(0, XMVECTOR{0,-1,0});
		lightGroup->SetDirLightColor(0, Vector3{1,1,1});*/

		lightGroup->SetPointLightPos(0, Vector3{pointLightPos[0], pointLightPos[1], pointLightPos[2]});
		lightGroup->SetPointLightColor(0, Vector3(pointLightColor[0], pointLightColor[1], pointLightColor[2]));
		lightGroup->SetPointLightAtten(0, Vector3(pointLightAtten[0], pointLightAtten[1], pointLightAtten[2]));
	}
	lightGroup->Update();

#pragma endregion �ėp�X�V

	BaseScene::EndUpdate();
}

void SampleScene::Draw()
{
	BaseScene::Draw();

#pragma region _2D_�w�i�`��
	//sp->Draw();
#pragma endregion _2D_�w�i�`��

#pragma region _3D�`��
	//obj->Draw();

	//obj2->Draw();

	obj3_1->Draw();
	obj3_2->Draw();
	obj3_3->Draw();
	obj3_4->Draw();

#pragma endregion _3D�`��

#pragma region _2D_UI�`��
	Sprite::SetPipelineState();

#pragma endregion _2D_UI�`��

	BaseScene::EndDraw();
}

void SampleScene::Finalize()
{
#pragma region _3D���
	//obj->Finalize();

	//obj2->Finalize();

	obj3_1->Finalize();
	obj3_2->Finalize();
	obj3_3->Finalize();
	obj3_4->Finalize();
#pragma endregion _3D���

#pragma region _2D���
	//sp->Finalize();
#pragma endregion _2D���

#pragma region �ėp���

	delete lightGroup;
	lightGroup = nullptr;

#pragma endregion �ėp���

	BaseScene::Finalize();
}