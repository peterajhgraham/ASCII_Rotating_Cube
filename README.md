# ASCII Rotating Sphere

A C++ program that generates a rotating 3D sphere using ASCII characters. It is designed with dynamic shading and realistic rotation based on concepts within linear algebra such as dot product, vector normalization, as well as the projection and rendering of 3D points onto a 2D screen.

## Directory Structure

```
ASCII_Rotating_Sphere/
├── rotating_sphere.cpp
├── LICENSE
├── README.md
└── .gitignore
```

## Prerequisites

* **C++ Compiler**
* **Terminal** (Unix-like systems recommended)

## Usage

1. **Compile**:
   ```
   g++ -o rotating_sphere src/rotating_sphere.cpp -lm
   ```

2. **Run**:
   ```
   ./rotating_sphere
   ```

## Customization

* **Rotation Speed**: Adjust A and B increments

* **Light Direction**: Modify lightDir

* **Sphere Size**: Modify sphereRadius

## Notes
* Best viewed in a terminal with at least 160x44 characters.
