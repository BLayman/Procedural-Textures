# Program 3: Ray Tracer with Procedural Textures

## Description of Program

### Ray Tracing

The ray tracing aspect of my program works by storing triangle objects in a vector
called `world`. It then passes the `world` to the `render` function, which can use
either orthographic of perspective views. A camera is created, and rays from the camera
are passed to the `intersectTriangle` function along with the `world`, in order to find 
intersections. The closest triangle is found, and the color of that intersection pixel is computed
 using a combination of Phong shading, and base color calculation from textures. 
 
### Shading and Textures
 
#### Phong Shading and Barycentric Coordinates
 Texture and lighting calculations are made by the `FetchColor` and `Texture` classes.
  `FetchColor` has a method for calculating Phong shading based on the light position, intersection 
 position, and camera vector origin. It combines ambient, diffuse, and specular shading. Phong shading is always
  applied after the calculation of the base color. Another method in `FetchColor`, `getBaseColorBary`, calculates a base
  color values using Barycentric coordinates, and vertex color values.
 Finally, the other methods in `FetchColor` rely on the `Texture` class, which I used for procedural texture generation.
 
#### Procedural Textures
 The `Texture` class works by generating an array of random numbers with values between 0 and 1.
  Although 1 dimensional, the array also works for 2 dimensional grids of random
  numbers by setting the array length to the grid width squared (for a square grid), and using, "row * width + column"
   to access grid positions. The `lerp` method performs linear interpolation given two values, and
   the fraction of the way from one to the other. The method `sample1DTexture`
   is used to sample noise along one axis. The method `sample2DTexture` is used to 
   sample noise across 2 dimensions. Both sampling methods use a combination of linear interpolation,
   and smoothing with the cosine function to generate gradients between random values.
    
   The `FetchColor` class utilizes
   the `Texture` class in a number of ways. Its method `get1DTextureColor` uses one dimensional samples
   to generate color gradients in the x, y, and z directions. The method `get2DFractalSumColor` can be used
   for a variety of purposes. A simple 2D noise texture with linear interpolation and smoothing can be generated
   by setting the number of layers to 1. Adding additional layers allows for fractal patterns with decreasing scale.
   The parameter `turbulence` can be set to true to achieve a smokey effect, which works by shifting the noise function
   down, scaling it, and taking the absolute value. The `wood` parameter can be set to true to achieve a topographic result by multiplying the function
   by 10, and getting the decimal value left after subtracting its integer portion. Results can be further tweaked
   by changing some other variables:
   
   - baseScale: The starting scale of the noise function. Higher values correspond to zooming out on the function. 
   - offset: Sets one's reference point on the function. Can be changed to view different portions of the noise function.
   - baseAmp: Starting amplitude. An amplitude of 1 gives color values between 0 and 255 when using a single layer.
   - scaleMultiplier: Reduces scale by this factor for each layer.
   - ampMultiplier: Reduces amplitude by this factor for each layer, so you may want to use a high baseScale for layering. 
   
   See pdf, and SavedImages.zip for examples of textures generated.