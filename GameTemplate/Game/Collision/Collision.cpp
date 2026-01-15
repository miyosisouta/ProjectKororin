#include "stdafx.h"
#include "Collision.h"

/*********************************** 球のコリジョンクラス *****************************************/

void SphereCollision::Init(const Vector3 pos, const float radius)
{
	position_ = pos;	// 座標
	radius_ = radius;	// 半径
}


void SphereCollision::Update()
{
	aabb_ = AABBBox(position_, Vector3(radius_)); // 座標と半径の設定
}


bool SphereCollision::IsHit(ICollision* other)
{
	if (other->GetCollisionType() == CollisionType::Sphere) { // 相手がSphereだった場合
		// 球と球
		SphereCollision* otherSphere = static_cast<SphereCollision*>(other);

		const Vector3 distance = otherSphere->GetPosition() - GetPosition();	// 2つの中心点の距離を取得
		const float length = distance.Length();									// 距離の長さを取得
		const float totalRadius = otherSphere->GetRadius() - GetRadius();		// 半径が短い場合

		if (length <= totalRadius) { // コリジョンが当たっている場合
			return true;
		}
	}

	else if (other->GetCollisionType() == CollisionType::Box) { // 相手がBoxだった場合

		// 球とボックス
		BoxCollision* otherBox = static_cast<BoxCollision*>(other);

		AABBBox aabb(GetPosition(), Vector3(radius_));												// スフィアのAABB(最小限の直方体)を作成
		Vector3 otherBoxPos = otherBox->GetPosition();												// ボックスコリジョンの座標を取得
		AABBBox aabbOther(Vector3(otherBoxPos.x, 0.0f, otherBoxPos.z), otherBox->GetSize() / 2);	// 相手のボックスのAABB(最小限の直方体)を作成

		if (aabb.IsHit(aabbOther)) { // コリジョンが当たってる場合
			return true;
		}
	}
	return false;
}

/*********************************** ボックスのコリジョンクラス *****************************************/

void BoxCollision::Init(const Vector3 pos, const Vector3 size)
{
	position_ = pos;	// 座標
	size_ = size;		// 大きさ
}


void BoxCollision::Update()
{
	// 座標と大きさの設定
	aabb_ = AABBBox(GetPosition(), size_);
}


bool BoxCollision::IsHit(ICollision* other)
{
	if (other->GetCollisionType() == CollisionType::Box) // 相手がBoxだった場合
	{
		// ボックスと球
		SphereCollision* otherSphere = static_cast<SphereCollision*>(other);

		AABBBox aabb(GetPosition(), size_);																	// ボックスのAABB(最小限の直方体)を作成
		Vector3 otherBoxPos = otherSphere->GetPosition();													// 球の座標を取得
		AABBBox aabbOther(Vector3(otherBoxPos.x, 0.0f, otherBoxPos.z), Vector3(otherSphere->GetRadius()));	// 相手のスフィアのAABB(最小限の直方体)を作成

		if (aabb.IsHit(aabbOther)) { // コリジョンが当たっているなら
			return true;
		}

	}

	else if (other->GetCollisionType() == CollisionType::Box) // 相手がBoxだった場合
	{
		// ボックスとボックス
		BoxCollision* otherBox = static_cast<BoxCollision*>(other);

		AABBBox aabb(GetPosition(), size_);																	// ボックスのAABB(最小限の直方体)を作成
		Vector3 otherBoxPos = otherBox->GetPosition();														// ボックスの座標を取得
		AABBBox aabbOther(Vector3(otherBoxPos.x, 0.0f, otherBoxPos.z), Vector3(otherBox->GetSize() / 2));	// 相手のボックスのAABB(最小限の直方体)を作成

		if (aabb.IsHit(aabbOther)) { // コリジョンが当たっているなら
			return true;
		}
	}
	return false;
}
