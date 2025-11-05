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
		Defulat,
		InGameManager,
		Collision,
		InGameManagerLate,
		UI
	};
};