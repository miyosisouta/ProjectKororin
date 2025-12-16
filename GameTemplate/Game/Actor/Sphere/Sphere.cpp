#include "stdafx.h"
#include "Sphere.h"
#include <iostream>
#include "Actor/Object/AttachableObject.h"
#include "Collision/CollisionManager.h"
#include "Scene/SceneManager.h"


namespace 
{
	const float ALWAYS_SPEED = 500.0f;	// 固定移動速度
	const float INITIAL_RADIUS = 15.0f;	// 初期半径
	const float GOAL_RADIUS = 300.0f; // 目標サイズ
}


SphereStatus::SphereStatus()
{
	ParameterManager::Get().LoadParameter<MasterSphereStatusParameter>("Assets/Parameter/SphereStatusParameter.json", [](const nlohmann::json& j, MasterSphereStatusParameter& status)
		{
			status.level = j["Level"];
			status.levelUpNum = j["LevelUpNum"];
		});
}


SphereStatus::~SphereStatus()
{
	ParameterManager::Get().UnloadParameter<MasterSphereStatusParameter>();
}


void SphereStatus::Setup(const MasterSphereStatusParameter& parameter)
{
	levelUpNum_ = parameter.levelUpNum;
}



/*******************************/


Sphere::~Sphere()
{
	if (CollisionHitManager::IsAvailable()) {
		CollisionHitManager::Get().UnregisterCollisionObject(this);
	}
}

bool Sphere::Start()
{
	// ステータス生成
	status_ = std::make_unique<SphereStatus>();
	UpdateLevelUp(true);

	// コリジョンを生成
	auto sphereCollider = std::make_unique<SphereCollision>();
	sphereCollider->Init(GetPosition(), radius_);
	// ICollisionに所有権を移動
	collider_ = std::move(sphereCollider);

	// モデルのInit
	sphereRender_.Init("Assets/modelData/mass/model/mass.tkm");

	// 塊の初期の大きさを代入
	radius_ = INITIAL_RADIUS;

	// 初期化
	transform_.m_scale = Vector3::One;
	sphereRender_.SetPosition(transform_.m_position);
	sphereRender_.SetRotation(transform_.m_rotation);
	sphereRender_.SetScale(transform_.m_scale);
	sphereRender_.Update();


	// 当たり判定の登録
	CollisionHitManager::Get().RegisterCollisionObject(
		GameObjectType::Sphere,   // 種類
		this,                     // Sphere自身（IGameObject*）
		collider_.get()          // Sphereのコリジョン
	);

	//

	m_collider = new CCompoundCollider();
	m_collider->Init(radius_);

	//剛体を初期化。
	RigidBodyInitData rbInfo;
	rbInfo.collider = m_collider;
	rbInfo.mass = 0.0f;
	m_rigidBody = new RigidBody();
	m_rigidBody->Init(rbInfo);
	btTransform& trans = m_rigidBody->GetBody()->getWorldTransform();
	//剛体の位置を更新。
	trans.setOrigin(btVector3(transform_.m_position.x, transform_.m_position.y + radius_, transform_.m_position.z));


	//@todo 未対応。trans.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));
	m_rigidBody->GetBody()->setUserIndex(enCollisionAttr_Character);
	m_rigidBody->GetBody()->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	transform_.UpdateTransform();

	return true;
}

void Sphere::Update()
{
	UpdateLevelUp();

	// 前の座標を保存
	beforePosition_ = transform_.m_position;

	Move();
	Rotation();
	SetGravity();
	sphereRender_.Update();

	collider_->SetPosition(transform_.m_position);
	collider_->Update();
}

void Sphere::Render(RenderContext& rc)
{
	if (GetIsDraw())
	{
		sphereRender_.Draw(rc);

	}
}


bool Sphere::CheakGoalSize()
{
	if (radius_ >= GOAL_RADIUS) {
		return true;
	}
	return false;
}


