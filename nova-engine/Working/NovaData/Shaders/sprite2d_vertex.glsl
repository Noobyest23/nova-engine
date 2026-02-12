
#version 330 core
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_uv;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform vec2 u_img_size;
uniform vec2 u_offset;
uniform bool u_flip_h;
uniform bool u_flip_v;
uniform int u_layer;

out vec2 v_uv;

void main() {
	vec3 scaledPos = a_pos;
	scaledPos.xy *= u_img_size;
	gl_Position = u_projection * u_view * u_model * vec4(scaledPos + vec3(u_offset, u_layer), 1.0);

	vec2 uv = a_uv;
	if (u_flip_h) {
		uv.x = 1.0 - a_uv.x;
	}
	if (u_flip_v) {
		uv.y = 1.0 - a_uv.y;
	}
	v_uv = uv;
}
