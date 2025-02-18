#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <vector>

 void loadModel(const std::string& path) {
    Assimp::Importer importer;

    // ����ģ�Ͳ�������ص��������ʵ�
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipWindingOrder | aiProcess_GenSmoothNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    // Assimp ���Զ����� .mtl �ļ�
    // �����ͨ�� scene->mMaterials ��ȡ��������
    for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
        aiMaterial* material = scene->mMaterials[i];

        aiColor3D ambient, diffuse, specular;
        material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
        material->Get(AI_MATKEY_COLOR_SPECULAR, specular);

        std::cout << "Ambient: " << ambient.r << ", " << ambient.g << ", " << ambient.b << std::endl;
        std::cout << "Diffuse: " << diffuse.r << ", " << diffuse.g << ", " << diffuse.b << std::endl;
        std::cout << "Specular: " << specular.r << ", " << specular.g << ", " << specular.b << std::endl;

        // ��ȡ����·��
        aiString texturePath;
        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS) {
            std::cout << "Diffuse texture: " << texturePath.C_Str() << std::endl;
        }
    }
}
 /*
int main() {
    loadModel("res/PBRModels/student-chair/student-chair.obj");  // Assimp ���Զ����ع����� .mtl �ļ�
    return 0;
}
*/