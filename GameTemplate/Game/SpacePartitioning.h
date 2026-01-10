#pragma once


struct Cell
{
	int x = 0;
	int z = 0;
};

struct ObjectData;
class AttachableObject;
class StaticObject;
class StageLoader;
class StageObjectBase;
class SpacePartitioning
{
private:
	std::vector<int> aaa;
	std::vector<int> bbb;
	static SpacePartitioning* instance;

	SpacePartitioning();

public:
	~SpacePartitioning();

	SpacePartitioning(const SpacePartitioning&) = delete;
	SpacePartitioning& operator=(const SpacePartitioning&) = delete;

	static SpacePartitioning* GetInstance() {
		return instance;
	}

	static void CreateInstance() {
		if (instance == nullptr) {
			instance = new SpacePartitioning(); // 初回だけ作成
		}
	}

	static void DeleteInstance() {
		delete instance;
		instance = nullptr;
	}

	void Update();
	void AddObject(int num, ObjectData* object);
	void AddDeleteList(Cell& position);
	void AddCreateist(Cell& position);
	void DeleteObject();
	void CreateObject();
	void CreateStage();
	void UpdateStart() { isUpdate_ = true; }
	void UpdateStop() { isUpdate_ = false; }
	void OffRender();

private:
	void ListUpdate(int x, int z);

private:
	StageLoader* stageLoader_ = nullptr;

	std::unordered_map<int, ObjectData*> objectDataList_;
	std::vector<ObjectData*> stageObjectData;
	std::vector<StaticObject*> stageObject;
	std::array<std::array<std::unordered_map<int, ObjectData*>, 8>, 8> spacePartitioningList_;
	std::unordered_map<int, AttachableObject*> objectList_;
	std::vector<int> createObjectList_;
	std::vector<int> deleteObjectList_;

	float worldSizeX_ = 0.0f; //!< ステージの大きさx座標
	float worldSizeZ_ = 0.0f; //!< ステージの大きさz座標

	float worldSpaceSizeX_ = 0.0f; //!< 1個の空間のサイズx座標
	float worldSpaceSizeZ_ = 0.0f; //!< 1個の空間のサイズz座標

	float worldSpaceHalfSizeX_ = 0.0f; //!< 1個の空間のサイズの半分のサイズx座標
	float worldSpaceHalfSizeZ_ = 0.0f; //!< 1個の空間のサイズの半分のサイズz座標

	float baseX_ = 0.0f; //!< 基準開始地点x座標
	float baseZ_ = 0.0f; //!< 基準開始地点z座標

	int identificationNumber_ = 0;

	Cell oldPosition_ = {};

	bool isUpdate_ = false;
};
