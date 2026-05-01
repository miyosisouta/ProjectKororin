/**
 * StageCullingSystem.cpp
 *
 * ステージカリングのシステム
 */


#include "stdafx.h"
#include "StageCullingSystem.h"
#include "BoundingVolume.h"
#include "Actor/Object/AttachableObject.h"


StageCullingSystem::StageCullingSystem()
{
}


StageCullingSystem::~StageCullingSystem()
{
}


void StageCullingSystem::Update(std::vector<AttachableObject*> objects)
{
	if (objects.empty()) { return; }

	// 毎フレームフラスタムを構築する
	Frustum frustum;
	frustum.BuildFromViewProjectionMatrix(g_camera3D->GetViewProjectionMatrix());

	for (auto* object : objects) {

		if (!object->IsStarted()) {
			object->SetVisible(false); // 未初期化なので非表示のまま
			continue;
		}

		// モデルの頂点から AABB を計算
		Bounds bounds;
		bounds.Compute(object->GetModelRender().GetModel());

		bounds.maxPoint += object->GetTransform()->m_position;
		bounds.minPoint += object->GetTransform()->m_position;

		// フラスタムカリングによる描画結果はすべてSetVisible()へ
		object->SetVisible(frustum.IsVisible(bounds));
	}
}



