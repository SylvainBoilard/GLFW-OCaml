# GLFW-OCaml
A GLFW binding for OCaml.

## Usage
GLFW-OCaml is a pretty straight-forward binding from the original API. Please refer to [the GLFW manual](https://www.glfw.org/documentation.html) for detailed information. All functions and values are found in module `GLFW`.

### Naming conventions
Functions have the `glfw` prefix removed and their first letter uncapitalized. For example `glfwCreateWindow` would become `createWindow`.

Values have the `GLFW_` and where appropriate the `KEY_` or `MOD_` prefixes removed, are converted to CamelCase and typed according to their usage. For example `GLFW_KEY_LEFT_BRACKET` would become constructor `LeftBracket` of type `key` and `GLFW_NO_RESET_NOTIFICATION` would become constructor `NoResetNotification` of type `context_robustness`. Names with a single letter prefix directly appended such as `GLFW_IBEAM_CURSOR` become `IBeamCursor`.

### Mouse buttons and joysticks
Instead of defining values for individual mouse buttons and joysticks, a plain 0-indexed number is used, although there are `mouse_button_{left,right,middle}` values defined for the most common mouse buttons. The maximum number of mouse buttons and connected joysticks can be accessed with the `mouse_button_max_count` and `joystick_max_count` values.

### Window hints, attributes and input modes
Window hints, attributes and input modes make use of GADTs to retain the polymorphic aspect of the corresponding original functions while ensuring type correctness.

Unlike the other functions where you can omit the labels of arguments, you will find you have to use them with `windowHint`, `getWindowAttrib` and `setInputMode`. This is because the return type of these functions is polymorphic and the compiler will try to put any unlabeled arguments there. This makes for most peculiar errors.

### Error handling
Errors are propagated with exceptions instead of through a callback function. While there is a definition for an `InvalidEnum` exception, one should never be raised if you are not using unsafe features; otherwise that would be a GLFW-OCaml bug.

### Callbacks and user pointers
There is no need for user pointers in GLFW-OCaml as you can capture your data inside a closure and use that closure as a callback function while preserving type information.

When later versions of GLFW introduce user pointers for individual objects other than windows (such as joysticks and monitors) with a global callback shared by all objects of the same type, the plan for GLFW-OCaml is to instead allow setting a different callback per individual object so you can capture their related data individually in several closures for example.

### Gamma ramps
The `GammaRamp` module provides two functions `create` and `make` to create gamma ramps while insuring all channels have the same length. The former builds a gamma ramp from three user-supplied channels while the later builds an empty gamma ramp with three channels of the supplied length. You can freely access and modify the contents of an existing gamma ramp but are required to use one of these functions to build one.

### Return values
Some of the original functions return their result via pointer arguments. In GLFW-OCaml these functions instead return a tuple with the individual elements in the same order as they appear in the original function.

### `GLFW_DONT_CARE` and `NULL`-able values 
Wherever `GLFW_DONT_CARE` or a `NULL` pointer would be a legal value, an option type is used to wrap the value and `None` is used to represent `GLFW_DONT_CARE` or `NULL`.

### `glfwGetProcAddress`, `glfwExtensionSupported` and Vulkan
The `glfwGetProcAddress` function is not supported because it would require to write an entire OpenGL wrapper to make the functions returned by GLFW usable from OCaml. Incidentally the `glfwExtensionSupported` function is not provided either. There are numerous OpenGL bindings available for OCaml that you can use instead.

The Vulkan related functions are not supported as of now but we might look into it at some point.                                                                                  
