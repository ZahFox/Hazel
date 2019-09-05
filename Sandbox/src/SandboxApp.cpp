#include <Hazel.h>
#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_SquarePosition(1.0f)
	{
		m_VertexArray.reset(Hazel::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f,  0.0f,
			 0.8f,  0.2f,  0.8f,
			 1.0f,  0.5f, -0.5f,
			 0.0f,  0.2f,  0.3f,
			 0.8f,  1.0f,  0.0f,
			 0.5f,  0.0f,  0.8f,
			 0.8f,  0.2f,  1.0f,
		};

		Hazel::Ref<Hazel::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));
		vertexBuffer->SetLayout(
			{
				{Hazel::ShaderDataType::Float3, "a_Position"},
				{Hazel::ShaderDataType::Float4, "a_Color"},
			}
		);

		m_VertexArray->AddVertexBuffer(vertexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };
		Hazel::Ref<Hazel::IndexBuffer> indexBuffer;
		indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};

		m_SquareVA.reset(Hazel::VertexArray::Create());
		Hazel::Ref<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->SetLayout(
			{
				{Hazel::ShaderDataType::Float3, "a_Position"},
				{Hazel::ShaderDataType::Float2, "a_TexCoord"},
			}
		);
		m_SquareVA->AddVertexBuffer(squareVB);
		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Hazel::Ref<Hazel::IndexBuffer> squareIB;
		squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		const std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
			  v_Position = a_Position;
			  v_Color = a_Color;
			  gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		const std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
			  color = vec4(v_Position * 0.5 + 0.5, 1.0);
			  color = v_Color;
			}
		)";

		m_Shader.reset(Hazel::Shader::Create(vertexSrc, fragmentSrc));

		const std::string flatShaderVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
			  v_Position = a_Position;
			  gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		const std::string flatColorShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			uniform vec3 u_Color;

			void main()
			{
			  color = vec4(u_Color, 1.0);
			}
		)";

		m_FlatColorShader.reset(Hazel::Shader::Create(flatShaderVertexSrc, flatColorShaderFragmentSrc));
		m_TextureShader.reset(Hazel::Shader::Create("assets/shaders/Texture.glsl"));

		m_Texture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogoTexture = Hazel::Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Hazel::Timestep ts) override
	{
		const float moveDiff = m_CameraMoveSpeed * ts;
		const float rotDiff = m_CameraRotationSpeed * ts;
		const float squareMoveDiff = m_SquareMoveSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
		{
			m_CameraPosition.x -= moveDiff;
		}
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
		{
			m_CameraPosition.x += moveDiff;
		}
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
		{
			m_CameraPosition.y += moveDiff;
		}
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
		{
			m_CameraPosition.y -= moveDiff;
		}

		if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
		{
			m_CameraRotation += rotDiff;
		}
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
		{
			m_CameraRotation -= rotDiff;
		}

		if (Hazel::Input::IsKeyPressed(HZ_KEY_J))
		{
			m_SquarePosition.x -= squareMoveDiff;
		}
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_L))
		{
			m_SquarePosition.x += squareMoveDiff;
		}
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_I))
		{
			m_SquarePosition.y += squareMoveDiff;
		}
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_K))
		{
			m_SquarePosition.y -= squareMoveDiff;
		}

		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Hazel::Renderer::BeginScene(m_Camera);

		const glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				glm::vec3 pos(i*0.11f + m_SquarePosition.x, j*0.11f + m_SquarePosition.y, 0.0f);
				const glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Hazel::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}

		m_Texture->Bind();
		Hazel::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_ChernoLogoTexture->Bind();
		Hazel::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Triangle
		// Hazel::Renderer::Submit(m_Shader, m_VertexArray);
		Hazel::Renderer::EndScene();

	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Hazel::Event& event) override
	{
		Hazel::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(Hazel::KeyPressedEvent& event)
	{
		return false;
	}

private:
	Hazel::Ref<Hazel::Shader> m_Shader;
	Hazel::Ref<Hazel::VertexArray> m_VertexArray;

	Hazel::Ref<Hazel::Shader> m_FlatColorShader, m_TextureShader;
	Hazel::Ref<Hazel::VertexArray> m_SquareVA;

	Hazel::Ref<Hazel::Texture2D> m_Texture, m_ChernoLogoTexture;

	Hazel::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0.0f;
	float m_CameraMoveSpeed = 5.0f;
	float m_CameraRotationSpeed = 180.0f;

	glm::vec3 m_SquarePosition;
	float m_SquareMoveSpeed = 1.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{
	}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}
