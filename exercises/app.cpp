
#include <sb7.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class ex2_app : public sb7::application
{
public:
	void startup() {
		rendering_program = compileShaders("vertex_shader.shader", "fragment_shader.shader");
		glGenVertexArrays(1, &vertex_array_object);
		glBindVertexArray(vertex_array_object);
	}

	void shutdown()
	{
		glDeleteVertexArrays(1, &vertex_array_object);
		glDeleteProgram(rendering_program);
		glDeleteVertexArrays(1, &vertex_array_object);
	}
	void render(double currentTime) {
		const GLfloat color[] = { (float)sin(currentTime)*0.5 + 0.5f, (float)cos(currentTime)*0.5 + 0.5f, 0.0, 1.0 };
		glClearBufferfv(GL_COLOR, 0, color);

		glUseProgram(rendering_program);

		glPointSize(40.0f);
		
		glDrawArrays(GL_POINTS, 0, 1);
	}
private:
	GLuint rendering_program;
	GLuint vertex_array_object;
	GLuint compileShaders(std::string vertex_shader_name, std::string fragment_shader_name)
	{
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;

		static const GLchar *fragment_shader_source[] = { readFile(fragment_shader_name) } ;
		static const GLchar *vertex_shader_source[] = { readFile(vertex_shader_name) };
		//static const GLchar * fragment_shader_source[] = readFile("fragment_shader.shader");
			/*
		{
		"#version 450 core \n"
		" \n"
		"out vec4 color; \n"
		" \n"
		"void main(void) \n"
		"{ \n"
		" color = vec4(0.0, 0.8, 1.0, 1.0); \n"
		"} \n"
		};
		
		static const GLchar * vertex_shader_source[] =
		{
		"#version 450 core \n"
		" \n"
		"void main(void) \n"
		"{ \n"
		" gl_Position = vec4(0.0, 0.0, 0.5, 1.0); \n"
		"} \n"
		};
		*/

		//static const GLchar *vertex_shader_source[] = { readFile(vertex_shader_name).c_str() };

		//create and compile vertex shader
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
		glCompileShader(vertex_shader);
		GLint success = 0;
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE) {
			GLint logSize = 0;
			glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &logSize);

			std::vector<GLchar> log(logSize);
			glGetShaderInfoLog(vertex_shader, logSize, &logSize, &log[0]);
			std::string error(log.begin(), log.end());
			glDeleteShader(vertex_shader);
		}
		//create and compile fragment shader
		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE) {
			GLint logSize = 0;
			glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &logSize);

			std::vector<GLchar> log(logSize);
			glGetShaderInfoLog(fragment_shader, logSize, &logSize, &log[0]);
			std::string error(log.begin(), log.end());
			glDeleteShader(fragment_shader);
		}

		//create program and attach shaders
		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program,fragment_shader);
		glLinkProgram(program);

		//delete shaders after attaching
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;



	}

	GLchar * readFile(std::string filename) {
		FILE *f = fopen(filename.c_str(), "rb");
		//moves the pointer to end of file
		fseek(f, 0, SEEK_END);
		//get the pointer location - the length of the file
		long file_size = ftell(f);
		//moves the pointer to the begining
		fseek(f, 0, SEEK_SET);
		//allocates enough space for the text on the heap
		char *text = (char *)malloc(file_size + 1);
		//reads the file content to text
		fread(text, file_size, 1, f);
		//close the file
		fclose(f);
		//zero at the end to tell its the end of the text
		text[file_size] = 0;
		return text;

	}
};

DECLARE_MAIN(ex2_app)
