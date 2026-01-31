/**
 * SoundManager.h
 * サウンド管理
 * 必要なサウンドファイルを読み込んだり再生したりなど管理する
 */
#pragma once
#include <memory>
#include "Sound/SoundSource.h"
#include "sound/Types.h"


 /** サウンド用のハンドル名 */
using SoundHandle = uint32_t;
/** ハンドル無効値 */
static constexpr SoundHandle INVALID_SOUND_HANDLE = 0xffffffff;


/**
 * サウンド管理クラス
 */
class SoundManager
{
private:
	/** BMG用のサウンドソースインスタンスを保持 */
	SoundSource* bgm_ = nullptr;
	SoundSource* se_ = nullptr;
	/** SE用のサウンドソースインスタンスを保持 */
	std::map<SoundHandle, SoundSource*> seList_;
	/**
	 * マップで参照するようにハンドル数を保持
	 * NOTE: 各サウンドソースを参照するための数値はユニークな数値になる
	 */
	SoundHandle soundHandleCount_ = 0;


private:
	float elapsedTime_ = 0.0f;		//!< フェードアウト用の経過時間
	float fadeTime_ = 0.0f;			//!< フェードアウト用の時間
	bool isVolumeFadeOut_ = false;	//!< ボリュームフェードアウト中かどうか
	bool isVolumeFadeIn_ = false;	//!< ボリュームフェードイン中かどうか

private:
	/* コンストラクタ */
	SoundManager();
	/* デストラクタ */
	~SoundManager();


public:
	/**
	 * 更新処理
	 * NOTE: 不要になったインスタンスの削除など
	 */
	void Update();


public:
	/** 
	 * BGM再生 
	 * 音をフェードインさせるかどうかは引数で指定(初期はfalse)
	 * フェード時間も引数で指定(初期は5.0f)
	 */

	void PlayBGM(const int kind, bool isVolumeFadeIn = false, float fadeTime = 5.0f);
	/** 
	 * BGM停止
	 * 音をフェードアウトさせるかどうかは引数で指定(初期はfalse)
	 * フェード時間も引数で指定(初期は5.0f)
	 */
	void StopBGM(bool isVolumeFadeOut = false, float fadeTime = 5.0f);

	/** SE再生 */
	SoundHandle PlaySE(const int kind, const bool isLood = false, const bool is3D = false);
	/** SE停止 */
	void StopSE(const SoundHandle handle);

	/* SEを探す */
	SoundSource* FindSE(const SoundHandle handle)
	{
		auto it = seList_.find(handle);
		if (it != seList_.end()) {
			return it->second;
		}
		K2_ASSERT(false, "削除済みか追加されていないSEにアクセスしようとしています。\n");
		return nullptr;
	}




	/**
	 * シングルトン用
	 */
public:
	/**
	 * インスタンスを作る
	 */
	static void CreateInstance()
	{
		if (instance_ == nullptr)
		{
			instance_ = new SoundManager();
		}
	}


	/**
	 * インスタンスを取得
	 */
	static SoundManager& Get()
	{
		return *instance_;
	}


	/**
	 * インスタンスを破棄
	 */
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
	static SoundManager* instance_;
};