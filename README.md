# OpenGL 3D Painter

<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a name="readme-top"></a>
<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->



<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![LinkedIn][linkedin-shield]][linkedin-url]




<br />
<div align="center">
  <a href="https://github.com/PaulKokhanov1/BindingofIssacRemake">
    <img src="https://github.com/user-attachments/assets/b5a48ef1-6436-4c3c-9e50-f7086b3ab4a0" alt="Logo" width="280" height="200">

  </a>

<h3 align="center">OpenGL 3D Painter</h3>

  <p align="center">
    Yes I am still doing more game dev! And you know I wont stop anytime soon ;) . But this time I took the challenge of jumping into engine development to further my C++ knowledge and learn how to create tools for programmers to use.
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#goal">Goal</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li><a href="#project-overview">Project Overview</a></li>
    <li><a href="#technologies-used">Technologies Used</a></li>
    <li><a href="#core-features">Core Features</a></li>
    <li><a href="#challenges-and-solutions">Challenges and Solutions</a></li>
    <li><a href="#key-learning-outcomes">Key Learning Outcomes</a></li>
     <li>
      <a href="#rant-and-informal-discussion">Rant and Informal discussion</a>
      <ul>
        <li><a href="#opengl-basics">OpenGL basics</a></li>
      </ul>
      <ul>
        <li><a href="#ray-tracing">Ray-tracing</a></li>
      </ul>
    </li>
    <li><a href="#conclusion">Conclusion</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>

<!-- GOAL -->
## Goal

My intentions with this project was to create a simple fishing mechanic that can be used in various 3D games and can easily be expanded whilist using my knowledge of genetic algorithms to create an ecosystem within the game. I also really wanted to get familiar with Unreal Engine and how to integrate direct C++ coding into the workflow.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



### Built With

* [![C++][Cpp.com]][Cpp-url]
* [![OpenGL][Opengl.com]][Opengl-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>

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

## Challenges and Solutions

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

## Rant and Informal discussion

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

<!-- RAY TRACING -->
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

<!-- CONCLUSION -->
## Conclusion

So all in all, this project was a great challenge to do over a couple weekends. Honestly, I think I will continue working on this project, but create this into a full engine. The one thing I noticed in implementing all of these functions from scratch is that doing so REALLY makes this like a sandbox game. I get to determine exactly what I want to do and create the necessary functionality for it! I’m not limited to existing functionality and as it is still a small engine, I completely understand each aspect of it and can play with it as I like.

I know this passion of mine for game development and working specifically with the engine will allow me to continuously learn, and create the underlying logic that will allow programmers, artists, designers alike to create phenomenal games. I want to be a jack of all trades but dive very deeply into one concept within engine development, and this project was my first few steps towards achieving that goal.

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- CONTACT -->
## Contact

Paul Kokhanov - [Website](https://paulkokhanov.com/) - paul.kokhanov@gmail.com

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

* [LearnOpenGL](https://learnopengl.com/Introduction)
* [Scratchapixel](https://www.scratchapixel.com/index.html)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/github_username/repo_name.svg?style=for-the-badge
[contributors-url]: https://github.com/github_username/repo_name/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/github_username/repo_name.svg?style=for-the-badge
[forks-url]: https://github.com/github_username/repo_name/network/members
[stars-shield]: https://img.shields.io/github/stars/github_username/repo_name.svg?style=for-the-badge
[stars-url]: https://github.com/github_username/repo_name/stargazers
[issues-shield]: https://img.shields.io/github/issues/github_username/repo_name.svg?style=for-the-badge
[issues-url]: https://github.com/github_username/repo_name/issues
[license-shield]: https://img.shields.io/github/license/github_username/repo_name.svg?style=for-the-badge
[license-url]: https://github.com/github_username/repo_name/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/paulkokhanov
[product-screenshot]: https://github.com/PaulKokhanov1/LunarLanderRL/assets/69466838/6ed735d2-09d7-42f3-bfe3-01473ff121c9
[product-screenshot-original]:https://github.com/PaulKokhanov1/LunarLanderRL/assets/69466838/efdba43a-cc5b-4a62-b447-c9cd2fac6ba2

[Next.js]: https://img.shields.io/badge/next.js-000000?style=for-the-badge&logo=nextdotjs&logoColor=white
[Next-url]: https://nextjs.org/
[React.js]: https://img.shields.io/badge/React-20232A?style=for-the-badge&logo=react&logoColor=61DAFB
[React-url]: https://reactjs.org/
[Vue.js]: https://img.shields.io/badge/Vue.js-35495E?style=for-the-badge&logo=vuedotjs&logoColor=4FC08D
[Vue-url]: https://vuejs.org/
[Unreal.com]: https://img.shields.io/badge/Unreal_Engine_5-blue?style=for-the-badge&logo=unrealengine&logoColor=black
[unreal-url]: https://www.unrealengine.com/en-US/unreal-engine-5
[Cpp.com]: https://img.shields.io/badge/C%2B%2B-white?style=for-the-badge&logo=cplusplus&logoColor=blue
[Cpp-url]: https://isocpp.org/
[Opengl.com]: https://img.shields.io/badge/Opengl-orange?style=for-the-badge&logo=opengl&logoColor=%23FFFFF&labelColor=%2300000
[Opengl-url]: https://www.opengl.org/
[CSharp.com]: https://img.shields.io/badge/C%23-%23512BD4?style=for-the-badge&logo=csharp&logoColor=white
[CSharp-url]: https://marketplace.visualstudio.com/items?itemName=ms-dotnettools.csdevkit
[Unity.com]: https://img.shields.io/badge/unity-0769AD?style=for-the-badge&logo=unity&logoColor=white
[Unity-url]: https://unity.com/
[Gimp.com]: https://img.shields.io/badge/GIMP-FFE01B?style=for-the-badge&logo=gimp&logoColor=%235C5543&color=FFE01B
[Gimp-url]: https://www.gimp.org/
