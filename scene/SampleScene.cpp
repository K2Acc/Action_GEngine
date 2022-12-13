#include "SampleScene.h"

#include "../Engine/math//Easing/Easing.h"

#include "../Game/Collision/CollisionSystem/Collision.h"
#include "../Game/Collision/SphereCollider.h"

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
	//���C�g
	lightGroup = LightGroup::Create();
	//�F�ݒ�
	lightGroup->SetAmbientColor({1,1,1});
	//3D�I�u�W�F�N�g(.obj)�ɃZ�b�g
	ObjModelObject::SetLight(lightGroup);

	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, true);

	//�ۉe
	lightGroup->SetCircleShadowActive(0, true);

	//�Փ˃}�l�[�W���[
	collisionManager = CollisionManager::GetInstance();

	//�p�[�e�B�N��
	particle = ParticleManager::GetInstance();
	particleWorld.Initialize();

#pragma endregion �ėp������

#pragma region _3D������

	player = make_unique<Player>();
	player->Initialize("chr_sword");
	player->SetPosition({1, 0, 0});

	obj3_2 = make_unique<SampleObjObject>();
	obj3_2->Initialize("ground");
	obj3_2->SetPosition({0,-1,0});

	obj3_3 = make_unique<SampleObjObject>();
	obj3_3->Initialize("sphere", true);
	obj3_3->SetPosition({-1,0,0});
	obj3_3->SetCollider(new SphereCollider);

	obj3_4 = make_unique<SampleObjObject>();
	obj3_4->Initialize("skydome");

#pragma endregion _3D������

#pragma region _2D������

#pragma endregion _2D������

#ifdef _DEBUG
	imgui = new imguiManager();
	imgui->Initialize(window, dxCommon);
#endif // _DEBUG

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

	if(input->Push(DIK_SPACE)){
		CreateParticles();
	}

#pragma endregion ���͏���

#pragma region _3D�X�V
	player->Update(camera);

	obj3_2->Update(camera);
	Vector3 rot2 = obj3_3->GetRotation();
	rot2.y += XMConvertToRadians(1.f);
	obj3_3->SetRotation(rot2);
	obj3_3->Update(camera);

	obj3_4->Update(camera);
#pragma endregion _3D�X�V

#pragma region _2D�X�V

#pragma endregion _2D�X�V

#pragma region �ėp�X�V
	{
		//�ۉe
		lightGroup->SetCircleShadowDir(0, DirectX::XMVECTOR({circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0}));
		lightGroup->SetCircleShadowAtten(0, Vector3(circleShadowAtten[0], circleShadowAtten[1], circleShadowAtten[2]));
		lightGroup->SetCircleShadowFactorAngle(0, Vector2(circleShadowFactorAngle[0], circleShadowFactorAngle[1]));

		//�v���C���[�A�ۉe���W
		lightGroup->SetCircleShadowCasterPos(0, player->GetPosition());
	}
	lightGroup->Update();


	//���C�����蔻��
	Ray ray;
	ray.start = {10.0f, 0.5f, 0.0f, 1};
	ray.dir = {-1,0,0,0};
	RaycastHit raycastHit;

	if(collisionManager->Raycast(ray, &raycastHit)){
		debugText->Print("RaycastHit", 0.0f, 32.0f);
		debugText->Printf(0.f, 48.f, 1.f, "hitPos (X:%f, Y:%f, Z:%f)", raycastHit.inter.m128_f32[0], raycastHit.inter.m128_f32[1], raycastHit.inter.m128_f32[2]);
		//�Փ˓_�Ƀp�[�e�B�N���𔭐�������
		for(int i = 0; i < 1; i++){
			Vector3 pos{};
			pos.x = raycastHit.inter.m128_f32[0];
			pos.y = raycastHit.inter.m128_f32[1];
			pos.z = raycastHit.inter.m128_f32[2];
		
			const float md_vel = 0.1f;
			Vector3 vel{};
			vel.x = (float)rand() / RAND_MAX * md_vel - md_vel/2.0f;
			vel.y = (float)rand() / RAND_MAX * md_vel - md_vel/2.0f;
			vel.z = (float)rand() / RAND_MAX * md_vel - md_vel/2.0f;

			//ParticleManager::GetInstance()->Add(10, pos, vel, Vector3{0,0,0}, 0.0f, 1.0f);
		}
	}


	//���ׂĂ̏Փ˂��`�F�b�N
	collisionManager->CheckAllCollisions();

	//�p�[�e�B�N��
	particle->Update(particleWorld, camera);
	particleWorld.UpdateMatrix();

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
	ImGui::DragFloat3("circlePos", (float*)&player->GetPosition(), 0.1f);
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
	player->Draw();
	obj3_2->Draw();
	obj3_3->Draw();
	obj3_4->Draw();

#pragma region �p�[�e�B�N��
	particle->Draw();
#pragma endregion �p�[�e�B�N��

#pragma endregion _3D�`��

#pragma region _2D_UI�`��
	Sprite::SetPipelineState();

#ifdef _DEBUG
	debugText->Printf(0,0,1.f,"Camera Target  X:%f, Y:%f, Z:%f", camera->GetTarget().x, camera->GetTarget().y, camera->GetTarget().z);
	debugText->Printf(0,16,1.f,"Camera Eye  X:%f, Y:%f, Z:%f", camera->GetEye().x, camera->GetEye().y, camera->GetEye().z);

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
	player->Finalize();
	obj3_2->Finalize();
	obj3_3->Finalize();
	obj3_4->Finalize();
#pragma endregion _3D���

#pragma region _2D���

#pragma endregion _2D���

#pragma region �ėp���

	delete lightGroup;
	lightGroup = nullptr;

	particleWorld = {};

#pragma endregion �ėp���

	BaseScene::Finalize();
}

void SampleScene::CreateParticles()
{
	for (int i = 0; i < 10; i++) {
		// X,Y,Z�S��[-5.0f,+5.0f]�Ń����_���ɕ��z
		const float rnd_pos = 10.0f;
		Vector3 pos{};
		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		const float rnd_vel = 0.1f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		Vector3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		// �ǉ�
		particle->Add(60, pos, vel, acc, 1.0f, 0.0f);
	}
}