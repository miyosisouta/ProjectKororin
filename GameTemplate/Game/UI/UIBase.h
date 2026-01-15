/**
 * UIBase.h
 * UIの基本的な処理をするクラス群
 */
#pragma once
#include "UIAnimation.h"


class UIBase : public Noncopyable
{
public:
	Transform m_transform; //!< トランスフォーム
	Vector4 color_ = Vector4::White; //!< カラー

	std::vector<std::unique_ptr<UIAnimationBase>> uiAnimationList_; //!< UIアニメーションリスト

	bool isStart = false;	//!< スタートフラグ
	bool isUpdate = true;	//!< 更新フラグ
	bool isDraw = true;		//!< 描画フラグ


public:
	UIBase()
	{
	}
	virtual ~UIBase()
	{
	}

	virtual bool Start() = 0;
	virtual void Update() = 0;
	virtual void Render(RenderContext& rc) = 0;


public:
	void UpdateAnimation()
	{
		for (auto& ui : uiAnimationList_) {
			ui.get()->Update();
		}
	}
	void PlayAnimation()
	{
		for (auto& ui : uiAnimationList_) {
			ui.get()->Play();
		}
	}
	bool IsPlayAnimation() const
	{
		for (auto& ui : uiAnimationList_) {
			return ui.get()->IsPlay();
		}
	}

	void SetUIAnimation(std::unique_ptr<UIAnimationBase>animation);
};




// ============================================
// 画像を使うUI関連
// ============================================


class UIImage : public UIBase
{
protected:
	SpriteRender m_spriteRender;

public:
	UIImage();
	~UIImage();

	virtual bool Start() override;
	virtual void Update() override;
	virtual void Render(RenderContext& rc) override;

public:
	/** スプライトレンダーの取得 */
	SpriteRender* GetSpriteRender() { return &m_spriteRender; }
};



/**
 * ゲージUI
 */
class UIGauge : public UIImage
{
	friend class UICanvas;


private:
	UIGauge();
	~UIGauge();


public:
	virtual bool Start() override;
	virtual void Update() override;
	virtual void Render(RenderContext& rc) override;

public:
	void Initialize(const char* assetName, const float width, const float height, const Vector3& position, const Vector3& scale, const Quaternion& rotation);
};


/**
 * アイコンUI
 */
class UIIcon : public UIImage
{
	friend class UICanvas;

public:
	UIIcon();
	~UIIcon();

	virtual bool Start() override;
	virtual void Update() override;
	virtual void Render(RenderContext& rc) override;


public:
	void Initialize(const char* assetName, const float width, const float height, const Vector3& position, const Vector3& scale, const Quaternion& rotation);
};




// ============================================
// 文字を使うUI関連
// ============================================


class UIText : public UIBase
{
protected:
	FontRender m_fontRender;


private:
	UIText();
	~UIText();


public:
	virtual bool Start() override;
	virtual void Update() override;
	virtual void Render(RenderContext& rc) override;
};




// ============================================
// キャンバス
// ============================================


/**
 * 絵を書くキャンバスのイメージ
 * UIを作るときにこのクラスを作ってください
 */
class UICanvas : public UIBase
{
	friend class UIBase;
	friend class UIImage;
	friend class UIGauge;
	friend class UIIcon;
	friend class UIText;
	friend class UIButton;


private:
	/**
	 * NOTE: 各UI自体に親子関係持たせたいけど使わない可能性があるので、一旦ここだけにしてみる
	 */
	std::vector<UIBase*> m_uiList;


public:
	UICanvas();
	~UICanvas();


	bool Start();
	void Update();
	void Render(RenderContext& rc);


public:
	template <typename T>
	T* CreateUI()
	{
		T* ui = new T();
		ui->m_transform.SetParent(&m_transform);
		m_uiList.push_back(ui);
		return ui;
	}
};