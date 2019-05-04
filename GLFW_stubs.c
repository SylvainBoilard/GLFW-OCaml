#include <GLFW/glfw3.h>
#include <caml/mlvalues.h>
#include <caml/alloc.h>
#include <caml/memory.h>
#include <caml/fail.h>
#include <caml/callback.h>
#include <caml/bigarray.h>

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
                raise_if_error();                           \
                caml_remove_global_root(&name##_closure);   \
                name##_closure = Val_unit;                  \
            }                                               \
        }                                                   \
        else                                                \
        {                                                   \
            if (name##_closure == Val_unit)                 \
            {                                               \
                glfw_setter(name##_callback_stub);          \
                raise_if_error();                           \
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
        raise_if_error();                                               \
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

static value error_tag = Val_unit;
static value error_arg = Val_unit;

static void error_callback(int error, const char* description)
{
    switch (error)
    {
    case GLFW_NOT_INITIALIZED:
        error_tag = *caml_named_value("GLFW.NotInitialized");
        break;
    case GLFW_NO_CURRENT_CONTEXT:
        error_tag = *caml_named_value("GLFW.NoCurrentContext");
        break;
    case GLFW_INVALID_ENUM:
        error_tag = *caml_named_value("GLFW.InvalidEnum");
        break;
    case GLFW_INVALID_VALUE:
        error_tag = *caml_named_value("GLFW.InvalidValue");
        break;
    case GLFW_OUT_OF_MEMORY:
        error_tag = *caml_named_value("GLFW.OutOfMemory");
        break;
    case GLFW_API_UNAVAILABLE:
        error_tag = *caml_named_value("GLFW.ApiUnavailable");
        break;
    case GLFW_VERSION_UNAVAILABLE:
        error_tag = *caml_named_value("GLFW.VersionUnavailable");
        break;
    case GLFW_PLATFORM_ERROR:
        error_tag = *caml_named_value("GLFW.PlatformError");
        break;
    case GLFW_FORMAT_UNAVAILABLE:
        error_tag = *caml_named_value("GLFW.FormatUnavailable");
        break;
    case GLFW_NO_WINDOW_CONTEXT:
        error_tag = *caml_named_value("GLFW.NoWindowContext");
    }
    error_arg = caml_copy_string(description);
}

static inline void raise_if_error(void)
{
    if (error_tag != Val_unit)
    {
        value error_tag_dup = error_tag;
        value error_arg_dup = error_arg;

        error_tag = Val_unit;
        error_arg = Val_unit;
        caml_raise_with_arg(error_tag_dup, error_arg_dup);
    }
}

CAMLprim value init_stub(CAMLvoid)
{
    glfwSetErrorCallback(error_callback);
    return Val_unit;
}

CAMLprim value caml_glfwInit(CAMLvoid)
{
    glfwInit();
    raise_if_error();
    return Val_unit;
}

CAMLprim value caml_glfwTerminate(CAMLvoid)
{
    glfwTerminate();
    raise_if_error();
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

CAMLprim value caml_glfwGetMonitors(CAMLvoid)
{
    int monitor_count;
    GLFWmonitor** monitors = glfwGetMonitors(&monitor_count);

    raise_if_error();
    return caml_list_of_pointer_array((void**)monitors, monitor_count);
}

CAMLprim value caml_glfwGetPrimaryMonitor(CAMLvoid)
{
    GLFWmonitor* ret = glfwGetPrimaryMonitor();
    raise_if_error();
    return (value)ret;
}

CAMLprim value caml_glfwGetMonitorPos(value monitor)
{
    int xpos, ypos;
    value ret;

    glfwGetMonitorPos((GLFWmonitor*)monitor, &xpos, &ypos);
    raise_if_error();
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
    raise_if_error();
    ret = caml_alloc_small(2, 0);
    Field(ret, 0) = Val_int(width);
    Field(ret, 1) = Val_int(height);
    return ret;
}

CAMLprim value caml_glfwGetMonitorName(value monitor)
{
    const char* ret = glfwGetMonitorName((GLFWmonitor*)monitor);
    raise_if_error();
    return caml_copy_string(ret);
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

    raise_if_error();
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
    const GLFWvidmode* ret = glfwGetVideoMode((GLFWmonitor*)monitor);
    raise_if_error();
    return caml_copy_vidmode(ret);
}

CAMLprim value caml_glfwSetGamma(value monitor, value gamma)
{
    glfwSetGamma((GLFWmonitor*)monitor, Double_val(gamma));
    raise_if_error();
    return Val_unit;
}

CAMLprim value caml_glfwGetGammaRamp(value monitor)
{
    CAMLparam0();
    CAMLlocal1(ret);
    const GLFWgammaramp* gamma_ramp = glfwGetGammaRamp((GLFWmonitor*)monitor);
    raise_if_error();
    const int flags = CAML_BA_UINT16 | CAML_BA_C_LAYOUT;
    intnat size = gamma_ramp->size;

    ret = caml_alloc_small(3, 0);
    Field(ret, 0) = Val_unit;
    Field(ret, 1) = Val_unit;
    Field(ret, 2) = Val_unit;
    Store_field(ret, 0, caml_ba_alloc(flags, 1, gamma_ramp->red, &size));
    Store_field(ret, 1, caml_ba_alloc(flags, 1, gamma_ramp->green, &size));
    Store_field(ret, 2, caml_ba_alloc(flags, 1, gamma_ramp->blue, &size));
    CAMLreturn(ret);
}

CAMLprim value caml_glfwSetGammaRamp(value monitor, value ml_gamma_ramp)
{
    GLFWgammaramp gamma_ramp;

    gamma_ramp.size =
        caml_ba_num_elts(Caml_ba_array_val(Field(ml_gamma_ramp, 0)));
    gamma_ramp.red = Caml_ba_data_val(Field(ml_gamma_ramp, 0));
    gamma_ramp.green = Caml_ba_data_val(Field(ml_gamma_ramp, 1));
    gamma_ramp.blue = Caml_ba_data_val(Field(ml_gamma_ramp, 2));
    glfwSetGammaRamp((GLFWmonitor*)monitor, &gamma_ramp);
    raise_if_error();
    return Val_unit;
}

CAMLprim value caml_glfwDefaultWindowHints(CAMLvoid)
{
    glfwDefaultWindowHints();
    raise_if_error();
    return Val_unit;
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
    raise_if_error();
    return Val_unit;
}

CAMLprim value caml_glfwCreateWindow(
    value width, value height, value title, value mntor, value share, CAMLvoid)
{
    GLFWwindow* window = glfwCreateWindow(
        Int_val(width), Int_val(height), String_val(title),
        mntor == Val_none ? NULL : (GLFWmonitor*)Field(mntor, 0),
        share == Val_none ? NULL : (GLFWwindow*)Field(share, 0));
    raise_if_error();
    void* user_pointer = malloc(sizeof(value));
    value callbacks = caml_alloc_small(ML_WINDOW_CALLBACKS_WOSIZE, 0);

    for (unsigned int i = 0; i < ML_WINDOW_CALLBACKS_WOSIZE; ++i)
        Field(callbacks, i) = Val_unit;
    *(value*)user_pointer = callbacks;
    caml_register_global_root(user_pointer);
    glfwSetWindowUserPointer(window, user_pointer);
    return (value)window;
}

CAMLprim value caml_glfwCreateWindow_byte(value* val_array, int val_count)
{
    (void)val_count;
    return caml_glfwCreateWindow(val_array[0], val_array[1], val_array[2],
                                 val_array[3], val_array[4], Val_unit);
}

CAMLprim value caml_glfwDestroyWindow(value window)
{
    void* user_pointer = glfwGetWindowUserPointer((GLFWwindow*)window);

    raise_if_error();
    caml_remove_global_root(user_pointer);
    free(user_pointer);
    glfwDestroyWindow((GLFWwindow*)window);
    raise_if_error();
    return Val_unit;
}

CAMLprim value caml_glfwWindowShouldClose(value window)
{
    int ret = glfwWindowShouldClose((GLFWwindow*)window);
    raise_if_error();
    return Val_bool(ret);
}

CAMLprim value caml_glfwSetWindowShouldClose(value window, value val)
{
    glfwSetWindowShouldClose((GLFWwindow*)window, Bool_val(val));
    raise_if_error();
    return Val_unit;
}

CAMLprim value caml_glfwSetWindowTitle(value window, value title)
{
    glfwSetWindowTitle((GLFWwindow*)window, String_val(title));
    raise_if_error();
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
    raise_if_error();
    return Val_unit;
}

CAMLprim value caml_glfwGetWindowPos(value window)
{
    int xpos, ypos;
    value ret;

    glfwGetWindowPos((GLFWwindow*)window, &xpos, &ypos);
    raise_if_error();
    ret = caml_alloc_small(2, 0);
    Field(ret, 0) = Val_int(xpos);
    Field(ret, 1) = Val_int(ypos);
    return ret;
}

CAMLprim value caml_glfwSetWindowPos(value window, value xpos, value ypos)
{
    glfwSetWindowPos((GLFWwindow*)window, Int_val(xpos), Int_val(ypos));
    raise_if_error();
    return Val_unit;
}

CAMLprim value caml_glfwGetWindowSize(value window)
{
    int width, height;
    value ret;

    glfwGetWindowSize((GLFWwindow*)window, &width, &height);
    raise_if_error();
    ret = caml_alloc_small(2, 0);
    Field(ret, 0) = Val_int(width);
    Field(ret, 1) = Val_int(height);
    return ret;
}

CAMLprim value caml_glfwSetWindowSizeLimits(
    value window, value minW, value minH, value maxW, value maxH)
{
    int glfw_minW = minW == Val_none ? GLFW_DONT_CARE : Int_val(Field(minW, 0));
    int glfw_minH = minH == Val_none ? GLFW_DONT_CARE : Int_val(Field(minH, 0));
    int glfw_maxW = maxW == Val_none ? GLFW_DONT_CARE : Int_val(Field(maxW, 0));
    int glfw_maxH = maxH == Val_none ? GLFW_DONT_CARE : Int_val(Field(maxH, 0));

    glfwSetWindowSizeLimits(
        (GLFWwindow*)window, glfw_minW, glfw_minH, glfw_maxW, glfw_maxH);
    raise_if_error();
    return Val_unit;
}

CAMLprim value caml_glfwSetWindowAspectRatio(value window, value num, value den)
{
    glfwSetWindowAspectRatio((GLFWwindow*)window, Int_val(num), Int_val(den));
    raise_if_error();
    return Val_unit;
}

CAMLprim value caml_glfwSetWindowSize(value window, value width, value height)
{
    glfwSetWindowSize((GLFWwindow*)window, Int_val(width), Int_val(height));
    raise_if_error();
    return Val_unit;
}

CAMLprim value caml_glfwGetFramebufferSize(value window)
{
    int width, height;
    value ret;

    glfwGetFramebufferSize((GLFWwindow*)window, &width, &height);
    raise_if_error();
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
    raise_if_error();
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
    raise_if_error();
    return Val_unit;
}

CAMLprim value caml_glfwRestoreWindow(value window)
{
    glfwRestoreWindow((GLFWwindow*)window);
    raise_if_error();
    return Val_unit;
}

CAMLprim value caml_glfwMaximizeWindow(value window)
{
    glfwMaximizeWindow((GLFWwindow*)window);
    raise_if_error();
    return Val_unit;
}

CAMLprim value caml_glfwShowWindow(value window)
{
    glfwShowWindow((GLFWwindow*)window);
    raise_if_error();
    return Val_unit;
}

CAMLprim value caml_glfwHideWindow(value window)
{
    glfwHideWindow((GLFWwindow*)window);
    raise_if_error();
    return Val_unit;
}

CAMLprim value caml_glfwFocusWindow(value window)
{
    glfwFocusWindow((GLFWwindow*)window);
    raise_if_error();
    return Val_unit;
}

CAMLprim value caml_glfwGetWindowMonitor(value window)
{
    GLFWmonitor* monitor = glfwGetWindowMonitor((GLFWwindow*)window);
    value ret;

    raise_if_error();
    if (monitor == NULL)
        return Val_none;
    ret = caml_alloc_small(1, 0);
    Field(ret, 0) = (value)monitor;
    return ret;
}

CAMLprim value caml_glfwSetWindowMonitor(
    value window, value monitor, value xpos, value ypos,
    value width, value height, value refreshRate)
{
    GLFWmonitor* glfw_monitor =
        monitor == Val_none ? NULL : (GLFWmonitor*)Field(monitor, 0);
    int glfw_refresh_rate = refreshRate == Val_none
        ? GLFW_DONT_CARE
        : Int_val(Field(refreshRate, 0));

    glfwSetWindowMonitor(
        (GLFWwindow*)window, glfw_monitor, Int_val(xpos), Int_val(ypos),
        Int_val(width), Int_val(height), glfw_refresh_rate);
    raise_if_error();
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
    raise_if_error();
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
    raise_if_error();
    return Val_unit;
}

CAMLprim value caml_glfwWaitEvents(CAMLvoid)
{
    glfwWaitEvents();
    raise_if_error();
    return Val_unit;
}

CAMLprim value caml_glfwWaitEventsTimeout(value timeout)
{
    glfwWaitEventsTimeout(Double_val(timeout));
    raise_if_error();
    return Val_unit;
}

CAMLprim value caml_glfwPostEmptyEvent(CAMLvoid)
{
    glfwPostEmptyEvent();
    raise_if_error();
    return Val_unit;
}

CAMLprim value caml_glfwGetInputMode(value window, value mode)
{
    int v = glfwGetInputMode((GLFWwindow*)window, Int_val(mode) + GLFW_CURSOR);

    raise_if_error();
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
    raise_if_error();
    return Val_unit;
}

CAMLprim value caml_glfwGetKeyName(value key, value scancode)
{
    const char* name =
        glfwGetKeyName(ml_to_glfw_key[Int_val(key)], Int_val(scancode));

    raise_if_error();
    if (name == NULL)
        return Val_none;
    CAMLparam0();
    CAMLlocal2(str, ret);
    str = caml_copy_string(name);
    ret = caml_alloc_small(1, 0);
    Field(ret, 0) = str;
    CAMLreturn(ret);
}

CAMLprim value caml_glfwGetKey(value window, value key)
{
    int ret = glfwGetKey((GLFWwindow*)window, ml_to_glfw_key[Int_val(key)]);
    raise_if_error();
    return Val_bool(ret);
}

CAMLprim value caml_glfwGetMouseButton(value window, value button)
{
    int ret = glfwGetMouseButton((GLFWwindow*)window, Int_val(button));
    raise_if_error();
    return Val_bool(ret);
}

CAMLprim value caml_glfwGetCursorPos(value window)
{
    CAMLparam0();
    CAMLlocal3(ml_xpos, ml_ypos, ret);
    double xpos, ypos;

    glfwGetCursorPos((GLFWwindow*)window, &xpos, &ypos);
    raise_if_error();
    ml_xpos = caml_copy_double(xpos);
    ml_ypos = caml_copy_double(ypos);
    ret = caml_alloc_small(2, 0);
    Field(ret, 0) = ml_xpos;
    Field(ret, 1) = ml_ypos;
    CAMLreturn(ret);
}

CAMLprim value caml_glfwSetCursorPos(value window, value xpos, value ypos)
{
    glfwSetCursorPos((GLFWwindow*)window, Double_val(xpos), Double_val(ypos));
    raise_if_error();
    return Val_unit;
}

CAMLprim value caml_glfwCreateCursor(value image, value xhot, value yhot)
{
    GLFWimage glfw_image;
    GLFWcursor* ret;

    glfw_image.width = Int_val(Field(image, 0));
    glfw_image.height = Int_val(Field(image, 1));
    glfw_image.pixels = Bytes_val(Field(image, 2));
    ret = glfwCreateCursor(&glfw_image, Int_val(xhot), Int_val(yhot));
    raise_if_error();
    return (value)ret;
}

CAMLprim value caml_glfwCreateStandardCursor(value shape)
{
    GLFWcursor* ret =
        glfwCreateStandardCursor(Int_val(shape) - GLFW_ARROW_CURSOR);
    raise_if_error();
    return (value)ret;
}

CAMLprim value caml_glfwDestroyCursor(value cursor)
{
    glfwDestroyCursor((GLFWcursor*)cursor);
    raise_if_error();
    return Val_unit;
}

CAMLprim value caml_glfwSetCursor(value window, value cursor)
{
    glfwSetCursor((GLFWwindow*)window, (GLFWcursor*)cursor);
    raise_if_error();
    return Val_unit;
}

void key_callback_stub(
    GLFWwindow* window, int key, int scancode, int action, int mods)
{
    struct ml_window_callbacks* ml_window_callbacks =
        *(struct ml_window_callbacks**)
        glfwGetWindowUserPointer((GLFWwindow*)window);
    value args[] = {
        (value)window, Val_int(glfw_to_ml_key[key - GLFW_KEY_FIRST]),
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
    CAMLreturn0;
}

CAML_WINDOW_SETTER_STUB(glfwSetDropCallback, drop)

CAMLprim value caml_glfwJoystickPresent(value joy)
{
    value ret = glfwJoystickPresent(Int_val(joy));
    raise_if_error();
    return Val_bool(ret);
}

CAMLprim value caml_glfwGetJoystickAxes(value joy)
{
    value ret;
    int count;
    const float* axes = glfwGetJoystickAxes(Int_val(joy), &count);

    raise_if_error();
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

    raise_if_error();
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

    raise_if_error();
    if (name == NULL)
        return Val_none;
    CAMLparam0();
    CAMLlocal2(str, ret);
    str = caml_copy_string(name);
    ret = caml_alloc_small(1, 0);
    Field(ret, 0) = str;
    CAMLreturn(ret);
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
    raise_if_error();
    return Val_unit;
}

CAMLprim value caml_glfwGetClipboardString(value window)
{
    const char* string = glfwGetClipboardString((GLFWwindow*)window);
    raise_if_error();
    return caml_copy_string(string);
}

CAMLprim value caml_glfwGetTime(CAMLvoid)
{
    double time = glfwGetTime();
    raise_if_error();
    return caml_copy_double(time);
}

CAMLprim value caml_glfwSetTime(value time)
{
    glfwSetTime(Double_val(time));
    raise_if_error();
    return Val_unit;
}

CAMLprim value caml_glfwGetTimerValue(CAMLvoid)
{
    uint64_t timer_value = glfwGetTimerValue();
    raise_if_error();
    return caml_copy_int64(timer_value);
}

CAMLprim value caml_glfwGetTimerFrequency(CAMLvoid)
{
    uint64_t timer_frequency = glfwGetTimerFrequency();
    raise_if_error();
    return caml_copy_int64(timer_frequency);
}

CAMLprim value caml_glfwMakeContextCurrent(value window)
{
    if (window == Val_none)
        glfwMakeContextCurrent(NULL);
    else
        glfwMakeContextCurrent((GLFWwindow*)Field(window, 0));
    raise_if_error();
    return Val_unit;
}

CAMLprim value caml_glfwGetCurrentContext(CAMLvoid)
{
    GLFWwindow* window = glfwGetCurrentContext();
    value ret;

    raise_if_error();
    if (window == NULL)
        return Val_none;
    ret = caml_alloc_small(1, 0);
    Field(ret, 0) = (value)window;
    return ret;
}

CAMLprim value caml_glfwSwapBuffers(value window)
{
    glfwSwapBuffers((GLFWwindow*)window);
    raise_if_error();
    return Val_unit;
}

CAMLprim value caml_glfwSwapInterval(value interval)
{
    glfwSwapInterval(Int_val(interval));
    raise_if_error();
    return Val_unit;
}
