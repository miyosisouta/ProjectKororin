#include "stdafx.h"
#include "StageLoader.h"
#include "Actor/Object/StageObjectBase.h"
#include <fstream>
#include <cstring>

// このクラスでしか使えないようにするためのnamespace
// また、他のファイルからアクセスできないため名前の競合も防ぐ
namespace
{
	const std::array<std::string, 51> objectNamePatterns = {
		"BHouse_01_color01",
		"BHouse_02_color01",
		"BHouse_02_color02",
		"BHouse_02_color03",
		"BHouse_03_color01",
		"BHouse_03_color02",
		"BHouse_04_color02",
		"BHouse_04_color03",
		"Burger",
		"ChessBoard",
		"Donut",
		"NBush01",
		"NGrassFence",
		"NHouseFloor",
		"NRockBig",
		"NPot_Bush_Small",
		"Natures_Big Tree",
		"Natures_Fir Tree",
		"Natures_Bush_01",
		"Natures_Cube Tree",
		"Natures_Grass Fence",
		"Natures_Grass Tile",
		"Natures_GrassTile",
		"Natures_House Floor",
		"Natures_Rock_Big",
		"Natures_Rock_small",
		"PBench1",
		"Props_Bench_1",
		"Props_Bench_2",
		"Props_BillBoard_medium",
		"Props_Bus Stop",
		"PHydrant",
		"PStreetLight",
		"Props_Street Light",
		"RIntersection01",
		"Road Lane_01",
		"RLaneBusStop",
		"VBus_color01",
		"Vehicle_Ambulance_separate",
		"Vehicle_Bus_color03",
		"Vehicle_Container_color02_separate",
		"VContainer_color03",
		"Vehicle_Pick up Truck_color01",
		"Vehicle_Pick up Truck_color03",
		"Vehicle_Police Car_separate",
		"Vehicle_SUV_color01",
		"Vehicle_SUV_color02",
		"Vehicle_Taxi",
		"Vehicle_Truck_color02_separate",
		"Vehicle_Truck_color03_separate",
		"Wine"
	};

	// オブジェクト同士多：1で名前が一致しているかを調べる
	bool IsForwardMatchObjectsName(const std::string& name)
	{
		for (const auto& pattern : objectNamePatterns) {
			if (name.size() >= pattern.size() &&
				name.compare(0, pattern.size(), pattern) == 0) {
				return true;
			}
		}
		return false;
	}

	// ---- バイナリ読み取りヘルパー -------------------------------------------

	// T型のデータを1つ読み込む
	template<typename T>
	bool BinRead(std::ifstream& fs, T& out)
	{
		return static_cast<bool>(
			fs.read(reinterpret_cast<char*>(&out), sizeof(T)));
	}

	// uint16(長さ) + UTF-8文字列 を読み込む
	bool BinReadString(std::ifstream& fs, std::string& out)
	{
		uint16_t len = 0;
		if (!BinRead(fs, len)) return false;
		out.resize(len);
		return len == 0 || static_cast<bool>(fs.read(&out[0], len));
	}

	// Vector3 を読み込む
	bool BinReadVector3(std::ifstream& fs, Vector3& v)
	{
		return BinRead(fs, v.x) && BinRead(fs, v.y) && BinRead(fs, v.z);
	}

	// Quaternion を読み込む
	bool BinReadQuaternion(std::ifstream& fs, Quaternion& q)
	{
		return BinRead(fs, q.x) && BinRead(fs, q.y) && BinRead(fs, q.z) && BinRead(fs, q.w);
	}
}

void StageLoader::LoadObject(std::unordered_map<int, ObjectData*>& objectList)
{
	std::ifstream fs("Assets/Scene/SceneExportTest.bin", std::ios::binary);
	if (!fs) {
		K2_LOG("StageLoader: ファイルを開けません\n");
		return;
	}

	// マジックナンバー確認 "SCNE"
	char magic[4] = {};
	fs.read(magic, 4);
	if (std::memcmp(magic, "SCNE", 4) != 0) {
		K2_LOG("StageLoader: 不正なファイルフォーマットです\n");
		return;
	}

	uint32_t version     = 0;
	uint32_t objectCount = 0;
	BinRead(fs, version);
	BinRead(fs, objectCount);

	int num = 0;

	for (uint32_t i = 0; i < objectCount; ++i)
	{
		// name
		std::string name;
		if (!BinReadString(fs, name)) break;

		// Transform
		Vector3    position, scale;
		Quaternion rotation;
		if (!BinReadVector3(fs, position))    break;
		if (!BinReadQuaternion(fs, rotation)) break;
		if (!BinReadVector3(fs, scale))       break;

		// Attributeフラグ
		uint8_t hasAttribute = 0;
		if (!BinRead(fs, hasAttribute)) break;

		// Attributeデータ（あれば読む）
		int32_t  attachableValue    = 0;
		int32_t  requiredSphereSize = 0;
		int32_t  objectRadius       = 0;
		int32_t  attachSoundNum     = 0;
		float    uiDisplayScale     = 0.0f;
		std::string assetName;
		Vector3  colliderCenter, colliderSize;

		if (hasAttribute) {
			BinRead(fs, attachableValue);
			BinRead(fs, requiredSphereSize);
			BinRead(fs, objectRadius);
			BinRead(fs, attachSoundNum);
			BinRead(fs, uiDisplayScale);
			BinReadString(fs, assetName);
			BinReadVector3(fs, colliderCenter);
			BinReadVector3(fs, colliderSize);
		}

		// 確認したいオブジェクトの名前がリストの中にあるかを調べる
		if (!hasAttribute || !IsForwardMatchObjectsName(name)) {
			continue;
		}

		ObjectData* objectData = new ObjectData;

		objectData->position     = position;
		objectData->rotation     = rotation;
		objectData->scale        = scale;
		objectData->attachValue  = attachableValue;		// 吸着可能かどうかのデータを格納
		objectData->size         = requiredSphereSize;	// 塊が吸着できるサイズを格納
		objectData->assetName    = assetName;			// アセットの名前を格納

		K2_LOG("Name : %s \n", objectData->assetName.c_str());

		objectData->grouthAmount  = objectRadius;		// 塊の成長量
		objectData->colliderPivot = colliderCenter;		// コライダーの起点のポジションを格納
		objectData->colliderSize  = colliderSize;		// コライダーの大きさを格納
		objectData->uiObjectScal  = uiDisplayScale;		// UIとして表示するオブジェクトの大きさを格納
		objectData->soundNum      = attachSoundNum;
		objectData->ID            = num;

		objectList.emplace(num, objectData);
		num++;
	}
}
