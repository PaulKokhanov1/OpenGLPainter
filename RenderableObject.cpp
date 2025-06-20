#include "RenderableObject.h"

RenderableObject::RenderableObject(std::string name, GLfloat* verts, GLsizeiptr vSize, GLuint* inds, GLsizeiptr iSize)
{
	this->name = name;
	this->indices = inds;
	this->vertices = verts;
	this->vertexSize = vSize;
	this->indexSize = iSize;

	// MUST BIND VAO FIRST BEFORE CREATING VBO AND EBO
	vao.Bind();
	vbo = new VBO(verts, vSize);
	ebo = new EBO(inds, iSize);

	// Links VBO to VAO, we use two LinkAttrib because now we have two inputs to our vertex shader
	vao.LinkAttrib(*vbo, 0, 3, GL_FLOAT, PMath::shapeVertexStrides, (void*)0);
	vao.LinkAttrib(*vbo, 1, 3, GL_FLOAT, PMath::shapeVertexStrides, (void*)(3 * sizeof(float)));
	vao.LinkAttrib(*vbo, 2, 3, GL_FLOAT, PMath::shapeVertexStrides, (void*)(6 * sizeof(float)));

	vao.Unbind();
	vbo->Unbind();
	ebo->Unbind();

	position = glm::vec3(0.0f, 0.0f, 0.0f);// CHANGE BACK TO LATER PMath::offScreenPos;
	model = glm::mat4(1.0f);	// maybe add this as a parameter of the constructor
	model = glm::translate(model, position);	// basically our pyramid position in world space

	// TODO make Algo to determine farthest point from origin within the vertices array

	bounding = new BoundingSphere(name, position, glm::length(glm::vec3(0.5f, 0.8f, 0.5f)) ,verts, vSize / sizeof(GLfloat), inds, iSize / sizeof(GLuint), &model);


}

void RenderableObject::setPosition(glm::vec3 pos)
{
	position = pos;
	model = glm::translate(glm::mat4(1.0f), pos);
	bounding->center = pos;
}

void RenderableObject::draw(Shader& shader)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	// Bind the VAO so OpenGL knows to use it
	vao.Bind();
	// Draw primitives, number of indices, datatype of indices, index of indices
	glDrawElements(GL_TRIANGLES, indexSize / sizeof(int), GL_UNSIGNED_INT, 0);

}

void RenderableObject::testIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& color)
{
	//Testing Bounding Sphere intersection
	int resultIndex[3] = { -1, -1, -1 };
	bounding->rayIntersectionTest(rayOrigin, rayDir, resultIndex);
	std::string intersect = resultIndex[0] != -1 ? "True" : "False";
	std::cout << "Shape: " + name + " Intersection: " + intersect << std::endl;

	if (resultIndex[0] != -1) {


		for (int j = 0; j < 3; ++j) {
			int idx = resultIndex[j];
			int base = idx * 9;

			// change color
			vertices[base + UtilsP::colorOffset] = color[0];
			vertices[base + UtilsP::colorOffset + 1] = color[1];
			vertices[base + UtilsP::colorOffset + 2] = color[2];
		}
		UtilsP::printVertexData(name, vertices, vertexSize / sizeof(GLfloat));

		vao.Bind();
		vbo->Bind();
		glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW); //(GL_ARRAY_BUFFER, 0, sizeof(pyramidVertices), pyramidVertices);

	}
}

void RenderableObject::updateColorAt(int index, glm::vec3 color)
{
}

void RenderableObject::updateGPU()
{
}

void RenderableObject::Delete()
{
	vao.Delete();
	vbo->Delete();
	ebo->Delete();

}

RenderableObject::~RenderableObject()
{

}
