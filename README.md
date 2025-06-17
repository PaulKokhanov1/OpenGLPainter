# 3D Painter Application — Project Summary Template

## Project Overview
Many people have talked about how awful, time consuming, incomprehensible OpenGL can be to learn. However, in my experience, the countless times I banged my head, looked at the screen in disbelief that THIS is what I need to do to draw a simple triangle and questioned my ability as a programmer made me COMPLETELY agree with them.

In reality, I actually thoroughly enjoyed the process of learning the basics of openGL to create this project. I hope to continue iterating on this project and learning more and more skills that I can apply to my game development journey. This project was my take on a 3D painting tool made using modern openGL in C++. The program allows users to “paint” objects with different colors by clicking on the respective objects’ surface. I leveraged ray casting, GPU buffer manipulation, and ray-object interaction algorithms to create this project.

---

## Technologies Used
- C++
- OpenGL
- GLFW
- GLM

---

## Core Features
- VBO, VAO and EBO classes to simplify creation and management of 3D objects
- Interactive 3D ray picking: Maps mouse clicks to world-space rays and determines intersections with scene geometry.
- Triangle-level intersection using Möller–Trumbore algorithm and object-level interaction using Bounding Sphere’s
- Real-time vertex color modification based on ray hits
- Debug rendering of intersection rays
- Modular math utility class (PMath) for reusable vector and matrix operations

---

## Technical Challenges & Solutions

| Challenge | Solution |
| --------- | -------- |
| During creation of debug line, passing value of w = 1 into the ray direction making it a point | As OpenGL uses homogenous coordinate systems (x, y, z, w) , w = 1 for points and w = 0 for directions |
| Ray–triangle intersections failing on some triangles | Needed to correct the edge cases in the Möller–Trumbore algorithm (e.g., t < 0, backface culling) |
| Updating only some vertex colors didn’t reflect | Had to ensure proper binding of the associated VBO and use of glBufferData() |
| Tracking coordinates of vertices across each coordinate space | Instead of trying to access the vertex shader’s calculation, simply repeating the calculation outside the shader |

---

## Key Learning Outcomes
- I familiarised myself with the moden OpenGL rendering pipeline and how to manipulate vertex data in memory dynamically.
- I understood the various coordinate space transformations and that it is most useful to perform ray-intersections in world space.
- I learned how to implement a fully custom ray casting system in 3D from scratch.
- I grasped different intersection algorithms and methods used to determine choosing which triangle is currently selected in the scene.
- I discovered how to create and manipulate a “camera” object within OpenGL and how to include perspective

---

## Rants/ Informal discussions

Now that the formalities are out of the way, I can get into more of a discussion of the challenges, the successes and overall what I think about OpenGL.

Honestly, I chose this project because I envy the work that engineers do in engine creation, and a big part of that is rendering, In the future, I also want to experiment with the physics programming. Regardless, this was all in an effort to learn more about how we create, manage engines and provide tools to higher-level programmers to use in gameplay creation.

### OpenGL basics

To begin, the idea to reserve memory using VBO’s (vertex buffer objects) is a common idea that I have seen across different aspects of C++, for example creating raw arrays and needing to define their size. In the case of VBO’s we create them to be able to send a large number of vertices to the GPU’s memory rather than doing it one by one (because we already reserved the memory). Now in using the vertex data, we use VAO’s (Vertex array objects) to create a centralized place where we define our vertex input setup so that the vertex shaders can easily read and use it. Finally, one other super useful object is the EBO (Element Buffer Object), these simply allow use to define which vertices to reuse instead of needing to rewrite the vertices in our VBO’s. These abstractions definitely make creating and storing primitives (shapes) much easier, and honestly its interesting to see the whole process of creating a simple object.

Then, we move onto the space transformations. This is simply different ways in which the shapes/objects exist in our program and can be useful for manipulating them at various points in the pipeline. For example, when I did my ray-intersection testing, this was all performed in world space. But anyways, the process goes as so:

```
Local -> World -> View -> Clip -> Screen
```

Without going into much detail to each of them, basically the idea is, starting at local space, we define and create our object where it is the ONLY thing in our scene and everything revolves around it (literally because its positioned at the origin in that space) , then we move through the spaces to slowly combine it with other objects and aspects of our scene to then have it be able to be seen from a user's perspective who is our “camera”.

The transformation’s are done using matrix multiplication. Specifically,

```
V_clip = M_projection * M_view * M_model * V_local
```

Where M is a matrix and V is a vector. For example, the Model matrix would contain translations, scaling and/or rotations we apply to each of the vertices. Then the view model will move the scene as we want it to be seen from our camera. Finally, the projection matrix is used to either create orthographic projection or perspective projection.

Though I’m not here to reiterate the things that can be found reading any OpenGL tutorial online, so I will be brief on these concepts and only really point out the ones that were interesting/new to me. Having said that, a cool idea when creating a “camera” object was that the scene moves around the camera, not the camera moving around the scene. Basically, we first find 3 perpendicular axes respective to the camera in world space, and using those axes we create what’s called a “LookAt” matrix that we use to transform all needed objects into the camera/view space simply through multiplication!

