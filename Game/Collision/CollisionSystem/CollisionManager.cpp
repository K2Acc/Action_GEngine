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
	//走査用のイテレータ
	std::forward_list<BaseCollider*>::iterator it;
	//今まで最も近いコライダーを記録する為のイテレータ
	std::forward_list<BaseCollider*>::iterator it_hit;
	//今までで最も近いコライダーの距離を記録する変数
	float distance = maxDistance;
	//今までで最も近いコライダーとの交点を記録する変数
	XMVECTOR inter;

	//すべてコライダーと総当たりチェック
	it = colliders.begin();
	for(; it != colliders.end(); ++it){
		BaseCollider* colA = *it;
		//球の場合
		if(colA->GetShapeType() == COLLISIONSHAPE_SPHERE){
			Sphere* sphere = dynamic_cast<Sphere*>(colA);
			float tempDistance;
			XMVECTOR tempinter;
			//当たらなければ除外
			if(!Collision::CheckRay2Sphere(ray, *sphere, &tempDistance, &tempinter)) continue;
			//距離が最小でなければ除外
			if(tempDistance >= distance) continue;
			//今までで最も近いので記録をとる
			result = true;
			distance = tempDistance;
			inter = tempinter;
			it_hit = it;
		}
	}

	//最終的になにかに当たっていたら結果を書き込み
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

	//すべての組合せについて総当たりチェック
	itA = colliders.begin();
	for(; itA != colliders.end(); ++itA){
		itB = itA;
		++itB;

		for(; itB != colliders.end(); ++itB){
			BaseCollider* colA = *itA;
			BaseCollider* colB = *itB;

			//ともに球
			if(colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
				colB->GetShapeType() == COLLISIONSHAPE_SPHERE){
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
