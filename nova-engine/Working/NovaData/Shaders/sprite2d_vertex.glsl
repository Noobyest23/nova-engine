
#version 330 core
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_uv;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform vec2 u_img_size;

out vec2 v_uv;

void main() {
	vec3 scaledPos = a_pos;
	scaledPos.xy *= u_img_size;
	gl_Position = u_projection * u_view * u_model * vec4(scaledPos, 1.0);
	v_uv = a_uv;
}
