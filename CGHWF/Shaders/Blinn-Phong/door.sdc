#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos; 		// 顶点位置
layout (location = 1) in vec3 aNormal;		// 顶点法线
layout (location = 2) in vec2 aTexCoords;	// 顶点纹理坐标

out vec3 FragPos;		// 世界空间位置
out vec3 Normal;		// 顶点法线（变换后的）
out vec2 TexCoords;		// 顶点纹理坐标（变换后的）
out vec4 FragPosLightSpace;	// 光源空间位置

uniform mat4 lightSpaceMatrix; // 光源空间矩阵

uniform mat4 model; // 模型矩阵
uniform mat4 view;
uniform mat4 projection;	// 投影矩阵
uniform float time; 		// 时间变量（用于动画效果）
uniform int find_key; 		// 是否找到钥匙

void main() {

    const float maxAngle = 20.0;        // 门的最大打开角度
    vec3 pos = aPos;

    // 旋转门的动画，当 isfind 为 true 时进行旋转
    if (find_key != 0) {
        // 控制门旋转的角度，门开始时的角度为0度，最大旋转到 30 度
        float rotationAngle = maxAngle * (sin(time * 1.0) + 1.0) * 0.5; 	// 使旋转从0到30度
        float rad = radians(rotationAngle);	 // 转换为弧度
        
        // 旋转轴向量 (例如绕 Y 轴旋转)
	vec3 axis = normalize(vec3(0.0f, 1.0f, 0.3f)); // 假设绕 Y 轴旋转
	//vec3 rotateCenter = vec3(65.0f, 25.0f, 67.0f);  // 初始化旋转中心为原点

	// 1. 平移顶点到旋转中心
	
        //vec3 translatedPos = pos - rotateCenter;

	// 罗德里格旋转公式的旋转矩阵
	mat4 rotation = mat4(
    		cos(rad) + axis.x * axis.x * (1.0 - cos(rad)), axis.x * axis.y * (1.0 - cos(rad)) - axis.z * 			sin(rad), axis.x * axis.z * (1.0 - cos(rad)) + axis.y * sin(rad), 0.0f,
   	 	axis.y * axis.x * (1.0 - cos(rad)) + axis.z * sin(rad), cos(rad) + axis.y * axis.y * (1.0 - 			cos(rad)), axis.y * axis.z * (1.0 - cos(rad)) - axis.x * sin(rad), 0.0f,
    		axis.z * axis.x * (1.0 - cos(rad)) - axis.y * sin(rad), axis.z * axis.y * (1.0 - cos(rad)) + axis.x * 		sin(rad), cos(rad) + axis.z * axis.z * (1.0 - cos(rad)), 0.0f,
   	 	0.0f, 0.0f, 0.0f, 1.0f
	);
	
	pos = (rotation * vec4(pos, 1.0)).xyz; // 应用旋转矩阵
    	// 3. 对平移后的顶点进行旋转
        //translatedPos = (rotation * vec4(translatedPos, 1.0)).xyz;
	// 4. 将顶点从旋转中心平移回来
        //pos = translatedPos + rotateCenter;
    }

    FragPos = vec3(model * vec4(pos, 1.0));
    FragPosLightSpace = lightSpaceMatrix*vec4(FragPos,1.0f);
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    TexCoords = aTexCoords;
    
    gl_Position = projection * view * model * vec4(pos, 1.0);
}





#shader fragment
#version 330 core
#define NR_POINT_LIGHT 7

out vec4 FragColor;

in  vec3 FragPos;
in  vec3 Normal;
in  vec2 TexCoords;
in  vec4 FragPosLightSpace;

uniform    vec3 aAmbient;
uniform    vec3 aDiffuse;
uniform    vec3 aSpecular;
    

struct DirLight {
    vec3 direction;	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

uniform vec3 viewPos;
uniform float shininess;
uniform DirLight dirLight;
uniform PointLight plight[NR_POINT_LIGHT];

uniform sampler2D texture_diffuse1;	// 漫反射纹理
uniform sampler2D shadowmap;

// uniform sampler2D texture_specular1;

float ShadowCalculation(PointLight light,vec4 fragPosLightSpace);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
 
    //vec3 texColor = texture(texture_diffuse1,TexCoords);

    float shadow = ShadowCalculation(plight[6],FragPosLightSpace);
    shadow = min(shadow,0.85f);
    
    // 获取纹理颜色
    vec4 texColor = texture(texture_diffuse1, TexCoords);

    // 不透明材质，直接使用纹理的颜色进行计算
    if (texColor.a < 0.1)
        discard;

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    for(int i =0;i<NR_POINT_LIGHT;i++){
        result += CalcPointLight(plight[i], norm, FragPos, viewDir);    
    }
    // FragColor = vec4((1.0f-shadow)*result,1.0f);	// material
    // 最终颜色计算
    FragColor = vec4((1.0f - shadow) * (result + vec3(texColor)), 1.0f);
}


// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // combine results
    vec3 ambient = light.ambient * aAmbient;
    vec3 diffuse = light.diffuse * diff * aDiffuse;
    vec3 specular = light.specular * spec * aSpecular;
    return (ambient + diffuse + specular);
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    
    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * aAmbient;
    vec3 diffuse = light.diffuse * diff * aDiffuse;
    vec3 specular = light.specular * spec * aSpecular;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

float ShadowCalculation(PointLight light,vec4 fragPosLightSpace)
{
    float shadow=0;
    vec3 lightDir = normalize(light.position - FragPos);
    // float bias =max(0.020 * (1.0 - dot(Normal, lightDir)), 0.005);	// material.sdc 设定
    float bias = max(0.010 * (1.0 - dot(Normal, lightDir)), 0.001);	// 阴影偏移量设置较小（柔和阴影）
    vec2 texelSize = 1.0 / textureSize(shadowmap, 0);
 
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowmap, projCoords.xy).r; 
 
    float currentDepth = projCoords.z;
    // PCF
    for(int i=-4;i<=4;++i)
    {
        for(int j=-4;j<=4;j++)
        {

            float closestDepth = texture(shadowmap, projCoords.xy+vec2(i,j)*texelSize).r; 
            shadow += currentDepth-bias > closestDepth  ? 1.0 : 0.0;
        }
    }

    

    return shadow/81.0f;
}

