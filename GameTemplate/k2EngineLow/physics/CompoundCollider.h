/*!
 * @brief	�J�v�Z���R���C�_�[�B
 */

#pragma once

#include "ICollider.h"

namespace nsK2EngineLow {
	class CCompoundCollider : public ICollider
	{
	public:
		/// <summary>
		/// ������
		/// </summary>
		/// <param name="radius"></param>
		void Init(float radius)
		{
			m_shape = std::make_unique<btCompoundShape>();
			m_radius = radius;
		}
		/// <summary>
		/// BulletPhysics�̃R���W�����`����擾�B
		/// </summary>
		/// <returns></returns>
		btCompoundShape* GetBody() const override
		{
			return m_shape.get();
		}
		/// <summary>
		/// ���a���擾�B
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