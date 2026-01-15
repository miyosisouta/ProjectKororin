/**
 * AttachableObject.h
 * 塊に吸着可能なオブジェクトを管理するクラス
 */

#pragma once
#include "StageObjectBase.h"


class AttachableObject : public StageObjectBase
{
public:
	/* コンストラクタ */
	AttachableObject() {};

	/* デストラクタ */
	~AttachableObject();

	/* スタート処理 */
	bool Start()override;
	/* 更新処理 */
	void Update()override;
	/* 描画処理 */
	void Render(RenderContext& rc)override;

public:
	/* UI時のオブジェクトの大きさを取得 */
	inline float GetUIScale() const { return UIDisplayscale_; }

	/* オブジェクトの吸着可能なレベルを取得 */
	inline int GetObjectSize() const { return objectSize_; }

	/* 吸着タイプの取得 */
	inline int GetObjectValue() const { return attachableValue_; }

	/* 塊のサイズの増加量取得 */
	inline int GetGrowAmount() const { return grouthAmount_; }

	/* 吸着時のSEの番号を取得 */
	inline int GetAttachSoundNum() const { return soundNum_; }

	/* SEを鳴らしたかどうかを設定 */
	inline void SetPlayedSE(bool isPlayedSE) { isPlayedSE_ = isPlayedSE; }

	/* SEを鳴らしたかのフラグ */
	inline bool IsPlayedSE() const { return isPlayedSE_; }

	/* 吸着しているオブジェクトを描画するかどうかのフラグ */
	inline void SetVisibleAttachedObject(const bool flg) { isVisibleAttachedObject_ = flg; }

	/* 吸着しているオブジェクトを描画するかどうかのフラグ */
	inline bool GetVisibleAttachedObject() { return isVisibleAttachedObject_; }

public:
	/* 当たり判定の破棄 */
	void DeletePhysicsStatics();


private:
	bool isPlayedSE_ = false;				// SEをならしたかフラグ(初期はfalse)
	bool isVisibleAttachedObject_ = false;	// くっついたオブジェクトの表示

};

