let version_major = 3
let version_minor = 3
let version_revision = 0

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

type key_action =
  | Release
  | Press
  | Repeat

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

type key_mod =
  | Shift
  | Control
  | Alt
  | Super

let mouse_button_max_count = 8

let mouse_button_left = 0
let mouse_button_right = 1
let mouse_button_middle = 2

let joystick_max_count = 16

type client_api =
  | NoApi
  | OpenGLApi
  | OpenGLESApi

type context_robustness =
  | NoRobustness
  | NoResetNotification
  | LoseContextOnReset

type opengl_profile =
  | AnyProfile
  | CoreProfile
  | CompatProfile

type context_release_behavior =
  | AnyReleaseBehavior
  | ReleaseBehaviorFlush
  | ReleaseBehaviorNone

type context_creation_api =
  | NativeContextApi
  | EGLContextApi

module WindowHint =
  struct
    type _ t =
      | Focused : bool t
      | Resizable : bool t
      | Visible : bool t
      | Decorated : bool t
      | AutoIconify : bool t
      | Floating : bool t
      | Maximized : bool t
      | FocusOnShow : bool t
      | RedBits : int option t
      | GreenBits : int option t
      | BlueBits : int option t
      | AlphaBits : int option t
      | DepthBits : int option t
      | StencilBits : int option t
      | AccumRedBits : int option t
      | AccumGreenBits : int option t
      | AccumBlueBits : int option t
      | AccumAlphaBits : int option t
      | AuxBuffers : int option t
      | Stereo : bool t
      | Samples : int option t
      | SrgbCapable : bool t
      | RefreshRate : int option t
      | DoubleBuffer : bool t
      | ClientApi : client_api t
      | ContextVersionMajor : int t
      | ContextVersionMinor : int t
      | ContextRobustness : context_robustness t
      | OpenGLForwardCompat : bool t
      | OpenGLDebugContext : bool t
      | OpenGLProfile : opengl_profile t
      | ContextReleaseBehavior : context_release_behavior t
      | ContextNoError : bool t
      | ContextCreationApi : context_creation_api t
      | ScaleToMonitor : bool t
  end

module WindowAttribute =
  struct
    type _ t =
      | Focused : bool t
      | Iconified : bool t
      | Resizable : bool t
      | Visible : bool t
      | Decorated : bool t
      | AutoIconify : bool t
      | Floating : bool t
      | Maximized : bool t
      | FocusOnShow : bool t
      | ClientApi : client_api t
      | ContextVersionMajor : int t
      | ContextVersionMinor : int t
      | ContextRevision : int t
      | ContextRobustness : context_robustness t
      | OpenGLForwardCompat : bool t
      | OpenGLDebugContext : bool t
      | OpenGLProfile : opengl_profile t
      | ContextCreationApi : context_creation_api t
  end

module WindowUpdateableAttribute =
  struct
    type _ t =
      | Resizable : bool t
      | Decorated : bool t
      | AutoIconify : bool t
      | Floating : bool t
      | FocusOnShow : bool t
  end

type cursor_mode =
  | Normal
  | Hidden
  | Disabled

type _ input_mode =
  | Cursor : cursor_mode input_mode
  | StickyKeys : bool input_mode
  | StickyMouseButtons : bool input_mode

type cursor_shape =
  | ArrowCursor
  | IBeamCursor
  | CrosshairCursor
  | HandCursor
  | HResizeCursor
  | VResizeCursor

type connection_event =
  | Connected
  | Disconnected

type video_mode = {
    width : int;
    height : int;
    red_bits : int;
    green_bits : int;
    blue_bits : int;
    refresh_rate : int
  }

type monitor

type window

type cursor

