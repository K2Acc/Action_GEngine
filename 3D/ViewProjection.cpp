#include "ViewProjection.h"
#include "DirectXCommon.h"
#include <d3dx12.h>
#include <cassert>

using namespace DirectX;

void ViewProjection::Initialize()
{
	UpdateMatrix();
}

void ViewProjection::UpdateMatrix()
{
	// �r���[�s��̐���
	XMFLOAT3 leye = {eye.x,eye.y,eye.z};
	XMFLOAT3 ltarget = {target.x,target.y,target.z};
	XMFLOAT3 lup = {up.x,up.y,up.z};
	matView = XMMatrixLookAtLH(XMLoadFloat3(&leye), XMLoadFloat3(&ltarget), XMLoadFloat3(&lup));

	// �������e�ɂ��ˉe�s��̐���
	matProjection = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);
}
