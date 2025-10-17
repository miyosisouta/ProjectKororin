/*!
 * @brief	カプセルコライダー。
 */

#pragma once

#include "ICollider.h"

namespace nsK2EngineLow {
	class CCompoundCollider : public ICollider
	{
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="radius"></param>
		void Init(float radius)
		{
			m_shape = std::make_unique<btCompoundShape>();
			m_radius = radius;
		}
		/// <summary>
		/// BulletPhysicsのコリジョン形状を取得。
		/// </summary>
		/// <returns></returns>
		btCompoundShape* GetBody() const override
		{
			return m_shape.get();
		}
		/// <summary>
		/// 半径を取得。
		/// </summary>
		/// <returns></returns>
		float GetRadius() const
		{
			return m_radius;
		}
	private:
		std::unique_ptr<btCompoundShape>	m_shape;
		float m_radius = 0.0f;
	};
}