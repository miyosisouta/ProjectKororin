/**
 * SpriteAnimation.h
 * SpriteRenderをアニメーションさせる機能群
 */
#pragma once
#include "Util/Curve.h"


class UIBase;


template <typename T>
using UIAnimationApplyFunc = std::function<void(const T&)>;

class UIAnimationBase
{
protected:
	UIBase* m_ui = nullptr;


public:
	UIAnimationBase() {}
	~UIAnimationBase() {}

	virtual void Update() = 0;
	virtual void Play() = 0;
	virtual bool IsPlay() = 0;


	void SetUI(UIBase* ui) { m_ui = ui; }
};



class UIVector2Animation : public UIAnimationBase
{
private:
	Vector2Curve m_curve;
	/** カーブ用のパラメーター */
	Vector2 start_ = Vector2::Zero;
	Vector2 end_ = Vector2::Zero;
	float timeSec_ = 0.0f;
	EasingType type_ = EasingType::Linear;
	LoopMode loopMode_ = LoopMode::Once;

	UIAnimationApplyFunc<Vector2> applyFunc_;


public:
	UIVector2Animation() {}
	~UIVector2Animation() {}

	void Update() override
	{
		m_curve.Update(g_gameTime->GetFrameDeltaTime());
		applyFunc_(m_curve.GetCurrentValue());
	}


	void Play() override
	{
		m_curve.Play(start_, end_, timeSec_, type_, loopMode_);
	}


	bool IsPlay() override
	{
		return m_curve.IsPlaying();
	}


	void SetParameter(Vector2 start, Vector2 end, float timeSec, EasingType type, LoopMode loopMode)
	{
		start_ = start;
		end_ = end;
		timeSec_ = timeSec;
		type_ = type;
		loopMode_ = loopMode;
		m_curve.Play(start, end, timeSec, type, loopMode);
	}


	Vector2 GetCurrentValue()
	{
		return m_curve.GetCurrentValue();
	}

	void SetFunc(const UIAnimationApplyFunc<Vector2>& func)
	{
		applyFunc_ = func;
	}
};


class UIVector3Animation : public UIAnimationBase
{
protected:
	Vector3Curve m_curve;
	/** カーブ用のパラメーター */
	Vector3 start_ = Vector3::Zero;
	Vector3 end_ = Vector3::Zero;
	float timeSec_ = 0.0f;
	EasingType type_ = EasingType::Linear;
	LoopMode loopMode_ = LoopMode::Once;

	UIAnimationApplyFunc<Vector3> applyFunc_;


public:
	UIVector3Animation() {}
	~UIVector3Animation() {}

	void Update() override
	{
		m_curve.Update(g_gameTime->GetFrameDeltaTime());
		applyFunc_(m_curve.GetCurrentValue());
	}


	void Play() override
	{
		m_curve.Play(start_, end_, timeSec_, type_, loopMode_);
	}


	bool IsPlay() override
	{
		return m_curve.IsPlaying();
	}


	void SetParameter(Vector3 start, Vector3 end, float timeSec, EasingType type, LoopMode loopMode)
	{
		start_ = start;
		end_ = end;
		timeSec_ = timeSec;
		type_ = type;
		loopMode_ = loopMode;
		m_curve.Play(start, end, timeSec, type, loopMode);
	}


	Vector3 GetCurrentValue()
	{
		return m_curve.GetCurrentValue();
	}


	void SetFunc(const UIAnimationApplyFunc<Vector3>& func)
	{
		applyFunc_ = func;
	}
};


class UIVector4Animation : public UIAnimationBase
{
protected:
	Vector4Curve m_curve;
	/** カーブ用のパラメーター */
	Vector4 start_ = Vector4::White;
	Vector4 end_ = Vector4::White;
	float timeSec_ = 0.0f;
	EasingType type_ = EasingType::Linear;
	LoopMode loopMode_ = LoopMode::Once;

	UIAnimationApplyFunc<Vector4> applyFunc_;


public:
	UIVector4Animation() {}
	~UIVector4Animation() {}

	void Update() override
	{
		m_curve.Update(g_gameTime->GetFrameDeltaTime());
		applyFunc_(m_curve.GetCurrentValue());
	}


	void Play() override
	{
		m_curve.Play(start_, end_, timeSec_, type_, loopMode_);
	}


	bool IsPlay() override
	{
		return m_curve.IsPlaying();
	}


	void SetParameter(Vector4 start, Vector4 end, float timeSec, EasingType type, LoopMode loopMode)
	{
		start_ = start;
		end_ = end;
		timeSec_ = timeSec;
		type_ = type;
		loopMode_ = loopMode;
		m_curve.Play(start, end, timeSec, type, loopMode);
	}


	Vector4 GetCurrentValue()
	{
		return m_curve.GetCurrentValue();
	}


	void SetFunc(const UIAnimationApplyFunc<Vector4>& func)
	{
		applyFunc_ = func;
	}
};

/* 色変更アニメーション */
class UIColorAnimation : public UIVector4Animation
{
public:
	UIColorAnimation();
	~UIColorAnimation() {}

	void Update() override
	{
		m_curve.Update(g_gameTime->GetFrameDeltaTime());
		applyFunc_(m_curve.GetCurrentValue());
	}
};

/* 大きさ変更アニメーション */
class UIScaleAnimation : public UIVector3Animation
{
public:
	UIScaleAnimation();
	~UIScaleAnimation() {}

	void Update() override 
	{
		m_curve.Update(g_gameTime->GetFrameDeltaTime());
		applyFunc_(m_curve.GetCurrentValue());
	}
};


//
//
//class UIQuaternionAnimation : public UIAnimationBase
//{
//private:
//	QuaternionCurve m_curve;
//	/** カーブ用のパラメーター */
//	Quaternion start_ = Quaternion::Identity;
//	Quaternion end_ = Quaternion::Identity;
//	float timeSec_ = 0.0f;
//	EasingType type_ = EasingType::Linear;
//	LoopMode loopMode_ = LoopMode::Once;
//
//
//public:
//	UIQuaternionAnimation() {}
//	~UIQuaternionAnimation() {}
//
//	void Update() override
//	{
//		m_curve.Update(g_gameTime->GetFrameDeltaTime());
//	}
//
//
//	void Play() override
//	{
//		m_curve.Play(start_, end_, timeSec_, type_, loopMode_);
//	}
//
//
//	bool IsPlay() override
//	{
//		return m_curve.IsPlaying();
//	}
//
//
//	void SetParameter(Quaternion start, Quaternion end, float timeSec, EasingType type, LoopMode loopMode)
//	{
//		m_curve.Play(start, end, timeSec, type, loopMode);
//	}
//
//
//	Quaternion GetCurrentValue()
//	{
//		return m_curve.GetCurrentValue();
//	}
//};