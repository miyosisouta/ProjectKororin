/**
 * @file Stage.h
 * @brief ステージのオブジェクトロードするクラスの宣言。
 */
#pragma once
struct ObjectData;
class StageLoader
{
public:
	

	/**
	 * @brief コンストラクタ。
	 */
	StageLoader() {}
	/**
	 * @brief デストラクタ。
	 */
	~StageLoader() {}


	void LoadObject(std::unordered_map<int, ObjectData*>& objectList);
	};

