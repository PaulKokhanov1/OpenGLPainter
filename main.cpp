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
#include"RenderableObject.h"
#include"UtilsP.h"

const unsigned int width = 800;
const unsigned int height = 800;


bool objectSpawn = false;
glm::vec3 offscreenPos = glm::vec3(0.0f, 1000.0f, 0.0f);


const unsigned int sectorCount = 12;
const unsigned int stackCount = 12;
unsigned int k1;
unsigned int k2;
GLfloat sectorStep = (2.0f * glm::pi<float>()) / (float)sectorCount;
GLfloat stackStep = glm::pi<float>() / (float)stackCount;
GLfloat sectorAngle, stackAngle;
GLfloat radius = 1.0f;
GLfloat x, y, z, invRadius = 1.0f / radius;
GLfloat R = 0.83f;
GLfloat G = 0.70f;
GLfloat B = 0.44f;
glm::vec3 planeCenter = glm::vec3(-2.0f, 0.0f, 0.0f);
glm::mat4 planeModel = glm::mat4(1.0f);

GLfloat shapeVertexStrides = 9 * sizeof(float);
const int colorOffset = 3;
glm::vec3 color = { 1.0f, 0.0f, 0.0f };


GLfloat sphereVertices[3];
GLfloat sphereColors[3];
GLfloat sphereNormals[3];
std::vector<GLfloat> sphereInfo;
std::vector<GLuint> sphereIndices;


GLfloat pyramidVertices[] = {
	//Square 2D
	//-0.5f, -0.5f, // Lower left corner
	//0.5f, -0.5f, // Lower right corner
	//0.5f, 0.5f, // Upper right corner
	//-0.5f, 0.5f, // Upper left corner


	// Pyramid 3D 
	     //COORDINATES     /        COLORS          /       NORMALS       //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,     0.0f, -1.0f, 0.0f, // Bottom side
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, -1.0f, 0.0f, // Bottom side
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, -1.0f, 0.0f, // Bottom side
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, -1.0f, 0.0f, // Bottom side

	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 -0.8f, 0.5f,  0.0f, // Left Side
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 -0.8f, 0.5f,  0.0f, // Left Side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 -0.8f, 0.5f,  0.0f, // Left Side

	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.5f, -0.8f, // Non-facing side
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.5f, -0.8f, // Non-facing side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 0.0f, 0.5f, -0.8f, // Non-facing side

	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.8f, 0.5f,  0.0f, // Right side
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 0.8f, 0.5f,  0.0f, // Right side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 0.8f, 0.5f,  0.0f, // Right side

	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.5f,  0.8f, // Facing side
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.5f,  0.8f, // Facing side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 0.0f, 0.5f,  0.8f  // Facing side


	//// Triangle
	////     COORDINATES     /        COLORS      /   TexCoord  //
	//-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
	//0.0f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	1.5f, 3.0f, // Upper corner
	//0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	3.0f, 0.0f  // Lower right corner

	// Square 3D
	//-0.5f, -0.5f, 0.0f,		// lower left corner
	//0.5f, -0.5f, 0.0f,		// lower right corner
	//-0.5f, 0.5f, 0.0f,		// upper left corner
	//0.5f, 0.5f, 0.0f,		// upper right corner
};

GLuint pyramidIndices[] = {

	// Single Triangle
	//0, 1, 2

	//Triangles
	//0, 2, 1, // Upper triangle
	//0, 3, 2 // Lower triangle

	//Square
	//0, 2, 3, 1, 0,

	// Pyramid 3D
	0, 1, 2, // Bottom side
	0, 2, 3, // Bottom side
	4, 6, 5, // Left side
	7, 9, 8, // Non-facing side
	10, 12, 11, // Right side
	13, 15, 14 // Facing side

	// Plane
	//0, 1, 2,
	//0, 2, 3
};

GLfloat planeVertices[] = {
	// Plane

-0.5f, 0.0f,  0.5f,		0.83f, 0.70f, 0.44f,		0.0f, 1.0f, 0.0f,
-0.5f, 0.0f, -0.5f,		0.83f, 0.70f, 0.44f,		0.0f, 1.0f, 0.0f,
 0.5f, 0.0f, -0.5f,		0.83f, 0.70f, 0.44f,		0.0f, 1.0f, 0.0f,
 0.5f, 0.0f,  0.5f,		0.83f, 0.70f, 0.44f,		0.0f, 1.0f, 0.0f
};

GLuint planeIndices[] = {

	// Plane
	0, 1, 2,
	0, 2, 3
};



