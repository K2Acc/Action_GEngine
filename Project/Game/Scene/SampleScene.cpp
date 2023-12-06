#include "SampleScene.h"
#include <TextureUtility.h>

SampleScene::SampleScene(DirectXCommon *dxCommon, Window *window):
	BaseScene(
		dxCommon,
		window)
{
}

SampleScene::~SampleScene()
{
	Finalize();
}

void SampleScene::Application()
{
	BaseScene::Application();
	camera = FollowCamera::GetInstance();
}

void SampleScene::Initialize()
{
	//Input初期化
	input->Initialize();
	//カメラ
	camera->Initialize();

	//ライト
	lightGroup_ = LightGroup::Create();
	//3Dオブジェクト(.obj)にセット
	ObjModelObject::SetLight(lightGroup_);


	levelData = LevelLoader::Load("levelSample");

	//レベルデータからオブジェクトを生成
	for(auto& objectData : levelData->objects){
		//ファイル名からモデルを生成
		ObjModelManager* model_ = new ObjModelManager();
		model_->CreateModel(objectData.fileName);

		//モデルを指定して3Dオブジェクトを生成
		BaseObjObject* newObj_ = new BaseObjObject();
		newObj_->Initialize(model_);

		//座標
		newObj_->SetPosition(objectData.translation);
		//回転
		newObj_->SetRotation(objectData.rotation);
		//スケール
		newObj_->SetScale(objectData.scaling);

		//配列に登録
		objects.push_back(newObj_);
	}


	player = new Action();
	player->Initialize();
	player->SetPosition({0,1,0});

#ifdef _DEBUG
	debugCamera = DebugCamera::GetInstance();
	debugCamera->Initialize();
#endif // _DEBUG
}

void SampleScene::Update()
{
		//入力情報更新
	input->Update();

	lightGroup_->Update();

	for(auto& object : objects){
		object->Update(camera);
	}

	player->Update(camera);
	camera->Update(player->GetmatWorld(), {0,1,-15});


#ifdef _DEBUG
	//デバックカメラ
	if(isDebugCamera){
		debugCamera->Update();
		//camera->SetWorld(debugCamera->GetWorld());
		camera->Update(debugCamera->GetWorld());
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

	{
		//座標
		ImGui::SetNextWindowPos(ImVec2{0,60});
		//サイズ
		ImGui::SetNextWindowSize(ImVec2{300,100});
		ImGui::Begin(player->GetName());
		player->SetPosition(imgui->ImGuiDragVector3("Pos", player->GetPosition(),0.1f));
		player->SetRotation(imgui->ImGuiDragVector3("Rot", player->GetRotation(), 0.1f));
		player->SetScale(imgui->ImGuiDragVector3("Sca", player->GetScale(),0.1f));
		ImGui::End();
	}

	{
		//座標
		ImGui::SetNextWindowPos(ImVec2{0,260});
		//サイズ
		ImGui::SetNextWindowSize(ImVec2{300,140});
		ImGui::Begin(camera->GetName());
		camera->SetPosition(imgui->ImGuiDragVector3("Pos", camera->GetPosition(), 0.1f));
		camera->SetRotation(imgui->ImGuiDragVector3("Rot", camera->GetRotation(), 0.1f));
		if(ImGui::Button("Shake")) camera->ShakeStart();
		Vector2 temp = imgui->ImGuiDragVector2("frame/power ", {camera->GetShakeMaxFrame(), (float)camera->GetShakeMaxPower()}, 0.1f, 2.f);
		camera->SetShake(temp.x,(int)temp.y);
		ImGui::Checkbox("Debug", &isDebugCamera);
		ImGui::End();
	}

	lightGroup_->DebugUpdate();

#endif // _DEBUG

}

void SampleScene::Draw()
{
	for(auto& object : objects){
		object->Draw();
	}

	player->Draw();
}

void SampleScene::DrawBack()
{
}

void SampleScene::DrawNear()
{
	Sprite::SetPipelineState();
}

void SampleScene::Finalize()
{
	for(auto& object : objects){
		object->Finalize();
		delete object;
	}

	player->Finalize();
	delete player;
}
