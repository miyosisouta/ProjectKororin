#include "stdafx.h"
#include "SpacePartitioning.h"
#include "Actor/Object/AttachableObject.h"
#include "Actor/Sphere/Sphere.h"
#include "StageLoader.h"
#include "Actor/Object/StaticObject.h"


namespace {
	static const int Z_NUM_ = 8;
	static const int X_NUM_ = 8;
	static const int SECURE_INITIAL_CAPACITY = 500;
	const Vector3 WORLD_HALF_SIZE = Vector3(14000.0f, 0.0f, 5000.0f);

}
SpacePartitioning* SpacePartitioning::instance = nullptr;


SpacePartitioning::SpacePartitioning()
{
	/* ステージ全体のサイズ */
	worldSizeX_ = WORLD_HALF_SIZE.x * 2.0f;
	worldSizeZ_ = WORLD_HALF_SIZE.z * 2.0f;

	/* 1個の空間のサイズ */
	worldSpaceSizeX_ = worldSizeX_ / static_cast<float>(X_NUM_);
	worldSpaceSizeZ_ = worldSizeZ_ / static_cast<float>(Z_NUM_);

	/* 1個の空間の半分のサイズ*/
	worldSpaceHalfSizeX_ = worldSpaceSizeX_ / 2.0f;
	worldSpaceHalfSizeZ_ = worldSpaceSizeZ_ / 2.0f;

	/**
	 * グリッドの左上（または左下）の基準開始位置
	 */
	baseX_ = -WORLD_HALF_SIZE.x + worldSpaceHalfSizeX_;
	baseZ_ = -WORLD_HALF_SIZE.z + worldSpaceHalfSizeZ_;

	objectList_.reserve(SECURE_INITIAL_CAPACITY);
	createObjectList_.reserve(SECURE_INITIAL_CAPACITY / 3);
	deleteObjectList_.reserve(SECURE_INITIAL_CAPACITY / 3);

	stageLoader_ = new StageLoader();
	stageLoader_->LoadObject(objectDataList_);

	for (auto list : objectDataList_) {
		AddObject(list.first, list.second);
	}

	float x = WORLD_HALF_SIZE.x / worldSpaceSizeX_;
	float z = WORLD_HALF_SIZE.z / worldSpaceSizeZ_;

	oldPosition_.x = x;
	oldPosition_.z = z;

	for (int i = -1; i <= 1; i++) {
		int positionX = x + i;

		if (positionX < 0) {
			continue;
		}

		for (int j = -1; j <= 1; j++) {
			int positionZ = z + j;

			if (positionZ < 0) {
				continue;
			}

			Cell c = { positionX,positionZ };
			AddCreateist(c);
		}
	}
	CreateStage();
}

SpacePartitioning::~SpacePartitioning()
{
}

void SpacePartitioning::Update()
{
	Sphere* spher = FindGO<Sphere>("sphere");
	
	if (spher == nullptr) {
		return;
	}

	int x = static_cast<int>((spher->GetPosition().x + WORLD_HALF_SIZE.x) / worldSpaceSizeX_);
	int z = static_cast<int>((spher->GetPosition().z + WORLD_HALF_SIZE.z) / worldSpaceSizeZ_);

	Cell a = { x,z };
	if (oldPosition_.x != x ||
		oldPosition_.z != z)
	{
		ListUpdate(x, z);
	}
	CreateObject();
	DeleteObject();
}

void SpacePartitioning::AddObject(int num, ObjectData* object)
{
	if (object->attachValue == 1) {
		stageObjectData.push_back(object);
		return;
	}

	int x = static_cast<int>((object->position.x + WORLD_HALF_SIZE.x) / worldSpaceSizeX_);
	int z = static_cast<int>((object->position.z + WORLD_HALF_SIZE.z) / worldSpaceSizeZ_);

	spacePartitioningList_.at(x).at(z).emplace(num, object);
}

void SpacePartitioning::AddDeleteList(Cell& position)
{
	for (auto it : spacePartitioningList_.at(position.x).at(position.z)) {
		auto createObjectListIt = std::find(createObjectList_.begin(), createObjectList_.end(), it.first);
		if (createObjectListIt != createObjectList_.end()) {
			createObjectList_.erase(createObjectListIt);
		}
		else {
			deleteObjectList_.push_back(it.first);
		}
	}
}

void SpacePartitioning::AddCreateist(Cell& position)
{
	for (auto it : spacePartitioningList_.at(position.x).at(position.z)) {
		auto deleteObjectListIt = std::find(deleteObjectList_.begin(), deleteObjectList_.end(), it.first);
		if (deleteObjectListIt != deleteObjectList_.end()) {
			deleteObjectList_.erase(deleteObjectListIt);
		}
		else {
			createObjectList_.push_back(it.first);
		}
	}
}



void SpacePartitioning::DeleteObject()
{
	if (deleteObjectList_.size() == 0) {
		return;
	}
	DeleteGO(objectList_[deleteObjectList_.back()]);
	deleteObjectList_.pop_back();
}

void SpacePartitioning::CreateObject()
{
	if (createObjectList_.size() == 0) {
		return;
	}
	int number = createObjectList_.back();
	auto* attachableObject = NewGO<AttachableObject>(0, "AttachableObject");
	attachableObject->Initialize(objectDataList_[number]);
	objectList_.emplace(number, attachableObject);
	createObjectList_.pop_back();
}



void SpacePartitioning::CreateStage()
{
	for(auto data:stageObjectData){
		auto* staticObject = NewGO<StaticObject>(0, "StaticObject");
		staticObject->Initialize(data);
		stageObject.push_back(staticObject);
	}
}

void SpacePartitioning::OffRender()
{
	for (auto stage : stageObject) {
		stage->OffRender();
	}
	for (auto object : objectList_) {
		object.second->OffRender();
	}
}




void SpacePartitioning::ListUpdate(int x, int z)
{
	int moveX = x - oldPosition_.x;
	int moveZ = z - oldPosition_.z;

	if (moveX != 0) {
		int newColX = x + moveX; 
		int oldColX = oldPosition_.x - moveX;

		for (int i = -1; i <= 1; i++) {
			int targetZ = z + i;

			if (targetZ >= 0 && targetZ < Z_NUM_) {
				if (newColX >= 0 && newColX < X_NUM_) {
					Cell c = { newColX, targetZ };
					AddCreateist(c);
				}
				if (oldColX >= 0 && oldColX < X_NUM_) {
					Cell c = { oldColX, targetZ };
					AddDeleteList(c);
				}
			}
		}
	}
	else if (moveZ != 0) {
		int newColZ = z + moveZ; 
		int oldColZ = oldPosition_.z - moveZ;

		for (int i = -1; i <= 1; i++) {
			int targetX = x + i;

			if (targetX >= 0 && targetX < X_NUM_) {
				if (newColZ >= 0 && newColZ < Z_NUM_) {
					Cell c = { newColZ, targetX };
					AddCreateist(c);
				}
				if (oldColZ >= 0 && oldColZ < Z_NUM_) {
					Cell c = { oldColZ, targetX };
					AddDeleteList(c);
				}
			}
		}
	}
	 oldPosition_.x = x;
	 oldPosition_.z = z;
	
}
