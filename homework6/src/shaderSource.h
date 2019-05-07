#ifndef SHADER_SOURCE
#define SHADER_SOURCE
// ������Ⱦ��Դ��shader����-------------------------------------------------
const char* lampVerticeShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"	gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"}\n\0";

const char* lampFragmentShaderSource ="#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec3 lightColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(lightColor,1.0f);\n"
"}\n\0";

// ������Ⱦ�����Phong shading ����-----------------------------------------
const char* phongVerticeSharderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aNormal;\n"
"out vec3 Position;\n"
"out vec3 Normal;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"	Position = vec3(model * vec4(aPos, 1.0));\n"
"	Normal = mat3(transpose(inverse(model))) * aNormal;\n"
"	gl_Position = projection * view * vec4(Position, 1.0);\n"
"}\n\0";
const char* phongFragmentSharderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 Position;\n"
"in vec3 Normal;\n"
"uniform vec3 lightPos;\n"
"uniform vec3 viewPos;\n"
"uniform vec3 lightColor;\n"
"uniform vec3 ObjectColor;\n"
"uniform float ambientStrength;\n"
"uniform float diffuseStrength;\n"
"uniform float specularStrength;\n"
"uniform float shiness;\n"
"void main()\n"
"{\n"
"	// phong shading\n"
"	// ------------------------\n"
"	// ambient\n"
"	vec3 ambient = ambientStrength * lightColor;\n"
"	// diffuse \n"
"	vec3 norm = normalize(Normal);\n"
"	vec3 lightDir = normalize(lightPos - Position);\n"
"	float diff = max(dot(norm, lightDir), 0.0);\n"
"	vec3 diffuse = diffuseStrength * diff * lightColor;\n"
"	// specular\n"
"	vec3 viewDir = normalize(viewPos - Position);\n"
"	vec3 reflectDir = reflect(-lightDir, norm);\n"
"	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shiness);\n"
"	vec3 specular = specularStrength * spec * lightColor;\n"
"	vec3 color = ambient + diffuse + specular;\n"
"   FragColor = vec4(color * ObjectColor,1.0f);\n"
"}\n\0";

// ������Ⱦ�����Gouraud shading ����---------------------------------------
const char* gouraudVerticeSharderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aNormal;\n"
"out vec3 LightingColor; // resulting color from lighting calculations\n"
"uniform vec3 lightPos;\n"
"uniform vec3 viewPos;\n"
"uniform vec3 lightColor;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"uniform float ambientStrength;\n"
"uniform float diffuseStrength;\n"
"uniform float specularStrength;\n"
"uniform float shiness;\n"
"void main()\n"
"{\n"
"	gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"	// gouraud shading\n"
"	// ------------------------\n"
"	vec3 Position = vec3(model * vec4(aPos, 1.0));\n"
"	vec3 Normal = mat3(transpose(inverse(model))) * aNormal;\n"
"	// ambient\n"
"	vec3 ambient = ambientStrength * lightColor;\n"
"	// diffuse \n"
"	vec3 norm = normalize(Normal);\n"
"	vec3 lightDir = normalize(lightPos - Position);\n"
"	float diff = max(dot(norm, lightDir), 0.0);\n"
"	vec3 diffuse = diffuseStrength * diff * lightColor;\n"
"	// specular\n"
"	vec3 viewDir = normalize(viewPos - Position);\n"
"	vec3 reflectDir = reflect(-lightDir, norm);\n"
"	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shiness);\n"
"	vec3 specular = specularStrength * spec * lightColor;\n"
"	LightingColor = ambient + diffuse + specular;\n"
"}\n\0";

const char* gouraudFragmentSharderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 LightingColor;\n"
"uniform vec3 ObjectColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ObjectColor * LightingColor,1.0f);\n"
"}\n\0";
#endif