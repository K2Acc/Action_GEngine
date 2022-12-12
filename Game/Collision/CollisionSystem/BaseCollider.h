#pragma once

#include "CollisionTypes.h"
#include "CollisionInfo.h"
#include "../../3D/BaseObjObject.h"

/// <summary>
/// �R���C�_�[���N���X
/// </summary>
class BaseCollider
{
public:
	BaseCollider() = default;
	//���z�f�X�g���N�^
	virtual ~BaseCollider() = default;

public:
	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;

	inline void OnCollision(const CollisionInfo& info){
		objObject->OnCollision(info);
	}

	//Getter
	inline BaseObjObject* GetObjObject()	{return objObject;}
	inline CollisionShapeType GetShapeType()	{return shapeType;}

	//Setter
	inline void SetObjObject(BaseObjObject* objObject)	{this->objObject = objObject;}

protected:
	//�I�u�W�F�N�g
	BaseObjObject* objObject = nullptr;
	//�`��^�C�v
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
};