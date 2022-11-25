#pragma once

#include "Camera.h"

#include "../../Engine/3D/GeometryObject.h"

class BaseGeometryObjects
{
public:
	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize(UINT texNumber);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="matBillboard">�r���{�[�h�J�n</param>
	virtual void Update(Camera* camera, bool IsBillboard = false);

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// �㏈��
	/// </summary>
	virtual void Finalize();

	//Getter
	const Vector3& GetPosition()	{return world.translation;}
	const Vector3& GetRotation()	{return world.rotation;}

	//Setter
	void SetPosition(const Vector3& position)	{world.translation = position;}
	void SetRotation(const Vector3& rotation)	{world.rotation = rotation;}

protected:
	GeometryObject* object;
	WorldTransform world;

	Camera* camera = nullptr;
};

