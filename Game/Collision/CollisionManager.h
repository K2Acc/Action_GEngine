#pragma once
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

