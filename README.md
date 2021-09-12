# Raytracing

Does ray tracing and outputs an image.

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

By default it will compile in Release but you can compile it in Debug like this:

```sh
make clean # clean first
make run BUILD_TYPE=Debug
```