### Ray-tracing

Moving on to a little more mathematical concepts specific to this project. I’d like to talk about 3 things that I used my GREAT … PHENOMENAL … UNBEATABLE waterloo mathematics knowledge for.

Firstly, the raycast. Raycasts are just an infinite line from a point that we cast using a position and a direction. I couldn’t tell you all the uses for them, but for the most part, they’re used for intersection detection. And that’s exactly what I used them for in this project.

Since my intention was to check intersections, I needed to convert the mouse coordinates to a world-space ray. This is where I faced my first challenge of trying to access the needed matrices to convert from raster space (or for simplicity let’s just say the viewport) to world space. I realized that I could not access the vertex shader and instead needed to manually hold each of these matrices. I wonder if I just missed something or if this is the typical way to work backwards through the coordinate systems.

The process was as follows:

```
Viewport to NDC:
x_ndc = (2 * x_window) / width - 1
y_ndc = (2 * y_window) / height - 1

NDC to view space:
d = 1 / tan(FOV)
x_view = x_ndc / d
y_view = y_ndc / d * aspect_ratio
z_view = 1

View to world:
w_world = inverse(M_view) * v_view
```

Where `v_view` is the resulting vector of our previous calculations.

Following these steps I then was able to create a simple debug line to visualize the raycasts and see if they were behaving as intended.

Then, moving to intersection, I first started by creating a simple plane and testing intersection using:

![image](https://github.com/user-attachments/assets/66471493-18d5-4450-a0b8-7360adaf95b9)


Where l_0 is the ray origin, l is the ray direction, p_0 is a point on the plane, n is the plane normal, which are all used to determine t.

But what’s this variable used for you may ask? Well its actually pretty understandable. We define a point on a line with the following parametric equation:

```
P = O + D * t
```

Where P is a point on the ray's half-line, O is the ray's origin, and D is the ray's direction, so we use t to find the point on the planes line.

Now one case I had trouble with was that I was intersecting with the plane even when not looking at it. This is due to the fact that t can be NEGATIVE, meaning the intersection algorithm is checking the line in both directions. Thus, a simple early return if t is negative would solve the issue.

So, after some head banging in managing converting points between coordinate spaces, the plane intersection actually worked well, and it was time to move onto extending this intersection logic to ALL shapes.

I discovered that a common practice is to create either bounding sphere or bounding boxes. I opted for bounding spheres because we can conduct a much faster intersection test, it only requires the shapes center in world space and the radius of its furthest point. Furthermore, we also don’t have to take rotation into account even though in my current shape configuration I do not rotate the objects.

I realized that to change the shapes colors I would need to get the vertex data of the shapes, however, it would be wayyyy too costly to perform this for every shape every time a ray is cast. This was the intuition behind choosing to encompass each object in a bounding sphere to perform a quick check on all existing objects in the scene and see which were intersected by the ray. Then, on the intersected objects, we could perform our triangle intersection algorithm.

I looked through a couple options, such as Plane Equation, or Watertight Ray-triangle intersection algorithms. But opted for the Möller–Trumbore Algorithm, due to its barycentric coordinate output. Barycentric coordinates are a coordinate system that we use to express a point in a triangle. The basic idea is that the sum of the coordinates will never exceed 1 if the point is within the triangle, which allows for an easy check to see if the intersection occurred on that triangle. Furthermore, with the MT algo, we have the ability to cull back-facing triangles or not, depending on if the determinant is negative. Specifically, the equation we must solve is as follows:

![image](https://github.com/user-attachments/assets/97ac8f2f-36a8-4303-ac04-b7729cafbf49)
![image](https://github.com/user-attachments/assets/212b4cc6-82a2-48a4-abda-bd0c74bc842f)

where we are checking the intersection of triangle ABC. Then in the case we do hit a triangle, there is the possibility that we hit a second triangle further away from our camera, either from the same shape or hitting another shape. In this case we can simply take the smaller t value calculated.

Finally, to wrap things up, we will pass our found triangle vertices to the main function and update the raw pointer declared for each of these shapes, and adjust the color of each vertex in that triangle to the current chosen color. Though, despite being near the end, I still had a final head bang left when I forgot to bind the VBO AND VAO, not just the VBO, as we need to ensure our attributes defined for our vertex information is matching correctly.

---

## Conclusion

So all in all, this project was a great challenge to do over a couple weekends. Honestly, I think I will continue working on this project, but create this into a full engine. The one thing I noticed in implementing all of these functions from scratch is that doing so REALLY makes this like a sandbox game. I get to determine exactly what I want to do and create the necessary functionality for it! I’m not limited to existing functionality and as it is still a small engine, I completely understand each aspect of it and can play with it as I like.

I know this passion of mine for game development and working specifically with the engine will allow me to continuously learn, and create the underlying logic that will allow programmers, artists, designers alike to create phenomenal games. I want to be a jack of all trades but dive very deeply into one concept within engine development, and this project was my first few steps towards achieving that goal.

