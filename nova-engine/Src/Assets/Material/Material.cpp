#include "Material.h"
#include <fstream>
#include "../../Core/Engine.h"

static std::string ReadFile(const std::string& filepath) {
	std::string result;
	std::fstream file(filepath);
	
	if (!file.is_open()) {
		Engine::GetInstance()->PushError("[Material] Could not open file at " + filepath);
	}

	std::string line;
	while (std::getline(file, line)) {
		result += line + "\n";
	}

	return result;
}

Material::Material(const std::string& vert_path, const std::string& frag_path) : vertex_filepath(vert_path), fragment_filepath(frag_path) {
	Load();
}

void Material::Load() {
	if (Compile()) {
		ReflectUniforms();
	}
}

bool Material::Compile() {
	std::string vert_src = ReadFile(vertex_filepath);
	std::string frag_src = ReadFile(fragment_filepath);

	GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
	const char* vert_cstr = vert_src.c_str();
	glShaderSource(vert_shader, 1, &vert_cstr, nullptr);
	glCompileShader(vert_shader);
	
	GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* frag_cstr = frag_src.c_str();
	glShaderSource(frag_shader, 1, &frag_cstr, nullptr);
	glCompileShader(frag_shader);

	GLuint new_program = glCreateProgram();
	glAttachShader(new_program, vert_shader);
	glAttachShader(new_program, frag_shader);
	glLinkProgram(new_program);

	auto CheckShader = [&](GLuint shader, const std::string& name) {
		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			char log[1024];
			glGetShaderInfoLog(shader, 1024, nullptr, log);
			Engine::GetInstance()->PushError("[OpenGL Compile] " + name + ": " + log);
			return false;
		}
		return true;
	};

	if (!CheckShader(vert_shader, "Vertex")) return false;
	if (!CheckShader(frag_shader, "Fragment")) return false;

	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);

	if (program) glDeleteProgram(program);
	program = new_program;

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked){
		char log[512];
		glGetProgramInfoLog(program, 512, nullptr, log);
		Engine::GetInstance()->PushError("[OpenGL Linker] " + std::string(log));
		glDeleteProgram(new_program);
		return false;
	}

	return true;
}

void Material::ReflectUniforms() {
	uniforms.clear();
	if (program == 0) return;

	GLint count = 0;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);

	constexpr GLsizei buf_size = 128;
	char name[buf_size];

	for (GLint i = 0; i < count; ++i) {
		GLsizei length = 0;
		GLint size = 0;
		GLenum type = 0;
		glGetActiveUniform(program, i, buf_size, &length, &size, &type, name);

		GLint loc = glGetUniformLocation(program, name);
		if (loc == -1) continue;

		uniform_locations[name] = loc;

		switch (type) {
		case GL_FLOAT: uniforms[name] = 0.0f; break;
		case GL_INT: uniforms[name] = 0; break;
		case GL_BOOL: uniforms[name] = false; break;
		case GL_FLOAT_VEC2: uniforms[name] = glm::vec2(0.0f); break;
		case GL_FLOAT_VEC3: uniforms[name] = glm::vec3(0.0f); break;
		case GL_FLOAT_VEC4: uniforms[name] = glm::vec4(0.0f); break;
		case GL_FLOAT_MAT4: uniforms[name] = glm::mat4(1.0f); break;
		case GL_SAMPLER_2D: uniforms[name] = 0; break;
		default: break;
		}
	}
}

void Material::SetUniform(const std::string& name, const Uniform& value) {
	if (uniforms.find(name) != uniforms.end()) {
		uniforms[name] = value;
		glUseProgram(program);
		GLint loc = uniform_locations[name];

		std::visit([loc](auto&& v) {
			using T = std::decay_t<decltype(v)>;
			if constexpr (std::is_same_v<T, int>) glUniform1i(loc, v);
			else if constexpr (std::is_same_v<T, float>) glUniform1f(loc, v);
			else if constexpr (std::is_same_v<T, bool>) glUniform1i(loc, v ? 1 : 0);
			else if constexpr (std::is_same_v<T, glm::vec2>) glUniform2fv(loc, 1, &v[0]);
			else if constexpr (std::is_same_v<T, glm::vec3>) glUniform3fv(loc, 1, &v[0]);
			else if constexpr (std::is_same_v<T, glm::vec4>) glUniform4fv(loc, 1, &v[0]);
			else if constexpr (std::is_same_v<T, glm::mat4>) glUniformMatrix4fv(loc, 1, GL_FALSE, &v[0][0]);
		}, value);
	}
	else {
		Engine::GetInstance()->PushError("Uniform: " + name + " not found on shader");
	}
}

void Material::Bind() const {
	if (program != 0) {
		glUseProgram(program);
	}
}

void Material::Unbind() const {
	glUseProgram(0);
}

void Material::Reload() {
	Load();
}

void Material::OnDestroy() {
	if (program) {
		glDeleteProgram(program);
		program = 0;
	}
	uniforms.clear();
}