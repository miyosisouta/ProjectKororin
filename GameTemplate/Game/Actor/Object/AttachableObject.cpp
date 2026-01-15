#include "stdafx.h"
#include "Stage.h"
#include "Collision/CollisionManager.h"

AttachableObject::~AttachableObject()
{
	// 当たり判定の破棄
	{
		// 静的オブジェクト
		if (physicsStaticObject_)
		{
			physicsStaticObject_->Release();
			delete physicsStaticObject_;
			physicsStaticObject_ = nullptr;
		}

		// 登録した当たり判定
		CollisionHitManager::Get().UnregisterCollisionObject(this);

		// ゴーストオブジェクト
		if (collisionObject_) {
			DeleteGO(collisionObject_);
			collisionObject_ = nullptr;
		}
	}
}

bool AttachableObject::Start()
{
	// 親クラスの初期化処理
	StageObjectBase::Start();

	// コライダーの設定
	{
		// BoxCollisionのユニークポインタの作成と初期化
		auto boxCollider = std::make_unique<BoxCollision>();
		boxCollider->Init(GetPosition(), GetSize());

		// 更新
		boxCollider->Update();

		// ICollisionに所有権を移動
		collider_ = std::move(boxCollider);

		// CollisionHitManagerへ当たり判定を登録
		CollisionHitManager::Get().RegisterCollisionObject(
			GameObjectType::AttachableObject,   // 種類（オブジェクトの場合）
			this,								// ひっつくオブジェクト自身
			collider_.get()						// そのコリジョン
		);
	}

	return true;
}
void AttachableObject::Update()
{
	// トランスフォームの更新
	transform_.UpdateTransform();

	// モデルの座標の設定
	modelRender_.SetPosition(transform_.m_position);
	modelRender_.SetRotation(transform_.m_rotation);
	modelRender_.Update();
}
void AttachableObject::Render(RenderContext& rc)
{
	// 描画可能ならDraw
	if (isVisible_) {
		modelRender_.Draw(rc);
	}
}


void AttachableObject::DeletePhysicsStatics()
{
	// 当たり判定の破棄
	{
		// 静的オブジェクト
		if (physicsStaticObject_)
		{
			physicsStaticObject_->Release();
			delete physicsStaticObject_;
			physicsStaticObject_ = nullptr;
		}

		// 登録した当たり判定
		CollisionHitManager::Get().UnregisterCollisionObject(this);

		// ゴーストオブジェクト
		if (collisionObject_) {
			DeleteGO(collisionObject_);
			collisionObject_ = nullptr;
		}
	}
}
