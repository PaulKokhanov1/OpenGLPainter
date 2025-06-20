#ifndef UTILSP_H
#define UTILSP_H

#include <iostream>
#include <iomanip>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace UtilsP
{

	constexpr int colorOffset = 3;

	void printVec3(const glm::vec3& v, const std::string& label = "Vec3");
	void printMat4(const glm::mat4& mat, const std::string& label = "Matrix");
	void printVertexData(const std::string& name, const GLfloat* vertices, size_t vertexCount);
}

#endif // UTILSP_H
