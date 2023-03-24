#pragma once
#include "BaseScene.h"

#include "../Engine/3D/TouchableObject.h"

#include "../Game/3D/Player/Player.h"

#include "../Game/2D/SampleSprite/SampleSprite.h"

#include "../Game/Collision/CollisionSystem/CollisionPrimitive.h"
#include "../Game/Collision/CollisionSystem/CollisionManager.h"

class SampleScene : public BaseScene
{
public:
	SampleScene(DirectXCommon* dxCommon, Window* window);

	/// <summary>
	/// �N����
	/// </summary>
	void Application() override;

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// UI�`��
	/// </summary>
	void UIDraw();

	/// <summary>
	/// �㏈��
	/// </summary>
	void Finalize() override;

	//�V�[���J��
	void NextSceneChange() override;

private:
    //Collision
    CollisionManager* collisionManager = nullptr;

    //player
    std::unique_ptr<Player> player;

    //Plane
    static const int DIV_NUM = 10;
    static const int Plane_Size = 2;
    std::unique_ptr<TouchableObject> plane[DIV_NUM][DIV_NUM];

    //box
    std::unique_ptr<TouchableObject> box;

    //pyramid
    std::unique_ptr<TouchableObject> pyramid;
    
    //sphere
    std::unique_ptr<SampleObjObject> sphere;

#ifdef _DEBUG
	//�J�����ړ��A��]�ύX�t���O
	bool IsCameraMovementChange = false;
#endif // _DEBUG
};

