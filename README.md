# A Toy RayTracer

## Implemented Features

### Material
	1.Lambert
	2.Metal

### Texture
	1.Checker Texture
	2.Perlin Noise

### Multiple Thread Rendering
	Use std::thread to render in multiple threads.
	Generally a image with resolution at 1920 x 960 ,sample one , cost about 3 min. 
## Images
All images are rendering at 1 sample , so it looks ugly...

![Metal and Lambert_1920 x 1080 , 10 Sample](https://raw.githubusercontent.com/skmang/RayTracer/master/RayTracer/Image/rendering_1920x1080_10Sample.jpg)
![Random Spheres_1920 x 960 ,1 Sample](https://raw.githubusercontent.com/skmang/RayTracer/master/RayTracer/Image/RayTracer_Lambert%26Metal_balls.jpg)
![Checker texutre 0.5 length _800 x 400 , 1 Sample](https://raw.githubusercontent.com/skmang/RayTracer/master/RayTracer/Image/RayTracer_CheckerTexture_w_2PI.jpg)
![Checker texture 0.1 length_1920 x 960, 1 Sample](https://raw.githubusercontent.com/skmang/RayTracer/master/RayTracer/Image/RayTracer_CheckerTexture_w_10PI.jpg)
![Checker texture with perlin noise_800 x 400, 1 Sample](https://github.com/skmang/RayTracer/blob/master/RayTracer/Image/RayTracer_PerlinNoise.jpg)