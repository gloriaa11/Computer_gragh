#shader vertex
#version 330 core

// �˴� aNormal �ò���
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;



out  vec3 FragPos;
out  vec2 TexCoords;

out  vec3 Normal;




uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{   
    FragPos = vec3(model * vec4(aPos, 1.0));

    TexCoords = aTexCoords;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);  
}



#shader fragment
#version 330 core
#define NR_POINT_LIGHT  7

out vec4 FragColor;

in  vec3 FragPos;
in  vec2 TexCoords;

in  vec3 Normal;



uniform sampler2D texture_diffuse;
uniform vec3 skyColor;
uniform vec3 snowColor = vec3(1.0);
uniform float alpha = 0.8;




void main()
{   


    vec4 texcolor = texture(texture_diffuse, TexCoords);

    // ȥ����ɫ�ı߿�
    if(texcolor.r < 0.5) 
        discard;

    //vec3 finalColor = mix(snowColor, texcolor.rgb, 0.5);

    FragColor = vec4(texcolor.rgb * skyColor * 0.8, 1.0);
}