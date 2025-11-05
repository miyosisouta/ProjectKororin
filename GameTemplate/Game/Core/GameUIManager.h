/**
 * ゲームUIの管理者
 */
#include "UI/SphereSizeText.h"

class GameUIUpdate : public IGameObject
{
public:
	GameUIUpdate() {}
	~GameUIUpdate() {}

	virtual bool Start() override { return true; }	// 使用しない
	virtual void Update() override;
	virtual void Render(RenderContext& rc) {}		// 使用しない
};

// 構造体にすることでEnumの名前が同じでも問題ない状態にする
struct UINotifyType
{
	enum Enum
	{
		SphereSizeText,
		ObjectView,
	};
};

struct UIINotify
{
	UINotifyType::Enum type;
	// int型で受け取り、特定の型へ変更する
	UIINotify(int type) : type(static_cast<UINotifyType::Enum>(type)) {}
};

struct NotifySphereSizeText : UIINotify
{
	float radius = 0.0f;
	SphereSizeText* sphereSizeText;

	// SphereSizeTextを引数としてコンストラクタに渡す
	NotifySphereSizeText()
		: UIINotify(UINotifyType::SphereSizeText)
	{}
};

struct NotifyObjectView : UIINotify
{
	// ObjectViewを引数としてコンストラクタに渡す
	NotifyObjectView()
		: UIINotify(UINotifyType::ObjectView)
	{
	}
};


class SphereSizeText;
class ObjectView;
class GameUIManager
{
private:
	std::vector<UIINotify*> notifyList_;
	SphereSizeText* sphereSizeText = nullptr;
	ObjectView* objectView = nullptr;


private:
	GameUIManager();
	~GameUIManager();


public:
	/**
	 * @brief 通常更新
	 */
	void Update();


public:
	/**
	 * @brief 通知追加
	 */
	void Notify(UIINotify* notify);

	// SphereSizeTextのポインターを設定する関数を追加
	void SetSphereSizeText(SphereSizeText* text) { sphereSizeText = text; }
	void SetObjectView(ObjectView* object) { objectView = object; }

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
	 * シングルトン関連
	 */
private:
	static GameUIManager* instance_;


public:
	/* インスタンスを作成 */
	static void CreateInstance()
	{
		if (instance_ == nullptr) {
			instance_ = new GameUIManager();
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
	static GameUIManager& Get()
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