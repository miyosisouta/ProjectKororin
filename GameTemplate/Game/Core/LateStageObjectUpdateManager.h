/**
 * LateStageObjectUpdateManager.h
 * 遅れてステージオブジェクトを更新するための処理
 * NOTE: 
 */
#pragma once


class Sphere; // 塊
class StageObjectBase; // オブジェクト管理


/**
 * 遅れてステージオブジェクトを更新する管理者
 */
class LateStageObjectUpdateManager
{
private:
	/**
	 * スフィア
	 * NOTE:オブジェクトを更新するための対象
	 */
	Sphere* sphere_;

	/** オブジェクト群 */
	std::vector<StageObjectBase*> objectList_;


private:
	LateStageObjectUpdateManager();
	~LateStageObjectUpdateManager();


public:
	void Update();


public:
	/** オブジェクトを登録 */
	void RegisterObject(StageObjectBase* object);
	void UnregisterObject(StageObjectBase* object);

	/** スフィアを登録 */
	void RegisterSphere(Sphere* sphere)
	{
		sphere_ = sphere;
	}
	void UnRegisterSphere()
	{
		sphere_ = nullptr;
	}

private:
	StageObjectBase* FindTargetObject(btCollisionShape* target);




private:
	static LateStageObjectUpdateManager* instance_;


public:
	static void Create()
	{
		if (instance_ == nullptr)
		{
			instance_ = new LateStageObjectUpdateManager();
		}
	}
	static LateStageObjectUpdateManager& Get()
	{
		return *instance_;
	}
	static void Delete()
	{
		if (instance_)
		{
			delete instance_;
			instance_ = nullptr;
		}
	}
};