#pragma once

#include "Hazel/Core.h"

namespace Hazel
{
	class Texture
	{
	public:
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual void Bind() const = 0;

	private:
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);
	};
}
