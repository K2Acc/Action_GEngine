#pragma once
#include "../BaseGeometryObjects.h"

class SampleGeometryObject : public BaseGeometryObjects
{
	//�����o�֐�
public:
	~SampleGeometryObject();

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
};

