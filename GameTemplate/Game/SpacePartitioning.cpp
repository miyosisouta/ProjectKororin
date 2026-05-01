#include "stdafx.h"
#include "SpacePartitioning.h"
#include "Actor/Object/AttachableObject.h"
#include "Actor/Sphere/Sphere.h"
#include "StageLoader.h"
#include "Actor/Object/StaticObject.h"
#include "StageCullingSystem.h"

namespace
{
	// グリッドの分割数
	constexpr int Z_NUM = 8;
	constexpr int X_NUM = 8;

	// ワールドの半径（中心から端までの距離）
	static const Vector3 WORLD_HALF_SIZE = Vector3(14000.0f, 0.0f, 5000.0f);

	// 周囲1マス分のオフセット（3x3 = -1〜+1）
	constexpr int NEIGHBOR_RANGE = 1;

	// NewGOの優先度
	constexpr int PRIORITY_ZERO = 0;

	// 直径を求めるための除数
	constexpr float HALF_DIVISOR = 2.0f;

	// グリッド範囲チェック用
	constexpr int GRID_MIN = 0;

	// グリッドインデックスを安全な範囲に収める（C++14対応）
	inline int Clamp(int value, int minVal, int maxVal)
	{
		if (value < minVal) return minVal;
		if (value > maxVal) return maxVal;
		return value;
	}
}

SpacePartitioning* SpacePartitioning::instance_ = nullptr;

SpacePartitioning::SpacePartitioning()
{
	// ステージ全体のサイズを計算
	worldSizeX_ = WORLD_HALF_SIZE.x * HALF_DIVISOR;
	worldSizeZ_ = WORLD_HALF_SIZE.z * HALF_DIVISOR;

	// 1セルのサイズを計算
	worldSpaceSizeX_ = worldSizeX_ / static_cast<float>(X_NUM);
	worldSpaceSizeZ_ = worldSizeZ_ / static_cast<float>(Z_NUM);

	// ステージデータを読み込む
	stageLoader_ = new StageLoader();

	// カリングシステムを生成する
	cullingSystem_ = std::make_unique<StageCullingSystem>();

	// objectDataList に読み込む
	std::unordered_map<int, ObjectData*> objectDataList;
	stageLoader_->LoadObject(objectDataList);

	// 読み込んだデータを全て実体化し、グリッドに登録する
	for (auto& pair : objectDataList)
	{
		ObjectData* data = pair.second;

		// 背景など常駐オブジェクト（attachValue==1）はそのまま常時表示で生成
		if (data->attachValue == 1)
		{
			auto* staticObject = NewGO<StaticObject>(PRIORITY_ZERO, "StaticObject");
			staticObject->Initialize(data);
			// デストラクタで DeleteGO するために登録しておく
			staticObjects_.push_back(staticObject);
			// [FIX] ObjectData は Initialize でコピー済みなので使用後に delete する
			delete data;
			continue;
		}

		// グリッド座標を計算
		const int gx = ToGridX(data->position.x);
		const int gz = ToGridZ(data->position.z);

		// グリッド範囲外は生成しない
		if (gx < GRID_MIN || gx >= X_NUM || gz < GRID_MIN || gz >= Z_NUM) {
			// [FIX] 範囲外でも ObjectData は delete する
			delete data;
			continue;
		}

		// 実体を生成し、最初は非表示にしておく
		auto* obj = NewGO<AttachableObject>(PRIORITY_ZERO, "AttachableObject");
		obj->Initialize(data);
		obj->SetVisible(false);

		// グリッドに登録
		grid_[gx][gz].push_back(obj);

		// デストラクタで安全に削除するために全オブジェクトリストにも登録する
		// grid_は別途管理が必要
		allObjects_.push_back(obj);

		// データはコピーしているので削除
		delete data;
	}

	// プレイヤーの初期グリッド座標（ワールド中心）を設定
	oldPosition_.x = ToGridX(0.0f);
	oldPosition_.z = ToGridZ(0.0f);

	// 初期位置の周辺（3x3マス）を表示する
	for (int i = -NEIGHBOR_RANGE; i <= NEIGHBOR_RANGE; i++) {
		for (int j = -NEIGHBOR_RANGE; j <= NEIGHBOR_RANGE; j++) {
			const int px = oldPosition_.x + i;
			const int pz = oldPosition_.z + j;
			if (px >= GRID_MIN && px < X_NUM && pz >= GRID_MIN && pz < Z_NUM) {
				// ShowCell を呼ぶことで visibleCellObjects_ にも登録される
				ShowCell(px, pz);
			}
		}
	}
}

SpacePartitioning::~SpacePartitioning()
{
	if (stageLoader_) {
		delete stageLoader_;
		stageLoader_ = nullptr;
	}

	// 吸着可能オブジェクトをすべて削除
	for (auto* obj : allObjects_) {
		if (obj) {
			DeleteGO(obj);
		}
	}
	allObjects_.clear();

	// StaticObjectをすべて削除
	for (auto* obj : staticObjects_) {
		if (obj) {
			DeleteGO(obj);
		}
	}
	staticObjects_.clear();

	// grid_ のポインタはすでに DeleteGO 済みなのでクリアだけする
	for (int x = GRID_MIN; x < X_NUM; x++) {
		for (int z = GRID_MIN; z < Z_NUM; z++) {
			grid_[x][z].clear();
		}
	}

	attachedObjects_.clear();
	visibleCellObjects_.clear();
}

