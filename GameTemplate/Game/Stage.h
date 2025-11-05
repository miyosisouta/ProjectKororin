/**
 * @file Stage.h
 * @brief ステージのオブジェクトのデータを格納するクラスの宣言。
 */
#pragma once

#include "Actor/Object/AttachableObject.h"
#include "Actor/Object/StaticObject.h"
#include "Actor/Object/MovableObject.h"

class Stage : public IGameObject
{
private:
	// すべてのオブジェクトに変更、削除を加えるためのList変数
	std::vector<AttachableObject*> attachableObjectList_; // 吸着可能オブジェクトのデータを格納
	std::vector<StaticObject*> staticObjectList_; // 吸着不可オブジェクトのデータを格納
	std::vector<MovableObject*> movableObjectList_; // 動的オブジェクトのデータを格納


public:
	

	/**
	 * @brief コンストラクタ。
	 */
	Stage() {}
	/**
	 * @brief デストラクタ。
	 */
	~Stage() {}


	/**
	 * @brief 初期化処理。
	 * @return 成功した場合は true。
	 */
	bool Start()override;
	void Update() override {}
	void Render(RenderContext& rc) override {}


	const std::vector<AttachableObject*>& const GetAttachableObjectList(){	return attachableObjectList_;	}
	const std::vector<StaticObject*>& const GetStaticObjectList(){	return staticObjectList_;	}
	const std::vector<MovableObject*>& const GetMovableObjectList(){	return movableObjectList_;	}

};

