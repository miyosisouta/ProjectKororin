/**
 * LateStageObjectUpdateManager.h
 * 遅れてステージオブジェクトを更新するための処理
 * NOTE:
 */
#pragma once


class Sphere;			//!< 塊
class StageObjectBase;	//!< オブジェクト管理用クラス


/**
 * 遅れてステージオブジェクトを更新する管理者
 */
class LateStageObjectUpdateManager
{
private:
	/* 塊 */
	Sphere* sphere_;

	/* StageObjectBase型のポインタリスト */
	std::vector<StageObjectBase*> objectList_;


private:
	/* コンストラクタ */
	LateStageObjectUpdateManager();
	/* デストラクタ */
	~LateStageObjectUpdateManager();


public:
	/* 更新処理 */
	void Update();


public:
	/** オブジェクトを登録 */
	void RegisterObject(StageObjectBase* object);
	/* リストから要素を削除 */
	void UnregisterObject(StageObjectBase* object);

	/** スフィアを登録 */
	void RegisterSphere(Sphere* sphere)
	{
		sphere_ = sphere;
	}
	/* 塊を削除 */
	void UnRegisterSphere()
	{
		sphere_ = nullptr;
	}

private:
	/* オブジェクトの形のデータを探す */
	StageObjectBase* FindTargetObject(btCollisionShape* target);


private:
	static LateStageObjectUpdateManager* instance_; //!< インスタンス


	/************************************* シングルトン *****************************************/
public:

	/* インスタンスを作成 */
	static void CreateInstance()
	{
		if (instance_ == nullptr)
		{
			instance_ = new LateStageObjectUpdateManager();
		}
	}

	/* インスタンスを取得 */
	static LateStageObjectUpdateManager& Get()
	{
		return *instance_;
	}

	/* インスタンスを削除 */
	static void Delete()
	{
		if (instance_)
		{
			delete instance_;
			instance_ = nullptr;
		}
	}
};