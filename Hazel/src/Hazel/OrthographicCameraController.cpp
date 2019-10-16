#include "hzpch.h"
#include "OrthographicCameraController.h"
#include "Hazel/KeyCodes.h"
#include "Hazel/Input.h"

namespace Hazel
{

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_Rotation(rotation),
		m_Camera(-aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		m_AspectRatio(aspectRatio)
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		const float moveDiff = m_CameraTranslationSpeed * ts;

		if (Input::IsKeyPressed(HZ_KEY_A))
		{
			m_CameraPosition.x -= moveDiff;
		}
		else if (Input::IsKeyPressed(HZ_KEY_D))
		{
			m_CameraPosition.x += moveDiff;
		}
		else if (Input::IsKeyPressed(HZ_KEY_W))
		{
			m_CameraPosition.y += moveDiff;
		}
		else if (Input::IsKeyPressed(HZ_KEY_S))
		{
			m_CameraPosition.y -= moveDiff;
		}

		if (m_Rotation)
		{
			const float rotDiff = m_CameraRotationSpeed * ts;
			if (Input::IsKeyPressed(HZ_KEY_Q))
			{
				m_CameraRotation += rotDiff;
			}
			else if (Input::IsKeyPressed(HZ_KEY_E))
			{
				m_CameraRotation -= rotDiff;
			}

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.5f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}