// Have to make Cube use more vertices so we can specify the normals and tex coords easier
GLfloat cubeVertices[] = {
	//COORDINATES     /        COLORS          /        NORMALS       //
	-0.5f, -0.5f,  0.5f,	0.83f, 0.70f, 0.44f, 	0.0f, -1.0f, 0.0f,		// Bottom side
	-0.5f, -0.5f, -0.5f,	0.83f, 0.70f, 0.44f,	0.0f, -1.0f, 0.0f,		// Bottom side
	 0.5f, -0.5f, -0.5f,	0.83f, 0.70f, 0.44f,	0.0f, -1.0f, 0.0f,		// Bottom side
	 0.5f, -0.5f,  0.5f,	0.83f, 0.70f, 0.44f,	0.0f, -1.0f, 0.0f,		// Bottom side
	
	-0.5f,  0.5f,  0.5f,	0.83f, 0.70f, 0.44f,	0.0f, 1.0f, 0.0f,		// Top side
	-0.5f,  0.5f, -0.5f,	0.83f, 0.70f, 0.44f,	0.0f, 1.0f, 0.0f,		// Top side
	 0.5f,  0.5f, -0.5f,	0.83f, 0.70f, 0.44f,	0.0f, 1.0f, 0.0f,		// Top side
	 0.5f,  0.5f,  0.5f,	0.83f, 0.70f, 0.44f,	0.0f, 1.0f, 0.0f,		// Top side

	 0.5f, -0.5f, -0.5f,	0.83f, 0.70f, 0.44f,	1.0f, 0.0f, 0.0f,		// Right side
	 0.5f, -0.5f, -0.5f,	0.83f, 0.70f, 0.44f,	1.0f, 0.0f, 0.0f,		// Right side
	 0.5f,  0.5f, -0.5f,	0.83f, 0.70f, 0.44f,	1.0f, 0.0f, 0.0f,		// Right side
	 0.5f,  0.5f,  0.5f,	0.83f, 0.70f, 0.44f,	1.0f, 0.0f, 0.0f,		// Right side

	 -0.5f,  0.5f,  0.5f,	0.83f, 0.70f, 0.44f,	-1.0f, 0.0f, 0.0f,		// Left side
	 -0.5f,  0.5f, -0.5f,	0.83f, 0.70f, 0.44f,	-1.0f, 0.0f, 0.0f,		// Left side
	 -0.5f, -0.5f, -0.5f,	0.83f, 0.70f, 0.44f,	-1.0f, 0.0f, 0.0f,		// Left side
	 -0.5f, -0.5f,  0.5f,	0.83f, 0.70f, 0.44f, 	-1.0f, 0.0f, 0.0f,		// Left side

	 -0.5f, -0.5f,  0.5f,	0.83f, 0.70f, 0.44f, 	0.0f, 0.0f, 1.0f,		// Front side
	 -0.5f,  0.5f,  0.5f,	0.83f, 0.70f, 0.44f,	0.0f, 0.0f, 1.0f,		// Front side
	  0.5f,  0.5f,  0.5f,	0.83f, 0.70f, 0.44f,	0.0f, 0.0f, 1.0f,		// Front side
	  0.5f, -0.5f,  0.5f,	0.83f, 0.70f, 0.44f,	0.0f, 0.0f, 1.0f,		// Front side

	  -0.5f, -0.5f, -0.5f,	0.83f, 0.70f, 0.44f,	0.0f, 0.0f, -1.0f,		// Back side
	  -0.5f,  0.5f, -0.5f,	0.83f, 0.70f, 0.44f,	0.0f, 0.0f, -1.0f,		// Back side
	  0.5f,  0.5f, -0.5f,	0.83f, 0.70f, 0.44f,	0.0f, 0.0f, -1.0f,		// Back side
	  0.5f, -0.5f, -0.5f,	0.83f, 0.70f, 0.44f,	0.0f, 0.0f, -1.0f,		// Back side
};

GLuint cubeIndices[] = {
	0, 1, 2,	// Bottom side
	0, 2, 3,	// Bottom side
	4, 5, 6,	// Top side
	4, 6, 7,	// Top side
	8, 11, 10,	// Right side
	8, 10, 9,	// Right side
	12, 13, 14,	// Left side
	12, 14, 15,	// Left side
	16, 17, 18,	// Front side
	16, 18, 19,	// Front side
	20, 21, 22,	// Back side
	20, 22, 23	// Back side
};

GLfloat lightVertices[] = {
	// COORDINATES //
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f

};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

