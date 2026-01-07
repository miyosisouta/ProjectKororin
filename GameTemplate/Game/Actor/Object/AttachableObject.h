/**
 * AttachableObject.h
 * 塊に吸着可能なオブジェクトを管理するクラス
 */

#pragma once
#include "StageObjectBase.h"


class AttachableObject : public StageObjectBase
{
public:
	/**
	 * @brief コンストラクタ
	 */
	AttachableObject() {};
	/**
	 * @brief デストラクタ
	 */
	~AttachableObject();

	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;

public:
	inline float GetUIScale() const
	{
		return UIDisplayscale_;
	}

	inline int GetObjectSize() const
	{
		return objectSize_;
	}

	inline int GetObjectValue() const
	{
		return attachableValue_;
	}

	inline int GetGrowAmount() const
	{
		return grouthAmount_;
	}

	inline int GetAttachSoundNum() const
	{
		return soundNum_;
	}
	inline void SetPlayedSE(bool isPlayedSE)
	{
		isPlayedSE_ = isPlayedSE;
	}

	inline bool IsPlayedSE() const
	{
		return isPlayedSE_;
	}

	inline void SetVisibleAttachedObject(const bool flg) { isVisibleAttachedObject_ = flg; }	// 吸着しているオブジェクトを描画するかどうかのフラグ
	inline bool GetVisibleAttachedObject() { return isVisibleAttachedObject_; }	// 吸着しているオブジェクトを描画するかどうかのフラグ

public:
	/**
	 * @brief 当たり判定の破棄とnullptr
	 */
	void DeletePhysicsStatics();


private:
	bool isPlayedSE_ = false;
	bool isVisibleAttachedObject_ = false;	// くっついたオブジェクトの表示

};

