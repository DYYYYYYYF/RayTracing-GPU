#include "Renderer.hpp"
#include <cstdlib>

using namespace renderer;

Renderer::Renderer(){
    _RendererImpl = new VulkanRenderer();
    if (_RendererImpl == nullptr) {
        FATAL("Create Renderer failed.");
    }
}

Renderer::~Renderer(){
    _RendererImpl = nullptr;
}

bool Renderer::Init(){

    _RendererImpl->Init();

    std::string prePath = "../";
    if (_ConfigFile != nullptr) {
        prePath = _ConfigFile->GetVal("PrePath");
    }

    // Models
    LoadMesh((prePath + std::string("asset/model/room.obj")).data(), "room");
    LoadMesh((prePath + std::string("asset/model/ball.obj")).data(), "ball");
    LoadMesh((prePath + std::string("asset/model/CornellBox.obj")).data(), "CornellBox");
    LoadMesh((prePath + std::string("asset/model/sponza.obj")).data(), "sponza");
    LoadMesh((prePath + std::string("asset/model/bunny.obj")).data(), "Bunny");
    LoadMesh((prePath + std::string("asset/obj/wooden_boat/Boat.obj")).data(), "Boat");
    LoadTriangleMesh();
    LoadRectangleMesh();

    // Textures
    LoadTexture("room", (prePath + std::string("asset/texture/room.png")).data());
    LoadTexture("wooden", (prePath + std::string("asset/obj/wooden_boat/BaseColor.png")).data());

    // Materials
    std::string defaultVertShader = (prePath + std::string("shader/glsl/default_vert.spv")).data();
    std::string defaultFragShader = (prePath + std::string("shader/glsl/default_frag.spv")).data();
    std::string meshVertShader = (prePath + std::string("shader/glsl/texture_mesh_vert.spv")).data();
    std::string meshFragShader = (prePath + std::string("shader/glsl/texture_mesh_frag.spv")).data();
    std::string meshFloorVertShader = (prePath + std::string("shader/glsl/mesh_grid_vert.spv")).data();
    std::string meshFloorFragShader = (prePath + std::string("shader/glsl/mesh_grid_frag.spv")).data();

    ((VulkanRenderer*)_RendererImpl)->UseTextureSet(false);
    Material deafaultMaterial;
    CreatePipeline(deafaultMaterial, defaultVertShader.data(), defaultFragShader.data());
    AddMaterial("Default", deafaultMaterial);

    ((VulkanRenderer*)_RendererImpl)->UseTextureSet(true);
    Material deafaultMeshMaterial;
    CreatePipeline(deafaultMeshMaterial, meshVertShader.data(), meshFragShader.data());
    AddMaterial("Texture", deafaultMeshMaterial);

    Material deafaultFloorMaterial;
    CreatePipeline(deafaultFloorMaterial, meshFloorVertShader.data(), meshFloorFragShader.data(), true);
    AddMaterial("Floor", deafaultFloorMaterial);

    Material drawLineMaterial;
    CreateDrawlinePipeline(drawLineMaterial, defaultVertShader.data(), defaultFragShader.data());
    AddMaterial("DrawLine", drawLineMaterial);

    ((VulkanRenderer*)_RendererImpl)->BindTextureDescriptor(GetMaterial("Texture"), GetTexture("room"));

    return true;
}

void Renderer::CreatePipeline(Material& mat, const char* vert_shader, const char* frag_shader, bool alpha) {
    _RendererImpl->CreatePipeline(mat, vert_shader, frag_shader, alpha);
}

void Renderer::BeforeDraw(){
}

void Renderer::Draw(RenderObject* first, int count){
    _RendererImpl->DrawPerFrame(first, count);
}

void Renderer::AfterDraw(){

}

void Renderer::LoadMesh(const char* filename, const char* mesh_name) {
    Mesh tempMesh;
    tempMesh.LoadFromObj(filename);
    _RendererImpl->UpLoadMeshes(tempMesh);
    AddMesh(mesh_name, tempMesh);
}

void Renderer::LoadMesh(const char* filename, Mesh& mesh) {
    _RendererImpl->UpLoadMeshes(mesh);
    AddMesh(filename, mesh);
}

void Renderer::LoadTexture(const char* filename, const char* texture_path) {
    Texture texture;

    renderer::LoadImageFromFile(*((VulkanRenderer*)_RendererImpl), texture_path, texture.image);
    ASSERT(texture.image.image)

    texture.imageView = ((VulkanRenderer*)_RendererImpl)->CreateImageView(vk::Format::eR8G8B8A8Srgb, texture.image.image, vk::ImageAspectFlagBits::eColor);
    AddTexture(filename, texture);
    INFO("Load Texture %s", filename);
}

