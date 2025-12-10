#include "stdafx.h"
#include "InputDetection.h"

void InputDetection::UpdateTriggerState()
{
	// ‘OƒtƒŒ[ƒ€‚Ìó‘Ô‚ð•Û‘¶
	isOldTiggerButtonA_ = isCurrentTriggerButtonA_;

	// Œ»Ý‰Ÿ‚µ‚Ä‚¢‚é‚©‚Ç‚¤‚©
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		isCurrentTriggerButtonA_ = true;
	}
	else 
	{
		isCurrentTriggerButtonA_ = false;
	}
}


bool InputDetection::IsTriggerButtonA()
{
	return (isCurrentTriggerButtonA_ && !isOldTiggerButtonA_);
}
