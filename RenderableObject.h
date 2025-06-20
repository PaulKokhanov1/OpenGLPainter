#ifndef RENDERABLEOBJECT_CLASS_H
#define RENDERABLEOBJECT_CLASS_H

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<vector>
#include <cmath>
#include <iomanip>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Texture.h"
#include"Camera.h"
#include"Line.h"
#include"BoundingSphere.h"
#include"PMath.h"
#include"UtilsP.h"

class RenderableObject 
{
public:
    std::string name;
    glm::vec3 position;
    glm::mat4 model;
    VAO vao;
    VBO* vbo;
    EBO* ebo;
    GLfloat* vertices;
    GLuint* indices;
    GLsizeiptr vertexSize;
    GLsizeiptr indexSize;
    BoundingSphere* bounding;

    RenderableObject(std::string name, GLfloat* verts, GLsizeiptr vSize, GLuint* inds, GLsizeiptr iSize);
    void setPosition(glm::vec3 pos);
    void draw(Shader& shader);
    void testIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& color);
    void updateColorAt(int index, glm::vec3 color);
    void updateGPU();
    void Delete();
    ~RenderableObject();
};
#endif