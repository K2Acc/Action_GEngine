#include "CollisionManager.h"
#include "BaseCollider.h"
#include "Collision.h"

using namespace DirectX;

CollisionManager *CollisionManager::GetInstance()
{
	static CollisionManager instance;
	return &instance;
}

bool CollisionManager::Raycast(const Ray &ray, RaycastHit *hitinfo, float maxDistance)
{
	bool result = false;
	//�����p�̃C�e���[�^
	std::forward_list<BaseCollider*>::iterator it;
	//���܂ōł��߂��R���C�_�[���L�^����ׂ̃C�e���[�^
	std::forward_list<BaseCollider*>::iterator it_hit;
	//���܂łōł��߂��R���C�_�[�̋������L�^����ϐ�
	float distance = maxDistance;
	//���܂łōł��߂��R���C�_�[�Ƃ̌�_���L�^����ϐ�
	XMVECTOR inter;

	//���ׂăR���C�_�[�Ƒ�������`�F�b�N
	it = colliders.begin();
	for(; it != colliders.end(); ++it){
		BaseCollider* colA = *it;
		//���̏ꍇ
		if(colA->GetShapeType() == COLLIDIONSHAPE_SPHERE){
			Sphere* sphere = dynamic_cast<Sphere*>(colA);
			float tempDistance;
			XMVECTOR tempinter;
			//������Ȃ���Ώ��O
			if(!Collision::CheckRay2Sphere(ray, *sphere, &tempDistance, &tempinter)) continue;
			//�������ŏ��łȂ���Ώ��O
			if(tempDistance >= distance) continue;
			//���܂łōł��߂��̂ŋL�^���Ƃ�
			result = true;
			distance = tempDistance;
			inter = tempinter;
			it_hit = it;
		}
	}

	//�ŏI�I�ɂȂɂ��ɓ������Ă����猋�ʂ���������
	if(result && hitinfo){
		hitinfo->distance = distance;
		hitinfo->inter = inter;
		hitinfo->collider = *it_hit;
		hitinfo->object = hitinfo->collider->GetObjObject();
	}

	return result;
}

void CollisionManager::CheckAllCollisions()
{
	std::forward_list<BaseCollider*>::iterator itA;
	std::forward_list<BaseCollider*>::iterator itB;

	//���ׂĂ̑g�����ɂ��đ�������`�F�b�N
	itA = colliders.begin();
	for(; itA != colliders.end(); ++itA){
		itB = itA;
		++itB;

		for(; itB != colliders.end(); ++itB){
			BaseCollider* colA = *itA;
			BaseCollider* colB = *itB;

			//�Ƃ��ɋ�
			if(colA->GetShapeType() == COLLIDIONSHAPE_SPHERE &&
				colB->GetShapeType() == COLLIDIONSHAPE_SPHERE){
				Sphere* SphereA = dynamic_cast<Sphere*>(colA);
				Sphere* SphereB = dynamic_cast<Sphere*>(colB);
				DirectX::XMVECTOR inter;
				if(Collision::CheckSphere2Sphere(*SphereA, *SphereB, &inter)){
					colA->OnCollision(CollisionInfo(colB->GetObjObject(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetObjObject(), colA, inter));
				}
			}
		}
	}
}
