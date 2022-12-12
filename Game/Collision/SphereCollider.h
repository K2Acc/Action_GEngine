#pragma once
#include "CollisionSystem//BaseCollider.h"
#include "CollisionSystem//CollisionPrimitive.h"

#include <DirectXMath.h>

/// <summary>
/// ���Փ˔���I�u�W�F�N�g
/// </summary>
class SphereCollider : public BaseCollider, public Sphere
{
//�G�C���A�X
private:
	using XMVECTOR = DirectX::XMVECTOR;

//�����o�֐�
public:
	SphereCollider(XMVECTOR offset = {0,0,0,0}, float radius = 1.0f):
		offset(offset),
		radius(radius)
	{
		//���`����Z�b�g
		shapeType = COLLIDIONSHAPE_SPHERE;
	}

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	inline void SetRadius(float radius)	{this->radius = radius;}

private:
	//�I�u�W�F�N�g���S����̃I�t�Z�b�g
	XMVECTOR offset;
	//���a
	float radius;
};

