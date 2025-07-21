
# Building a Raytracer in Modern C  
A blog-style journey through systems programming performance tuning and clean code design.

---

##  Overview

This project began as a weekend experiment with The Ray Tracer Challenge book and grew into something far more ambitious: building a fast maintainable and extensible raytracer in modern C. The primary goals are:

This project is written in **C++20** and has been extensively tested using the **Google Test** framework.

- Learn-by-building: From math to materials implement it all from scratch.
- Optimize deeply: Understand where performance goesnot just guess.
- Apply real systems techniques: memory locality vectorization threading API design.

This README doubles as a technical devlog documenting the projects evolution.

---

##  Part 1: Naive Baseline

###  First Principles

At its core raytracing is about tracing rays into a scene and computing intersections:

```cpp
const auto a  ray.direction_.dot(ray.direction_)
const auto b  2  ray.direction_.dot(sphereToRay)
const auto c  sphereToRay.dot(sphereToRay) - radius  radius
const auto discriminant  b  b - 4  a  c
```

###  Initial Architecture

The early version followed a traditional object-oriented structure:

```
libraries/
 Utility/       # Math primitives (Tuple Matrix Ray)
 Geometry/      # Shapes: Sphere Plane Cube etc.
 Material/      # Surface properties lighting
 Pattern/       # Procedural textures
 Scene/         # Camera world lights
 Canvas/        # Image output
```

The focus was on correctness and readability. Each shape had its own `intersect()` and `normalAt()` methods and all scene logic flowed from that. Performance was... acceptable for small images.

###  Render Loop

A simple single-threaded render loop:

```cpp
Canvas Camera::render(const World world) 
    Canvas image(width_ height_)
    for (size_t y  0 y  height_ y) 
        for (size_t x  0 x  width_ x) 
            Ray ray  rayForPixel(x y)
            Color color  world.colorAt(ray)
            image.pixelWrite(color x y)
        
    
    return image

```

This worked great... until scenes grew past 200x200 resolution. Render times ballooned and it was clear that naive design wouldnt scale.

---

##  Part 2: Profiling  Bottlenecks

###  First Profiling Pass

Performance issues arent solved by guesswork. I used [`samply`](https://github.com/mstange/samply)  Firefox Profiler for detailed traces.

```bash
g -O3 -g -marchnative -fno-omit-frame-pointer ...
samply record ./TestPrograms/ConcentricGlassSpheres profile.json
open https://profiler.firefox.com/
```

###  What I Found

1. Excessive memory allocations  
   - Millions of `std::vectorIntersection` constructions per frame.
   - Many created populated and discarded without reuse.

2. Function call overhead  
   - Small math functions like `dot()` and `magnitude()` werent inlined.
   - Virtual functions slowed things down in hot loops.
   - Compiler couldnt auto-vectorize due to abstraction.

---

##  Part 3: Concurrency Wins

###  First Parallel Pass

The raytracers pixel loop is embarrassingly parallel. I added basic multithreading with the STL parallel execution policies:

```cpp
std::vectorsize_t pixelIndices(width_  height_)
std::iota(pixelIndices.begin() pixelIndices.end() 0)

std::for_each(std::execution::par_unseq
              pixelIndices.begin() pixelIndices.end()
              [this image world](auto idx) 
    size_t x  idx  width_
    size_t y  idx / width_

    std::vectorIntersection intersections
    Ray ray  rayForPixel(x y)
    Color color  world.colorAt(ray intersections)
    image.pixelWrite(color x y)
)
```

###  Thread Safety

Key decisions:
- Each thread uses its own local buffer (no shared vectors).
- Avoided heap contention and false sharing.
- No need for locks or mutexes.

This was a quick win and provided a visible speedup immediately.

---

##  Part 4: Memory Optimization (WIP)

This part of the project is still under construction but heres the current plan and progress.

###  The Problem

Every intersection function used to return `std::vectorIntersection` resulting in constant heap churn. For a 500x500 render thats hundreds of thousands of allocations.

###  The Fix (In Progress)

Redesign all intersection APIs to use output parameters and vector reuse.

Also moving toward arena allocation per-thread for transient allocations.

###  TODO
- Refactor all shape classes to use output vectors.
- Add a thread-local arena allocator.
- Eliminate redundant sorting in grouped shapes.
- Benchmark post-refactor.

---

##  Part 5: Math Inlining  Vectorization

###  STL Overhead

Generic algorithms like `std::transform_reduce` are greatbut only when the compiler can inline and optimize them. For small fixed-size vectors (like `Tuple4`) manual math is better.

###  Refactored Math API

Converted `Tuple` operations to inline header-only methods:

```cpp
inline double dot(const Tuple rhs) const noexcept 
    return x()rhs.x()  y()rhs.y()  z()rhs.z()  w()rhs.w()

```

This enabled:
- Inlining
- Compiler auto-vectorization
- Better register allocation
- Fewer cache misses

---

##  Future Work

Planned improvements:

- Add bounding volume hierarchies (BVH) for accelerated geometry traversal.
- Support triangle meshes and complex imported models.
- Add a per-thread arena allocator for temporary structures.
- Replace STL execution policies with a custom thread pool.
- Add support for environment maps and physically based materials.
- Explore real-time acceleration using the a GPU backend like CUDA or Metal.

---

##  Sample Scenes

This raytracer supports:

- Reflections and refractions
- Geometric primitives: spheres cubes planes cones cylinders
- Procedural patterns: stripes gradients checkers
- Scene graphs with grouped transforms

###  Example Renders

 TODO: Add screenshots

```bash
# Render example
./TestPrograms/ConcentricGlassSpheres
```

---

##  Build  Platform Support

###  Requirements

- CMake  3.20
- C++20 compiler (tested with GCC Clang)
- External dependencies: [Google Test](https://github.com/google/googletest), [oneTBB](https://github.com/oneapi-src/oneTBB)

###  Build

```bash
git clone https://github.com/yourusername/raytracer
cd raytracer
mkdir build  cd build
cmake -DCMAKE_BUILD_TYPERelease ..
make -j(nproc)
```

---

##  Project Structure

```
Raytracer/
 libraries/
    Utility/
    Geometry/
    Material/
    Pattern/
    Scene/
    Canvas/
 TestPrograms/
 test/
 CMakeLists.txt
 README.md
```

---

##  Final Thoughts

What started as a learning project became an opportunity to think like a performance engineer. Some takeaways:

- Abstractions are helpfulbut in hot paths you pay for what you use.
- Memory layout cache locality and reuse matter more than people think.
- Profiling reveals real problems. Always measure.

If youre interested in systems programming rendering or performance work raytracing is a surprisingly deep and rewarding project.
