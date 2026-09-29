# ReTo

Render auxiliary Tools

ReTo provides the following features:

- Camera
- Shader manager
- Picking functionality
- Object loader (triangle/quad mode)
- Spline navigation
- Texture manager

## Building

```bash
git clone https://github.com/vg-lab/ReTo.git
cd ReTo

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

In-source builds are not allowed, so always configure into a separate build directory. If `CMAKE_BUILD_TYPE` is not specified, it defaults to `Debug`, so make sure to build the `Release` version to get the best performance possible.

## CMake options

| Option | Default | Description |
| ------ | ------- | ----------- |
| `BUILD_SHARED_LIBS` | `ON` | Build ReTo as a shared library (set to `OFF` for static) |
| `BUILD_DOCS` | `ON` | Generate Doxygen documentation when Doxygen is found |
| `RETO_WITH_EXAMPLES` | `ON` | Build the programs in `examples/` (only when ReTo is the top-level project) |
| `RETO_WITH_TESTS` | `ON` | Build the tests in `tests/` (only when ReTo is the top-level project) |
| `RETO_OPTIONALS_AS_REQUIRED` | `OFF` | Treat optional dependencies as required |
| `RETO_WITH_COMPUTE_SHADERS` | `ON` | Enable compute shaders |
| `RETO_WITH_GEOMETRY_SHADERS` | `ON` | Enable geometry shaders |
| `RETO_WITH_TESSELATION_SHADERS` | `ON` | Enable tessellation shaders |
| `RETO_WITH_SUBPROGRAMS` | `ON` | Enable shader subprograms |
| `RETO_WITH_OCC_QUERY` | `ON` | Enable occlusion queries |
| `RETO_WITH_TRANSFORM_FEEDBACK` | `ON` | Enable transform feedback |

Other options are set automatically depending on whether extra libraries are found, and can be overridden:
 
| Option | Default | Description |
| ------ | ------- | ----------- |
| `RETO_USE_GLUT` | `ON` if GLUT is found | Enable GLUT support (used by some examples) |
| `RETO_USE_FREEIMAGE` | `ON` if FreeImage is found | Enable loading `Texture2D` from image files |

Example:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DRETO_WITH_EXAMPLES=OFF -DBUILD_DOCS=OFF
```

## Dependencies

ReTo resolves its dependencies through CMake, using `find_package()`. Dependencies are not downloaded automatically, so they must be installed on your system or made discoverable by adding their install location to `CMAKE_PREFIX_PATH`.

Optional features are enabled automatically when their dependency is found. Set `RETO_OPTIONALS_AS_REQUIRED=ON` to make configuration fail if any of them are missing.

## Testing

After building ReTo, you can run the unit tests from the build directory (requires Boost's Unit Test Framework and `RETO_WITH_TESTS=ON`):

```bash
ctest --test-dir build
```

The examples are also available in the build tree. Some of them need to find their shaders through the `RETO_SHADERS_PATH` environment variable, which must point to the examples directory (available on both the source and installation trees). For example, from the source directory:

```bash
RETO_SHADERS_PATH=examples/ ./build/examples/ReToDemoCubes
RETO_SHADERS_PATH=examples/ ./build/examples/ReToTextureReader examples/gmrv_grande.png
```

## Using ReTo in your project

`ReTo` and `ReTo::ReTo` refer to the same CMake library target; the namespaced form is recommended for downstream projects.

### With `find_package`

Install ReTo, then consume it:

```bash
cmake --install build --prefix /your/prefix
```

```cmake
find_package(ReTo REQUIRED)
target_link_libraries(my_app PRIVATE ReTo::ReTo)
```

Add `-DCMAKE_PREFIX_PATH=/your/prefix` when configuring your project if the prefix is not a default search location.

The installed package also provides the `reto_generate_shaders.cmake` helper module.

### With `FetchContent`

```cmake
include(FetchContent)
FetchContent_Declare(
  ReTo
  GIT_REPOSITORY https://github.com/vg-lab/ReTo.git
  GIT_TAG        master  # pin to a tag or commit
)
FetchContent_MakeAvailable(ReTo)

target_link_libraries(my_app PRIVATE ReTo::ReTo)

list(APPEND CMAKE_MODULE_PATH "${ReTo_BINARY_DIR}")
```

## Documentation

The API documentation is generated from the source with Doxygen. To build it locally (requires Doxygen and `BUILD_DOCS=ON`):

```bash
cmake --build build --target ReTo_doxygen
```

The HTML output is written to `build/docs_doxygen/html`.

## License

ReTo is released under the [GNU General Public License v3.0](LICENSE.txt).

Developed at the [Visualization and Graphics Lab (VG-Lab)](https://github.com/vg-lab).
