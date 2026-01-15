/**
 * ゲームUIの管理者
 */
#include "UI/SphereSizeText.h"

class GameUIUpdate : public IGameObject
{
public:
	/* コンストラクタ */
	GameUIUpdate() {}
	/* デストラクタ */
	~GameUIUpdate() {}

	/* スタート処理 */
	virtual bool Start() override { return true; }	//!< 使用しない
	/* 更新処理 */
	virtual void Update() override;
	/* 描画処理 */
	virtual void Render(RenderContext& rc) {}		//!< 使用しない
};


/* 通知のタイプ */
struct UINotifyType
{
	enum Enum
	{
		SphereSizeText,
	};
};


/* 通知取得時の情報 */
struct UIINotify
{
	UINotifyType::Enum type;
	// int型で受け取り、特定の型へ変更する
	UIINotify(int type) : type(static_cast<UINotifyType::Enum>(type)) {}
};


/* UIに伝える情報をまとめる通知 */
struct NotifySphereSizeText : UIINotify
{
	float radius = 0.0f;
	SphereSizeText* sphereSizeText;

	// SphereSizeTextを引数としてコンストラクタに渡す
	NotifySphereSizeText()
		: UIINotify(UINotifyType::SphereSizeText)
	{
	}
};


class SphereSizeText;
class ObjectView;
class GameUIManager
{
private:
	std::vector<UIINotify*> notifyList_;		//!< 通知のリスト
	SphereSizeText* sphereSizeText = nullptr;	//!< スフィアサイズテキスト


private:
	/* コンストラクタ */
	GameUIManager();
	/* デストラクタ */
	~GameUIManager();


public:
	/* 更新処理 */
	void Update();


public:
	/* 通知の追加 */
	void Notify(UIINotify* notify);

	/* SphereSizeTextのポインターを設定する関数を追加 */
	void SetSphereSizeText(SphereSizeText* text) { sphereSizeText = text; }


private:
	/* リストの中を全て削除 */
	void ClearNotify()
	{
		for (auto* notify : notifyList_) {
			delete notify;
			notify = nullptr;
		}
		notifyList_.clear();
	}



	/*************************** シングルトン *****************************/

private:
	static GameUIManager* instance_; //!< インスタンス


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

	/* インスタンスの取得 */
	static GameUIManager& Get()
	{
		return *instance_;
	}

	/* インスタンスの削除 */
	static void DeleteInstance()
	{
		if (instance_) {

		}
	}
};