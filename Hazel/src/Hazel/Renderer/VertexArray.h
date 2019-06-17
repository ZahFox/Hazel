#pragma once

#include <memory>
#include "Hazel/Renderer/Buffer.h"

namespace Hazel {
	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& vertexBuffer) = 0;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& const GetVertexBuffers() = 0;
		virtual const std::shared_ptr<IndexBuffer>& const GetIndexBuffer() = 0;

		static VertexArray* Create();
	};
}
