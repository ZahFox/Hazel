#include "hzpch.h"
#include "Renderer.h"

namespace Hazel {

	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;

	const std::string viewUniformName = "u_ViewProjection";
	const std::string transformUniformName = "u_Transform";

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->UploadUniformMat4(viewUniformName, s_SceneData->ViewProjectionMatrix);
		shader->UploadUniformMat4(transformUniformName, transform);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}
