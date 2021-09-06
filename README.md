# CPU Raytracing

Does raycasting on the CPU and outputs an image.

## Building

Make sure to clone the repository recursively to get the submodules:

```
git clone https://github.com/Calbabreaker/raytracing --recursive --shallow-submodules
```

### Unix

You need to have a c++ compiler, cmake and make installed.

This will compile and run the code:

```sh
make run
```

You can set BUILD_TYPE to build as release:

```sh
make clean # clean first
make run BUILD_TYPE=Release
```
