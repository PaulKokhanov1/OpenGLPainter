#include"Camera.h"
#include"Line.h"


Camera::Camera(int width, int height, glm::vec3 position, float FOV)
{
	Camera::width = width;
	Camera::height = height;
	Camera::FOV = FOV;
	Position = position;

}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	FOV = FOVdeg;

	// Makes camera look in the right direction from the right position
	view = glm::lookAt(Position, Position + Orientation, Up);
	// Adds perspective to the scene
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	// Sets new camera matrix
	cameraMatrix = projection * view;

}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	// Exports camera matrix
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

// Used for controling the camera's position
void Camera::Inputs(GLFWwindow* window){
	// Handles key inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += speed * Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position += speed * -glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position += speed * -Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += speed * glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Position += speed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Position += speed * -Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = 0.4f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		speed = 0.1f;
	}


	// Handle looking around
	
	// Stores the coordinates of the cursor
	double mouseX;
	double mouseY;
	// Fetches the coordinates of the cursor
	glfwGetCursorPos(window, &mouseX, &mouseY);

	// Hides mouse cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
	// and then "transforms" them into degrees 
	float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;	// adjust coordinates so that they start in the middle of the screen rather than the top left of the screen
	float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

	// Calculates upcoming vertical change in the Orientation
	glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

	// Decides whether or not the next vertical Orientation is legal or not
	if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
	{
		Orientation = newOrientation;
	}

	// Rotates the Orientation left and right
	Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

	// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
	glfwSetCursorPos(window, (width / 2), (height / 2));

	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && false == true)
	{
		glm::vec4 worldRay = mosPosToWorldSpace(window);

		// Create debug line from camera position to world position
		glm::vec3 end = Position + glm::vec3(worldRay) * 10.0f;

		mouseHeld = true;		
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Prevent making multiple lines on a click
		mouseHeld = false;
	}

}

glm::vec4 Camera::mosPosToWorldSpace(GLFWwindow* window)
{
	// Stores the coordinates of the cursor
	double mouseX;
	double mouseY;
	// Fetches the coordinates of the cursor (Viewport)
	glfwGetCursorPos(window, &mouseX, &mouseY);

	// Viewport to NDC
	float x_ndc = (2.0f * mouseX) / (float)width - 1.0f;
	float y_ndc = 1.0f - (2.0f * mouseY) / (float)height;

	// NDC to view space
	float x_view = x_ndc * tanf(glm::radians(FOV / 2.0f));
	float y_view = y_ndc * tanf(glm::radians(FOV / 2.0f)) * ((float)width / (float)height) ;
	float z_view = 1;
	glm::vec4 ray_view = glm::vec4(x_view, y_view, -z_view, 0.0f);

	// View space to world space
	glm::mat4 invView = glm::inverse(view);
	glm::vec4 ray_world = invView * ray_view;

	// THIS is camera pos
	glm::vec3 camPos = glm::vec3(glm::inverse(view)[3]);

	return ray_world;
}
