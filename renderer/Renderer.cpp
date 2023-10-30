#include "Renderer.hpp"
#include "VulkanRenderer.hpp"
#include <cstdlib>

using namespace renderer;

Renderer::Renderer(){
    _Context = nullptr;
}

Renderer::~Renderer(){

}

bool Renderer::Init(){
    _Context = new VkContext();
    if (!_Context->InitContext()){
        DEBUG("Create Context Failed.");
        return false;
    }

    _Renderer = new VulkanRenderer();
    _Renderer->CreateInstance();

    return true;
}

void Renderer::Draw(){
    _Context->Draw();
}
