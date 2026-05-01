/**
 * StageCullingSystem.h
 * ステージカリングのシステム
 */
#pragma once
class AttachableObject;

class StageCullingSystem
{
public:
	StageCullingSystem();
	~StageCullingSystem();
	
	void Update(std::vector<AttachableObject*> objects);
};