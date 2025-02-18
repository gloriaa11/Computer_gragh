<font style="color:#000000;"></font>

<font style="color:rgb(0,0,0);">该报告主要介绍作者在“基于 OpenGL 的教室寻宝游戏”中实现的部分技术，包括制作天空盒，场景渲染，物品渲染，网格细分等</font><font style="color:#000000;"> 。</font>

# <font style="color:#000000;">引言</font>
<font style="color:rgb(0,0,0);">本次期末大作业，我们小组选定的主题为“基于 OpenGL 的教室寻宝游戏”，项目的框架基于 Qt 的 OpenGL 模块。</font>

本游戏项目的核心目标是创造一个沉浸式的虚拟教室环境，玩家在其中可以通过探索和解谜，寻找隐藏的物品和线索，最终达成目标。这一过程中，游戏结合了 **OpenGL** 提供的强大图形渲染能力，通过细致的 **3D 建模**、**阴影映射**、**光照效果** 和 **动态纹理加载** 等技术，构建了一个富有层次感和真实感的游戏世界。每个场景、每个物体的细节dy都得到了精心设计和优化，力求为玩家提供一个流畅且富有挑战性的互动体验。

<font style="color:rgb(0,0,0);">游戏中的每一项技术实现都充分展示了 </font>**OpenGL**<font style="color:rgb(0,0,0);"> 在高性能渲染中的优势，尤其是在阴影计算、光照模拟和材质渲染方面的精细表现。在这一平台上，玩家不仅仅是简单地解谜，还能够体验到通过光影变化、环境互动和动态模型渲染所带来的视觉享受。通过对教室环境的精心构建和逻辑设计，玩家能够在探索的过程中体验到更多的创意和想象力，游戏的互动性和趣味性得到了有效的提升。</font>

# <font style="color:#000000;">个人工作</font>
在本项目中，我主要负责了 **基于 OpenGL 的教室寻宝游戏** 中的多个关键技术模块的实现，包括 **天空盒的制作**、**场景渲染**、**物品渲染**、**网格细分技术** 等内容。

#### **天空盒的制作**
在游戏的初期，我便着手实现了游戏世界的背景部分 —— **天空盒**。天空盒的制作是为了增强游戏的沉浸感，让玩家仿佛置身于一个完整的 3D 空间中。制作天空盒的挑战在于如何通过纹理映射技术，让玩家的视角能够在全方位看到一个无缝的天空背景。我通过加载六面纹理，并结合 OpenGL 中的 **立方体映射** 技术，成功地将这一效果实现。

#### **场景渲染与物品渲染**
场景的渲染部分是我在项目中最为核心的一部分。首先，我需要处理场景中所有物体的加载与渲染，包括 **墙壁、地板、门** 等元素。在这部分工作中，最具挑战性的部分是如何确保每个物体能够被正确地渲染，并且不发生性能瓶颈。为了实现这一目标，我采用了 **实例化渲染（Instancing）** 技术，通过共享几何数据来减少 GPU 的负担。由于物体的重复性较高（例如多个相同的桌子和墙壁），这项优化显著提高了渲染效率。

在物品渲染方面，我特别关注了材质和光照效果的实现。在初期，光照计算的准确性和物体表面的材质效果并不理想，尤其是在渲染墙面和地板时，表面缺乏细腻的光泽感。经过多次尝试和调整，我最终通过调整 **漫反射光照（Diffuse）** 和 **镜面反射光照（Specular）** 的参数，结合适当的 **光源位置** 和 **阴影效果**，实现了物体表面更加真实的渲染效果。

#### **网格细分技术**
在物品渲染中，为了让一些简单的模型（如桌子、椅子等）更加平滑且富有细节，我引入了 **网格细分（Subdivision）** 技术。最初我选择了 **Catmull-Clark Subdivision** 算法，这个算法能够将简单的网格细分成更加平滑的曲面，尤其适用于那些需要细腻曲面效果的模型。虽然这个算法在理论上非常有效，但在实现过程中，我遇到了 **性能瓶颈**。尤其是在处理复杂模型时，细分的网格数量急剧增加，导致渲染效率下降。

为了解决这个问题，我进行了大量的 **性能调试** 和优化，最终通过 **逐步细分** 和 **适当的 LOD（细节层次）技术**，减少了不必要的细分操作，确保了在保持高质量渲染的同时，不会对性能造成过大的影响。此外，在细分过程中，我还特别关注了如何优化 **法线平滑** 和 **曲面光照**，从而在渲染时获得更加自然的光照过渡。



