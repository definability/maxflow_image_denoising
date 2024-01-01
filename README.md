# Max-Flow Image Denoising

Example of using the [Boykov-Kolmogorov Max-Flow/Min-Cut algorithm]
implementation from [The Boost Graph Library] for image denoising.

To dive in the implementation details,
check out
[my Medium post][Binary Image Denoising with The Boost Graph Max-Flow]
dedicated to this repository.

## Dependencies

The project depends on CMake (at least [3.12][CMake 3.12]),
[OpenCV] 4, and The Boost Graph Library
(tested with versions [1.82][BGL 1.82] and [1.74][BGL 1.74]).

If you use `apt`, which is the default package manager
for Ubuntu, Debian, and many others, use
```shell
sudo apt update
sudo apt install -y libopencv-dev libboost-graph-dev
```

Under Windows 11, the code was tested
with OpenCV 4.8.0 and Boost Graph 1.82.0
installed via [vcpkg].

## Build

The project benefits from CMake.
With [CMake 3.13][CMake 3.13 manual] or newer,
you can easily build the project using
```shell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build
```
In earlier versions, the `-S` parameter is not available.
So, if you use [CMake 3.12][CMake 3.12 manual], build the project using
```shell
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build
```

If you use [vcpkg], the things are trickier.
Read [vcpkg in CMake projects] for more details
or use an IDE that supports [vcpkg].

## Use

The application accepts three positional command-line arguments:
```shell
maxflow_image_denoising <input image> <output image> <discontinuity penalty>
```
The paths can be relative or absolute.
The discontinuity penalty (smoothness term) must be a non-negative integer.

The program contains the input arguments validation.

## License

The project uses [MIT license],
so you can freely use the source code in pet projects,
commercial projects, and home assignments.
You can also modify it, distribute it, and sell it.
Note that the project is provided "as is",
so the author is not responsible for your usage of it.

[Boykov-Kolmogorov Max-Flow/Min-Cut algorithm]:
  https://discovery.ucl.ac.uk/id/eprint/13383/1/13383.pdf
[Binary Image Denoising with The Boost Graph Max-Flow]:
  https://medium.com/@valeriy.krygin/binary-image-denoising-with-the-boost-graph-max-flow-6ec817d62858
[The Boost Graph Library]:
  https://www.boost.org/doc/libs/1_80_0/libs/graph/doc/index.html
[CMake 3.12]:
  https://www.kitware.com/cmake-3-12-0-available-for-download
[CMake 3.12 manual]:
  https://cmake.org/cmake/help/v3.12/manual/cmake.1.html
[CMake 3.13 manual]:
  https://cmake.org/cmake/help/v3.13/manual/cmake.1.html
[OpenCV]:
  https://opencv.org
[BGL 1.74]:
  https://www.boost.org/doc/libs/1_74_0/libs/graph/doc/index.html
[BGL 1.82]:
  https://www.boost.org/doc/libs/1_82_0/libs/graph/doc/index.html
[vcpkg]:
  https://vcpkg.io
[vcpkg in CMake projects]:
  https://learn.microsoft.com/en-us/vcpkg/users/buildsystems/cmake-integration
[MIT License]:
  https://choosealicense.com/licenses/mit