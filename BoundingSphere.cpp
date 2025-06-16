#include"BoundingSphere.h"

BoundingSphere::BoundingSphere(const std::string& name, glm::vec3 centerWS, GLfloat radius, GLfloat verts[], size_t vertsCount, GLuint indices[], size_t indicesCount, glm::mat4* model)
{
	this->name = name;
	this->center = centerWS;
	this->radius = radius;
    this->model = model;

    // use a menaul insertion of the size of the raw array since in C++ when passing an array to func or constructor it will decay into a pointer
    for (int i = 0; i < vertsCount; i += 9) {
        glm::vec3 triangleCoords(verts[i], verts[i + 1], verts[i + 2]);
        this->vertices.push_back(triangleCoords);
    }

    for (int i = 0; i < indicesCount; i++) {
        this->indices.push_back(indices[i]);
    }

    // Used for Debugging
    printMeshData();
}

void BoundingSphere::rayIntersectionTest(glm::vec3 rayOrigin, glm::vec3 rayDir, int* resultIndexes)
{
    float t0, t1; // solutions for t if the ray intersects
    // Analytic solution
    glm::vec3 L = rayOrigin - center;
    GLfloat a = glm::dot(rayDir, rayDir);
    GLfloat b = 2 * glm::dot(rayDir, L);
    GLfloat c = glm::dot(L, L) - radius * radius;
    if (!PMath::solveQuadratic(a, b, c, t0, t1)) return;
    if (t0 > t1) std::swap(t0, t1);

    if (t0 < 0) {
        t0 = t1; // If t0 is negative, let's use t1 instead.
        if (t0 < 0) return; // Both t0 and t1 are negative.
    }

    glm::vec3 hitPoint = rayOrigin + rayDir * t0;	// calculate hit point using ray's parametric eq\

    std::cout << "Hit Point for shape: " << name << "("
        << hitPoint.x << ", "
        << hitPoint.y << ", "
        << hitPoint.z << ")\n";

    // Call MT Algo to test intersection on the meshes triangles
    for (int i = 0; i < indices.size(); i += 3) {
        glm::vec3 v0 = vertices[indices[i]];
        glm::vec3 v1 = vertices[indices[i + 1]];
        glm::vec3 v2 = vertices[indices[i + 2]];


        // HAVE TO TRANSLATE THE VERTICES TO WORLD SPACE
        glm::vec3 wv0 = PMath::localToWorldSpaceConv((*model), v0);
        glm::vec3 wv1 = PMath::localToWorldSpaceConv((*model), v1);
        glm::vec3 wv2 = PMath::localToWorldSpaceConv((*model), v2);


        if (MTAlgo(rayOrigin, rayDir, wv0, wv1, wv2)) {
            std::cout << "Intersection found on triangle:\n"
                << "Indices: [" << indices[i] << ", "
                << indices[i + 1] << ", "
                << indices[i + 2] << "]\n"
                << "v0: (" << wv0.x << ", " << wv0.y << ", " << wv0.z << ")\n"
                << "v1: (" << wv1.x << ", " << wv1.y << ", " << wv1.z << ")\n"
                << "v2: (" << wv2.x << ", " << wv2.y << ", " << wv2.z << ")\n";

            resultIndexes[0] = indices[i];
            resultIndexes[1] = indices[i + 1];
            resultIndexes[2] = indices[i + 2];

            return;
        }
    }

    return;
}

bool BoundingSphere::MTAlgo(const glm::vec3 & orig, const glm::vec3& dir, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
{
    //orig = ray origin
    // dir = ray dir
    // v0 - v2 are triangle vertices
    // t = distanace from ray origin to hit point
    // u, v are barycentric coords that define a point inside a unit triangle


    float t;
    glm::vec3 v0v1 = v1 - v0;
    glm::vec3 v0v2 = v2 - v0;
    glm::vec3 pvec = glm::cross(dir, v0v2);
    float det = glm::dot(v0v1, pvec);

    // If the determinant is negative, the triangle is back-facing.
    // If the determinant is close to 0, the ray misses the triangle.
    // If det is close to 0, the ray and triangle are parallel.
    if (det < kEpsilon) return false;
    float invDet = 1 / det;

    glm::vec3 tvec = orig - v0;
    u = glm::dot(tvec, pvec) * invDet;
    if (u < 0 || u > 1) return false;

    glm::vec3 qvec = glm::cross(tvec, v0v1);
    v = glm::dot(dir, qvec) * invDet;
    if (v < 0 || u + v > 1) return false;

    t = glm::dot(v0v2,qvec) * invDet;

    if (t < 0) return false;

    return true;

}

void BoundingSphere::printMeshData() {

    std::cout << "Center WS for object: " << this->name << " is (" << this->center.x << ", "
        << this->center.y << ", "
        << this->center.z << ")\n";

    std::cout << "Vertices for object: " << name << "\n";
    for (const auto& v : vertices) {
        std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
    }

    std::cout << "Indices for object: " << name << "\n";
    for (size_t i = 0; i < indices.size(); ++i)
        std::cout << indices[i] << ((i + 1) % 3 == 0 ? "\n" : ", ");
}