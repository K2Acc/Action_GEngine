#pragma once
#include "../BaseParticleObjects.h"

class SampleParticleObject : public BaseParticleObjects
{
	//�����o�֐�
public:
	~SampleParticleObject();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(UINT texNumber) override;

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

	void Add(int life, Vector3 position, Vector3 velocity, Vector3 accel, float start_scale, float end_scale);
};

