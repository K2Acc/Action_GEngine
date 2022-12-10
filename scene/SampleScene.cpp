#include "SampleScene.h"

#include "../Engine/math//Easing/Easing.h"

#include "../Game/Collision/Collision.h"

#include <sstream>
#include <iomanip>

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG


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

	//�ۉe
	lightGroup->SetCircleShadowActive(0, true);

#pragma endregion �ėp������

#pragma region _3D������
	//obj = make_unique<SampleFbxObject>();
	//obj->Initialize("cube");
	//obj->SetPosition({0, -50, 200});

	//obj2 = make_unique<SampleParticleObject>();
	//obj2->Initialize(1);

	obj3_1 = make_unique<SampleObjObject>();
	obj3_1->Initialize("chr_sword");
	obj3_1->SetPosition(fighterPos);

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
	//sp->Initialize(1);
	//sp->SetPosition({100,100});
#pragma endregion _2D������

#ifdef _DEBUG
	imgui = new imguiManager();
	imgui->Initialize(window, dxCommon);
#endif // _DEBUG


	//���̏����l��ݒ�
	sphere.center = XMVECTOR{0,2,0,1};
	sphere.radius = 1.0f;
	//���ʂ̏����l��ݒ�
	plane.normal = XMVECTOR{0,1,0,0};
	plane.distance = 0.0f;
}

void SampleScene::Update()
{
	BaseScene::Update();

#ifdef _DEBUG
	imgui->Begin();
#endif // _DEBUG

#pragma region ���͏���

	if(input->Push(DIK_A)){
		camera->RotVector({0.f, XMConvertToRadians(3.f), 0.f});
	}
	else if(input->Push(DIK_D)){
		camera->RotVector({0.f,XMConvertToRadians(-3.f), 0.f});
	}

	if(input->Push(DIK_W)){
		camera->RotVector({XMConvertToRadians(-3.f), 0.f, 0.f});
	}
	else if(input->Push(DIK_S)){
		camera->RotVector({XMConvertToRadians(3.f), 0.f, 0.f});
	}

	// ���ړ�
	{
		XMVECTOR moveY = XMVectorSet(0, 0.01f, 0, 0);
		if (input->Push(DIK_I)) { sphere.center += moveY; }
		else if (input->Push(DIK_K)) { sphere.center -= moveY; }

		XMVECTOR moveX = XMVectorSet(0.01f, 0, 0, 0);
		if (input->Push(DIK_L)) { sphere.center += moveX; }
		else if (input->Push(DIK_J)) { sphere.center -= moveX; }
	}

#pragma endregion ���͏���

#pragma region _3D�X�V
	/*Vector3 pos = Easing_Linear_Point2({0, 0, 300}, {0, -100, 300}, Time_OneWay(time, 2.f));
	Vector3 pos = Easing_Linear_Point3({0, 0, 300}, {0, -50, 500}, {0, -100, 300}, Time_Loop(time, 2.f));
	obj->SetPosition(pos);*/

	//obj->Update(camera);

	//obj2->Update(camera);

	obj3_1->SetPosition(fighterPos);
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
		//�ۉe
		lightGroup->SetCircleShadowDir(0, DirectX::XMVECTOR({circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0}));
		lightGroup->SetCircleShadowAtten(0, Vector3(circleShadowAtten[0], circleShadowAtten[1], circleShadowAtten[2]));
		lightGroup->SetCircleShadowFactorAngle(0, Vector2(circleShadowFactorAngle[0], circleShadowFactorAngle[1]));

		//�v���C���[�A�ۉe���W
		lightGroup->SetCircleShadowCasterPos(0, fighterPos);
	}
	lightGroup->Update();

#pragma endregion �ėp�X�V

#ifdef _DEBUG
	if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

	//�E�B���h�E�T�C�Y
	ImGui::SetNextWindowSize(ImVec2{500,55});
	//�E�B���h�E���W
	ImGui::SetNextWindowPos(ImVec2{0,130});
	//�J�n�A�^�C�g�����ݒ�
	ImGui::Begin("PlayerPos && SpotLightPos && CircleShadowPos");
	ImGui::SetNextWindowPos(ImVec2{0,0});
	ImGui::DragFloat3("circlePos", (float*)&fighterPos, 0.1f);
	//�I��
	ImGui::End();

	//�E�B���h�E�T�C�Y
	ImGui::SetNextWindowSize(ImVec2{500,100});
	//�E�B���h�E���W
	ImGui::SetNextWindowPos(ImVec2{0,0});
	//�J�n�A�^�C�g�����ݒ�
	ImGui::Begin("CircleShadow");
	ImGui::DragFloat3("circleShadowDir", circleShadowDir, 0.1f);
	ImGui::DragFloat3("circleShadowAtten", circleShadowAtten, 0.1f);
	ImGui::DragFloat2("circleShadowFactorAngle", circleShadowFactorAngle, 0.1f);
	//�I��
	ImGui::End();

	imgui->End();
#endif // _DEBUG

	BaseScene::EndUpdate();
}

void SampleScene::Draw()
{
	BaseScene::Draw();

#pragma region _2D_�w�i�`��
	
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
	//sp->Draw();

#ifdef _DEBUG
	debugText->Printf(0,0,1.f,"Camera Target  X:%f, Y:%f, Z:%f", camera->GetTarget().x, camera->GetTarget().y, camera->GetTarget().z);
	debugText->Printf(0,16,1.f,"Camera Eye  X:%f, Y:%f, Z:%f", camera->GetEye().x, camera->GetEye().y, camera->GetEye().z);

	debugText->Printf(0, 48, 1.f, "Sphere(X:%f, Y:%f, Z:%f)", sphere.center.m128_f32[0], sphere.center.m128_f32[1], sphere.center.m128_f32[2]);
	//���ƕ��ʂ̓����蔻��
	bool hit = Collision::CheckSphere2Plane(sphere, plane);
	if(hit){
		debugText->Print("Hit", 0, 64, 1.0f);
	}

#endif // _DEBUG
	BaseScene::EndDraw();
#pragma endregion _2D_UI�`��

#ifdef _DEBUG
	imgui->Draw();
#endif // _DEBUG

}

void SampleScene::Finalize()
{
#ifdef _DEBUG
	imgui->Finalize();
	delete imgui;
#endif // _DEBUG

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