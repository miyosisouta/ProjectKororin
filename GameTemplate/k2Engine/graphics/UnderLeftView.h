#pragma once
namespace nsK2Engine {
	class UnderLeftView
	{
	public:
		UnderLeftView();
		~UnderLeftView();

		void Init();
		void MRender(RenderContext& rc, RenderTarget& mainRenderTarget);
		void SetModel(ModelRender& model, float size = 1);
		void SRender(RenderContext& rc);
		void InitModel(std::string filePaht, float size = 1);

		int count;
		bool isDraw;
		RenderTarget rt;
		Sprite sprite;
		Model* m_model;
		Camera camera;
		Quaternion rotation = Quaternion::Identity;

		std::map<std::string, Model*> m_modelList;

	};

}