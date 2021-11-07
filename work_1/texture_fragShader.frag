#version 330 core
out vec4 FragColor;


struct Light{
    vec3 dir;
    float intensity;
    vec3 color;
    vec3 pos;
};

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;

//漫反射纹理
uniform Light SkyLight[2];    //光线
uniform Light PointLight[2];    //点光源
uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
//高光纹理

void main()
{    
    vec3 specular;
    vec3 diffuse;
    vec3 ambient = vec3(0.05,0.05,0.05);

    //平行光源
    for(int i = 0; i < 2; i++)
    {
        vec3 lightDir = -normalize(SkyLight[i].dir);
        vec3 normal = normalize(Normal);
        vec3 kd = vec3(texture(texture_diffuse1, TexCoords));
        float diff = max(dot(lightDir, normal), 0.0);
        diffuse += kd * SkyLight[i].intensity * SkyLight[i].color * diff;

        /*
        // Phong 光照模型 高光项计算
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        vec3 ks = vec3(0.2,0.2,0.2);
        float spec= pow(max(dot(viewDir, reflectDir), 0.0), 32);
        specular += ks *   SkyLight[i].intensity * SkyLight[i].color * spec;
        */        

        //Blin Phong 光照模型光照项
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 halfVec = normalize(viewDir + lightDir);
        vec3 ks = vec3(0.2, 0.2, 0.2);
        float spec = pow(max(dot(halfVec, normal), 0.0), 32);
        specular += ks *   SkyLight[i].intensity * SkyLight[i].color * spec;
    }


    //点光源
    for(int i = 0; i < 2; i++)
    {
        vec3 lightDir = normalize(PointLight[i].pos - FragPos); // 光线方向
        float dis = distance(PointLight[i].pos, FragPos);
        dis = dis*dis;


        vec3 normal = normalize(Normal);
        vec3 kd = vec3(texture(texture_diffuse1, TexCoords));
        float diff = max(dot(lightDir, normal), 0.0);
        diffuse += kd * PointLight[i].intensity * PointLight[i].color * diff / dis;

        /*
        //Phong 光照模型高光项
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        vec3 ks = vec3(0.2,0.2,0.2);
        float spec= pow(max(dot(viewDir, reflectDir), 0.0), 32);
        specular += ks *  PointLight[i].intensity * PointLight[i].color * spec / dis;
        */

        
        //Blin Phong 光照模型高光项
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 halfVec = normalize(viewDir + lightDir);
        vec3 ks = vec3(0.2, 0.2, 0.2);
        float spec = pow(max(dot(halfVec, normal), 0.0), 32);
        specular += ks *   PointLight[i].intensity * PointLight[i].color * spec /dis;
        
    }

    FragColor = vec4(ambient + diffuse + specular, 1.0);
   
}