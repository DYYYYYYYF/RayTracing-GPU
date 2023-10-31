#include "Renderer.hpp"
#include "VulkanRenderer.hpp"
#include "renderer/interface/IRendererImpl.hpp"
#include <cstdlib>

using namespace renderer;

Renderer::Renderer(){
    _Context = nullptr;
    _Renderer = nullptr;
}

Renderer::~Renderer(){
    if (_Renderer != nullptr){
        _Renderer->Release();
        _Renderer = nullptr;
    }
}

bool Renderer::Init(){
    // _Context = new VkContext();
    // if (!_Context->InitContext()){
    //     DEBUG("Create Context Failed.");
    //     return false;
    // }

    _Renderer = new VulkanRenderer();
    if (_Renderer == nullptr) {
        FATAL("Create Renderer failed.");
        return false;
    }

    _Renderer->Init();

    return true;
}

void Renderer::Draw(){
    // _Context->Draw();
}
