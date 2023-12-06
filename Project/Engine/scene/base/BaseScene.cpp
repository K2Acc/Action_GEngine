#include "BaseScene.h"
#include "TextureUtility.h"

using namespace std;

BaseScene::BaseScene(DirectXCommon* dxCommon, Window* window)
{
	this->dxCommon = dxCommon;
	this->window = window;
}

void BaseScene::Application()
{
	input = Input::GetInstance();
	//camera = Camera::GetInstance();
	postEffect = PostEffect::GetInstance();
	postEffect = PostEffect::Create(white1x1_tex.number, {0,0}, {500,500});
}

void BaseScene::Initialize()
{
	//Input初期化
	input->Initialize();

	//カメラ
	//camera->Initialize();

	isDrawStop = false;


	//ライト
	lightGroup_ = LightGroup::Create();
	//3Dオブジェクト(.obj)にセット
	ObjModelObject::SetLight(lightGroup_);


#ifdef _DEBUG
	debugCamera = DebugCamera::GetInstance();
	debugCamera->Initialize();
#endif // _DEBUG
}

void BaseScene::Update()
{
	//入力情報更新
	input->Update();

	//camera->Update();
#ifdef _DEBUG
	//デバックカメラ
	if(isDebugCamera){
		debugCamera->Update();
		//camera->SetWorld(debugCamera->GetWorld());
	}
#endif // _DEBUG

#ifdef _DEBUG
	{
		//座標
		ImGui::SetNextWindowPos(ImVec2{0,0});
		//サイズ
		ImGui::SetNextWindowSize(ImVec2{300,55});
		ImGui::Begin("Demo");
		//デモウィンドウ 
		ImGui::Checkbox("demoWindow", &show_demo_window);
		//フラグによる出現物
		if(show_demo_window)	ImGui::ShowDemoWindow(&show_demo_window);
		ImGui::End();
	}

	lightGroup_->DebugUpdate();
#endif // _DEBUG
}

void BaseScene::EndUpdate()
{
}

void BaseScene::Draw()
{
}

void BaseScene::DrawBack()
{
}

void BaseScene::DrawNear()
{
	Sprite::SetPipelineState();
}

void BaseScene::Finalize()
{
}
