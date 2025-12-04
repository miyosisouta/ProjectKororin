#include "stdafx.h"
#include "Collision.h"


void SphereCollision::Init(const Vector3 pos, const float radius)
{
	position_ = pos;
	radius_ = radius;
}


void SphereCollision::Update()
{
	aabb_ = AABBBox(position_, Vector3(radius_));
}


bool SphereCollision::IsHit(ICollision* other)
{
	if (other->GetCollisionType() == CollisionType::Sphere) // 相手がSphereだった場合
	{
		// スフィアとスフィア
		SphereCollision* otherSphere = static_cast<SphereCollision*>(other);

		const Vector3 distance = otherSphere->GetPosition() - GetPosition(); // 2つの中心点の距離を取得
		const float length = distance.Length();	 // 距離の長さを取得
		const float totalRadius = otherSphere->GetRadius() - GetRadius(); // 半径が短い場合
		if (length <= totalRadius) { // コリジョンが当たっている場合
			return true;
		}
	}

	else if (other->GetCollisionType() == CollisionType::Box) // 相手がBoxだった場合
	{
		// スフィアとボックス
		BoxCollision* otherBox = static_cast<BoxCollision*>(other);

		AABBBox aabb(GetPosition(), Vector3(radius_)); // スフィアのAABB(最小限の直方体)を作成
		Vector3 otherBoxPos = otherBox->GetPosition();
		AABBBox aabbOther(Vector3(otherBoxPos.x, 0.0f, otherBoxPos.z), otherBox->GetSize() / 2); // 相手のボックスのAABB(最小限の直方体)を作成
		if (aabb.IsHit(aabbOther)) {
			return true;
		}
	}
	return false;
}


void BoxCollision::Init(const Vector3 pos, const Vector3 size)
{
	position_ = pos;
	size_ = size;
}


void BoxCollision::Update()
{
	aabb_ = AABBBox(GetPosition(), size_);
}


bool BoxCollision::IsHit(ICollision* other)
{
	if (other->GetCollisionType() == CollisionType::Box) // 相手がBoxだった場合
	{
		// ボックスとスフィア
		SphereCollision* otherSphere = static_cast<SphereCollision*>(other);

		AABBBox aabb(GetPosition(), size_); // ボックスのAABB(最小限の直方体)を作成
		Vector3 otherBoxPos = otherSphere->GetPosition();
		AABBBox aabbOther(Vector3(otherBoxPos.x, 0.0f, otherBoxPos.z), Vector3(otherSphere->GetRadius())); // 相手のスフィアのAABB(最小限の直方体)を作成
		if (aabb.IsHit(aabbOther)) {
			return true;
		}

	}
	// ボックスとボックス
	else if(other->GetCollisionType() == CollisionType::Box) // 相手がBoxだった場合
	{
		BoxCollision* otherBox = static_cast<BoxCollision*>(other);
		AABBBox aabb(GetPosition(), size_); // ボックスのAABB(最小限の直方体)を作成
		Vector3 otherBoxPos = otherBox->GetPosition();
		AABBBox aabbOther(Vector3(otherBoxPos.x, 0.0f, otherBoxPos.z), Vector3(otherBox->GetSize() / 2)); // 相手のボックスのAABB(最小限の直方体)を作成
		if (aabb.IsHit(aabbOther)) {
			return true;
		}
	}
	return false;
}