void SpacePartitioning::Update()
{
	if (!isUpdate_) {
		return;
	}

	Sphere* sphere = FindGO<Sphere>("sphere");
	if (sphere == nullptr) {
		return;
	}

	// プレイヤーの現在グリッド座標を計算
	int x = ToGridX(sphere->GetPosition().x);
	int z = ToGridZ(sphere->GetPosition().z);

	// グリッド範囲内にクランプ
	x = Clamp(x, GRID_MIN, X_NUM - 1);
	z = Clamp(z, GRID_MIN, Z_NUM - 1);

	// グリッドが変わった場合のみ表示を更新する
	if (oldPosition_.x != x || oldPosition_.z != z) {
		ListUpdate(x, z);
	}

	// AttachableObject のフラスタムカリングを毎フレーム実行する
	cullingSystem_->Update(visibleCellObjects_);
}

void SpacePartitioning::HideAll()
{
	// 全グリッドを走査して、吸着済み以外のオブジェクトを全て非表示にする
	// リザルト画面遷移時に一度だけ呼ぶ
	for (int x = GRID_MIN; x < X_NUM; x++) {
		for (int z = GRID_MIN; z < Z_NUM; z++) {
			for (auto* obj : grid_[x][z]) {
				if (attachedObjects_.count(obj) > 0) continue;
				obj->SetVisible(false);
			}
		}
	}

	// 更新も止める（リザルト中はグリッド表示切替が不要）
	UpdateStop();
}

void SpacePartitioning::ReleaseOwnership(AttachableObject* object)
{
	if (object == nullptr) return;

	// 吸着済みセットに追加する
	// 以降 ShowCell / HideCell でこのオブジェクトには触れなくなる
	attachedObjects_.insert(object);

	// グリッドからも除外する（次回の ShowCell/HideCell の走査対象から外す）
	for (int x = GRID_MIN; x < X_NUM; x++) {
		for (int z = GRID_MIN; z < Z_NUM; z++) {
			auto& list = grid_[x][z];
			list.erase(
				std::remove(list.begin(), list.end(), object),
				list.end()
			);
		}
	}

	visibleCellObjects_.erase(
		std::remove(visibleCellObjects_.begin(), visibleCellObjects_.end(), object),
		visibleCellObjects_.end()
	);

	// 吸着済みなので常時表示にする
	object->SetVisible(true);
}

void SpacePartitioning::ShowCell(int x, int z)
{
	for (auto* obj : grid_[x][z]) {
		// 吸着済みは触らない
		if (attachedObjects_.count(obj) > 0) continue;

		// グリッド範囲内に入ったのでカリング対象リストに追加する
		// 実際の SetVisible はフラスタムカリングが毎フレーム判定して切り替える
		visibleCellObjects_.push_back(obj);
	}
}

void SpacePartitioning::HideCell(int x, int z)
{
	for (auto* obj : grid_[x][z]) {
		// 吸着済みは触らない
		if (attachedObjects_.count(obj) > 0) continue;

		// グリッド範囲外に出たので即非表示にする（カリング判定不要）
		obj->SetVisible(false);

		// カリング対象リストからも除外する
		visibleCellObjects_.erase(
			std::remove(visibleCellObjects_.begin(), visibleCellObjects_.end(), obj),
			visibleCellObjects_.end()
		);
	}
}

void SpacePartitioning::ListUpdate(int x, int z)
{
	const int moveX = x - oldPosition_.x;
	const int moveZ = z - oldPosition_.z;

	if (moveX != 0)
	{
		// 進行方向の新しい列を表示する
		const int newCol = x + moveX;
		// 通り過ぎた古い列を非表示にする
		const int oldCol = oldPosition_.x - moveX;

		for (int i = -NEIGHBOR_RANGE; i <= NEIGHBOR_RANGE; i++) {
			const int tz = z + i;
			if (tz < GRID_MIN || tz >= Z_NUM) continue;

			if (newCol >= GRID_MIN && newCol < X_NUM) ShowCell(newCol, tz);
			if (oldCol >= GRID_MIN && oldCol < X_NUM) HideCell(oldCol, tz);
		}
	}
	else if (moveZ != 0)
	{
		// 進行方向の新しい行を表示する
		const int newRow = z + moveZ;
		// 通り過ぎた古い行を非表示にする
		const int oldRow = oldPosition_.z - moveZ;

		for (int i = -NEIGHBOR_RANGE; i <= NEIGHBOR_RANGE; i++) {
			const int tx = x + i;
			if (tx < GRID_MIN || tx >= X_NUM) continue;

			if (newRow >= GRID_MIN && newRow < Z_NUM) ShowCell(tx, newRow);
			if (oldRow >= GRID_MIN && oldRow < Z_NUM) HideCell(tx, oldRow);
		}
	}

	oldPosition_.x = x;
	oldPosition_.z = z;
}

int SpacePartitioning::ToGridX(float worldX) const
{
	return static_cast<int>((worldX + WORLD_HALF_SIZE.x) / worldSpaceSizeX_);
}

int SpacePartitioning::ToGridZ(float worldZ) const
{
	return static_cast<int>((worldZ + WORLD_HALF_SIZE.z) / worldSpaceSizeZ_);
}
