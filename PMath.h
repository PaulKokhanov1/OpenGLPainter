#ifndef PMATH_CLASS_H
#define PMATH_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<vector>
#include<iostream>



class PMath {

public:
	static const GLfloat shapeVertexStrides;
	static const glm::vec3 offScreenPos;

	static bool solveQuadratic(const float& a, const float& b, const float& c, float& x0, float& x1)
	{
		float discr = b * b - 4 * a * c;
		if (discr < 0) return false;
		else if (discr == 0) x0 = x1 = -0.5 * b / a;
		else {
			float q = (b > 0) ?
				-0.5 * (b + sqrt(discr)) :
				-0.5 * (b - sqrt(discr));
			x0 = q / a;
			x1 = c / q;
		}
		if (x0 > x1) std::swap(x0, x1);

		return true;
	}

	static glm::vec3 localToWorldSpaceConv(glm::mat4 model, glm::vec3 point) {

		return glm::vec3(model * glm::vec4(point, 1.0f));
	};


};

#endif