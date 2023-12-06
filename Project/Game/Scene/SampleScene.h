#pragma once
#include "BaseScene.h"
#include "Action/Action.h"
#include "FollowCamera.h"

class SampleScene : public BaseScene
{
public:
	SampleScene(DirectXCommon* dxCommon, Window* window);
	~SampleScene();

	void Application() override;
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void DrawBack() override;
	void DrawNear() override;
	void Finalize() override;

private:
	Action* player = nullptr;
	FollowCamera* camera = nullptr;

	//json
	LevelData* levelData = nullptr;
	std::vector<BaseObjObject*>objects;

#ifdef _DEBUG
	//借り物
	DebugScreenText* debugText = nullptr;

	DebugCamera* debugCamera = nullptr;
	bool isDebugCamera = false;
	
	imguiManager* imgui = nullptr;
	bool show_demo_window = false;
#endif // _DEBUG
};

