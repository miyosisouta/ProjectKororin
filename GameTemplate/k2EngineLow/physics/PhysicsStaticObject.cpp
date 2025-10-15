/*!
* @brief	�ÓI�I�u�W�F�N�g�B
*/


#include "k2EngineLowPreCompile.h"
#include "physics/PhysicsStaticObject.h"

namespace nsK2EngineLow {
	PhysicsStaticObject::PhysicsStaticObject()
	{
	}
	PhysicsStaticObject::~PhysicsStaticObject()
	{
	}
	void PhysicsStaticObject::CreateFromModel(Model& model, const Matrix& worldMatrix, const float restitution)
	{
		m_meshCollider.CreateFromModel(model, worldMatrix);
		RigidBodyInitData rbInfo;
		rbInfo.collider = &m_meshCollider;
		rbInfo.mass = 0.0f;
		rbInfo.restitution = restitution;
		m_rigidBody.Init(rbInfo);

	}

	void PhysicsStaticObject::CreateBoxCollider(const Vector3& pos, const Vector3& size, const Matrix& worldMatrix, const float restitution)
	{
		m_boxCollider.Create(size);
		RigidBodyInitData rbInfo;
		rbInfo.pos = pos;
		rbInfo.collider = &m_boxCollider;
		rbInfo.mass = 0.0f;
		rbInfo.restitution = restitution;
		m_rigidBody.Init(rbInfo);
	}
	
}
