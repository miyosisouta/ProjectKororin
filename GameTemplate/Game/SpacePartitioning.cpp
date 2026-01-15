#include "stdafx.h"
#include "SpacePartitioning.h"
#include "Actor/Object/AttachableObject.h"
#include "Actor/Sphere/Sphere.h"
#include "StageLoader.h"
#include "Actor/Object/StaticObject.h"

namespace {
	// グリッドの分割数（Z方向、X方向）
	static const int Z_NUM_ = 8;
	static const int X_NUM_ = 8;
	// ベクターのメモリ確保用（再確保を防ぐための初期容量）
	static const int SECURE_INITIAL_CAPACITY = 500;
	// ワールドの半径（中心から端までの距離）
	const Vector3 WORLD_HALF_SIZE = Vector3(14000.0f, 0.0f, 5000.0f);
}

SpacePartitioning* SpacePartitioning::instance = nullptr;

SpacePartitioning::SpacePartitioning()
{
	/* ステージ全体のサイズ（直径）を計算 */
	worldSizeX_ = WORLD_HALF_SIZE.x * 2.0f;
	worldSizeZ_ = WORLD_HALF_SIZE.z * 2.0f;

	/* 1個のグリッド空間（セル）のサイズを計算 */
	worldSpaceSizeX_ = worldSizeX_ / static_cast<float>(X_NUM_);
	worldSpaceSizeZ_ = worldSizeZ_ / static_cast<float>(Z_NUM_);

	/* 1個の空間の半分のサイズ（中心位置計算用など） */
	worldSpaceHalfSizeX_ = worldSpaceSizeX_ / 2.0f;
	worldSpaceHalfSizeZ_ = worldSpaceSizeZ_ / 2.0f;

	/**
	 * グリッド座標計算の基準点。
	 * ワールド座標（-14000など）を配列インデックス（0～7）に変換するために使用。
	 */
	baseX_ = -WORLD_HALF_SIZE.x + worldSpaceHalfSizeX_;
	baseZ_ = -WORLD_HALF_SIZE.z + worldSpaceHalfSizeZ_;

	// 配列のメモリをあらかじめ確保してパフォーマンスを最適化
	objectList_.reserve(SECURE_INITIAL_CAPACITY);
	createObjectList_.reserve(SECURE_INITIAL_CAPACITY / 3);
	deleteObjectList_.reserve(SECURE_INITIAL_CAPACITY / 3);

	// ステージデータを外部ファイル等から読み込み
	stageLoader_ = new StageLoader();
	stageLoader_->LoadObject(objectDataList_);

	// 読み込んだデータを空間管理システム（グリッド）に登録
	for (auto list : objectDataList_) {
		AddObject(list.first, list.second);
	}

	// 現在のプレイヤー（または基準点）がいるグリッド座標を計算
	float x = WORLD_HALF_SIZE.x / worldSpaceSizeX_;
	float z = WORLD_HALF_SIZE.z / worldSpaceSizeZ_;

	oldPosition_.x = x;
	oldPosition_.z = z;

	// 初期位置の周辺（3x3マス）にあるオブジェクトを生成リストに追加
	for (int i = -1; i <= 1; i++) {
		int positionX = x + i;

		// グリッド外の参照を防ぐ
		if (positionX < 0) {
			continue;
		}

		for (int j = -1; j <= 1; j++) {
			int positionZ = z + j;

			if (positionZ < 0) {
				continue;
			}

			// 生成リストへ登録
			Cell c = { positionX, positionZ };
			AddCreateist(c);
		}
	}
	// 背景などの静的オブジェクトは最初に一括生成
	CreateStage();
}

SpacePartitioning::~SpacePartitioning()
{
}

void SpacePartitioning::Update()
{
	// プレイヤー（Sphere）の位置を取得
	// ※毎フレームFindGOするのは重いため、メンバ変数にキャッシュすることを推奨
	Sphere* spher = FindGO<Sphere>("sphere");

	if (spher == nullptr) {
		return;
	}

	// プレイヤーの現在位置から、所属するグリッド座標（インデックス）を算出
	// ワールド座標を正の値に補正してからセルサイズで割る
	int x = static_cast<int>((spher->GetPosition().x + WORLD_HALF_SIZE.x) / worldSpaceSizeX_);
	int z = static_cast<int>((spher->GetPosition().z + WORLD_HALF_SIZE.z) / worldSpaceSizeZ_);

	// 所属するグリッドが変わった場合のみ、ロード/アンロード処理のリスト更新を行う
	if (oldPosition_.x != x ||
		oldPosition_.z != z)
	{
		ListUpdate(x, z);
	}

	// リストに基づき、実際にオブジェクトの生成・削除を行う
	// 1フレームに1つずつ処理することでスパイク（カクつき）を防いでいる
	CreateObject();
	DeleteObject();
}

// オブジェクトデータを適切なリスト（常駐orグリッド管理）に振り分ける
void SpacePartitioning::AddObject(int num, ObjectData* object)
{
	// attachValueが1の場合は「常に表示する背景オブジェクト」として扱う
	if (object->attachValue == 1) {
		stageObjectData.push_back(object);
		return;
	}

	// それ以外は位置に基づいてグリッド座標を計算
	int x = static_cast<int>((object->position.x + WORLD_HALF_SIZE.x) / worldSpaceSizeX_);
	int z = static_cast<int>((object->position.z + WORLD_HALF_SIZE.z) / worldSpaceSizeZ_);

	// グリッドごとのデータリストに格納（この時点ではまだ実体化されない）
	spacePartitioningList_.at(x).at(z).emplace(num, object);
}

