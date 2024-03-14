#pragma once

#include <string>
#include <filesystem>
#include <iostream>

#include <GL/glew.h> 

class Shader {
public:
	// you can add more constructors for pipeline with GS, TS etc.
	Shader(void) = default; //does nothing
	Shader(const std::filesystem::path& VS_file, const std::filesystem::path& FS_file); // TODO: load, compile, and link shader

	void activate(void);    // TODO: activate shader
	void deactivate(void);  // TODO: deactivate current shader program (i.e. activate shader no. 0)
	void clear(void);       // TODO: deallocate shader program

	// set uniform according to name 
	// https://docs.gl/gl4/glUniform
	void setUniform(const std::string& name, const float val);
	void setUniform(const std::string& name, const int val);         // TODO: implement 
	void setUniform(const std::string& name, const glm::vec3 val);   // TODO: implement 

	void setUniform(const std::string& name, const glm::vec4 val);

	void setUniform(const std::string& name, const glm::mat3 val);   // TODO: implement
	void setUniform(const std::string& name, const glm::mat4 val);

	//...

private:
	GLuint ID{ 0 }; // default = 0, empty shader
	std::string getShaderInfoLog(const GLuint obj); 
	std::string getProgramInfoLog(const GLuint obj);

	GLuint compile_shader(const std::filesystem::path& source_file, const GLenum type); // TODO: try to load and compile shader
	GLuint link_shader(const std::vector<GLuint> shader_ids);                            // TODO: try to link all shader IDs to final program
	std::string textFileRead(const std::filesystem::path& filename);                    // TODO: load text file
};

