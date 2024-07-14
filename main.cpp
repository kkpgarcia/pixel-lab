#include "src/PixelLab/EditorApplication.h"

Application* application;

int main()
{
	application = new EditorApplication();
	application->Init();
	application->Run();
	application->Shutdown();
	delete application;

	// Assimp::Importer importer;
	// const aiScene* scene = importer.ReadFile("assets/tests/cube.glb", aiProcess_Triangulate | aiProcess_FlipUVs);
	//
	// if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
	// 	std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
	// 	return 1;
	// }
	//
	// std::cout << "Number of meshes: " << scene->mRootNode->mNumMeshes << std::endl;

	// ModelImporter importer;
	// Model* model = static_cast<Model*>(importer.Import("assets/tests/box.obj"));
	//
	// std::cout << "Model: " << model->GetMeshes().size() << std::endl;

	return 0;
}