bool planeIntersection(glm::vec3 l0, glm::vec3 l, glm::vec3 n, glm::vec3 p0 ) {

	// l0 = ray origin
	// l = ray direction
	// n = plane normal
	// p0 = point on plane

	float epsilon = 1e-6f;
	float t = 0.0f;
	float denom = glm::dot(n, l);

	if (fabs(denom) < epsilon) return false;

	glm::vec3 p010 = p0 - l0;
	t = (glm::dot(p010, n)) / denom;
	if (t < 0) return false;

	glm::vec3 hitPoint = l0 + l * t;	// calculate hit point using ray's parametric eq

	std::cout << "Hit Point: ("
		<< hitPoint.x << ", "
		<< hitPoint.y << ", "
		<< hitPoint.z << ")\n";

	// Determine if hitPoint is within planeBoundaries

	// Statically using premade plane, corners of the plane
	glm::vec3 center = planeCenter;
	float halfWidth = 0.5f;
	float halfDepth = 0.5f;

	if (hitPoint.x >= (planeCenter.x - halfWidth) && hitPoint.x <= (planeCenter.x + halfWidth) &&
		hitPoint.z >= (planeCenter.z - halfDepth) && hitPoint.z <= (planeCenter.z + halfDepth))
	{
		//std::cout << "Hit inside finite plane\n";
		return true;
	}
	else {
		//std::cout << "Hit outside plane bounds\n";
		return false;
	}
};



