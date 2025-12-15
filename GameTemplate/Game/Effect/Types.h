#pragma once
#include<string>
/**
 * Types.h
 * エフェクトの定数や情報をまとめるクラス
 */

/** エフェクトの種類 */
enum EffectKinds 
{
	enEffectKind_Hit_Reject,	//!< 吸着不可能なオブジェクトにぶつかったときの反動エフェクト
	enEffectKind_Dash_Wind,		//!< ダッシュ時の風エフェクト
	enEffectKind_Max
};


/** エフェクトの情報を定義 */
struct EffectInformation
{
	const char16_t* assetPath; //!< アセットパス
	//
	EffectInformation(const char16_t* path) : assetPath(path) {} //!< EffectInformation effectInfoにてコンストラクタを呼び、アセットパスを設定 
};


/** エフェクトの情報を登録 */
static EffectInformation effectInfo[enEffectKind_Max] =
{
	EffectInformation(u"Assets/Effect/Hit_Reject.efk"),
	EffectInformation(u"Assets/Effect/Dash_Wind.efk"),

};