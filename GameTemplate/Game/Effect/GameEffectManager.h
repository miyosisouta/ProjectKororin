#pragma once

/**
 * GameEffectManager.h
 * エフェクト管理
 * 必要なエフェクトファイルを読み込んだり再生したりなど管理する
 */
#pragma once
#include <memory>
#include "graphics/effect/EffectEmitter.h"
#include "Effect/Types.h"


 /**
  * GameEffectManager管理用クラス
  *
  * mainクラスにてGameEffectManagerインスタンスを作成すると、
  * エフェクシアの初期化が終わっていないためエラーが発生。
  * そのため1フレーム遅らせて作るためのクラスです。
  */
class GameEffectObject : public IGameObject
{
public:
	/* コンストラクタ */
	GameEffectObject() {};
	/* デストラクタ */
	~GameEffectObject();

	/* スタート処理 */
	bool Start();
	/* 更新処理 */
	void Update() {}
	/* 描画処理 */
	void Render(RenderContext& rc) {}
};


/** サウンド用のハンドル名 */
using EffectHandle = uint32_t;
/** ハンドル無効値 */
static constexpr EffectHandle INVALID_EFFECT_HANDLE = 0xffffffff;


/**
 * サウンド管理クラス
 */
class GameEffectManager
{
private:
	/** エフェクトエミッターインスタンスを保持 */
	EffectEmitter* effect_ = nullptr;
	/** キー：管理番号(ハンドル), 値：エフェクトの実体(インスタンス) */
	std::map<EffectHandle, EffectEmitter*> effectList_;
	/**
	 * マップで参照するようにハンドル数を保持
	 * NOTE: 各エフェクトソースを参照するための数値はユニークな数値になる
	 */
	EffectHandle effectHandleCount_ = 0;


private:
	/* コンストラクタ */
	GameEffectManager();
	/* デストラクタ */
	~GameEffectManager();


public:
	/** エフェクトの再生 */
	EffectHandle Play(const int kind, const Vector3& pos, const Quaternion& rot, Vector3& scal);
	/** エフェクトの停止 */
	void Stop(const EffectHandle handle);


	EffectEmitter* FindEffect(const EffectHandle handle)
	{
		auto it = effectList_.find(handle); // 呼び出した番号のエフェクトをイテレーターに格納
		if (it != effectList_.end()) // イテレーターの中身が最後まで見つからなかった場合
		{
			// エフェクトエミッターを返す
			return it->second;
		}
		K2_ASSERT(false, "削除済みまたは追加されていないエフェクトにアクセスしようとしています。\n");
		return nullptr;
	}


	/******************* シングルトン ************************/
public:
	/* シングルトンを作成 */
	static void CreateInstance()
	{
		if (instance_ == nullptr)
		{
			instance_ = new GameEffectManager();
		}
	}


	/* シングルトンを取得 */
	static GameEffectManager& Get()
	{
		return *instance_;
	}


	/* シングルトンを破棄 */
	static void DestroyInstance()
	{
		if (instance_ != nullptr)
		{
			delete instance_;
			instance_ = nullptr;
		}
	}

private:
	/** シングルトンインスタンス */
	static GameEffectManager* instance_;
};

