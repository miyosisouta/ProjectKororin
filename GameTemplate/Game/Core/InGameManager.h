/**
 * インゲーム管理者
 */
#include "Actor/Object/AttachableObject.h"
#include "Actor/Sphere/Sphere.h"

// 最初に実行するUpdate
class InGameUpdateObject : public IGameObject
{
public:
	InGameUpdateObject() {}
	~InGameUpdateObject() {}

	virtual bool Start() override { return true; }	// 使用しない
	virtual void Update() override;
	virtual void Render(RenderContext& rc) {}		// 使用しない
};

// 次に実行するUpdate
class InGameLateUpdateObject : public IGameObject
{
public:
	InGameLateUpdateObject() {}
	~InGameLateUpdateObject() {}

	virtual bool Start() override { return true; }	// 使用しない
	virtual void Update() override;
	virtual void Render(RenderContext& rc) {}		// 使用しない
};



/**************************/


// 構造体にすることでEnumの名前が同じでも問題ない状態にする
struct NotifyType
{
	enum Enum
	{
		CollisionHit,
	};
};

struct INotify
{
	NotifyType::Enum type;
	// int型で受け取り、特定の型へ変更する
	INotify(int type) : type(static_cast<NotifyType::Enum>(type)) {}
};


struct NotifyCollisionHit : INotify
{
	IGameObject* left;
	IGameObject* right;
	GameObjectType::Enum leftType;
	GameObjectType::Enum rightType;
	
	// CollisionHitを引数としてコンストラクタに渡す
	NotifyCollisionHit()
		: INotify(NotifyType::CollisionHit)
	{
	}
};


class InGameManager
{
private:
	std::vector<INotify*> notifyList_;



private:
	InGameManager();
	~InGameManager();

	bool CanAttach(AttachableObject& target, Sphere& sphere);

public:
	/**
	 * @brief 通常更新
	 */
	void Update();

	/**
	 * @brief 遅れて更新
	 */
	void LateUpdate();


public:
	/**
	 * @brief 通知追加
	 */
	void Notify(INotify* notify);




private:
	/**
	 * @brief リストの情報をすべて削除、nullptrにする
	 */
	void ClearNotify()
	{
		for (auto* notify : notifyList_) {
			delete notify;
			notify = nullptr;
		}
		notifyList_.clear();
	}

	/**
	 * 指定したクラスを取得する
	 * NOTE: 指定したクラスが存在しない場合はnullptrを返す
	 */
	template <typename T>
	T* GetTargetObject(NotifyCollisionHit& pair, const GameObjectType::Enum targetType)
	{
		if (pair.leftType == targetType)
		{
			return static_cast<T*>(pair.left);
		}
		else if (pair.rightType == targetType)
		{
			return static_cast<T*>(pair.right);
		}
		return nullptr;
	}



	/**
	 * シングルトン関連
	 */
private:
	static InGameManager* instance_;


public:
	/* インスタンスを作成 */
	static void CreateInstance()
	{
		if (instance_ == nullptr) {
			instance_ = new InGameManager();
		}
	}

	/* 利用可能状態か確認する関数 */
	static bool IsAvailable()
	{
		return instance_ != nullptr;
	}
	/**
	 * @brief インスタンスを入手
	 * @return 
	 */
	static InGameManager& Get()
	{
		return *instance_;
	}

	/**
	 * @brief インスタンスを削除
	 */
	static void DeleteInstance()
	{
		if (instance_) {

		}
	}
};