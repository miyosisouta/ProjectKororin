#include "stdafx.h"
#include "Stage.h"
#include "SceneLoader/SceneLoader.h"
#include <vector>

// このクラスでしか使えないようにするためのnamespace
// また、他のファイルからアクセスできないため名前の競合も防ぐ
namespace
{

	// 判定したいオブジェクト名のリスト
	// (1)や(2)入れず、オブジェクトの名前だけを入れる
	// array<型,データの数>
	/*const std::array<std::string, 35> objectNamePatterns = {
		"BHouse_01_color01",
		"BHouse_02_color01",
		"BHouse_02_color02",
		"BHouse_02_color03",
		"BHouse_03_color02",
		"BHouse_04_color02",
		"BHouse_04_color03",
		"NBush01",
		"NCubeTree",
		"NFirTree",
		"NGrassFence",
		"NGrassTile",
		"NHouseFloor",
		"NRock_Big",
		"PBench_01",
		"PBusStop",
		"PHydrant",
		"PStreetLight",
		"PTrafficSignal_Big",
		"PLaneBusStop",
		"RIntersection",
		"RLane01",
		"RLane04",
		"RLaneBusStop",
		"VBus_color01",
		"VContainer_color03",
		"VPickUpTruck_color01",
		"VPickUpTruck_color03",
		"VSUV_color01",
		"VSUV_color02",
		"VTaxi",
		"Donut",
		"ChessBoard",
		"Burger",
		"Wine",
	};*/

	// todo for test
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


	// シーンから読み込んだオブジェクトの名前が完全一致か
	bool IsMatchObjectName(const char* jsonNameA, const char* nameB)
	{
		if (strcmp(jsonNameA, nameB) == 0) {
			// 完全一致
			return true;
		}
		// 一致しない
		return false;
	}

	// 先頭から len 文字分が一致しているか判定する関数
	// オブジェクト同士1：1で名前が一致しているかを調べる
	bool IsForwardMatchObjectName(const char* jsonNameA, const char* nameB)
	{
		auto len = strlen(nameB);
		auto namelen = strlen(jsonNameA);
		if (len > namelen) {
			//名前が長い。不一致。
			return false;
		}
		if (strncmp(jsonNameA, nameB, len) == 0) {
			// 完全一致
			return true;
		}
		// 一致しない
		return false;
	}

	// オブジェクト同士多：1で名前が一致しているかを調べる
	bool IsForwardMatchObjectsName(const char* jsonObjectName)
	{
		for (auto targetName : objectNamePatterns) {
			if (IsForwardMatchObjectName(jsonObjectName, targetName.c_str())) {
				return true;
			}
		}
		return false;
	}


	// AttributeValueデータを取得しint型で返すための関数
	auto ParseAttachableValue(const nlohmann::json& attributeJson)
	{
		return attributeJson["attachableValue"].get<int>();
	}

	// attachableValueデータを取得しint型で返すための関数
	auto ParseRequiredSphereSize(const nlohmann::json& attributeJson)
	{
		return attributeJson["requiredSphereSize"].get<int>();
	}

	// attachableValueデータを取得しint型で返すための関数
	auto ParseGrowthAmount(const nlohmann::json& attributeJson)
	{
		return attributeJson["objectRadius"].get<int>();
	}

	// objectAssetNameデータを取得しstring型で返すための関数
	auto ParseObjectAssetName(const nlohmann::json& attributeJson) {
		return attributeJson["objectAssetName"].get<std::string>();
	}

	auto ParseUIDisPlayScale(const nlohmann::json& attributeJson) {
		return attributeJson["UIDisplayScale"].get<float>();
	}
}


bool Stage::Start() 
{
	// for分で繰り返される
	//LoadScene("Assets/Scene/SceneExport.json", [&](const nlohmann::json& j)
	LoadScene("Assets/Scene/SceneExportTest.json", [&](const nlohmann::json& j)
		{
			// JSONデータから "name" フィールドを取得し、name 変数に格納
			const std::string name = j["name"];


			// オブジェクト名が "  " で始まるかどうかを判定する。
			// 例えば "block01" や "blockA" なども一致する。
			if (IsForwardMatchObjectsName(name.c_str()))	// 確認したいオブジェクトの名前がリストの中にあるかを調べる
			{ 

				// JSONデータの "Transform" フィールド（位置・回転・スケール情報）を
				// ParseTransformComponents 関数でパースし、transform 変数に格納します。
				// パース:データ（文字列やファイルなど）をプログラムで“分解・解析”して、使いやすい形（構造体やクラスなど）に変換すること
				const auto transform = ParseTransformComponents(j["Transform"]);

				// Attributeのアドレスを取得
				const auto& attributeJson = j["Attribute"];

				// アドレスの中のデータを格納
				const auto attributeValue = ParseAttachableValue(attributeJson); // 吸着可能かどうかのデータを格納
				const auto requiredSphereSize = ParseRequiredSphereSize(attributeJson); // 塊が吸着できるサイズを格納
				const auto objectAssetName = ParseObjectAssetName(attributeJson); // アセットの名前を格納
				const auto growthAmount = ParseGrowthAmount(attributeJson); // 塊の成長量
				const auto colliderPivot = ParseVector3(attributeJson.at("colliderCenter")); // コライダーの起点のポジションを格納
				const auto colliderSize = ParseVector3(attributeJson.at("colliderSize")); // コライダーの大きさを格納
				const auto uiDisPlayScale = ParseUIDisPlayScale(attributeJson); // UIとして表示するオブジェクトの大きさを格納


				// 属性別のNewGO
				switch (attributeValue)
				{
					case 0:
					{
						auto* obj = NewGO<AttachableObject>(0, "AttachableObject");
						const auto colliderPivot = ParseVector3(attributeJson.at("colliderCenter")); // コライダーの起点のポジションを格納
						obj->Initialize(attributeValue, transform.position, transform.scale, transform.rotation, requiredSphereSize, objectAssetName, growthAmount, colliderPivot, colliderSize, uiDisPlayScale);
						attachableObjectList_.push_back(obj);
						break;
					}
					case 1:
					{
						auto* obj = NewGO<StaticObject>(0, "StaticObject");
						obj->Initialize(attributeValue, transform.position, transform.scale, transform.rotation, requiredSphereSize, objectAssetName, growthAmount, colliderPivot, colliderSize, uiDisPlayScale);
						staticObjectList_.push_back(obj);
						break;
					}
					case 2:
					{
						auto* obj = NewGO<MovableObject>(0, "MovableObject");
						obj->Initialize(attributeValue, transform.position, transform.scale, transform.rotation, requiredSphereSize, objectAssetName, growthAmount, colliderPivot, colliderSize, uiDisPlayScale);
						movableObjectList_.push_back(obj);
						break;
					}
					default:
					{
						K2_ASSERT(false, "未対応です。追加してください。");
						break;
					}
				}
			}
			return true;
		});



	return true;
}


void Stage::Update()
{
	// 吸着可能オブジェクトに対しての処理
	{
		const bool isVisible = isVisibleAll_ || isVisibleAttackObject_;	// どちらかtrueじゃないと表示されない
		for (auto* obj : attachableObjectList_) {
			obj->SetVisible(isVisible);
		}
	}
	// 吸着不可オブジェクト
	{
		const bool isVisible = isVisibleAll_;
		for (auto* obj : staticObjectList_) {
			obj->SetVisible(isVisible);
		}
	}
	// // 動的オブジェクト
	{
		const bool isVisible = isVisibleAll_;
		for (auto* obj : movableObjectList_) {
			obj->SetVisible(isVisible);
		}
	}
}