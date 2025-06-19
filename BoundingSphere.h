#ifndef BOUNDINGSPHERE_CLASS_H
#define BOUNDINGSPHERE_CLASS_H

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<stb/stb_image.h>
#include <string>
#include <limits>
#include"PMath.h"


class BoundingSphere
{
public:
	std::string name;
	glm::vec3 center;
	GLfloat radius;

	glm::mat4* model;

	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;

	const float kEpsilon = 1e-8; // Used in MT Algo
	float u, v; // Barycentric coords used in MT Algo


	BoundingSphere(const std::string& name, glm::vec3 centerWS, GLfloat radius, GLfloat verts[], size_t vertsCount, GLuint indices[], size_t indicesCount, glm::mat4* model); // Center and radius in World Space

	// Test incoming rayIntersection against bounding sphere
	void rayIntersectionTest(glm::vec3 rayOrigin, glm::vec3 rayDir, int* resultIndex);

	//Moller-Trumbore Algorithm to test ray against each of the meshes triangles
	bool MTAlgo(const glm::vec3& orig, const glm::vec3& dir, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, float &t);

	void printMeshData();
};
#endif