#include <string.h>
#include <GLFW/glfw3.h>
#include <caml/mlvalues.h>
#include <caml/alloc.h>
#include <caml/memory.h>
#include <caml/fail.h>
#include <caml/callback.h>

#define CAMLvoid CAMLunused_start value unit CAMLunused_end

#define Val_none Val_int(0)

#define CAML_SETTER_STUB(glfw_setter, name)                 \
    CAMLprim value caml_##glfw_setter(value new_closure)    \
    {                                                       \
        CAMLparam1(new_closure);                            \
        CAMLlocal1(previous_closure);                       \
                                                            \
        if (name##_closure == Val_unit)                     \
            previous_closure = Val_none;                    \
        else                                                \
        {                                                   \
            previous_closure = caml_alloc_small(1, 0);      \
            Field(previous_closure, 0) = name##_closure;    \
        }                                                   \
        if (new_closure == Val_none)                        \
        {                                                   \
            if (name##_closure != Val_unit)                 \
            {                                               \
                glfw_setter(NULL);                          \
                caml_remove_global_root(&name##_closure);   \
                name##_closure = Val_unit;                  \
            }                                               \
        }                                                   \
        else                                                \
        {                                                   \
            if (name##_closure == Val_unit)                 \
            {                                               \
                glfw_setter(name##_callback_stub);          \
                caml_register_global_root(&name##_closure); \
            }                                               \
            name##_closure = Field(new_closure, 0);         \
        }                                                   \
        CAMLreturn(previous_closure);                       \
    }

struct ml_window_callbacks
{
    value window_pos;
    value window_size;
    value window_close;
    value window_refresh;
    value window_focus;
    value window_iconify;
    value framebuffer_size;
    value key;
    value character;
    value character_mods;
    value mouse_button;
    value cursor_pos;
    value cursor_enter;
    value scroll;
    value drop;
};

#define ML_WINDOW_CALLBACKS_WOSIZE \
    (sizeof(struct ml_window_callbacks) / sizeof(value))

#define CAML_WINDOW_SETTER_STUB(glfw_setter, name)                      \
    CAMLprim value caml_##glfw_setter(value window, value new_closure)  \
    {                                                                   \
        CAMLparam1(new_closure);                                        \
        CAMLlocal1(previous_closure);                                   \
        struct ml_window_callbacks* ml_window_callbacks =               \
            *(struct ml_window_callbacks**)                             \
            glfwGetWindowUserPointer((GLFWwindow*)window);              \
                                                                        \
        if (ml_window_callbacks->name == Val_unit)                      \
            previous_closure = Val_none;                                \
        else                                                            \
        {                                                               \
            previous_closure = caml_alloc_small(1, 0);                  \
            Field(previous_closure, 0) = ml_window_callbacks->name;     \
        }                                                               \
        if (new_closure == Val_none)                                    \
        {                                                               \
            if (ml_window_callbacks->name != Val_unit)                  \
            {                                                           \
                glfw_setter((GLFWwindow*)window, NULL);                 \
                ml_window_callbacks->name = Val_unit;                   \
            }                                                           \
        }                                                               \
        else                                                            \
        {                                                               \
            if (ml_window_callbacks->name == Val_unit)                  \
                glfw_setter((GLFWwindow*)window, name##_callback_stub); \
            caml_modify(&ml_window_callbacks->name, Field(new_closure, 0)); \
        }                                                               \
        CAMLreturn(previous_closure);                                   \
    }

enum value_type
{
    Int,
    IntOption,
    ClientApi,
    ContextRobustness,
    OpenGLProfile,
    ContextReleaseBehavior,
    ContextCreationApi
};

struct ml_window_hint
{
    int glfw_window_hint;
    enum value_type value_type;
};

static const struct ml_window_hint ml_window_hint[] = {
    {GLFW_FOCUSED, Int},
    {GLFW_RESIZABLE, Int},
    {GLFW_VISIBLE, Int},
    {GLFW_DECORATED, Int},
    {GLFW_AUTO_ICONIFY, Int},
    {GLFW_FLOATING, Int},
    {GLFW_MAXIMIZED, Int},
    {GLFW_RED_BITS, IntOption},
    {GLFW_GREEN_BITS, IntOption},
    {GLFW_BLUE_BITS, IntOption},
    {GLFW_ALPHA_BITS, IntOption},
    {GLFW_DEPTH_BITS, IntOption},
    {GLFW_STENCIL_BITS, IntOption},
    {GLFW_ACCUM_RED_BITS, IntOption},
    {GLFW_ACCUM_GREEN_BITS, IntOption},
    {GLFW_ACCUM_BLUE_BITS, IntOption},
    {GLFW_ACCUM_ALPHA_BITS, IntOption},
    {GLFW_AUX_BUFFERS, IntOption},
    {GLFW_STEREO, Int},
    {GLFW_SAMPLES, IntOption},
    {GLFW_SRGB_CAPABLE, Int},
    {GLFW_REFRESH_RATE, IntOption},
    {GLFW_DOUBLEBUFFER, Int},
    {GLFW_CLIENT_API, ClientApi},
    {GLFW_CONTEXT_VERSION_MAJOR, Int},
    {GLFW_CONTEXT_VERSION_MINOR, Int},
    {GLFW_CONTEXT_ROBUSTNESS, ContextRobustness},
    {GLFW_OPENGL_FORWARD_COMPAT, Int},
    {GLFW_OPENGL_DEBUG_CONTEXT, Int},
    {GLFW_OPENGL_PROFILE, OpenGLProfile},
    {GLFW_CONTEXT_RELEASE_BEHAVIOR, ContextReleaseBehavior},
    {GLFW_CONTEXT_NO_ERROR, Int},
    {GLFW_CONTEXT_CREATION_API, ContextCreationApi}
};

struct ml_window_attrib
{
    int glfw_window_attrib;
    enum value_type value_type;
};

static const struct ml_window_attrib ml_window_attrib[] = {
    {GLFW_FOCUSED, Int},
    {GLFW_ICONIFIED, Int},
    {GLFW_RESIZABLE, Int},
    {GLFW_VISIBLE, Int},
    {GLFW_DECORATED, Int},
    {GLFW_FLOATING, Int},
    {GLFW_MAXIMIZED, Int},
    {GLFW_CLIENT_API, ClientApi},
    {GLFW_CONTEXT_VERSION_MAJOR, Int},
    {GLFW_CONTEXT_VERSION_MINOR, Int},
    {GLFW_CONTEXT_REVISION, Int},
    {GLFW_CONTEXT_ROBUSTNESS, ContextRobustness},
    {GLFW_OPENGL_FORWARD_COMPAT, Int},
    {GLFW_OPENGL_DEBUG_CONTEXT, Int},
    {GLFW_OPENGL_PROFILE, OpenGLProfile},
    {GLFW_CONTEXT_CREATION_API, ContextCreationApi}
};

#include "GLFW_key_conv_arrays.inl"

static inline value caml_list_of_pointer_array(void** array, int count)
{
    CAMLparam0();
    CAMLlocal2(ret, tmp);
    ret = Val_emptylist;
    while (count > 0)
    {
        tmp = caml_alloc_small(2, 0);
        Field(tmp, 0) = (value)array[--count];
        Field(tmp, 1) = ret;
        caml_modify(&ret, tmp);
    }
    CAMLreturn(ret);
}

static inline value caml_list_of_flags(int flags, int count)
{
    CAMLparam0();
    CAMLlocal2(ret, tmp);
    ret = Val_emptylist;
    for (int i = 0; i < count; ++i)
        if (flags >> i & 1)
        {
            tmp = caml_alloc_small(2, 0);
            Field(tmp, 0) = Val_int(i);
            Field(tmp, 1) = ret;
            caml_modify(&ret, tmp);
        }
    CAMLreturn(ret);
}

static inline value caml_copy_vidmode(const GLFWvidmode* vidmode)
{
    value v = caml_alloc_small(6, 0);

    Field(v, 0) = Val_int(vidmode->width);
    Field(v, 1) = Val_int(vidmode->height);
    Field(v, 2) = Val_int(vidmode->redBits);
    Field(v, 3) = Val_int(vidmode->greenBits);
    Field(v, 4) = Val_int(vidmode->blueBits);
    Field(v, 5) = Val_int(vidmode->refreshRate);
    return v;
}

CAMLprim intnat caml_GammaRamp_get(
    value gamma_ramp, intnat channel, intnat offset)
{
    if (offset < 0 || offset >= Int_val(Field(gamma_ramp, 3)))
        caml_invalid_argument("GLFW.GammaRamp.get: index out of bounds.");
    return ((unsigned short*)Bytes_val(Field(gamma_ramp, channel)))[offset];
}

CAMLprim value caml_GammaRamp_get_byte(
    value gamma_ramp, value channel, value offset)
{
    return Val_int(
        caml_GammaRamp_get(gamma_ramp, Int_val(channel), Int_val(offset)));
}

CAMLprim value caml_GammaRamp_set(
    value gamma_ramp, intnat channel, intnat offset, intnat val)
{
    if (offset < 0 || offset >= Int_val(Field(gamma_ramp, 3)))
        caml_invalid_argument("GLFW.GammaRamp.set: index out of bounds.");
    ((unsigned short*)Bytes_val(Field(gamma_ramp, channel)))[offset] = val;
    return Val_unit;
}

CAMLprim value caml_GammaRamp_set_byte(
    value gamma_ramp, value channel, value offset, value val)
{
    return caml_GammaRamp_set(gamma_ramp, Int_val(channel), Int_val(offset),
                              Int_val(val));
}

CAMLprim value caml_glfwInit(CAMLvoid)
{
    return Val_bool(glfwInit());
}

CAMLprim value caml_glfwTerminate(CAMLvoid)
{
    glfwTerminate();
    return Val_unit;
}

CAMLprim value caml_glfwGetVersion(CAMLvoid)
{
    int major, minor, rev;
    value ret;

    glfwGetVersion(&major, &minor, &rev);
    ret = caml_alloc_small(3, 0);
    Field(ret, 0) = Val_int(major);
    Field(ret, 1) = Val_int(minor);
    Field(ret, 2) = Val_int(rev);
    return ret;
}

CAMLprim value caml_glfwGetVersionString(CAMLvoid)
{
    return caml_copy_string(glfwGetVersionString());
}

static value error_closure = Val_unit;

void error_callback_stub(int error_code, const char* description)
{
    caml_callback2(error_closure, Val_int(error_code - GLFW_NOT_INITIALIZED),
                   caml_copy_string(description));
}

CAML_SETTER_STUB(glfwSetErrorCallback, error)

CAMLprim value caml_glfwGetMonitors(CAMLvoid)
{
    int monitor_count;
    GLFWmonitor** monitors = glfwGetMonitors(&monitor_count);

    return caml_list_of_pointer_array((void**)monitors, monitor_count);
}

CAMLprim value caml_glfwGetPrimaryMonitor(CAMLvoid)
{
    return (value)glfwGetPrimaryMonitor();
}

CAMLprim value caml_glfwGetMonitorPos(value monitor)
{
    int xpos, ypos;
    value ret;

    glfwGetMonitorPos((GLFWmonitor*)monitor, &xpos, &ypos);
    ret = caml_alloc_small(2, 0);
    Field(ret, 0) = Val_int(xpos);
    Field(ret, 1) = Val_int(ypos);
    return ret;
}

CAMLprim value caml_glfwGetMonitorPhysicalSize(value monitor)
{
    int width, height;
    value ret;

    glfwGetMonitorPhysicalSize((GLFWmonitor*)monitor, &width, &height);
    ret = caml_alloc_small(2, 0);
    Field(ret, 0) = Val_int(width);
    Field(ret, 1) = Val_int(height);
    return ret;
}

CAMLprim value caml_glfwGetMonitorName(value monitor)
{
    return caml_copy_string(glfwGetMonitorName((GLFWmonitor*)monitor));
}

static value monitor_closure = Val_unit;

void monitor_callback_stub(GLFWmonitor* monitor, int event)
{
    caml_callback2(
        monitor_closure, (value)monitor, Val_int(event - GLFW_CONNECTED));
}

CAML_SETTER_STUB(glfwSetMonitorCallback, monitor)

CAMLprim value caml_glfwGetVideoModes(value monitor)
{
    CAMLparam0();
    CAMLlocal3(ret, vm, tmp);
    int videomode_count;
    const GLFWvidmode* videomodes =
        glfwGetVideoModes((GLFWmonitor*)monitor, &videomode_count);

    ret = Val_emptylist;
    while (videomode_count > 0)
    {
        vm = caml_copy_vidmode(videomodes + --videomode_count);
        tmp = caml_alloc_small(2, 0);
        Field(tmp, 0) = vm;
        Field(tmp, 1) = ret;
        caml_modify(&ret, tmp);
    }
    CAMLreturn(ret);
}

CAMLprim value caml_glfwGetVideoMode(value monitor)
{
    return caml_copy_vidmode(glfwGetVideoMode((GLFWmonitor*)monitor));
}

CAMLprim value caml_glfwSetGamma(value monitor, value gamma)
{
    glfwSetGamma((GLFWmonitor*)monitor, Double_val(gamma));
    return Val_unit;
}

CAMLprim value caml_glfwGetGammaRamp(value monitor)
{
    CAMLparam0();
    CAMLlocal1(ret);
    const GLFWgammaramp* gamma_ramp = glfwGetGammaRamp((GLFWmonitor*)monitor);
    const unsigned int byte_size = gamma_ramp->size * sizeof(*gamma_ramp->red);

    ret = caml_alloc_small(3, 0);
    Field(ret, 0) = Val_unit;
    Field(ret, 1) = Val_unit;
    Field(ret, 2) = Val_unit;
    Store_field(ret, 0, caml_alloc_string(byte_size));
    memcpy(Bytes_val(Field(ret, 0)), gamma_ramp->red, byte_size);
    Store_field(ret, 1, caml_alloc_string(byte_size));
    memcpy(Bytes_val(Field(ret, 1)), gamma_ramp->green, byte_size);
    Store_field(ret, 2, caml_alloc_string(byte_size));
    memcpy(Bytes_val(Field(ret, 2)), gamma_ramp->blue, byte_size);
    CAMLreturn(ret);
}

CAMLprim value caml_glfwSetGammaRamp(value monitor, value ml_gamma_ramp)
{
    GLFWgammaramp gamma_ramp;

    gamma_ramp.size =
        caml_string_length(Field(ml_gamma_ramp, 0)) / sizeof(*gamma_ramp.red);
    gamma_ramp.red = (unsigned short*)Bytes_val(Field(ml_gamma_ramp, 0));
    gamma_ramp.green = (unsigned short*)Bytes_val(Field(ml_gamma_ramp, 1));
    gamma_ramp.blue = (unsigned short*)Bytes_val(Field(ml_gamma_ramp, 2));
    glfwSetGammaRamp((GLFWmonitor*)monitor, &gamma_ramp);
    return Val_unit;
}

CAMLprim value caml_glfwDefaultWindowHints(CAMLvoid)
{
    glfwDefaultWindowHints();
    return (Val_unit);
}

CAMLprim value caml_glfwWindowHint(value hint, value ml_val)
{
    const int offset = Int_val(hint);
    int glfw_val = GLFW_DONT_CARE;

    switch (ml_window_hint[offset].value_type)
    {
    case Int:
        glfw_val = Int_val(ml_val);
        break;

    case IntOption:
        if (ml_val != Val_none)
            glfw_val = Int_val(Field(ml_val, 0));
        break;

    case ClientApi:
        if (ml_val == Val_int(0))
            glfw_val = GLFW_NO_API;
        else if (ml_val == Val_int(1))
            glfw_val = GLFW_OPENGL_API;
        else
            glfw_val = GLFW_OPENGL_ES_API;
        break;

    case ContextRobustness:
        if (ml_val == Val_int(0))
            glfw_val = GLFW_NO_ROBUSTNESS;
        else if (ml_val == Val_int(1))
            glfw_val = GLFW_NO_RESET_NOTIFICATION;
        else
            glfw_val = GLFW_LOSE_CONTEXT_ON_RESET;
        break;

    case OpenGLProfile:
        if (ml_val == Val_int(0))
            glfw_val = GLFW_OPENGL_ANY_PROFILE;
        else if (ml_val == Val_int(1))
            glfw_val = GLFW_OPENGL_CORE_PROFILE;
        else
            glfw_val = GLFW_OPENGL_COMPAT_PROFILE;
        break;

    case ContextReleaseBehavior:
        if (ml_val == Val_int(0))
            glfw_val = GLFW_ANY_RELEASE_BEHAVIOR;
        else if (ml_val == Val_int(1))
            glfw_val = GLFW_RELEASE_BEHAVIOR_FLUSH;
        else
            glfw_val = GLFW_RELEASE_BEHAVIOR_NONE;
        break;

    case ContextCreationApi:
        if (ml_val == Val_int(0))
            glfw_val = GLFW_NATIVE_CONTEXT_API;
        else
            glfw_val = GLFW_EGL_CONTEXT_API;
    }
    glfwWindowHint(ml_window_hint[offset].glfw_window_hint, glfw_val);
    return (Val_unit);
}

CAMLprim value caml_glfwCreateWindow(
    value width, value height, value title, value mntor, value share)
{
    GLFWwindow* window = glfwCreateWindow(
        Val_int(width), Val_int(height), String_val(title), (GLFWmonitor*)mntor,
        share == Val_none ? NULL : (GLFWwindow*)Field(share, 0));
    void* user_pointer = malloc(sizeof(value));
    value callbacks = caml_alloc_small(ML_WINDOW_CALLBACKS_WOSIZE, 0);

    for (unsigned int i = 0; i < ML_WINDOW_CALLBACKS_WOSIZE; ++i)
        Field(callbacks, i) = Val_unit;
    *(value*)user_pointer = callbacks;
    caml_register_global_root(user_pointer);
    glfwSetWindowUserPointer(window, user_pointer);
    return (value)window;
}

CAMLprim value caml_glfwDestroyWindow(value window)
{
    void* user_pointer = glfwGetWindowUserPointer((GLFWwindow*)window);

    caml_remove_global_root(user_pointer);
    free(user_pointer);
    glfwDestroyWindow((GLFWwindow*)window);
    return Val_unit;
}

CAMLprim value caml_glfwWindowShouldClose(value window)
{
    return Val_bool(glfwWindowShouldClose((GLFWwindow*)window));
}

CAMLprim value caml_glfwSetWindowShouldClose(value window, value val)
{
    glfwSetWindowShouldClose((GLFWwindow*)window, Bool_val(val));
    return Val_unit;
}

CAMLprim value caml_glfwSetWindowTitle(value window, value title)
{
    glfwSetWindowTitle((GLFWwindow*)window, String_val(title));
    return Val_unit;
}

CAMLprim value caml_glfwSetWindowIcon(value window, value images)
{
    unsigned int count = 0;
    value iter = images;
    GLFWimage* glfw_images;

    while (iter != Val_emptylist)
    {
        ++count;
        iter = Field(iter, 1);
    }
    glfw_images = malloc(sizeof(*glfw_images) * count);
    iter = images;
    for (unsigned int i = 0; i < count; ++i)
    {
        value ml_image = Field(iter, 0);
        glfw_images[i].width = Int_val(Field(ml_image, 0));
        glfw_images[i].height = Int_val(Field(ml_image, 1));
        glfw_images[i].pixels = Bytes_val(Field(ml_image, 2));
        iter = Field(iter, 1);
    }
    glfwSetWindowIcon((GLFWwindow*)window, count, glfw_images);
    free(glfw_images);
    return Val_unit;
}

CAMLprim value caml_glfwGetWindowPos(value window)
{
    int xpos, ypos;
    value ret;

    glfwGetWindowPos((GLFWwindow*)window, &xpos, &ypos);
    ret = caml_alloc_small(2, 0);
    Field(ret, 0) = Val_int(xpos);
    Field(ret, 1) = Val_int(ypos);
    return ret;
}

CAMLprim value caml_glfwSetWindowPos(value window, value xpos, value ypos)
{
    glfwSetWindowPos((GLFWwindow*)window, Int_val(xpos), Int_val(ypos));
    return Val_unit;
}

CAMLprim value caml_glfwGetWindowSize(value window)
{
    int width, height;
    value ret;

    glfwGetWindowSize((GLFWwindow*)window, &width, &height);
    ret = caml_alloc_small(2, 0);
    Field(ret, 0) = Val_int(width);
    Field(ret, 1) = Val_int(height);
    return ret;
}

CAMLprim value caml_glfwSetWindowSizeLimits(
    value window, value minW, value minH, value maxW, value maxH)
{
    glfwSetWindowSizeLimits((GLFWwindow*)window, Int_val(minW), Int_val(minH),
                            Int_val(maxW), Int_val(maxH));
    return Val_unit;
}

CAMLprim value caml_glfwSetWindowAspectRatio(value window, value num, value den)
{
    glfwSetWindowAspectRatio((GLFWwindow*)window, Int_val(num), Int_val(den));
    return Val_unit;
}

CAMLprim value caml_glfwSetWindowSize(value window, value width, value height)
{
    glfwSetWindowSize((GLFWwindow*)window, Int_val(width), Int_val(height));
    return Val_unit;
}

CAMLprim value caml_glfwGetFramebufferSize(value window)
{
    int width, height;
    value ret;

    glfwGetFramebufferSize((GLFWwindow*)window, &width, &height);
    ret = caml_alloc_small(2, 0);
    Field(ret, 0) = Val_int(width);
    Field(ret, 1) = Val_int(height);
    return ret;
}

CAMLprim value caml_glfwGetWindowFrameSize(value window)
{
    int left, top, right, bottom;
    value ret;

    glfwGetWindowFrameSize((GLFWwindow*)window, &left, &top, &right, &bottom);
    ret = caml_alloc_small(4, 0);
    Field(ret, 0) = Val_int(left);
    Field(ret, 1) = Val_int(top);
    Field(ret, 2) = Val_int(right);
    Field(ret, 3) = Val_int(bottom);
    return ret;
}

CAMLprim value caml_glfwIconifyWindow(value window)
{
    glfwIconifyWindow((GLFWwindow*)window);
    return Val_unit;
}

CAMLprim value caml_glfwRestoreWindow(value window)
{
    glfwRestoreWindow((GLFWwindow*)window);
    return Val_unit;
}

CAMLprim value caml_glfwMaximizeWindow(value window)
{
    glfwMaximizeWindow((GLFWwindow*)window);
    return Val_unit;
}

CAMLprim value caml_glfwShowWindow(value window)
{
    glfwShowWindow((GLFWwindow*)window);
    return Val_unit;
}

CAMLprim value caml_glfwHideWindow(value window)
{
    glfwHideWindow((GLFWwindow*)window);
    return Val_unit;
}

CAMLprim value caml_glfwFocusWindow(value window)
{
    glfwFocusWindow((GLFWwindow*)window);
    return Val_unit;
}

CAMLprim value caml_glfwGetWindowMonitor(value window)
{
    return (value)glfwGetWindowMonitor((GLFWwindow*)window);
}

CAMLprim value caml_glfwSetWindowMonitor(
    value window, value monitor, value xpos, value ypos,
    value width, value height, value refreshRate)
{
    glfwSetWindowMonitor(
        (GLFWwindow*)window, (GLFWmonitor*)monitor,
        Int_val(xpos), Int_val(ypos), Int_val(width), Int_val(height),
        Int_val(refreshRate));
    return Val_unit;
}

CAMLprim value caml_glfwSetWindowMonitor_byte(value* val_array, int val_count)
{
    (void)val_count;
    return caml_glfwSetWindowMonitor(
        val_array[0], val_array[1], val_array[2], val_array[3], val_array[4],
        val_array[5], val_array[6]);
}

CAMLprim value caml_glfwGetWindowAttrib(value window, value attribute)
{
    const int offset = Int_val(attribute);
    int glfw_val = glfwGetWindowAttrib(
        (GLFWwindow*)window, ml_window_attrib[offset].glfw_window_attrib);
    value ret = Val_unit;

    switch (ml_window_attrib[offset].value_type)
    {
    case Int:
        ret = Val_int(glfw_val);
        break;

    case ClientApi:
        if (glfw_val == GLFW_NO_API)
            ret = Val_int(0);
        else if (glfw_val == GLFW_OPENGL_API)
            ret = Val_int(1);
        else
            ret = Val_int(2);
        break;

    case ContextRobustness:
        if (glfw_val == GLFW_NO_ROBUSTNESS)
            ret = Val_int(0);
        else if (glfw_val == GLFW_NO_RESET_NOTIFICATION)
            ret = Val_int(1);
        else
            ret = Val_int(2);
        break;

    case OpenGLProfile:
        if (glfw_val == GLFW_OPENGL_ANY_PROFILE)
            ret = Val_int(0);
        else if (glfw_val == GLFW_OPENGL_CORE_PROFILE)
            ret = Val_int(1);
        else
            ret = Val_int(2);
        break;

    case ContextCreationApi:
        if (glfw_val == GLFW_NATIVE_CONTEXT_API)
            ret = Val_int(0);
        else
            ret = Val_int(1);

    default:;
    }
    return (ret);
}

void window_pos_callback_stub(GLFWwindow* window, int xpos, int ypos)
{
    struct ml_window_callbacks* ml_window_callbacks =
        *(struct ml_window_callbacks**)
        glfwGetWindowUserPointer((GLFWwindow*)window);

    caml_callback3(ml_window_callbacks->window_pos, (value)window,
                   Val_int(xpos), Val_int(ypos));
}

CAML_WINDOW_SETTER_STUB(glfwSetWindowPosCallback, window_pos)

void window_size_callback_stub(GLFWwindow* window, int width, int height)
{
    struct ml_window_callbacks* ml_window_callbacks =
        *(struct ml_window_callbacks**)
        glfwGetWindowUserPointer((GLFWwindow*)window);

    caml_callback3(ml_window_callbacks->window_size, (value)window,
                   Val_int(width), Val_int(height));
}

CAML_WINDOW_SETTER_STUB(glfwSetWindowSizeCallback, window_size)

void window_close_callback_stub(GLFWwindow* window)
{
    struct ml_window_callbacks* ml_window_callbacks =
        *(struct ml_window_callbacks**)
        glfwGetWindowUserPointer((GLFWwindow*)window);

    caml_callback(ml_window_callbacks->window_close, (value)window);
}

CAML_WINDOW_SETTER_STUB(glfwSetWindowCloseCallback, window_close)

void window_refresh_callback_stub(GLFWwindow* window)
{
    struct ml_window_callbacks* ml_window_callbacks =
        *(struct ml_window_callbacks**)
        glfwGetWindowUserPointer((GLFWwindow*)window);

    caml_callback(ml_window_callbacks->window_refresh, (value)window);
}

CAML_WINDOW_SETTER_STUB(glfwSetWindowRefreshCallback, window_refresh)

void window_focus_callback_stub(GLFWwindow* window, int focused)
{
    struct ml_window_callbacks* ml_window_callbacks =
        *(struct ml_window_callbacks**)
        glfwGetWindowUserPointer((GLFWwindow*)window);

    caml_callback2(
        ml_window_callbacks->window_focus, (value)window, Val_bool(focused));
}

CAML_WINDOW_SETTER_STUB(glfwSetWindowFocusCallback, window_focus)

void window_iconify_callback_stub(GLFWwindow* window, int iconified)
{
    struct ml_window_callbacks* ml_window_callbacks =
        *(struct ml_window_callbacks**)
        glfwGetWindowUserPointer((GLFWwindow*)window);

    caml_callback2(ml_window_callbacks->window_iconify, (value)window,
                   Val_bool(iconified));
}

CAML_WINDOW_SETTER_STUB(glfwSetWindowIconifyCallback, window_iconify)

void framebuffer_size_callback_stub(GLFWwindow* window, int width, int height)
{
    struct ml_window_callbacks* ml_window_callbacks =
        *(struct ml_window_callbacks**)
        glfwGetWindowUserPointer((GLFWwindow*)window);

    caml_callback3(ml_window_callbacks->framebuffer_size, (value)window,
                   Val_int(width), Val_int(height));
}

CAML_WINDOW_SETTER_STUB(glfwSetFramebufferSizeCallback, framebuffer_size)

CAMLprim value caml_glfwPollEvents(CAMLvoid)
{
    glfwPollEvents();
    return Val_unit;
}

CAMLprim value caml_glfwWaitEvents(CAMLvoid)
{
    glfwWaitEvents();
    return Val_unit;
}

CAMLprim value caml_glfwWaitEventsTimeout(value timeout)
{
    glfwWaitEventsTimeout(Double_val(timeout));
    return Val_unit;
}

CAMLprim value caml_glfwPostEmptyEvent(CAMLvoid)
{
    glfwPostEmptyEvent();
    return Val_unit;
}

CAMLprim value caml_glfwGetInputMode(value window, value mode)
{
    int v = glfwGetInputMode((GLFWwindow*)window, Int_val(mode) + GLFW_CURSOR);

    if (Int_val(mode) == 0)
        return Val_int(v - GLFW_CURSOR_NORMAL);
    return Val_bool(v);
}

CAMLprim value caml_glfwSetInputMode(value window, value mode, value v)
{
    if (Int_val(mode) == 0)
        glfwSetInputMode((GLFWwindow*)window, Int_val(mode) + GLFW_CURSOR,
                         Int_val(v) + GLFW_CURSOR_NORMAL);
    else
        glfwSetInputMode(
            (GLFWwindow*)window, Int_val(mode) + GLFW_CURSOR, Bool_val(v));
    return Val_unit;
}

CAMLprim value caml_glfwGetKeyName(value key, value scancode)
{
    const char* name =
        glfwGetKeyName(Int_val(ml_to_glfw_key[key]), Int_val(scancode));
    return caml_copy_string(name != NULL ? name : "");
}

CAMLprim value caml_glfwGetKey(value window, value key)
{
    int ret = glfwGetKey((GLFWwindow*)window, Int_val(ml_to_glfw_key[key]));
    return Val_bool(ret);
}

CAMLprim value caml_glfwGetMouseButton(value window, value button)
{
    return Val_int(glfwGetMouseButton((GLFWwindow*)window, Int_val(button)));
}

CAMLprim value caml_glfwGetCursorPos(value window)
{
    double xpos, ypos;
    value ret = caml_alloc_float_array(2);

    glfwGetCursorPos((GLFWwindow*)window, &xpos, &ypos);
    Double_flat_field(ret, 0) = xpos;
    Double_flat_field(ret, 1) = ypos;
    return ret;
}

CAMLprim value caml_glfwSetCursorPos(value window, value xpos, value ypos)
{
    glfwSetCursorPos((GLFWwindow*)window, Double_val(xpos), Double_val(ypos));
    return Val_unit;
}

CAMLprim value caml_glfwCreateCursor(value image, value xhot, value yhot)
{
    GLFWimage glfw_image;

    glfw_image.width = Int_val(Field(image, 0));
    glfw_image.height = Int_val(Field(image, 1));
    glfw_image.pixels = Bytes_val(Field(image, 2));
    glfwCreateCursor(&glfw_image, Int_val(xhot), Int_val(yhot));
    return Val_unit;
}

CAMLprim value caml_glfwCreateStandardCursor(value shape)
{
    return (value)glfwCreateStandardCursor(Int_val(shape) - GLFW_ARROW_CURSOR);
}

CAMLprim value caml_glfwDestroyCursor(value cursor)
{
    glfwDestroyCursor((GLFWcursor*)cursor);
    return Val_unit;
}

CAMLprim value caml_glfwSetCursor(value window, value cursor)
{
    glfwSetCursor((GLFWwindow*)window, (GLFWcursor*)cursor);
    return Val_unit;
}

void key_callback_stub(
    GLFWwindow* window, int key, int scancode, int action, int mods)
{
    struct ml_window_callbacks* ml_window_callbacks =
        *(struct ml_window_callbacks**)
        glfwGetWindowUserPointer((GLFWwindow*)window);
    value args[] = {
        (value)window, Val_int(glfw_to_ml_key[key + GLFW_KEY_FIRST]),
        Val_int(scancode), Val_int(action), caml_list_of_flags(mods, 4)
    };

    caml_callbackN(
        ml_window_callbacks->key, sizeof(args) / sizeof(*args), args);
}

CAML_WINDOW_SETTER_STUB(glfwSetKeyCallback, key)

void character_callback_stub(GLFWwindow* window, unsigned int codepoint)
{
    struct ml_window_callbacks* ml_window_callbacks =
        *(struct ml_window_callbacks**)
        glfwGetWindowUserPointer((GLFWwindow*)window);

    caml_callback2(
        ml_window_callbacks->character, (value)window, Val_int(codepoint));
}

CAML_WINDOW_SETTER_STUB(glfwSetCharCallback, character)

void character_mods_callback_stub(
    GLFWwindow* window, unsigned int codepoint, int mods)
{
    struct ml_window_callbacks* ml_window_callbacks =
        *(struct ml_window_callbacks**)
        glfwGetWindowUserPointer((GLFWwindow*)window);

    caml_callback3(ml_window_callbacks->character_mods, (value)window,
                   Val_int(codepoint), caml_list_of_flags(mods, 4));
}

CAML_WINDOW_SETTER_STUB(glfwSetCharModsCallback, character_mods)

void mouse_button_callback_stub(
    GLFWwindow* window, int button, int action, int mods)
{
    struct ml_window_callbacks* ml_window_callbacks =
        *(struct ml_window_callbacks**)
        glfwGetWindowUserPointer((GLFWwindow*)window);
    value args[] = {
        (value)window, Val_int(button), Val_bool(action),
        caml_list_of_flags(mods, 4)
    };

    caml_callbackN(
        ml_window_callbacks->mouse_button, sizeof(args) / sizeof(*args), args);
}

CAML_WINDOW_SETTER_STUB(glfwSetMouseButtonCallback, mouse_button)

void cursor_pos_callback_stub(GLFWwindow* window, double xpos, double ypos)
{
    CAMLparam0();
    CAMLlocal2(ml_xpos, ml_ypos);
    struct ml_window_callbacks* ml_window_callbacks =
        *(struct ml_window_callbacks**)
        glfwGetWindowUserPointer((GLFWwindow*)window);

    ml_xpos = caml_copy_double(xpos);
    ml_ypos = caml_copy_double(ypos);
    caml_callback3(
        ml_window_callbacks->cursor_pos, (value)window, ml_xpos, ml_ypos);
    CAMLreturn0;
}

CAML_WINDOW_SETTER_STUB(glfwSetCursorPosCallback, cursor_pos)

void cursor_enter_callback_stub(GLFWwindow* window, int entered)
{
    struct ml_window_callbacks* ml_window_callbacks =
        *(struct ml_window_callbacks**)
        glfwGetWindowUserPointer((GLFWwindow*)window);

    caml_callback2(
        ml_window_callbacks->character, (value)window, Val_bool(entered));
}

CAML_WINDOW_SETTER_STUB(glfwSetCursorEnterCallback, cursor_enter)

void scroll_callback_stub(GLFWwindow* window, double xoffset, double yoffset)
{
    CAMLparam0();
    CAMLlocal2(ml_xoffset, ml_yoffset);
    struct ml_window_callbacks* ml_window_callbacks =
        *(struct ml_window_callbacks**)
        glfwGetWindowUserPointer((GLFWwindow*)window);

    ml_xoffset = caml_copy_double(xoffset);
    ml_yoffset = caml_copy_double(yoffset);
    caml_callback3(
        ml_window_callbacks->cursor_pos, (value)window, ml_xoffset, ml_yoffset);
    CAMLreturn0;
}

CAML_WINDOW_SETTER_STUB(glfwSetScrollCallback, scroll)

void drop_callback_stub(GLFWwindow* window, int count, const char** paths)
{
    CAMLparam0();
    CAMLlocal3(ml_paths, str, tmp);
    struct ml_window_callbacks* ml_window_callbacks =
        *(struct ml_window_callbacks**)
        glfwGetWindowUserPointer((GLFWwindow*)window);

    ml_paths = Val_emptylist;
    while (count > 0)
    {
        str = caml_copy_string(paths[--count]);
        tmp = caml_alloc_small(2, 0);
        Field(tmp, 0) = str;
        Field(tmp, 1) = ml_paths;
        caml_modify(&ml_paths, tmp);
    }
    caml_callback2(ml_window_callbacks->drop, (value)window, ml_paths);
}

CAML_WINDOW_SETTER_STUB(glfwSetDropCallback, drop)

CAMLprim value caml_glfwJoystickPresent(value joy)
{
    return Val_bool(glfwJoystickPresent(Int_val(joy)));
}

CAMLprim value caml_glfwGetJoystickAxes(value joy)
{
    value ret;
    int count;
    const float* axes = glfwGetJoystickAxes(Int_val(joy), &count);

    if (count == 0)
        return Atom(Double_array_tag);
    ret = caml_alloc_float_array(count);
    for (int i = 0; i < count; ++i)
        Double_field(ret, i) = axes[i];
    return ret;
}

CAMLprim value caml_glfwGetJoystickButtons(value joy)
{
    value ret;
    int count;
    const unsigned char* buttons = glfwGetJoystickButtons(Int_val(joy), &count);

    if (count == 0)
        return Atom(0);
    ret = caml_alloc_float_array(count);
    for (int i = 0; i < count; ++i)
        Field(ret, i) = Int_val(buttons[i]);
    return ret;
}

CAMLprim value caml_glfwGetJoystickName(value joy)
{
    const char* name = glfwGetJoystickName(Int_val(joy));
    return caml_copy_string(name != NULL ? name : "");
}

static value joystick_closure = Val_unit;

void joystick_callback_stub(int joy, int event)
{
    caml_callback2(
        joystick_closure, Val_int(joy), Val_int(event - GLFW_DISCONNECTED));
}

CAML_SETTER_STUB(glfwSetJoystickCallback, joystick)

CAMLprim value caml_glfwSetClipboardString(value window, value string)
{
    glfwSetClipboardString((GLFWwindow*)window, String_val(string));
    return Val_unit;
}

CAMLprim value caml_glfwGetClipboardString(value window)
{
    const char* string = glfwGetClipboardString((GLFWwindow*)window);
    return caml_copy_string(string != NULL ? string : "");
}

CAMLprim value caml_glfwGetTime(CAMLvoid)
{
    return caml_copy_double(glfwGetTime());
}

CAMLprim value caml_glfwSetTime(value time)
{
    glfwSetTime(Double_val(time));
    return Val_unit;
}

CAMLprim value caml_glfwGetTimerValue(CAMLvoid)
{
    return caml_copy_int64(glfwGetTimerValue());
}

CAMLprim value caml_glfwGetTimerFrequency(CAMLvoid)
{
    return caml_copy_int64(glfwGetTimerFrequency());
}

CAMLprim value caml_glfwMakeContextCurrent(value window)
{
    glfwMakeContextCurrent((GLFWwindow*)window);
    return Val_unit;
}

CAMLprim value caml_glfwGetCurrentContext(CAMLvoid)
{
    return (value)glfwGetCurrentContext();
}

CAMLprim value caml_glfwSwapBuffers(value window)
{
    glfwSwapBuffers((GLFWwindow*)window);
    return Val_unit;
}

CAMLprim value caml_glfwSwapInterval(value interval)
{
    glfwSwapInterval(Int_val(interval));
    return Val_unit;
}
