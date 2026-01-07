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

	bool isVisibleAll_ = true;						// 全部のオブジェクトを描画するかどうかのフラグ(ただ、個別表示フラグが優先される)
	bool isVisibleAttachedObject_ = true;			// 塊に吸着しているオブジェクトを全て表示するかどうかのフラグ(最優先される)


public:


	/**
	 * @brief コンストラクタ。
	 */
	Stage() {}
	/**
	 * @brief デストラクタ。
	 */
	~Stage();


	/**
	 * @brief 初期化処理。
	 * @return 成功した場合は true。
	 */
	bool Start()override;
	void Update() override;
	void Render(RenderContext& rc) override {}


	const std::vector<AttachableObject*>& const GetAttachableObjectList() { return attachableObjectList_; }
	const std::vector<StaticObject*>& const GetStaticObjectList() { return staticObjectList_; }
	const std::vector<MovableObject*>& const GetMovableObjectList() { return movableObjectList_; }


public:
	/**
	 * @brief 全てのオブジェクトを描画するかどうかを設定(個人の描画フラグを優先)
	 */
	inline void SetVisibleAll(const bool flg) { isVisibleAll_ = flg; } 

	/**
	 * @brief //!< 塊に吸着しているオブジェクトを描画するかどうかを設定
	 * @param flg 
	 */
	inline void SetVisibleAttachedObject(const bool flg) { isVisibleAttachedObject_ = flg; }	
};

