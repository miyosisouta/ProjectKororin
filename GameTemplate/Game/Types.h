#pragma once


struct GameObjectType
{
	enum Enum
	{
		Sphere,
		AttachableObject,
	};
};


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