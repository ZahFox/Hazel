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

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& vertexBuffer) = 0;

		virtual const std::vector<Ref<VertexBuffer>>& const GetVertexBuffers() = 0;
		virtual const Ref<IndexBuffer>& const GetIndexBuffer() = 0;

		static VertexArray* Create();
	};
}