module GammaRamp =
  struct
    open Bigarray

    type channel = (int, int16_unsigned_elt, c_layout) Array1.t
    type t = { red : channel; green : channel; blue : channel }

    let create ~red ~green ~blue =
      let red_dim = Array1.dim red in
      if red_dim = Array1.dim green && red_dim = Array1.dim blue
      then { red; green; blue }
      else invalid_arg "GammaRamp.make: inconsistent channel dimension."

    let make ~size = {
        red = Array1.create Int16_unsigned C_layout size;
        green = Array1.create Int16_unsigned C_layout size;
        blue = Array1.create Int16_unsigned C_layout size;
      }
  end

module Image =
  struct
    type t = { width : int; height : int; pixels : bytes }

    let create ~width ~height ~pixels =
      if width < 0 || height < 0
      then invalid_arg "Image.create: negative dimension."
      else if width * height * 4 > Bytes.length pixels
      then invalid_arg "Image.create: insufficient pixel data."
      else { width; height; pixels }
  end

type gamepad_state = {
    buttons : bool array;
    axes : float array;
  }

external init : unit -> unit = "caml_glfwInit"
external terminate : unit -> unit = "caml_glfwTerminate"
external getVersion : unit -> int * int * int = "caml_glfwGetVersion"
external getVersionString : unit -> string = "caml_glfwGetVersionString"
external getMonitors : unit -> monitor list = "caml_glfwGetMonitors"
external getPrimaryMonitor : unit -> monitor = "caml_glfwGetPrimaryMonitor"
external getMonitorPos : monitor:monitor -> int * int = "caml_glfwGetMonitorPos"
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
external windowHint : hint:'a WindowHint.t -> value:'a -> unit
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
external iconifyWindow : window:window -> unit = "caml_glfwIconifyWindow"
external restoreWindow : window:window -> unit = "caml_glfwRestoreWindow"
external maximizeWindow : window:window -> unit = "caml_glfwMaximizeWindow"
external showWindow : window:window -> unit = "caml_glfwShowWindow"
external hideWindow : window:window -> unit = "caml_glfwHideWindow"
external focusWindow : window:window -> unit = "caml_glfwFocusWindow"
external getWindowMonitor : window:window -> monitor option
  = "caml_glfwGetWindowMonitor"
external setWindowMonitor :
  window:window -> monitor:monitor option -> xpos:int -> ypos:int -> width:int
  -> height:int -> refreshRate:int option -> unit
  = "caml_glfwSetWindowMonitor_byte" "caml_glfwSetWindowMonitor"
external getWindowAttrib : window:window -> attribute:'a WindowAttribute.t -> 'a
  = "caml_glfwGetWindowAttrib"
external setWindowAttrib :
  window:window -> attribute:'a WindowUpdateableAttribute.t -> value:'a -> unit
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
external getKeyName : key:key -> scancode:int -> string option
  = "caml_glfwGetKeyName"
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
  = "caml_glfwSetCharModsCallback"
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
external setClipboardString : window:window -> string:string -> unit
  = "caml_glfwSetClipboardString"
external getClipboardString : window:window -> string
  = "caml_glfwGetClipboardString"
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

external init_stub : unit -> unit = "init_stub" [@@noalloc]

let () =
  Callback.register_exception "GLFW.NotInitialized" (NotInitialized "");
  Callback.register_exception "GLFW.NoCurrentContext" (NoCurrentContext "");
  Callback.register_exception "GLFW.InvalidEnum" (InvalidEnum "");
  Callback.register_exception "GLFW.InvalidValue" (InvalidValue "");
  Callback.register_exception "GLFW.OutOfMemory" (OutOfMemory "");
  Callback.register_exception "GLFW.ApiUnavailable" (ApiUnavailable "");
  Callback.register_exception "GLFW.VersionUnavailable" (VersionUnavailable "");
  Callback.register_exception "GLFW.PlatformError" (PlatformError "");
  Callback.register_exception "GLFW.FormatUnavailable" (FormatUnavailable "");
  Callback.register_exception "GLFW.NoWindowContext" (NoWindowContext "");
  init_stub ()
