## Lines
Draw moving lines on screen using OpenGL.

Made to have experience with `GL_STREAM_DRAW` (see [`GLBufferData()`](https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferData.xhtml))

To build run `./build.sh` or
```shell
g++ main.cpp -o a.out -Wall -lglfw -lGL -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -lm
```

Usage: `./a.out [speed] [scale] [movement strength]`

Configure in `main.cpp`