void Renderer::UpdateViewMat(glm::mat4 view_matrix){
    ((VulkanRenderer*)_RendererImpl)->UpdatePushConstants(view_matrix);
    ((VulkanRenderer*)_RendererImpl)->UpdateUniformBuffer();
    ((VulkanRenderer*)_RendererImpl)->UpdateDynamicBuffer();
}

void Renderer::Release() {
    if (_RendererImpl != nullptr) {
        ReleaseMeshes();
        ReleaseMaterials();
        ReleaseTextures();
        _RendererImpl->Release();
        free(_RendererImpl);
    }
}

Material* Renderer::GetMaterial(const std::string& name) {
    //search for the object, and return nullptr if not found
    auto it = _Materials.find(name);
    if (it == _Materials.end()) {
        return nullptr;
    }
    else {
        return &(*it).second;
    }
}

Mesh* Renderer::GetMesh(const std::string& name) {
    auto it = _Meshes.find(name);
    if (it == _Meshes.end()) {
        return nullptr;
    }
    else {
        return &(*it).second;
    }
}

Texture* Renderer::GetTexture(const std::string& name) {
    auto it = _Textures.find(name);
    if (it == _Textures.end()) {
        return nullptr;
    }
    else {
        return &(*it).second;
    }
}

void Renderer::LoadTriangleMesh() {

	Mesh triganleMesh;
	//make the array 3 vertices long
	triganleMesh.vertices.resize(3);

	//vertex positions
	triganleMesh.vertices[0].position = { 1.f, 2.f, 0.0f };
	triganleMesh.vertices[1].position = { -1.f,2.f, 0.0f };
	triganleMesh.vertices[2].position = { 0.f, 1.f, 0.0f };

	//vertex colors
	triganleMesh.vertices[0].color = { 1.f, 0.f, 0.0f };
	triganleMesh.vertices[1].color = { 0.f, 1.f, 0.0f };
	triganleMesh.vertices[2].color = { 0.f, 0.f, 1.0f };

	// Normal
	triganleMesh.vertices[0].normal = { 0.0f, 1.0f, 0.0f };
	triganleMesh.vertices[1].normal = { 0.0f, 1.0f, 0.0f };
	triganleMesh.vertices[2].normal = { 0.0f, 1.0f, 0.0f };

	// UV
	triganleMesh.vertices[0].texCoord = { 0.0f, 0.0f };
	triganleMesh.vertices[1].texCoord = { 1.0f, 0.0f };
	triganleMesh.vertices[2].texCoord = { 0.5f, 1.0f };

	//indices
	triganleMesh.indices.resize(3);
	triganleMesh.indices[0] = 0;
	triganleMesh.indices[1] = 1;
	triganleMesh.indices[2] = 2;

	LoadMesh("Triangle", triganleMesh);

	INFO("Loaded Triangle");
}

void Renderer::LoadRectangleMesh() {
	Mesh rectangleMesh;
	//make the array 6 vertices long
	rectangleMesh.vertices.resize(4);

	//vertex positions
	rectangleMesh.vertices[0].position = { 50.f, 0.f,  -50.f };	//?Һ?
	rectangleMesh.vertices[1].position = { 50.f, 0.f,   50.f };	//??ǰ
	rectangleMesh.vertices[2].position = { -50.f, 0.f,   50.f };	//??ǰ
	rectangleMesh.vertices[3].position = { -50.f, 0.f,  -50.f };	//????

	//vertex colors, all green
	rectangleMesh.vertices[0].color = { 0.9f, 0.9f, 0.9f };
	rectangleMesh.vertices[1].color = { 0.1f, 0.1f, 0.1f };
	rectangleMesh.vertices[2].color = { 0.9f, 0.9f, 0.9f };
	rectangleMesh.vertices[3].color = { 0.1f, 0.1f, 0.1f };

	// Normal
	rectangleMesh.vertices[0].normal = { 0.0f, 1.0f, 0.0f };
	rectangleMesh.vertices[1].normal = { 0.0f, 1.0f, 0.0f };
	rectangleMesh.vertices[2].normal = { 0.0f, 1.0f, 0.0f };
	rectangleMesh.vertices[3].normal = { 0.0f, 1.0f, 0.0f };

	// UV
	rectangleMesh.vertices[0].texCoord = { 0.0f, 1.0f };
	rectangleMesh.vertices[1].texCoord = { 1.0f, 1.0f };
	rectangleMesh.vertices[2].texCoord = { 1.0f, 0.0f };
	rectangleMesh.vertices[3].texCoord = { 0.0f, 0.0f };

	//indices
	rectangleMesh.indices.resize(6);
	rectangleMesh.indices[0] = 0;
	rectangleMesh.indices[1] = 1;
	rectangleMesh.indices[2] = 2;
	rectangleMesh.indices[3] = 0;
	rectangleMesh.indices[4] = 2;
	rectangleMesh.indices[5] = 3;

	LoadMesh("Rectangle",  rectangleMesh);

	INFO("Loaded Rectangle");
}
