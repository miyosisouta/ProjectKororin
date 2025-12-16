#include "k2EnginePreCompile.h"
#include "UnderLeftView.h"

namespace {
	static const Vector3 DEFAULT_CAMERA_POSITION = { 0.0f,20.0f,50.0f };
}

namespace nsK2Engine {
	UnderLeftView::UnderLeftView()
	{
	}

	UnderLeftView::~UnderLeftView()
	{
	}

	void UnderLeftView::Init()
	{
		float color[4] = { 0.0f,0.0f,0.0f,0.0f };
		rt.Create(1920, 1080, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT, color);

		SpriteInitData spriteInitData;
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_vsEntryPointFunc = "VSMain";
		spriteInitData.m_psEntryPoinFunc = "PSMain";
		spriteInitData.m_width = 800;
		spriteInitData.m_height = 450;
		spriteInitData.m_textures[0] = &rt.GetRenderTargetTexture();
		spriteInitData.m_colorBufferFormat[0] = rt.GetColorBufferFormat();
		spriteInitData.m_alphaBlendMode = AlphaBlendMode_Trans;
		sprite.Init(spriteInitData);
		sprite.Update({ -1100,-600,0 }, Quaternion::Identity, Vector3::One, Vector2::Zero);

		camera.SetFar(1000.0f);
		camera.SetNear(1.0f);
		camera.SetHeight(rt.GetHeight());
		camera.SetWidth(rt.GetWidth());
		camera.SetPosition(DEFAULT_CAMERA_POSITION);
		camera.SetTarget(Vector3::Zero);
		camera.Update();

		rotation.AddRotationDegY(2);
	}

	void UnderLeftView::MRender(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		if (count > 0) {
			rc.WaitUntilToPossibleSetRenderTarget(rt);
			rc.SetRenderTargetAndViewport(rt);
			rc.ClearRenderTargetView(rt);
			m_model->Draw(rc,camera);
			rc.WaitUntilFinishDrawingToRenderTarget(rt);

			rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
			rc.SetRenderTargetAndViewport(mainRenderTarget);
			sprite.Draw(rc);
			rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

			camera.RotateOriginTarget(rotation);
			count--;
		}
	}

	void UnderLeftView::SetModel(ModelRender& model)
	{
		if (m_modelList.find(model.GetFilePath()) == m_modelList.end()) {
			InitModel(model.GetFilePath());
		}
		else
		{
			m_model = m_modelList[model.GetFilePath()];
		}

		camera.SetPosition(DEFAULT_CAMERA_POSITION);

		count = 300;
	}

	void UnderLeftView::InitModel(std::string filePath)
	{
		ModelInitData modelInitData;			//モデルのデータ
		//tkmのファイルパスの指定
		modelInitData.m_tkmFilePath = filePath.c_str();
		//シェーダーのファイルパスの指定
		modelInitData.m_fxFilePath = "Assets/shader/UnderLeftViewShader.fx";

		Model* model = new Model;
		model->Init(modelInitData);
		model->UpdateWorldMatrix(Vector3::Zero, Quaternion::Identity, Vector3::One);

		m_modelList.emplace(filePath, model);
		m_model = model;
	}

	void UnderLeftView::SRender(RenderContext& rc)
	{
		if (count > 0) {
			sprite.Draw(rc);
		}
	}


}