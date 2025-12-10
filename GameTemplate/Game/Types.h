#pragma once

/**
 * @brief ゲームオブジェクトの種類を表す列挙型を含む構造体。
 */
struct GameObjectType
{
	enum Enum
	{
		Sphere,
		AttachableObject,
	};
};

/**
 * @brief UIの種類
 */
struct GameUIType
{
	enum Enum
	{
		SphereSizeText,
		ObjectView,
	};
};


/**
 * @brief Updateを実行する順番
 */
struct GameObjectPriority
{
	enum Enum
	{
		Default,
		InGameManager,
		Collision,
		InGameManagerLate,
		UI
	};
};


constexpr float MAX_SPRITE_WIDTH = 1920.0f;
constexpr float MAX_SPRITE_HIGHT = 1080.0f;