// 指定されたセルのオブジェクトを「削除予定リスト」に追加する
void SpacePartitioning::AddDeleteList(Cell& position)
{
	// そのセルに所属する全オブジェクトについて処理
	for (auto it : spacePartitioningList_.at(position.x).at(position.z)) {

		// もし「生成予定リスト」にまだ残っているなら、生成を取り消すだけで良い（削除リストには入れない）
		auto createObjectListIt = std::find(createObjectList_.begin(), createObjectList_.end(), it.first);
		if (createObjectListIt != createObjectList_.end()) {
			createObjectList_.erase(createObjectListIt);
		}
		else {
			// 生成済みであれば、削除リストに追加する
			deleteObjectList_.push_back(it.first);
		}
	}
}

// 指定されたセルのオブジェクトを「生成予定リスト」に追加する
void SpacePartitioning::AddCreateist(Cell& position)
{
	// そのセルに所属する全オブジェクトについて処理
	for (auto it : spacePartitioningList_.at(position.x).at(position.z)) {

		// もし「削除予定リスト」に入っているなら、削除を取り消すだけで良い（再生成のコスト削減）
		auto deleteObjectListIt = std::find(deleteObjectList_.begin(), deleteObjectList_.end(), it.first);
		if (deleteObjectListIt != deleteObjectList_.end()) {
			deleteObjectList_.erase(deleteObjectListIt);
		}
		else {
			// まだ存在しないなら、生成リストに追加する
			createObjectList_.push_back(it.first);
		}
	}
}

// 削除リストから1つ取り出し、ゲームオブジェクトを削除する
void SpacePartitioning::DeleteObject()
{
	if (deleteObjectList_.size() == 0) {
		return;
	}
	// リストの末尾から削除
	DeleteGO(objectList_[deleteObjectList_.back()]);
	deleteObjectList_.pop_back();
}

// 生成リストから1つ取り出し、ゲームオブジェクトを生成する
void SpacePartitioning::CreateObject()
{
	if (createObjectList_.size() == 0) {
		return;
	}
	// リストの末尾にあるIDを取得
	int number = createObjectList_.back();

	// ゲームオブジェクトの実体を生成
	auto* attachableObject = NewGO<AttachableObject>(0, "AttachableObject");
	attachableObject->Initialize(objectDataList_[number]);

	// 管理用マップに登録
	objectList_.emplace(number, attachableObject);

	createObjectList_.pop_back();
}

// 静的な背景オブジェクトなどを一括生成（初期化時に一度だけ呼ばれる）
void SpacePartitioning::CreateStage()
{
	for (auto data : stageObjectData) {
		auto* staticObject = NewGO<StaticObject>(0, "StaticObject");
		staticObject->Initialize(data);
		stageObject.push_back(staticObject);
	}
}

// 全オブジェクトの描画をオフにする
void SpacePartitioning::OffRender()
{
	for (auto stage : stageObject) {
		stage->OffRender();
	}
	for (auto object : objectList_) {
		object.second->OffRender();
	}
}

// グリッド移動時の更新処理
// プレイヤーが移動した方向に応じて、新しいエリアを生成し、古いエリアを削除する
void SpacePartitioning::ListUpdate(int x, int z)
{
	int moveX = x - oldPosition_.x; // X方向の移動量
	int moveZ = z - oldPosition_.z; // Z方向の移動量

	// X方向に移動があった場合
	if (moveX != 0) {
		int newColX = x + moveX;          // 新しく視界に入る列（1マス先）
		int oldColX = oldPosition_.x - moveX; // 視界から外れる列（後ろ）

		// Z方向の幅（3マス分）に対して更新をかける
		for (int i = -1; i <= 1; i++) {
			int targetZ = z + i;

			// Z方向の範囲チェック
			if (targetZ >= 0 && targetZ < Z_NUM_) {
				// 進行方向の新しい列を「生成」対象にする
				if (newColX >= 0 && newColX < X_NUM_) {
					Cell c = { newColX, targetZ };
					AddCreateist(c);
				}
				// 通り過ぎた古い列を「削除」対象にする
				if (oldColX >= 0 && oldColX < X_NUM_) {
					Cell c = { oldColX, targetZ };
					AddDeleteList(c);
				}
			}
		}
	}
	// Z方向に移動があった場合
	else if (moveZ != 0) {
		int newColZ = z + moveZ;          // 新しく視界に入る行
		int oldColZ = oldPosition_.z - moveZ; // 視界から外れる行

		// X方向の幅（3マス分）に対して更新をかける
		for (int i = -1; i <= 1; i++) {
			int targetX = x + i;

			// X方向の範囲チェック
			if (targetX >= 0 && targetX < X_NUM_) {
				// 進行方向の新しい行を「生成」
				if (newColZ >= 0 && newColZ < Z_NUM_) {
					Cell c = { newColZ, targetX };
					AddCreateist(c);
				}
				// 通り過ぎた古い行を「削除」
				if (oldColZ >= 0 && oldColZ < Z_NUM_) {
					Cell c = { oldColZ, targetX };
					AddDeleteList(c);
				}
			}
		}
	}

	// 現在位置を更新
	oldPosition_.x = x;
	oldPosition_.z = z;
}