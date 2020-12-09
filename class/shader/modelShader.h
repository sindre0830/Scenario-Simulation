#ifndef __MODELSHADER_H_
#define __MODELSHADER_H_
/* library */
#include <string>
//vertex shader
static const std::string modelVertexShader = R"(
	#version 430 core
    /* input */
    layout(location = 0) in vec3 a_gridPos;
    layout(location = 1) in vec3 a_normal;
    /* output */
	out vec4 vs_fragPos;
	out vec3 vs_normal;
	out mat4 vs_initialPos;
    /* uniform */
    uniform mat4 u_modelMatrix = mat4(1);
    uniform mat4 u_viewMatrix = mat4(1);
    uniform mat4 u_projectionMatrix = mat4(1);
	uniform vec3 offsets[1000];
    /**
     * Main vertex shader program.
     */
	void main() {
		//set static light position
		vs_initialPos[0] = vec4(1.f, 1.f, 3.f, 0.f);
		vs_initialPos[1] = vec4(0.f, 0.f, -1.f, 0.f);
		vs_initialPos[2] = vec4(0.f, 1.f, 0.f, 0.f);
		vs_initialPos[3] = vec4(0.f, 0.f, 0.f, 1.f);

    	vec3 offset = offsets[gl_InstanceID];

		vs_fragPos = vec4(a_gridPos + offset, 1.f);
		mat3 normalMatrix = transpose(inverse(mat3(vs_initialPos * u_modelMatrix)));
		vs_normal = normalize(normalMatrix * normalize(a_normal));
		
        gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(a_gridPos + offset, 1.f);

	}
)";
//fragment shader
static const std::string modelFragmentShader = R"(
	#version 430 core
    /* input */
	in vec4 vs_fragPos;
	in vec3 vs_normal;
	in mat4 vs_initialPos;
    /* output */
    out vec4 color;
    /* uniform */
    uniform mat4 u_modelMatrix = mat4(1);
    uniform mat4 u_viewMatrix = mat4(1);
	uniform vec3 u_objectColor = vec3(1.f);
	uniform vec3 u_lightColor = vec3(1.f);
	uniform vec3 u_lightPos = vec3(1.f);
	uniform vec3 u_lightDirection = vec3(1.f);  
	uniform float u_specularity = 0.5f;
	/**
	 * @brief Get directional light
	 * 
	 * @param color 
	 * @param direction 
	 * @return vec3 directional light value
	 */
	vec3 directionalLight(in vec3 color, in vec3 direction) {
		float ambientStrength = 0.1f;
		vec3 ambient = ambientStrength * color;

        vec3 lightDirection = normalize(direction);

		vec3 diffuse = color * max(0.f, dot(vs_normal, lightDirection));

		vec3 viewDirection = normalize(vec3(inverse(vs_initialPos) * vec4(0, 0, 0, 1) - u_modelMatrix * vs_fragPos));

		vec3 reflectionDirection = reflect(lightDirection, vs_normal);

		float specularPower = pow(max(0.f, dot(viewDirection, reflectionDirection)), 32);

		vec3 specular = u_specularity * specularPower * color;

		return ambient + diffuse + specular;
	}
    /**
     * Main fragment shader program.
     */
	void main() {
		vec3 light = directionalLight(u_lightColor, u_lightDirection);
		color = vec4(u_objectColor, 1.f);
		color = color * vec4(light, 1.f);
	}
)";
#endif