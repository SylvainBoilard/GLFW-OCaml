(** OCaml binding for GLFW 3.3.0 *)

(** GLFW-OCaml version numbers. *)
val version_major : int
val version_minor : int
val version_revision : int

(** Exceptions raised by the various functions of this module.

    @see <http://www.glfw.org/docs/latest/group__errors.html>

    If you ever get an InvalidEnum exception and are not using unsafe features
    that would be a bug in GLFW-OCaml. Please fill an issue on GitHub. *)
exception NotInitialized of string
exception NoCurrentContext of string
exception InvalidEnum of string
exception InvalidValue of string
exception OutOfMemory of string
exception ApiUnavailable of string
exception VersionUnavailable of string
exception PlatformError of string
exception FormatUnavailable of string
exception NoWindowContext of string

(** Key actions.

    @see <http://www.glfw.org/docs/latest/group__input.html> *)
type key_action =
  | Release
  | Press
  | Repeat

(** Keyboard keys.

    @see <http://www.glfw.org/docs/latest/group__keys.html> *)
type key =
  | Unknown
  | Space
  | Apostrophe
  | Comma
  | Minus
  | Period
  | Slash
  | Num0
  | Num1
  | Num2
  | Num3
  | Num4
  | Num5
  | Num6
  | Num7
  | Num8
  | Num9
  | Semicolon
  | Equal
  | A
  | B
  | C
  | D
  | E
  | F
  | G
  | H
  | I
  | J
  | K
  | L
  | M
  | N
  | O
  | P
  | Q
  | R
  | S
  | T
  | U
  | V
  | W
  | X
  | Y
  | Z
  | LeftBracket
  | Backslash
  | RightBracket
  | GraveAccent
  | World1
  | World2
  | Escape
  | Enter
  | Tab
  | Backspace
  | Insert
  | Delete
  | Right
  | Left
  | Down
  | Up
  | PageUp
  | PageDown
  | Home
  | End
  | CapsLock
  | ScrollLock
  | NumLock
  | PrintScreen
  | Pause
  | F1
  | F2
  | F3
  | F4
  | F5
  | F6
  | F7
  | F8
  | F9
  | F10
  | F11
  | F12
  | F13
  | F14
  | F15
  | F16
  | F17
  | F18
  | F19
  | F20
  | F21
  | F22
  | F23
  | F24
  | F25
  | Kp0
  | Kp1
  | Kp2
  | Kp3
  | Kp4
  | Kp5
  | Kp6
  | Kp7
  | Kp8
  | Kp9
  | KpDecimal
  | KpDivide
  | KpMultiply
  | KpSubtract
  | KpAdd
  | KpEnter
  | KpEqual
  | LeftShift
  | LeftControl
  | LeftAlt
  | LeftSuper
  | RightShift
  | RightControl
  | RightAlt
  | RightSuper
  | Menu

(** Keyboard key and mouse button modifiers.

    @see <http://www.glfw.org/docs/latest/group__mods.html> *)
type key_mod =
  | Shift
  | Control
  | Alt
  | Super

(** Maximum number of buttons handled for a mouse. *)
val mouse_button_max_count : int

(** Values of common mouse buttons. *)
val mouse_button_left : int
val mouse_button_right : int
val mouse_button_middle : int

(** Maximum number of joysticks connected. *)
val joystick_max_count : int

(** Client OpenGL API hint *)
type client_api =
  | NoApi
  | OpenGLApi
  | OpenGLESApi

(** Context robustness hint *)
type context_robustness =
  | NoRobustness
  | NoResetNotification
  | LoseContextOnReset

(** OpenGL profile hint *)
type opengl_profile =
  | AnyProfile
  | CoreProfile
  | CompatProfile

(** Context release behavior hint *)
type context_release_behavior =
  | AnyReleaseBehavior
  | ReleaseBehaviorFlush
  | ReleaseBehaviorNone

(** Context creation API hint *)
type context_creation_api =
  | NativeContextApi
  | EGLContextApi
  | OSMesaContextApi

(** Window hints and attributes. Use with windowHint, getWindowAttrib and
    setWindowAttrib like this:

    windowHint ~hint:Resizable     ~value:false
    windowHint ~hint:OpenGLProfile ~value:CoreProfile
    windowHint ~hint:RefreshRate   ~value:(Some 60)
    windowHint ~hint:DepthBits     ~value:None
    windowHint ~hint:X11ClassName  ~value:"MyApplicationName"

    getWindowAttrib ~attribute:Resizable       : bool
    getWindowAttrib ~attribute:OpenGLProfile   : opengl_profile
    getWindowAttrib ~attribute:Hovered         : bool
    getWindowAttrib ~attribute:ContextRevision : int

    setWindowAttrib ~attribute:Resizable ~value:true

    This type is wrapped inside a module only for technical reasons related to
    backward-compatibility. That module is opened right after it is declared and
    it is intended that you use the constructor names unprefixed as shown in the
    previous examples. It will be removed together with the old modules in the
    future (probably on the next major version).

    @see <http://www.glfw.org/docs/latest/window_guide.html#window_hints>
    @see <http://www.glfw.org/docs/latest/window_guide.html#window_attribs> *)
module A :
  sig
    type ('a, _) window_attr =
      | Focused : (bool, [<`hint|`attr]) window_attr
      | Iconified : (bool, [<`attr]) window_attr
      | Resizable : (bool, [<`hint|`attr|`update]) window_attr
      | Visible : (bool, [<`hint|`attr]) window_attr
      | Decorated : (bool, [<`hint|`attr|`update]) window_attr
      | AutoIconify : (bool, [<`hint|`attr|`update]) window_attr
      | Floating : (bool, [<`hint|`attr|`update]) window_attr
      | Maximized : (bool, [<`hint|`attr]) window_attr
      | CenterCursor : (bool, [<`hint]) window_attr
      | TransparentFramebuffer : (bool, [<`hint|`attr]) window_attr
      | Hovered : (bool, [<`attr]) window_attr
      | FocusOnShow : (bool, [<`hint|`attr|`update]) window_attr
      | RedBits : (int option, [<`hint]) window_attr
      | GreenBits : (int option, [<`hint]) window_attr
      | BlueBits : (int option, [<`hint]) window_attr
      | AlphaBits : (int option, [<`hint]) window_attr
      | DepthBits : (int option, [<`hint]) window_attr
      | StencilBits : (int option, [<`hint]) window_attr
      | AccumRedBits : (int option, [<`hint]) window_attr
      | AccumGreenBits : (int option, [<`hint]) window_attr
      | AccumBlueBits : (int option, [<`hint]) window_attr
      | AccumAlphaBits : (int option, [<`hint]) window_attr
      | AuxBuffers : (int option, [<`hint]) window_attr
      | Stereo : (bool, [<`hint]) window_attr
      | Samples : (int option, [<`hint]) window_attr
      | SrgbCapable : (bool, [<`hint]) window_attr
      | RefreshRate : (int option, [<`hint]) window_attr
      | DoubleBuffer : (bool, [<`hint]) window_attr
      | ClientApi : (client_api, [<`hint|`attr]) window_attr
      | ContextVersionMajor : (int, [<`hint|`attr]) window_attr
      | ContextVersionMinor : (int, [<`hint|`attr]) window_attr
      | ContextRevision : (int, [<`attr]) window_attr
      | ContextRobustness : (context_robustness, [<`hint|`attr]) window_attr
      | OpenGLForwardCompat : (bool, [<`hint|`attr]) window_attr
      | OpenGLDebugContext : (bool, [<`hint|`attr]) window_attr
      | OpenGLProfile : (opengl_profile, [<`hint|`attr]) window_attr
      | ContextReleaseBehavior :
          (context_release_behavior, [<`hint]) window_attr
      | ContextNoError : (bool, [<`hint]) window_attr
      | ContextCreationApi : (context_creation_api, [<`hint|`attr]) window_attr
      | ScaleToMonitor : (bool, [<`hint]) window_attr
      | CocoaRetinaFramebuffer : (bool, [<`hint]) window_attr
      | CocoaFrameName : (string, [<`hint]) window_attr
      | CocoaGraphicsSwitching : (bool, [<`hint]) window_attr
      | X11ClassName : (string, [<`hint]) window_attr
      | X11InstanceName : (string, [<`hint]) window_attr
  end

open A

(** Deprecated window hints and attributes modules.

    These modules used to wrap separate types that had some identical
    constructor names, allowing type safety for the window hints and attributes
    functions. Compared to the current implementation using GADTs and
    polymorphic variant types, it had two major disadvantages:

    * It required adding module name prefixes when using the constructors;
    * Constructors related to a same feature would be duplicated across some
      totally incompatible types.

    These modules are provided for API backward-compatibility and will be
    removed in the future (probably on the next major version). Most probably,
    updating your code is very straightforward and you simply need to remove the
    module name prefixes when using those constructors. *)
module type S =
  sig
    include
      module type of A with type ('a, 'k) window_attr = ('a, 'k) window_attr
    type 'a t
  end

module WindowHint :
  S with type 'a t = ('a, [`hint]) window_attr [@@deprecated]
module WindowAttribute :
  S with type 'a t = ('a, [`attr]) window_attr [@@deprecated]
module WindowUpdateableAttribute :
  S with type 'a t = ('a, [`update]) window_attr [@@deprecated]

(** Mouse cursor input mode. *)
type cursor_mode =
  | Normal
  | Hidden
  | Disabled

(** Input modes. Use with setInputMode and getInputMode in a similar manner as
    with window hints.

    @see <http://www.glfw.org/docs/latest/group__input.html#gaa92336e173da9c8834558b54ee80563b> *)
type _ input_mode =
  | Cursor : cursor_mode input_mode
  | StickyKeys : bool input_mode
  | StickyMouseButtons : bool input_mode
  | LockKeyMods : bool input_mode
  | RawMouseMotion : bool input_mode

(** Standard cursor shapes. *)
type cursor_shape =
  | ArrowCursor
  | IBeamCursor
  | CrosshairCursor
  | HandCursor
  | HResizeCursor
  | VResizeCursor

(** Joystick connection event. *)
type connection_event =
  | Connected
  | Disconnected

(** Initialization hints.

    @see <http://www.glfw.org/docs/latest/intro_guide.html#init_hints> *)
type _ init_hint =
  | JoystickHatButtons : bool init_hint
  | CocoaChdirResources : bool init_hint
  | CocoaMenubar : bool init_hint

(** Video mode description as returned by getVideoMode(s).

    @see <http://www.glfw.org/docs/latest/structGLFWvidmode.html> *)
type video_mode = {
    width : int;
    height : int;
    red_bits : int;
    green_bits : int;
    blue_bits : int;
    refresh_rate : int;
  }

type monitor

type window

type cursor

(** GammaRamp module. Describes the gamma ramp for a monitor.

    @see <http://www.glfw.org/docs/latest/structGLFWgammaramp.html> *)
module GammaRamp :
  sig
    type channel =
      (int, Bigarray.int16_unsigned_elt, Bigarray.c_layout) Bigarray.Array1.t
    type t = private { red : channel; green : channel; blue : channel }

    (** Create a gamma ramp from three user-supplied channels.

        @raise Invalid_argument if the supplied channels are of different
        lengths. *)
    val create : red:channel -> green:channel -> blue:channel -> t

    (** Create an empty gamma ramp with three channels of the given size. *)
    val make : size:int -> t
  end

(** Image data for creating custom cursors and window icons.

    @see <http://www.glfw.org/docs/latest/structGLFWimage.html> *)
module Image :
  sig
    type t = private { width : int; height : int; pixels : bytes }

    (** Create an image from the supplied pixel data with the specified width
        and height.

        @raise Invalid_argument if a dimension is negative or if there is not
        enough data to make an image with the specified dimensions. *)
    val create : width:int -> height:int -> pixels:bytes -> t
  end

(** Hat statuses as returned by getJoystickHats.

    @see <http://www.glfw.org/docs/latest/group__input.html#ga2d8d0634bb81c180899aeb07477a67ea> *)
type hat_status =
  | HatUp
  | HatRight
  | HatDown
  | HatLeft

(** Gamepad state data as returned by getGamepadState.

    @see <http://www.glfw.org/docs/latest/structGLFWgamepadstate.html> *)
type gamepad_state = {
    buttons : bool array;
    axes : float array;
  }

(** Module functions. These are mostly identical to their original GLFW
    counterparts.

    There are no bindings for the glfw{Set,Get}*UserPointer functions because
    you would have to use unsafe features to set the correct type for your data.
    If you need to make user data accessible inside a callback you can instead
    capture it in a closure and use that closure as your callback function.

    There is no binding for the glfwWindowHintString function. Simply pass your
    string to the windowHint function as you would for any other value type.

    The window parameter of the {set,get}ClipboardString functions has been
    deprecated and is no longer used. You may pass the unit value (or anything)
    as the window argument.

    There is no binding for the glfwGetProcAddress function because it would
    basicaly require to write an entire OpenGL wrapper to make the functions
    returned by GLFW usable from OCaml. Incidentaly the glfwExtensionSupported
    function is not provided either. There are numerous OpenGL bindings
    available for OCaml that you can use instead.

    The Vulkan related functions are not supported as of now but we might look
    into it at some point.

    @see <http://www.glfw.org/docs/latest/glfw3_8h.html#func-members> *)

external init : unit -> unit = "caml_glfwInit"
external terminate : unit -> unit = "caml_glfwTerminate"
external initHint : hint:'a init_hint -> value:'a -> unit = "caml_glfwInitHint"
external getVersion : unit -> int * int * int = "caml_glfwGetVersion"
external getVersionString : unit -> string = "caml_glfwGetVersionString"
external getMonitors : unit -> monitor list = "caml_glfwGetMonitors"
external getPrimaryMonitor : unit -> monitor = "caml_glfwGetPrimaryMonitor"
external getMonitorPos : monitor:monitor -> int * int = "caml_glfwGetMonitorPos"
external getMonitorWorkarea : monitor:monitor -> int * int * int * int
  = "caml_glfwGetMonitorWorkarea"
external getMonitorPhysicalSize : monitor:monitor -> int * int
  = "caml_glfwGetMonitorPhysicalSize"
external getMonitorContentScale : monitor:monitor -> float * float
  = "caml_glfwGetMonitorContentScale"
external getMonitorName : monitor:monitor -> string = "caml_glfwGetMonitorName"
external setMonitorCallback :
  f:(monitor -> connection_event -> unit) option
  -> (monitor -> connection_event -> unit) option
  = "caml_glfwSetMonitorCallback"
external getVideoModes : monitor:monitor -> video_mode list
  = "caml_glfwGetVideoModes"
external getVideoMode : monitor:monitor -> video_mode = "caml_glfwGetVideoMode"
external setGamma : monitor:monitor -> gamma:float -> unit = "caml_glfwSetGamma"
external getGammaRamp : monitor:monitor -> GammaRamp.t = "caml_glfwGetGammaRamp"
external setGammaRamp : monitor:monitor -> gamma_ramp:GammaRamp.t -> unit
  = "caml_glfwSetGammaRamp"
external defaultWindowHints : unit -> unit = "caml_glfwDefaultWindowHints"
external windowHint : hint:('a, [`hint]) window_attr -> value:'a -> unit
  = "caml_glfwWindowHint"
external createWindow :
  width:int -> height:int -> title:string -> ?monitor:monitor -> ?share:window
  -> unit -> window
  = "caml_glfwCreateWindow_byte" "caml_glfwCreateWindow"
external destroyWindow : window:window -> unit = "caml_glfwDestroyWindow"
external windowShouldClose : window:window -> bool
  = "caml_glfwWindowShouldClose"
external setWindowShouldClose : window:window -> b:bool -> unit
  = "caml_glfwSetWindowShouldClose"
external setWindowTitle : window:window -> title:string -> unit
  = "caml_glfwSetWindowTitle"
external setWindowIcon : window:window -> images:Image.t list -> unit
  = "caml_glfwSetWindowIcon"
external getWindowPos : window:window -> int * int = "caml_glfwGetWindowPos"
external setWindowPos : window:window -> xpos:int -> ypos:int -> unit
  = "caml_glfwSetWindowPos"
external getWindowSize : window:window -> int * int = "caml_glfwGetWindowSize"
external setWindowSizeLimits :
  window:window -> minWidth:int option -> minHeight:int option
  -> maxWidth:int option -> maxHeight:int option -> unit
  = "caml_glfwSetWindowSizeLimits"
external setWindowAspectRatio : window:window -> numer:int -> denom:int -> unit
  = "caml_glfwSetWindowAspectRatio"
external setWindowSize : window:window -> width:int -> height:int -> unit
  = "caml_glfwSetWindowSize"
external getFramebufferSize : window:window -> int * int
  = "caml_glfwGetFramebufferSize"
external getWindowFrameSize : window:window -> int * int * int * int
  = "caml_glfwGetWindowFrameSize"
external getWindowContentScale : window:window -> float * float
  = "caml_glfwGetWindowContentScale"
external getWindowOpacity : window:window -> float = "caml_glfwGetWindowOpacity"
external setWindowOpacity : window:window -> opacity:float -> unit
  = "caml_glfwSetWindowOpacity"
external iconifyWindow : window:window -> unit = "caml_glfwIconifyWindow"
external restoreWindow : window:window -> unit = "caml_glfwRestoreWindow"
external maximizeWindow : window:window -> unit = "caml_glfwMaximizeWindow"
external showWindow : window:window -> unit = "caml_glfwShowWindow"
external hideWindow : window:window -> unit = "caml_glfwHideWindow"
external focusWindow : window:window -> unit = "caml_glfwFocusWindow"
external requestWindowAttention : window:window -> unit
  = "caml_glfwRequestWindowAttention"
external getWindowMonitor : window:window -> monitor option
  = "caml_glfwGetWindowMonitor"
external setWindowMonitor :
  window:window -> monitor:monitor option -> xpos:int -> ypos:int -> width:int
  -> height:int -> refreshRate:int option -> unit
  = "caml_glfwSetWindowMonitor_byte" "caml_glfwSetWindowMonitor"
external getWindowAttrib :
  window:window -> attribute:('a, [`attr]) window_attr -> 'a
  = "caml_glfwGetWindowAttrib"
external setWindowAttrib :
  window:window -> attribute:('a, [`update]) window_attr -> value:'a -> unit
  = "caml_glfwSetWindowAttrib"
external setWindowPosCallback :
  window:window -> f:(window -> int -> int -> unit) option
  -> (window -> int -> int -> unit) option
  = "caml_glfwSetWindowPosCallback"
external setWindowSizeCallback :
  window:window -> f:(window -> int -> int -> unit) option
  -> (window -> int -> int -> unit) option
  = "caml_glfwSetWindowSizeCallback"
external setWindowCloseCallback :
  window:window -> f:(window -> unit) option -> (window -> unit) option
  = "caml_glfwSetWindowCloseCallback"
external setWindowRefreshCallback :
  window:window -> f:(window -> unit) option -> (window -> unit) option
  = "caml_glfwSetWindowRefreshCallback"
external setWindowFocusCallback :
  window:window -> f:(window -> bool -> unit) option
  -> (window -> bool -> unit) option
  = "caml_glfwSetWindowFocusCallback"
external setWindowIconifyCallback :
  window:window -> f:(window -> bool -> unit) option
  -> (window -> bool -> unit) option
  = "caml_glfwSetWindowIconifyCallback"
external setWindowMaximizeCallback :
  window:window -> f:(window -> bool -> unit) option
  -> (window -> bool -> unit) option
  = "caml_glfwSetWindowMaximizeCallback"
external setFramebufferSizeCallback :
  window:window -> f:(window -> int -> int -> unit) option
  -> (window -> int -> int -> unit) option
  = "caml_glfwSetFramebufferSizeCallback"
external setWindowContentScaleCallback :
  window:window -> f:(window -> float -> float -> unit) option
  -> (window -> float -> float -> unit) option
  = "caml_glfwSetWindowContentScaleCallback"
external pollEvents : unit -> unit = "caml_glfwPollEvents"
external waitEvents : unit -> unit = "caml_glfwWaitEvents"
external waitEventsTimeout : timeout:float -> unit
  = "caml_glfwWaitEventsTimeout"
external postEmptyEvent : unit -> unit = "caml_glfwPostEmptyEvent"
external getInputMode : window:window -> mode:'a input_mode -> 'a
  = "caml_glfwGetInputMode"
external setInputMode : window:window -> mode:'a input_mode -> value:'a -> unit
  = "caml_glfwGetInputMode"
external rawMouseMotionSupported : unit -> bool
  = "caml_glfwRawMouseMotionSupported"
external getKeyName : key:key -> scancode:int -> string option
  = "caml_glfwGetKeyName"
external getKeyScancode : key:key -> int = "caml_glfwGetKeyScancode"
external getKey : window:window -> key:key -> bool = "caml_glfwGetKey"
external getMouseButton : window:window -> button:int -> bool
  = "caml_glfwGetMouseButton"
external getCursorPos : window:window -> float * float = "caml_glfwGetCursorPos"
external setCursorPos : window:window -> xpos:float -> ypos:float -> unit
  = "caml_glfwSetCursorPos"
external createCursor : image:Image.t -> xhot:int -> yhot:int -> cursor
  = "caml_glfwCreateCursor"
external createStandardCursor : shape:cursor_shape -> cursor
  = "caml_glfwCreateStandardCursor"
external destroyCursor : cursor:cursor -> unit = "caml_glfwDestroyCursor"
external setCursor : window:window -> cursor:cursor -> unit
  = "caml_glfwSetCursor"
external setKeyCallback :
  window:window
  -> f:(window -> key -> int -> key_action -> key_mod list -> unit) option
  -> (window -> key -> int -> key_action -> key_mod list -> unit) option
  = "caml_glfwSetKeyCallback"
external setCharCallback :
  window:window -> f:(window -> int -> unit) option
  -> (window -> int -> unit) option
  = "caml_glfwSetCharCallback"
external setCharModsCallback :
  window:window -> f:(window -> int -> key_mod list -> unit) option
  -> (window -> int -> key_mod list -> unit) option
  = "caml_glfwSetCharModsCallback" [@@deprecated]
external setMouseButtonCallback :
  window:window -> f:(window -> int -> bool -> key_mod list -> unit) option
  -> (window -> int -> bool -> key_mod list -> unit) option
  = "caml_glfwSetMouseButtonCallback"
external setCursorPosCallback :
  window:window -> f:(window -> float -> float -> unit) option
  -> (window -> float -> float -> unit) option
  = "caml_glfwSetCursorPosCallback"
external setCursorEnterCallback :
  window:window -> f:(window -> bool -> unit) option
  -> (window -> bool -> unit) option
  = "caml_glfwSetCursorEnterCallback"
external setScrollCallback :
  window:window -> f:(window -> float -> float -> unit) option
  -> (window -> float -> float -> unit) option
  = "caml_glfwSetScrollCallback"
external setDropCallback :
  window:window -> f:(window -> string list -> unit) option
  -> (window -> string list -> unit) option
  = "caml_glfwSetDropCallback"
external joystickPresent : joy:int -> bool = "caml_glfwJoystickPresent"
external getJoystickAxes : joy:int -> float array = "caml_glfwGetJoystickAxes"
external getJoystickButtons : joy:int -> bool array
  = "caml_glfwGetJoystickButtons"
external getJoystickHats : joy:int -> hat_status list array
  = "caml_glfwGetJoystickHats"
external getJoystickName : joy:int -> string option = "caml_glfwGetJoystickName"
external getJoystickGUID : joy:int -> string option = "caml_glfwGetJoystickGUID"
external joystickIsGamepad : joy:int -> bool = "caml_glfwJoystickIsGamepad"
external setJoystickCallback :
  f:(int -> connection_event -> unit) option
  -> (int -> connection_event -> unit) option
  = "caml_glfwSetJoystickCallback"
external updateGamepadMappings : string:string -> unit
  = "caml_glfwUpdateGamepadMappings"
external getGamepadName : joy:int -> string option = "caml_glfwGetGamepadName"
external getGamepadState : joy:int -> gamepad_state = "caml_glfwGetGamepadState"
external setClipboardString : window:_ -> string:string -> unit
  = "caml_glfwSetClipboardString"
external getClipboardString : window:_ -> string = "caml_glfwGetClipboardString"
external getTime : unit -> float = "caml_glfwGetTime"
external setTime : time:float -> unit = "caml_glfwSetTime"
external getTimerValue : unit -> int64 = "caml_glfwGetTimerValue"
external getTimerFrequency : unit -> int64 = "caml_glfwGetTimerFrequency"
external makeContextCurrent : window:window option -> unit
  = "caml_glfwMakeContextCurrent"
external getCurrentContext : unit -> window option
  = "caml_glfwGetCurrentContext"
external swapBuffers : window:window -> unit = "caml_glfwSwapBuffers"
external swapInterval : interval:int -> unit = "caml_glfwSwapInterval"