void Sphere::Move()
{
	//移動方向の計算
	moveSpeedMultiply_ = ALWAYS_SPEED * (INITIAL_RADIUS / effectiveRadius_);

	// 移動方向をもとに速度を出す
	moveSpeed_.x = moveDirection_.x * moveSpeedMultiply_;
	moveSpeed_.y = moveDirection_.y * moveSpeedMultiply_;
	moveSpeed_.z = moveDirection_.z * moveSpeedMultiply_;

	if (m_addForce.Length() > 0.1f) {
		moveSpeed_ = m_addForce;
		m_addForce *= 0.8f;
	} else {
		m_addForce = Vector3::Zero;
	}

	// 移動先
	transform_.m_localPosition += (moveSpeed_ * g_gameTime->GetFrameDeltaTime());
	
	// 引っ付いたオブジェクトによって地面との距離が変わるようにしてみる（疑似）
	// 本当はレイキャストとか使いたい
	{
		if (isPlayable_) {
			float posY = transform_.m_position.y;
			Vector3 size = Vector3::Zero;
			for (auto* object : m_attachableObjectList) {
				const float objectY = object->GetTransform()->m_position.y;
				if (objectY < posY) {
					posY = objectY;
					size = object->GetColliderSize();
				}
			}
			const float a = transform_.m_position.y - posY;
			const float aa = a - size.Length() * 2.0f;
			const float disntace = fabs(aa) + INITIAL_RADIUS;
			transform_.m_localPosition.y = disntace;
		}
	}
	//transform_.m_localPosition.y = radius_;

	// TransformをUpdate　→　移動先を更新
	transform_.UpdateTransform();

	//// @todo for test
	btRigidBody* btBody = m_rigidBody->GetBody();
	//剛体を動かす。
	btBody->setActivationState(DISABLE_DEACTIVATION);
	btTransform& trans = btBody->getWorldTransform();
	//剛体の位置を更新。
	trans.setOrigin(btVector3(transform_.m_position.x, transform_.m_position.y + radius_, transform_.m_position.z));

	// 絵描きさんに座標を教える。
	sphereRender_.SetPosition(transform_.m_position);
}


void Sphere::Rotation()
{
	// 回転速度を計算
	const float rotationSpeed = ALWAYS_SPEED * (moveSpeedMultiply_ / ALWAYS_SPEED);

	// 前の座標と今の座標の差を計算
	Vector3 move = transform_.m_position - beforePosition_;

	//移動が全くされていない場合、処理を返す
	if (fabsf(move.x) < 0.001f && fabsf(move.z) < 0.001f) { return;	}

	// 移動量を求める
	float length = move.Length();

	//正規化
	move.Normalize();

	// 外積を求める
	vertical_ = Cross(Vector3::AxisY, move);
	// 外積ベクトルをもとに回転量を求める
	Quaternion rot;
	rot.SetRotationDeg(vertical_, length * rotationSpeed / 200);

	//求めたクォータニオンを乗算する
	transform_.m_localRotation.Multiply(transform_.m_localRotation, rot);
	
	transform_.UpdateTransform();
	sphereRender_.SetRotation(transform_.m_rotation);
}


void Sphere::SetGravity()
{
	// 地面に付いていたら
	//if (charaCon_.IsOnGround())
	//{
	//	// 重力を無くす。
	//	moveSpeed_.y = 0.0f;
	//}

	//// 地面に付いていなかったら。
	//else
	{
		// 重力を発生させる
		//moveSpeed_.y -= 10.0f;
	}


	// トランスフォームの更新
	//transform_.UpdateTransform();

	// 絵描きさんに座標を教える。
	sphereRender_.SetPosition(transform_.m_position);
}



void Sphere::SetParent(AttachableObject* attachableObject)
{
	attachableObject->GetTransform()->SetParent(&transform_);

	// 上のコードをわかりやすくしたもの
	//Transform* parentTransform = this->GetTransform(); // Sphereのトランスフォームを取得
	//Transform* childTransform = attachableObject->GetTransform(); // AttachableObjectのトランスフォームを取得

	//childTransform->SetParent(parentTransform);
}


void Sphere::UpdateLevelUp(const bool isInit)
{
	if (currentLevelUpNum_ >= status_->GetLevelUpNum()) {
		currentLevelUpNum_ -= status_->GetLevelUpNum();
		
		// ステータスセットアップ
		const MasterSphereStatusParameter* parameter = ParameterManager::Get().FindParameter<MasterSphereStatusParameter>([&](const MasterSphereStatusParameter& parameter)
			{
				if (status_->GetLevel() == parameter.level) {
					return true;
				}
				return false;
			});
		if (parameter) {
			// 初期化の時はレベルアップしません
			if (!isInit) {
				status_->AddLevel();
			}
			status_->Setup(*parameter);
		}
	}
}