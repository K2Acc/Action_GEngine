#pragma once
#include "CollisionPrimitive.h"
#include "../RaycastHit.h"

#include <d3d12.h>
#include <forward_list>

class BaseCollider;


class CollisionManager
{
//�ÓI�����o�֐�
public:
	static CollisionManager* GetInstance();

//�����o�֐�
public:
	
	/// <summary>
	/// �R���C�_�[�̒ǉ�
	/// </summary>
	/// <param name="collider">�R���C�_�[</param>
	inline void AddCollider(BaseCollider* collider){
		colliders.push_front(collider);
	}

	/// <summary>
	/// �R���C�_�[�폜
	/// </summary>
	/// <param name="collider">�R���C�_�[</param>
	inline void RemoveCollider(BaseCollider* collider){
		colliders.remove(collider);
	}

	/// <summary>
	/// ���C�L���X�g
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="hitinfo">�Փˏ��</param>
	/// <param name="maxDistance">�ő勗��</param>
	/// <returns>���C���C�ӂ̃R���C�_�[�ƌ����ꍇtrue, ����ȊO��false</returns>
	bool Raycast(const Ray& ray, RaycastHit* hitinfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);

	/// <summary>
	/// ���ׂĂ̏Փ˃`�F�b�N
	/// </summary>
	void CheckAllCollisions();

private:
	CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	~CollisionManager() = default;
	CollisionManager& operator=(const CollisionManager&) = delete;
	//�R���C�_�[�̃��X�g
	std::forward_list<BaseCollider*> colliders;
};

