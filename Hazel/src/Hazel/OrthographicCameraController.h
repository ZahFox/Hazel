#pragma once

#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Core/Timestep.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel
{

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera GetCamera() { return m_Camera; }
		const OrthographicCamera GetCamera() const { return m_Camera; }
	private:
		glm::vec3 m_CameraPosition{ 0.0f, 0.0f, 0.0f };

		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		bool m_Rotation = false;

		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 5.0f;
		float m_CameraRotationSpeed = 180.0f;
		float m_ZoomLevel = 1.0f;

		OrthographicCamera m_Camera;
		float m_AspectRatio;

	};

}
