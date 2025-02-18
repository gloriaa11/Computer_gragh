#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos; // 顶点位置

uniform mat4 lightSpaceMatrix; // 光源空间矩阵
uniform mat4 model; // 模型矩阵
uniform float time; // 时间变量（用于动画效果）
uniform int find_key; // 是否找到钥匙

float degreesToRadians(float degrees) {
    return degrees * 3.14159265358979323846 / 180.0;
}

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

    // 我们不需要计算颜色或其他值，只需要计算光源空间中的位置
    gl_Position = lightSpaceMatrix * model * vec4(pos, 1.0); // 仅用于创建深度缓冲区

}



#shader fragment
#version 330 core

void main()
{    
    // gl_FragDepth = gl_FragCoord.z;  // 将片段深度值写入深度缓冲 

}


