#include "stdafx.h"
#include "Sphere.h"
#include <iostream>
#include "Collision/CollisionManager.h"
#include "Actor/Object/AttachableObject.h"
namespace 
{
	const float ALWAYS_SPEED = 100.0f;	// 固定移動速度
	const float INITIAL_RADIUS = 15.0f;	// 初期半径
	//const float INITIAL_MASS = 1.0f; // 初期質量
}

bool Sphere::Start() 
{
	// モデルのInit
	sphereRender_.Init("Assets/modelData/mass/model/mass.tkm");

	// 初期化
	transform_.m_scale = Vector3(radius_, radius_, radius_);
	sphereRender_.SetPosition(transform_.m_position);
	sphereRender_.SetRotation(transform_.m_rotation);
	sphereRender_.SetScale(transform_.m_scale);
	sphereRender_.Update();


	// コリジョンを作成
	collisionObject_ = NewGO<CollisionObject>(0, "collisionObject");
	collisionObject_->CreateSphere(
		transform_.m_position,	// 座標
		transform_.m_rotation,	// 回転
		INITIAL_RADIUS			// 球形の半径
	);
	// コリジョンが消えないようにする
	collisionObject_->SetIsEnableAutoDelete(false);


	// 当たり判定の登録
	CollisionHitManager::Get().RegisterCollisionObject(
		enCollisionType_Sphere,   // 種類
		this,                     // Sphere自身（IGameObject*）
		collisionObject_          // Sphereのコリジョン
	);


	// todo for test : BurretPhysicsを使った新しい当たり判定とコリジョン設定
	//btCollisionShape* initialShape = new btSphereShape(INITIAL_RADIUS);
	//btVector3 localInertia(0, 0, 0); // 慣性モーメント : 回転運動における物体の回転のしにくさ
	//initialShape->calculateLocalInertia(INITIAL_MASS, localInertia);


	// キャラコンの初期
	charaCon_.Init(15.0f, 15.0f, transform_.m_position);
	return true;
}

void Sphere::Update()
{
	// 前の座標を保存
	beforePosition_ = transform_.m_position;

	Move();
	Rotation();
	SetGravity();
	sphereRender_.Update();
	collisionObject_->SetPosition(transform_.m_position);
	collisionObject_->SetRotation(transform_.m_rotation);
	collisionObject_->Update();
}

void Sphere::Render(RenderContext& rc) 
{
	sphereRender_.Draw(rc);
}


void Sphere::Move() 
{
	//移動方向の計算
	moveSpeedMultiply_ = ALWAYS_SPEED * (INITIAL_RADIUS / radius_);

	// 移動方向をもとに速度を出す
	moveSpeed_.x = moveDirection_.x * moveSpeedMultiply_;
	moveSpeed_.z = moveDirection_.z * moveSpeedMultiply_;

	// 移動先
	transform_.m_localPosition = charaCon_.Execute(moveSpeed_, 1.0f / 60.0f);
	
	// TransformをUpdate　→　移動先を更新
	transform_.UpdateTransform();

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
	rot.SetRotationDeg(vertical_, length * rotationSpeed / 20);

	//求めたクォータニオンを乗算する
	transform_.m_localRotation.Multiply(transform_.m_localRotation, rot);
	
	transform_.UpdateTransform();
	sphereRender_.SetRotation(transform_.m_rotation);
}


void Sphere::SetGravity()
{
	// 地面に付いていたら
	if (charaCon_.IsOnGround())
	{
		// 重力を無くす。
		moveSpeed_.y = 0.0f;
	}

	// 地面に付いていなかったら。
	else
	{
		// 重力を発生させる
		moveSpeed_.y -= 10.0f;
	}

	// キャラクターコントローラーを使って座標を移動させる。
	transform_.m_localPosition = charaCon_.Execute(moveSpeed_, 1.0f / 60.0f);
	// トランスフォームの更新
	transform_.UpdateTransform();

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



