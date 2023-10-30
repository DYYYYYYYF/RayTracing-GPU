#pragma once

#include "IRendererImpl.hpp"

namespace renderer {
	class IRenderer {
	public:
		virtual ~IRenderer() {};
		virtual bool Init() = 0;
		virtual void Draw() = 0;

  protected:
    IRendererImpl* _Renderer = nullptr;

	};
}
