#version 330 core
layout(location = 0) in vec4 posAttr;
layout(location = 1) in vec3 normalAttr;
layout(location = 2) in vec2 uv;    //not used

out vec3 fragPos;
out vec3 normal;

uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;

void main()
{
    fragPos = vec3(mMatrix * posAttr).xyz;  //fragment position in world space
    normal = mat3(transpose(inverse(mMatrix))) * normalAttr;    //should be done on CPU

    //gl_Position = pMatrix * vMatrix * mMatrix * posAttr;;
    gl_Position = pMatrix * vMatrix * vec4(fragPos, 1.0);   //not sure if this is quicker...
}