int main() {

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //tells what version of GLFW we are using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGLFirstProject", NULL, NULL); //width, height, 1st two parameters
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // tells glfw to actually use the window, make it part of the current context


	gladLoadGL();	// loads needed configs for openGL


	glViewport(0, 0, width, height);	//viewport is the area we want to render openGL in , which in our case is the size of the window, else image will be squished


	// ------------------------------------------------------------------------------------------ CREATING SPHERE OBJECT --------------------------------------------------------

	// Vertices
	for (int i = 0; i <= stackCount; i++) {
		stackAngle = (glm::pi<GLfloat>() / 2.0f) - ((float)i * stackStep);


		for (int j = 0; j <= sectorCount; j++) {


			sectorAngle = (float)j * sectorStep;

			x = ((radius * cosf(stackAngle) * cosf(sectorAngle)));
			y = ((radius * cosf(stackAngle) * sinf(sectorAngle)));
			z = (radius * sinf(stackAngle));

			//Vertices
			sphereVertices[0] = x;
			sphereVertices[1] = y;
			sphereVertices[2] = z;

			//Normals, can assume center is at origin since calculations are done in local space
			sphereNormals[0] = (x * invRadius);
			sphereNormals[1] = (y * invRadius);
			sphereNormals[2] = (z * invRadius);

			//Colors, for now, just same, color, will have to change as we implement painting ability
			sphereColors[0] = R;
			sphereColors[1] = G;
			sphereColors[2] = B;

			sphereInfo.insert(sphereInfo.end(), sphereVertices, sphereVertices + 3);
			sphereInfo.insert(sphereInfo.end(), sphereColors, sphereColors + 3);
			sphereInfo.insert(sphereInfo.end(), sphereNormals, sphereNormals + 3);


		}
	}

	// Indices
	for (int i = 0; i < stackCount; i++) {

		k1 = i * (sectorCount + 1);
		k2 = k1 + sectorCount + 1;

		for (int j = 0; j < sectorCount; j++, k1++, k2++) {

			if (i != 0) {
				sphereIndices.push_back(k1);
				sphereIndices.push_back(k2);
				sphereIndices.push_back(k1+1);
			}

			if (i != (stackCount-1)) {
				sphereIndices.push_back(k1+1);
				sphereIndices.push_back(k2);
				sphereIndices.push_back(k2 + 1);
			}


		}
	}

	// --------------------------------------------------------------------------------------------------------------------------------------------------



	// Generates Shader object using shaders defualt.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	// TESTING CLASS FOR SHAPES, FIRST MAKE PYRAMID
	RenderableObject pyramid("pyramid", pyramidVertices, sizeof(pyramidVertices), pyramidIndices, sizeof(pyramidIndices));
	RenderableObject cube("cube", cubeVertices, sizeof(cubeVertices), cubeIndices, sizeof(cubeIndices));
	RenderableObject sphere("sphere", sphereInfo.data(), sphereInfo.size() * sizeof(GLfloat), sphereIndices.data(), sphereIndices.size() * sizeof(GLfloat));
	RenderableObject plane("plane", planeVertices, sizeof(planeVertices), planeIndices, sizeof(planeIndices));

	//----------------------------------------------------------------------------------------------------------

	// Shader for light cube
	Shader lightShader("light.vert", "light.frag");
	// Generates Vertex Array Object and binds it
	VAO lightVAO;
	lightVAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO lightVBO(lightVertices, sizeof(lightVertices));
	// Generates Element Buffer Object and links it to indices
	EBO lightEBO(lightIndices, sizeof(lightIndices));
	// Links VBO attributes such as coordinates and colors to VAO
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	// Unbind all to prevent accidentally modifying them
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);	// Completely white light color
	glm::vec3 lightPos = glm::vec3(500.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);	// how much to move our light 

	pyramid.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	cube.setPosition(offscreenPos);
	sphere.setPosition(offscreenPos);
	plane.setPosition(offscreenPos);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramid.model));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	// Timing to 60 fps for camera
	auto prev_time = 0.0f;

	glEnable(GL_DEPTH_TEST);	// Tell opengl to account for the depth of each triangle

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f), 45.0f);

	// 3d debug raycast line
	Line line1;
	line1.setColor(glm::vec3(1, 1, 1));

	// RENDER LOOP
	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);	// setting color of one of the buffers, not sure which
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear depth buffer bit 

		if (glfwGetTime() - prev_time >= 1.0f / 60.0f) {

			// Using this to ensure camera moves the same across all machines (as they have different fps
			camera.Inputs(window);
			prev_time = glfwGetTime();
		}

		camera.updateMatrix(45.0f, 0.5f, 100.0f);

		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		camera.Matrix(shaderProgram, "camMatrix");

		pyramid.draw(shaderProgram);	// MUST BE CALLED AFTER ACTIVATION OF SHADER PROGRAM
		cube.draw(shaderProgram);
		sphere.draw(shaderProgram);
		plane.draw(shaderProgram);


		// Handles mouse inputs
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		{
			color = { 1.0f, 0.0f, 0.0f };
		}
		if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		{
			color = { 0.0f, 1.0f, 0.0f };
		}
		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		{
			color = { 0.0f, 0.0f, 1.0f };
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && camera.mouseHeld == false)
		{
			glm::vec4 worldRay = camera.mosPosToWorldSpace(window);

			// Create debug line from camera position to world position
			glm::vec3 rayDir = glm::normalize(glm::vec3(worldRay));
			glm::vec3 end = camera.Position + glm::vec3(worldRay.x, worldRay.y, worldRay.z) * 10.0f;
			line1.setup(camera.Position, end);


			//Calculate intersection
			glm::vec3 planeNormalWS = glm::normalize(glm::mat3(glm::transpose(glm::inverse(planeModel))) * glm::vec3(0.0f, 1.0f, 0.0f));	// Convert planeNormal to world space (NEEDS CHECKING)
			//glm::vec3 planePointWS = glm::vec3(planeModel * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));	// Convert point on plane to World Space (NEEDS CHECKING)
			glm::vec3 planePointWS = PMath::localToWorldSpaceConv(planeModel,glm::vec3(-0.5f, 0.0f, 0.5f) );	// Convert point on plane to World Space
			//printVec3(planePointWS);


			//std::string intersect = planeIntersection(camera.Position, rayDir, planeNormalWS, planePointWS) ? "True" : "False";
			//std::cout << "Intersection: " + intersect << std::endl;


			pyramid.testIntersection(camera.Position, rayDir, color );
			cube.testIntersection(camera.Position, rayDir, color);
			sphere.testIntersection(camera.Position, rayDir, color);
			plane.testIntersection(camera.Position, rayDir, color);


			camera.mouseHeld = true;
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		{
			// Prevent making multiple lines on a click
			camera.mouseHeld = false;
		}		
		

		if (!objectSpawn) {
			if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
				// Show Pyramid

				pyramid.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

				// Hide others
				cube.setPosition(offscreenPos);
				sphere.setPosition(offscreenPos);
				plane.setPosition(offscreenPos);


				objectSpawn = true;
				std::cout << "Pyramid visible\n";
			}
			else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
				// Show Cube
				cube.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

				// Hide others
				pyramid.setPosition(offscreenPos);
				sphere.setPosition(offscreenPos);
				plane.setPosition(offscreenPos);


				objectSpawn = true;
				std::cout << "Cube visible\n";
			}
			else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
				// Show Sphere
				sphere.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

				pyramid.setPosition(offscreenPos);
				cube.setPosition(offscreenPos);
				plane.setPosition(offscreenPos);


				objectSpawn = true;
				std::cout << "Sphere visible\n";
			}
			else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
				// Show Plane
				plane.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

				pyramid.setPosition(offscreenPos);
				cube.setPosition(offscreenPos);
				sphere.setPosition(offscreenPos);

				objectSpawn = true;
				std::cout << "Plane visible\n";
			}
		}

		// Reset flag when key is released
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_RELEASE)
		{
			objectSpawn = false;
		}


		if (line1.created) {
			line1.setMVP(camera.projection * camera.view);
			line1.draw();
		}

		// Tells OpenGL which Shader Program we want to use
		lightShader.Activate();
		// Export the camMatrix to the Vertex Shader of the light cube
		camera.Matrix(lightShader, "camMatrix");
		// Bind the VAO so OpenGL knows to use it
		lightVAO.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();

	}

	// Delete all the objects we've created
	pyramid.Delete();
	cube.Delete();
	sphere.Delete();
	plane.Delete();
	shaderProgram.Delete();
	lightVAO.Delete();
	lightVBO.Delete();
	lightEBO.Delete();
	lightShader.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}