#pragma once
#include <unordered_set>

struct Cell
{
	int x = 0;
	int z = 0;
};

struct ObjectData;
class AttachableObject;
class StaticObject;
class StageLoader;
class StageCullingSystem;

class SpacePartitioning
{
private:
	static SpacePartitioning* instance_;

	/* コンストラクタ */
	SpacePartitioning();

public:
	/* デストラクタ */
	~SpacePartitioning();

	/* コピープロテクト */
	SpacePartitioning(const SpacePartitioning&) = delete;
	SpacePartitioning& operator=(const SpacePartitioning&) = delete;

	/* インスタンスの取得 */
	static SpacePartitioning* GetInstance() { return instance_; }

	/* インスタンスの作成 */
	static void CreateInstance()
	{
		if (instance_ == nullptr) {
			instance_ = new SpacePartitioning();
		}
	}

	/* インスタンスの削除 */
	static void DeleteInstance()
	{
		delete instance_;
		instance_ = nullptr;
	}

	/* 更新処理 */
	void Update();

	/* 更新の開始・停止 */
	void UpdateStart() { isUpdate_ = true; }
	void UpdateStop()  { isUpdate_ = false; }

	/* 吸着時に呼ぶ。以降そのオブジェクトは空間管理の表示制御から外れ常に表示される */
	void ReleaseOwnership(AttachableObject* object);

	/* リザルト遷移時に呼ぶ。吸着済み以外のオブジェクトを全て非表示にする */
	void HideAll();

private:
	/* 指定セルのオブジェクトを表示する */
	void ShowCell(int x, int z);

	/* 指定セルのオブジェクトを非表示にする */
	void HideCell(int x, int z);

	/* グリッド移動時の表示更新 */
	void ListUpdate(int x, int z);

	/* ワールド座標 → グリッドインデックス変換 */
	int ToGridX(float worldX) const;
	int ToGridZ(float worldZ) const;

private:
	StageLoader* stageLoader_ = nullptr;

	/* フラスタムカリング（StaticObject 用） */
	std::unique_ptr<StageCullingSystem> cullingSystem_;

	/* グリッドごとの AttachableObject リスト */
	std::array<std::array<std::vector<AttachableObject*>, 8>, 8> grid_;

	/* 現在表示対象のセルにあるオブジェクト（フラスタムカリングの入力） */
	std::vector<AttachableObject*> visibleCellObjects_;

	/* 全 AttachableObject（Start()未実行含む）。デストラクタでの DeleteGO 用 */
	std::vector<AttachableObject*> allObjects_;

	/* 全 StaticObject。デストラクタでの DeleteGO 用 */
	std::vector<StaticObject*> staticObjects_;

	/* 吸着済みオブジェクト（表示制御から除外する） */
	std::unordered_set<AttachableObject*> attachedObjects_;

	float worldSizeX_      = 0.0f; //!< ステージの大きさ x
	float worldSizeZ_      = 0.0f; //!< ステージの大きさ z
	float worldSpaceSizeX_ = 0.0f; //!< 1セルのサイズ x
	float worldSpaceSizeZ_ = 0.0f; //!< 1セルのサイズ z

	Cell oldPosition_ = {};        //!< 前フレームのグリッド座標
	bool isUpdate_    = false;     //!< 更新するかどうか
};
