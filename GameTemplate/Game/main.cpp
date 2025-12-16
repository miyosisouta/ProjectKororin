#include "stdafx.h"
#include "system/system.h"

#include<InitGUID.h>
#include<dxgidebug.h>

#include "Core/Fade.h"
#include "Effect/GameEffectManager.h"
#include "Scene/SceneManager.h"


void ReportLiveObjects()
{
	IDXGIDebug* pDxgiDebug;

	typedef HRESULT(__stdcall* fPtr)(const IID&, void**);
	HMODULE hDll = GetModuleHandleW(L"dxgidebug.dll");
	fPtr DXGIGetDebugInterface = (fPtr)GetProcAddress(hDll, "DXGIGetDebugInterface");

	DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&pDxgiDebug);

	// 出力。
	pDxgiDebug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_DETAIL);
}

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));
	//////////////////////////////////////
	// ここから初期化を行うコードを記述する。
	//////////////////////////////////////

	// レイトレーシングを使用しないようにする
	g_renderingEngine->DisableRaytracing();
	g_renderingEngine->DisableTonemap();

	// GameManagerクラスのインスタンスを作成
	auto* SceneManager = NewGO<SceneManagerObject>(0, "SceneManagerObject");
	// fadeクラスのインスタンすを作成
	auto* fadeObject = NewGO<FadeObject>(0, "fadeObject");
	// Effectクラスのインスタンスを作成
	auto* effectObject = NewGO<GameEffectObject>(0, "gameEffectObject");
	// スカイキューブ
	auto* skyCube = NewGO<SkyCube>(0, "skyCube");
	skyCube->SetType(enSkyCubeType_DayToon);
	skyCube->SetScale(150.0f);

	ParameterManager::CreateInstance();


	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	
	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		//  GameManagerのUpdate処理を最初に実行 
		if (SceneManager::IsAvailable()) 
		{
			SceneManager::Get().Update();
		}

		//if (g_pad[0]->IsTrigger(enButtonA) ){
		//	g_pad[0]->SetVibration(/*durationSec=*/0.5f, /*normalizedPower=*/1.0f);
		//}
		K2Engine::GetInstance()->Execute();
	}

	DeleteGO(SceneManager);
	DeleteGO(fadeObject);
	DeleteGO(effectObject);
	DeleteGO(skyCube);

	ParameterManager::DestroyInstance();

	K2Engine::DeleteInstance();

#ifdef _DEBUG
	ReportLiveObjects();
#endif // _DEBUG
	return 0;
}

