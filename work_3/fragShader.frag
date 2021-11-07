#version 330 core
out vec4 FragColor;


in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;


struct Light
{
    vec3 pos;
    float intensity;
    vec3 color;
    vec3 dir;
};

const float PI = 3.1415926;

// PBR贴图
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

uniform Light pointLight[4];
uniform Light skyLight[2];
//高光纹理

uniform vec3 viewPos;

uniform vec3 albedo;       //反射率（相当于漫反射系数）
uniform float metallic;     //金属度
uniform float roughness;    //粗糙度
uniform float ao;   //环境光遮蔽
uniform vec3 f0;

//正态分布函数
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness * roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

//菲涅尔方程
// 求反射比率
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}  

//几何
//求自身微表面遮蔽
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

//TBN
//法线贴图转化
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(FragPos);
    vec3 Q2  = dFdy(FragPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}



void main()
{    
	vec3 normal = getNormalFromMap();
    vec3 viewDir = normalize(viewPos - FragPos);
    vec2 tex = TexCoords;
    
    vec3 albedo = pow(texture(albedoMap, tex).rgb, vec3(1));
    float metallic  = texture(metallicMap, tex).r;
    float roughness = texture(roughnessMap, tex).r;

    float ao        = texture(aoMap, tex).r;



    vec3 F0 = f0;
    F0 = mix(F0, albedo, metallic);

    //radiance
    vec3 Lo = vec3(0.0);
    for(int i = 1; i < 4; i ++)
    {
        vec3 lightDir = normalize(pointLight[i].pos - FragPos);
        vec3 halfVec = normalize(viewDir + lightDir);

        float distance = length(pointLight[i].pos - FragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = pointLight[i].color * pointLight[i].intensity * attenuation;

        //BRDF calculate
        float NDF = DistributionGGX(normal, halfVec, roughness);
        float G = GeometrySmith(normal, viewDir, lightDir, roughness);
        vec3 F = fresnelSchlick(max(dot(halfVec,viewDir), 0.0), F0);

       vec3 nominator = NDF * G * F;
       float denominator = 4 * max(dot(normal,viewDir), 0.0) * max(dot(normal, lightDir), 0.0) + 0.001;
       vec3 specular = nominator / denominator;

       vec3 ks = F;
       vec3 kd = vec3(1.0) - ks;

       kd = kd* (1.0 - metallic);

       float NdotL = max(dot(normal, lightDir), 0.0);

       Lo += (kd * albedo / PI + specular) * radiance * NdotL;
    }
    for(int i = 0; i < 2; i++)
    {
        vec3 lightDir = normalize(-skyLight[i].dir);
        vec3 halfVec = normalize(viewDir + lightDir);

        vec3 radiance = skyLight[i].color * skyLight[i].intensity;

        //BRDF calculate
        float NDF = DistributionGGX(normal, halfVec, roughness);
        float G = GeometrySmith(normal, viewDir, lightDir, roughness);
        vec3 F = fresnelSchlick(max(dot(halfVec,viewDir), 0.0), F0);

       vec3 nominator = NDF * G * F;
       float denominator = 4 * max(dot(normal,viewDir), 0.0) * max(dot(normal, lightDir), 0.0) + 0.001;
       vec3 specular = nominator / denominator;

       vec3 ks = F;
       vec3 kd = vec3(1.0) - ks;

       kd = kd* (1.0 - metallic);

       float NdotL = max(dot(normal, lightDir), 0.0);

       Lo += (kd * albedo / PI + specular) * radiance * NdotL;
    }
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;

    color = color/(color + vec3(1.0));
    //伽马矫正
    color = pow(color, vec3(1.0/2.2)); 
    FragColor = vec4(color, 1.0);
}


