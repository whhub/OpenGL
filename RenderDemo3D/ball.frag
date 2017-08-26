#version 330 core

in vec3 vNormal;  
in vec3 vVertex;  
in vec3 vLightPositionView;

uniform vec3    ambientColor;
uniform vec3    diffuseColor;   
uniform vec3    specularColor;
uniform float    sh;

out vec4 fcolor;

void main()
{
  
    vec3 vLightDir=normalize(vLightPositionView-vVertex);
    //vec3 vLightDir=normalize(vec3(0.0)-vVertex);
    float diff = max(0.0, dot(normalize(vNormal), vLightDir));

    fcolor.rgb = diff * diffuseColor;

    fcolor.rgb += ambientColor;

    vec3 viewDir=normalize(vec3(0.0)-vVertex);
    vec3 vReflection = normalize(reflect(-vLightDir, normalize(vNormal)));
    float spec = max(0.0, dot(viewDir,vReflection));
    //如果漫射光为0，就不必考虑指数函数
    if(diff != 0) 
    {
        float fSpec = pow(spec, sh);
        fcolor.rgb += fSpec*specularColor;
    }
    fcolor.a=1.0;

    //fcolor.rgba=vec4(1.0);
}