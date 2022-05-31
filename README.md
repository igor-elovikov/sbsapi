# Substance Designer API
Relatively thin wrapper around Substance Material SDK

**Early WIP**

Features:
* Slightly different structure. Graphs and their instances are just _Graphs_. It's just graph can be instanced or not
* Graphs have _Parameters_ and _Inputs_. Basically parameter is a numerical input, and input is an image one
* Rendering into memory and saving results via OIIO

Things to do:
* Async rendering
* Native versions (not the blend one)
* Saving settings 
* Proper error handling and logging
* Ability to create separate graph instances
* Support for package stack
* Parameters visibility should be cached (or be a user responsibility)
* Examples
* vcpkg manifest
* C#?

### Dependencies

All dependencies can be installed from vcpkg (including third-party coming with the SDK)
* boost::hana
* spdlog
* range-v3
* pybind11
* OpenImageIO
* utf8cpp
* tinyxml

### Build

You need an environment variable `SBS_SDK_PATH` pointing to Substance Material SDK directory.

Currently, for Win32 `d3d11pc_blend` is used and for OSX it's `ogl3_blend` (can be configured in CMakeLists)

To use custom python installation for bindings uncomment defining `CUTOM_PYTHON_DIR` in CMakeLists and set it accordingly.

### C++
```c++
auto ctx = sbsar::Context();

auto& pkg = ctx.load_package("ring.sbsar");
auto& graph = pkg.graph("ring");

graph.parm("Radius").set(1.0f);
graph.render();
graph.output("Output").save("ring.png"); // OIIO backend
```

```c++
auto ctx = sbsar::Context();

auto& pkg = ctx.load_package("blur.sbsar");
auto& graph = pkg.graph("blur");

graph.input("Input").load_from_file("my_input.png") // OIIO backend as well
graph.parm("Intensity").set(10.0f);

graph.render();
graph.output("Output").save("blurred_input.png");
```

```c++
auto graph = pkg.graph("ring") // won't work, graphs can't be copied
auto& graph = pkg.graph("ring") // use this
```

```c++
// You can get parameter value as std::variant for all supported types (including std::monostate)
// the alias is sbsar::Value
auto value = pkg.graph("ring").parm("Radius").get(); // value is sbsar::Value

// Or explicitly
auto value = pkg.graph("ring").parm("Radius").get_as<float>(); // value is float

if (pkg.graph("ring").parm("Offset").is<sbs::Vec2Float>()) {
    auto offset = pkg.graph("ring").parm("Offset").get_as<sbs::Vec2Float>();
}
```


```c++
// Setting is the same
auto value = sbsar::Value(1.0f);
pkg.graph("ring").parm("Radius").set(value); // value is sbsar::Value

// There are also explicit overloads not using std::variant
pkg.graph("ring").parm("Radius").set(1.0f);
```


### Python 

**Ownership**

Context owns packages and package owns graph and their instances.

```python
import pysbsar as sbsar

ctx = sbsar.Context()

pkg = ctx.load_package("ring.sbsar")
graph = pkg.graph("ring")

graph.parm("Radius").set(1.0)
graph.render()
graph.output("Output").save("ring.png") # Using OIIO
```

```python
...
# Some of the parameters metadata
graph = pkg.graph("ring")

for parm in graph.parms():
    print(parm.label)
    print(parm.widget)
    print(parm.default_value)
    print(parm.min_value)
    print(parm.max_value)
    print(parm.slider_step)
    print(parm.is_visible)
```

Outputs and inputs are providing buffer object interface.
```python
from PIL import Image
import numpy as np

...

output = graph.output("Output")
image_data = np.array(output, copy=False) # you can grab output as numpy array

# Using PIL for saving image 
image = Image.fromarray(image_data) 
image.save("pil_image.png")
```

```python
from skimage import data

...

# the same with inputs
graph.input("input").load_from_array(data.astronaut()) # you can upload numpy array to any input
graph.render()
graph.output("output").save("output.png")
```

### C API

It's a layer on top of the C++ API. Though C++ API is very thin, so overhead is minimal.
Strings are passed as references to descriptor strings data (not the best idea probably)