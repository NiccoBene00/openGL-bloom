# Bloom Sci-Fi Corridor (OpenGL Project)

## Overview

This project is an OpenGL-based real-time rendering application developed in C++.  
It showcases a **Bloom post-processing pipeline** applied to a stylized **sci-fi scene** (for now) with emissive elements and dynamic lighting.

## Project Structure
bloom/
│
├── main.cpp # Main application and render loop
│
├── src/
│ ├── shader.cpp # Shader class implementation
│ └── glad.c # OpenGL loader (GLAD)
│
├── include/
│ ├── glad/ # GLAD headers
│ ├── GLFW/ # GLFW headers
│ ├── glm/ # GLM math library
│ └── stb_image.h # Image loading library
│
├── shaders/
│ ├── basic.vert # Vertex shader (scene)
│ ├── basic.frag # Fragment shader (lighting + emissive + HDR output)
│ ├── screen.vert # Screen quad vertex shader
│ ├── blur.frag # Gaussian blur shader
│ └── bloom_final.frag # Final composition shader
│
├── resources/
│ └── textures/ # Textures used in the scene
│
├── lib/ # Precompiled libraries (GLFW, etc.)
│
└── README.md # Project documentation

## Requirements

To build and run this project, you need:

- C++ compiler (GCC / MinGW recommended)
- OpenGL 3.3+
- GLFW3
- GLAD
- GLM
- stb_image

### Tested Environment

- Windows (MinGW64)
- OpenGL 3.3 Core Profile

  ## Build and Execute Instructions

From the root project folder, compile using:

```
g++ main.cpp src/shader.cpp src/glad.c -o bloom.exe -Iinclude -Llib -lglfw3 -lopengl32 -lgdi32
```

To exectute

```
./bloom.exe```
