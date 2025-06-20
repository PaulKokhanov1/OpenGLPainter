#include "UtilsP.h"

namespace UtilsP
{
	void printVec3(const glm::vec3& v, const std::string& label)
	{
		std::cout << label << ": ("
			<< v.x << ", "
			<< v.y << ", "
			<< v.z << ")" << std::endl;
	}

	void printMat4(const glm::mat4& mat, const std::string& label)
	{
		std::cout << label << ":\n";
		for (int row = 0; row < 4; ++row) {
			std::cout << "| ";
			for (int col = 0; col < 4; ++col) {
				std::cout << mat[col][row] << "\t";  // GLM is column-major
			}
			std::cout << "|\n";
		}
		std::cout << std::endl;
	}

	void printVertexData(const std::string& name, const GLfloat* vertices, size_t vertexCount)
	{
		const int stride = 9; // position (3), color (3), normal (3)
		size_t numVertices = vertexCount / stride;

		std::cout << "=== Vertex Data for Object: " << name << " ===\n";

		for (size_t i = 0; i < numVertices; ++i) {
			size_t base = i * stride;

			float x = vertices[base + 0];
			float y = vertices[base + 1];
			float z = vertices[base + 2];

			float r = vertices[base + 3];
			float g = vertices[base + 4];
			float b = vertices[base + 5];

			float nx = vertices[base + 6];
			float ny = vertices[base + 7];
			float nz = vertices[base + 8];

			std::cout << std::fixed << std::setprecision(2)
				<< "Vertex " << std::setw(2) << i << ": ("
				<< x << ", " << y << ", " << z << ") | "
				<< "Color: (" << r << ", " << g << ", " << b << ") | "
				<< "Normal: (" << nx << ", " << ny << ", " << nz << ")\n";
		}
	}
}
