#pragma once
#include "../BaseObjects.h"
#include "../../../Engine/input/Input.h"

class SampleFbxObject : public BaseObjects
{
//�����o�֐�
public:
	~SampleFbxObject();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(std::string filePath) override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(Camera* camera);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �㏈��
	/// </summary>
	void Finalize() override;

//�����o�ϐ�
private:
	Input* input = nullptr;
};
