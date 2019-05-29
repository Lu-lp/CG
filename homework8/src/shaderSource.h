#ifndef SHADER_SOURCE
#define SHADER_SOURCE

// ----------------------------------------
const char *verticeShaderSource = "#version 330 core\n"
"layout(location = 0) in vec2 aPos;\n"
"uniform mat4 model;\n"
"void main()\n"
"{\n"
"	gl_Position = model * vec4(aPos, 0.0, 1.0);\n"
"}\n\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec3 color;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(color, 1.0);\n"
"}\n\0";

#endif