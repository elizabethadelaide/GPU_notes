# GPU_notes
Just some notes on OpenGL and CUDA in C++, may make a real project later

# Current progress

Can display triangles with shaders. Setup ImGUI to ease debugging.

Example perspective view of two squares with fragment shader showing sine of position.

![Gl Example](/images/sineshader.jpg)

# OpenFOAM data visualizer idea

Parse OpenFOAM foam files/folder, have base streamer classes to display vector information such as pressure and velocity information. Have faster 