# <font style="color:#000000;">技术实现</font>
## 天空盒（Skybox.cpp)
教室窗外的场景由于距离较远，无交互行为，且为了增强游戏沉浸感，降低渲染成本，我们采取了许多大型游戏中使用的技术——天空盒。用来模拟远处环境的技术。它通过将场景周围的视野包裹在一个立方体中，显示六个方向的图像：前、后、左、右、上、下。这种做法利用了立方体贴图，使得在三维环境中观察时，背景的边缘看不出接缝，呈现出真实的环境效果。

设计原理充分利用了计算机图形学中的纹理映射、深度测试、顶点缓冲区、着色器编程等技术，确保了天空盒能够在三维环境中自然地呈现，并与其他场景元素进行合理的交互。

+ 使用了 **立方体贴图**（Cubemap）技术，通过六个图像来创建一个完整的环境背景；
+ 通过 **深度测试** 确保天空盒在其他物体的后方渲染；
+ 使用 **OpenGL 的缓冲区和着色器** 来高效管理和渲染顶点数据及纹理；
+ 配置了 **纹理过滤和包裹模式** 来提高视觉效果，避免视觉接缝。

```cpp
void SkyBox::loadCubemap()
{
	GLCALL(glGenTextures(1, &m_RendererID));
	GLCALL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
}
```

## 网格mesh.cpp
**Mesh** 是所有模型的基础，它负责定义和存储物体的顶点数据、索引数据和纹理数据。首先，实现 **Mesh** 类，这样就可以通过它加载和渲染模型的几何数据。

**先从 Mesh 开始：** 网格定义了模型的 **几何形状**，其他所有的模块（如 `Model` 和 `ModelInstance`）都需要依赖它来获取模型的顶点和其他基本数据。

**设计创新与思考：**

```cpp
void Mesh::Mesh::Draw(Shader& shader,int instanceAmount)
void Mesh::Mesh::Draw(Shader& shader)
```

每个桌子和椅子的模型在 ** 3ds Max** 中是完全一样的，唯一不同的是它们的位置和一些变换。传统的渲染方法每个物体都会调用一次渲染命令。使用 `**glDrawElementsInstanced()**` 可以显著提高绘制多个相同物体时的性能，减少多次调用 `glDrawElements()` 需要的GPU开销。特别是在渲染大量相同物体时，实例化渲染会更加高效。  

```cpp
/**
 * 绘制网格
 * @param shader 用于渲染的着色器
 * @param instanceAmount 实例化绘制的数量
 */

void Mesh::Mesh::Draw(Shader& shader,int instanceAmount)
{
	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to string
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to string
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to string

		// now set the sampler to the correct texture unit
		//glUniform1i(glGetUniformLocation(shader.GetRendererID(), (name + number).c_str()), i);
		shader.Bind();
		shader.SetInt((name + number).c_str(), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	shader.Bind();
	shader.SetVec3("aAmbient", mat.Ka);
	shader.SetVec3("aDiffuse", mat.Kd);
	shader.SetVec3("aSpecular", mat.Ks);

	// draw mesh
	glBindVertexArray(VAO);
	GLCALL(glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0, instanceAmount));

	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	shader.Unbind();
	glActiveTexture(GL_TEXTURE0);
	//why not reset Bind?????
	glBindTexture(GL_TEXTURE_2D, 0);
}
```

