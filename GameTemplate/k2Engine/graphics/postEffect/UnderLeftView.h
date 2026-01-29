#pragma once
namespace nsK2Engine {
	class ModelRender;

	class UnderLeftView
	{
	public:
		UnderLeftView();
		~UnderLeftView();

		void Init();
		void Render(RenderContext& rc, RenderTarget& mainRenderTarget);
		void SetModel(ModelRender& model, float size = 1);
		void InitModel(std::string filePaht, float size = 1);

		int count;
		bool isDraw;
		RenderTarget rt;
		Sprite sprite;
		Sprite backSprite;
		Model* m_model;
		Camera camera;
		Quaternion rotation = Quaternion::Identity;

		std::map<std::string, Model*> m_modelList;

	};

}