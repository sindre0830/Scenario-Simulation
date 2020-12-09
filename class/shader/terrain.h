#ifndef __TERRAINSHADER_H_
#define __TERRAINSHADER_H_
/* library */
#include <string>
//vertex shader
static const std::string terrainVertexShader = R"(
	#version 430 core
    /* input */
	layout(location = 0) in vec3 a_gridPos;
	layout(location = 1) in vec2 a_texPos;
	layout(location = 2) in vec3 a_normal;
    /* output */
	out vec2 vs_texPos;
	out vec4 vs_fragPos;
	out vec3 vs_normal;
	out mat4 vs_initialPos;
    /* uniform */
    uniform mat4 u_modelMatrix = mat4(1);
    uniform mat4 u_viewMatrix = mat4(1);
    uniform mat4 u_projectionMatrix = mat4(1);
    /**
     * Main vertex shader program.
     */
	void main() {
		//set static light position
		vs_initialPos[0] = vec4(1.f, 1.f, 3.f, 0.f);
		vs_initialPos[1] = vec4(0.f, 0.f, -1.f, 0.f);
		vs_initialPos[2] = vec4(0.f, 1.f, 0.f, 0.f);
		vs_initialPos[3] = vec4(0.f, 0.f, 0.f, 1.f);

        vs_fragPos = vec4(a_gridPos, 1.f);
        mat3 normalMatrix = transpose(inverse(mat3(vs_initialPos * u_modelMatrix)));
        vs_normal = normalize(normalMatrix * normalize(a_normal));

		vs_texPos = a_texPos;
		gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(a_gridPos, 1.f);
	}
)";
//fragment shader
static const std::string terrainFragmentShader = R"(
	#version 430 core
    /* input */
	in vec2 vs_texPos;
	in vec4 vs_fragPos;
	in vec3 vs_normal;
	in mat4 vs_initialPos;
    /* output */
	out vec4 color;
    /* uniform */
	uniform sampler2D u_texture;

    uniform mat4 u_modelMatrix = mat4(1);
    uniform mat4 u_viewMatrix = mat4(1);
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

		//vec3 viewDirection = normalize(vec3(inverse(u_viewMatrix) * vec4(0, 0, 0, 1) - u_modelMatrix * vs_fragPos));
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
		color = texture(u_texture, vs_texPos);
		color = color * vec4(light, 1.f);
	}
)";
#endif
