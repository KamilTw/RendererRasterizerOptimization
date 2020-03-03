# Renderer rasterizer optimization
Optimized application from https://github.com/KamilTw/RendererRasterizer. Rendering time
was optimizaed **from 11.8 to 20.9 fps** (frames per second) on average. It **increases calculations efficiency by 76%**.
Different optimization techniques was used.
</br>

1. Data-oriented design. Using vectors to store most of the data. Vectors allow data addresses to be sequential. Calculations are exectured iterating
on simillar data at one time. It allows to decrease cache misses and process data more efficient.
</br>


2. Compiler intrinsic functions. Using compiler intrinsic functions to optimize mathematical operations.
- _mm_mul_ps
- _mm_set1_ps
- _mm_max_ps
- _mm_set_ps
- and other functions
</br>

3. Other techniques
- passing values by reference
- using printf instead of cout
- postpone variables declaration
- initialization instead of copying
- overloading += operators
- prefix operators
- inline functions
- return value optimization
</br>

# Image examples
Image before (left) and after optimization (right). There are no significant differences between them.

![alt text1](/RendererRasterizer/RendererRasterizer/Examples/ImageComparison.png)

<p align="center">
<img src="/RendererRasterizer/RendererRasterizer/Examples/SectionComparison.png">
</p>