### 网格细分原理推导和代码
![](https://cdn.nlark.com/yuque/0/2024/jpeg/40554294/1734596168433-3c5b5b53-f2c3-4011-86b6-da0476517abf.jpeg)



1. **顶点更新**：

![image](https://cdn.nlark.com/yuque/__latex/5974a7b476b14fb11c63b7e3d9c9f028.svg)

2. **边中点**：

![image](https://cdn.nlark.com/yuque/__latex/1d5cebc26a7be4fd49be6fda436b24a3.svg)

3. **面中心**：

![image](https://cdn.nlark.com/yuque/__latex/ae134d25c3e58735f77df445cb386058.svg)

```cpp
// Catmull-Clark Subdivision 示例实现
void Mesh::Subdivision() {
    std::vector<Vertex> newVertices;
    std::vector<unsigned int> newIndices;

    // 1. 计算每个四边形面的新顶点
    for (size_t i = 0; i < indices.size(); i += 4) {
        unsigned int idx0 = indices[i];
        unsigned int idx1 = indices[i + 1];
        unsigned int idx2 = indices[i + 2];
        unsigned int idx3 = indices[i + 3];

        // 计算四边形面的中点
        glm::vec3 mid01 = (vertices[idx0].Position + vertices[idx1].Position) / 2.0f;
        glm::vec3 mid12 = (vertices[idx1].Position + vertices[idx2].Position) / 2.0f;
        glm::vec3 mid23 = (vertices[idx2].Position + vertices[idx3].Position) / 2.0f;
        glm::vec3 mid30 = (vertices[idx3].Position + vertices[idx0].Position) / 2.0f;

        // 插入新顶点
        newVertices.push_back(Vertex{mid01});
        newVertices.push_back(Vertex{mid12});
        newVertices.push_back(Vertex{mid23});
        newVertices.push_back(Vertex{mid30});

        // 创建新的索引
        newIndices.push_back(newVertices.size() - 4);
        newIndices.push_back(newVertices.size() - 3);
        newIndices.push_back(newVertices.size() - 2);
        newIndices.push_back(newVertices.size() - 1);
    }

    // 2. 更新顶点和索引数据
    vertices = newVertices;
    indices = newIndices;

     // 3. 更新 VBO 和 IBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

```

调用方法

```plain
Mesh.Subdivision();
```

 通过在 **Mesh** 类中添加 **Subdivision** 函数，我们可以在渲染过程中动态地对网格进行细分，从而使模型更加平滑和细腻。  

## model.cpp
（  设计之初，我们思考过多种建模方法，也尝试过代码建模但是代码会非常繁琐且模型质量不高。 由于不确定导入obj是否可行，我们在开始之前先询问过老师，得到老师肯定的答案我们才确定这样做  ）

考虑到场景的复杂度和模型的质量，我们并没有通过opengl的代码直接建模，通过将 **3ds Max** 中设计的模型导入到 model.cpp部分， 将 `.obj` 文件通过 **Assimp**（一个开源的 3D 模型导入库）加载进来，并将其解析为 `**Model**` 对象  。

因此我们可以专注于 **OpenGL 渲染部分和游戏交互环节**，从而提高开发效率并保持代码的整洁。**3ds Max** 提供的强大建模功能使得可以创建复杂且富有细节的场景，确保模型的质量和表现力。

导入obj文件

```cpp

void Model::loadModel(std::string path)
{
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}
	// retrieve the directory path of the filepath
	directory = path.substr(0, path.find_last_of('/'));

	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene);
}
```

## modelinstance.cpp
  我们的场景中存在大量重复的物体，例如多个 **桌子** 和 **椅子**，这导致每一次重新渲染场景需要等待5分钟左右，效率极低，且不利于其他同学的调试。这些物体的数量庞大且完全相同。每个桌子和椅子的模型在 ** 3ds Max** 中是完全一样的，唯一不同的是它们的位置和一些变换。传统的渲染方法每个物体都会调用一次渲染命令。每次渲染一个相同的物体，都需要进行一次 **状态切换** 和 **数据传输**，这导致了大量不必要的计算。  

** 我们也考虑了多种可能的优化方法，如 合并网格**：

+ 然而合并网格会失去每个实例的独立性，修改位置或其他属性时需要重新进行合并，增加了管理的复杂性。

 在考虑了多种优化方案后，我们最终决定使用 **实例化渲染**（Instancing）技术。  

 实例化渲染的核心思想是：如果场景中存在大量相同的物体，可以将它们的几何数据共享，只需要传输一次 **顶点数据** 和 **纹理数据**。每个物体的位置、旋转和缩放信息通过不同的 **变换矩阵**（通常是通过 `**glDrawInstanced**` 调用）传递给 GPU，GPU 会根据这些变换矩阵自动对每个实例进行渲染。  

```cpp
/**
 * 为模型设置实例化渲染
 * @param instance 要设置实例化的模型
 */
void ModelInstances::SetUpInstancing(Model& instance)
{
    // 生成实例化顶点缓冲对象(VBO)
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    // 分配并初始化缓冲区对象的数据存储
    GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * model.size(), &model[0], GL_STATIC_DRAW));
    
    // 遍历模型的所有网格
    for (unsigned int i = 0; i < instance.meshes.size(); i++)
    {
        unsigned int VAO = instance.meshes[i].VAO;
        glBindVertexArray(VAO);
        
        // 设置矩阵的属性指针 (4个vec4)
        // 每个矩阵占用4个属性位置
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        // 设置属性除数，使得每个实例更新一次属性，而不是每个顶点
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        // 解绑VAO
        glBindVertexArray(0);
    }
}

```

# <font style="color:#000000;">效果</font>
天空盒的正确显示

![](https://cdn.nlark.com/yuque/0/2024/png/40554294/1734580664763-553c5891-1d68-4215-8c2d-2aca62a5d006.png?x-oss-process=image%2Fformat%2Cwebp%2Fresize%2Cw_1120%2Climit_0)

教室内物体正确摆放

![](https://cdn.nlark.com/yuque/0/2024/png/40554294/1734580635055-82042cb3-714a-4d42-95db-434d52e2ae23.png?x-oss-process=image%2Fformat%2Cwebp%2Fresize%2Cw_511%2Climit_0)

# <font style="color:#000000;">困难和解决</font>
### 导入模型时尺寸和方向，颜色设置都出现大量问题，没有成功绑定mtl文件：
#### 尺寸和方向问题  
3ds Max 和 OpenGL 的坐标系不同。（由于坐标调试的方向一直很奇怪，查阅了大量资料后才发现！！）
在 3ds Max 中，**Y 轴是垂直向上的**，而**在 OpenGL 中，通常 Z 轴是垂直向上的**。因此，当导入模型时，模型的方向会出现不对齐或旋转错误。  
单位不一致：建模软件中的单位与 OpenGL 渲染时使用的单位可能不同。比如，3ds Max 可能使用米或厘米作为单位，而 OpenGL 通常没有直接的单位概念，所有模型的大小**都依赖于坐标系缩放。**  
解决方案：  
**a.习惯Y 轴是垂直向上的，并告知队友这个反常识的坐标**  
**b.缩放处理：**

调整模型的大小，可以在加载模型后通过 缩放矩阵 来统一尺寸。

使用 glm::scale() 进行适当的缩放。

```plain
model = glm::scale(model, glm::vec3(0.1f));  // 统一缩小模型尺寸
```

####   
  
2. 颜色和材质（MTL 文件）问题  
问题原因：
MTL 文件未绑定：虽然 .obj 文件通常会引用 .mtl 文件来加载材质和纹理，但如果 .mtl 文件路径错误，或者 材质信息未正确处理，就会导致加载后的模型没有颜色和材质。  
材质属性不正确：在某些情况下，.mtl 文件中的某些参数（如 Ka, Kd, Ks 等）可能没有被正确设置，导致模型的颜色和反射效果缺失。  
解决方案：

确保 .mtl 文件路径正确：

通过检查 .obj 文件中指定的 .mtl 文件路径，**确保路径与 .obj 文件在同一目录**  
**一定要使用相对路径，否则队友很难启动代码！**  


****

![](https://cdn.nlark.com/yuque/0/2024/png/40554294/1734577911491-8fe140d4-3ec5-4410-803a-ef129aa20f4f.png)调整后![](https://cdn.nlark.com/yuque/0/2024/png/40554294/1734580635055-82042cb3-714a-4d42-95db-434d52e2ae23.png)

### 配置好天空盒后窗外全白
### **深度测试配置问题**
天空盒全白的另一个常见原因是深度测试配置不正确。OpenGL 默认在渲染物体时会执行 **深度测试**，这决定了一个物体是否应该被渲染在另一个物体之前。对于天空盒，我们通常需要禁用深度写入，确保它不会影响其他物体的渲染。

+ **禁用深度写入**：如果没有禁用深度写入，天空盒的深度值会被写入深度缓冲区，这会导致后续的物体渲染时被错误地遮挡。

#### **解决方法**：
1. **禁用深度写入**：在渲染天空盒之前，禁用深度写入。
2. **使用适当的深度比较函数**：设置深度函数为 `GL_LEQUAL`，确保天空盒渲染在最远处。

```plain

// 禁用深度写入
glDepthMask(GL_FALSE);  
......//其余代码
// 设置深度函数为 GL_LEQUAL，确保天空盒渲染在最远处
glDepthFunc(GL_LEQUAL);
```



![](https://cdn.nlark.com/yuque/0/2024/png/40554294/1734578396118-6b3a52e4-fd45-4363-8d87-08815c973805.png)

调整后

![](https://cdn.nlark.com/yuque/0/2024/png/40554294/1734580664763-553c5891-1d68-4215-8c2d-2aca62a5d006.png)

# 参考和相关资料
[The Asset-Importer Library Home](http://www.assimp.org/)

 阅读了关于 **Assimp** 处理 `.obj` 和 `.mtl` 文件的文档，确保了对不同材质、纹理和模型数据的正确解析。  

[Learn OpenGL, extensive tutorial resource for learning Modern OpenGL](https://learnopengl.com/)

+ 阅读了 OpenGL 中 **纹理和材质** 的相关部分，了解了如何正确地使用纹理坐标、材质信息以及如何绑定材质。

