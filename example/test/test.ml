external glClearColorBufferBit : unit -> unit = "caml_glClearColorBufferBit"

(* Callback logging system to avoid repetition *)

module CallbackLogger = struct
  type callback_name =
    | WindowPos
    | WindowSize
    | WindowClose
    | WindowRefresh
    | WindowFocus
    | WindowIconify
    | WindowMaximize
    | WindowContentScale
    | FramebufferSize
    | Key
    | Char
    | CharMods
    | MouseButton
    | CursorPos
    | CursorEnter
    | Scroll
    | Drop
    | Monitor
    | Joystick

  (* Callbacks that should have their repeated calls deduplicated *)
  let deduplicate = function
    | WindowPos | WindowSize | WindowRefresh | WindowContentScale
    | FramebufferSize | Scroll | CursorPos -> true
    | Key -> true (* only for Repeat actions *)
    | _ -> false

  type callback_state =
    | NoCall
    | Called of {
        name : callback_name;
        mutable count : int;
        first_data : string;
        mutable last_data : string;
        mutable acc_x : float;  (* Accumulated scroll x *)
        mutable acc_y : float;  (* Accumulated scroll y *)
      }

  let current_state = ref NoCall

  let clear_line () =
    Printf.eprintf "\r\x1b[K"

  let callback_name_to_string = function
    | WindowPos -> "WindowPos"
    | WindowSize -> "WindowSize"
    | WindowClose -> "WindowClose"
    | WindowRefresh -> "WindowRefresh"
    | WindowFocus -> "WindowFocus"
    | WindowIconify -> "WindowIconify"
    | WindowMaximize -> "WindowMaximize"
    | WindowContentScale -> "WindowContentScale"
    | FramebufferSize -> "FramebufferSize"
    | Key -> "Key"
    | Char -> "Char"
    | CharMods -> "CharMods"
    | MouseButton -> "MouseButton"
    | CursorPos -> "CursorPos"
    | CursorEnter -> "CursorEnter"
    | Scroll -> "Scroll"
    | Drop -> "Drop"
    | Monitor -> "Monitor"
    | Joystick -> "Joystick"

  let key_action_to_string = function
    | GLFW.Release -> "Release"
    | GLFW.Press -> "Press"
    | GLFW.Repeat -> "Repeat"

  let key_mod_to_string = function
    | GLFW.Shift -> "Shift"
    | GLFW.Control -> "Control"
    | GLFW.Alt -> "Alt"
    | GLFW.Super -> "Super"
    | GLFW.CapsLock -> "CapsLock"
    | GLFW.NumLock -> "NumLock"

  let mods_to_string = function
    | [] -> "(none)"
    | mods -> String.concat "+" (List.map key_mod_to_string mods)

  let key_should_deduplicate prev_data new_data =
    let prev_parts = String.split_on_char ' ' prev_data in
    let new_parts = String.split_on_char ' ' new_data in
    match prev_parts, new_parts with
    | prev_key :: prev_rest, new_key :: new_rest ->
        prev_key = new_key
        && List.exists ((=) "Repeat") prev_rest
        && List.exists ((=) "Repeat") new_rest
    | _ -> false

  let format_data name first_data last_data acc_x acc_y =
    match name with
    | WindowPos | CursorPos | WindowSize | WindowContentScale | FramebufferSize ->
        Printf.sprintf "%s -> %s" first_data last_data
    | Scroll ->
        Printf.sprintf "(%.2f,%.2f)" acc_x acc_y
    | WindowClose | WindowRefresh -> ""
    | _ -> last_data

  let log_call name data_str ?(acc_x=0.0) ?(acc_y=0.0) () =
    match !current_state with
    | Called prev when prev.name = name && (name <> Key || key_should_deduplicate prev.last_data data_str) ->
        let new_acc_x = if name = Scroll then prev.acc_x +. acc_x else prev.acc_x in
        let new_acc_y = if name = Scroll then prev.acc_y +. acc_y else prev.acc_y in
        let display_str = format_data name prev.first_data data_str new_acc_x new_acc_y in
        clear_line ();
        Printf.eprintf "[%s] %s (×%d)%!" (callback_name_to_string name) display_str (prev.count + 1);
        prev.count <- prev.count + 1;
        prev.last_data <- data_str;
        if name = Scroll then (prev.acc_x <- new_acc_x; prev.acc_y <- new_acc_y)
    | _ ->
        if !current_state <> NoCall then Printf.eprintf "\n";
        let display_str = format_data name data_str data_str acc_x acc_y in
        if deduplicate name then (
          Printf.eprintf "[%s] %s%!" (callback_name_to_string name) display_str;
          current_state := Called { name; count = 1; first_data = data_str;
                                    last_data = data_str; acc_x; acc_y }
        ) else (
          Printf.eprintf "[%s] %s\n%!" (callback_name_to_string name) display_str;
          current_state := NoCall
        )

  let finalize () =
    match !current_state with
    | NoCall -> ()
    | Called _ -> Printf.eprintf "\n%!"
end

(* Individual callback handlers *)

let window_pos_callback _window xpos ypos =
  let open CallbackLogger in
  let data = Printf.sprintf "(%d,%d)" xpos ypos in
  log_call WindowPos data ()

let window_size_callback _window width height =
  let open CallbackLogger in
  let data = Printf.sprintf "%d×%d" width height in
  log_call WindowSize data ()

let window_close_callback _window =
  let open CallbackLogger in
  log_call WindowClose "" ()

let window_refresh_callback _window =
  let open CallbackLogger in
  log_call WindowRefresh "" ()

let window_focus_callback _window focused =
  let open CallbackLogger in
  let state = if focused then "focused" else "unfocused" in
  log_call WindowFocus state ()

let window_iconify_callback _window iconified =
  let open CallbackLogger in
  let state = if iconified then "iconified" else "restored" in
  log_call WindowIconify state ()

let window_maximize_callback _window maximized =
  let open CallbackLogger in
  let state = if maximized then "maximized" else "restored" in
  log_call WindowMaximize state ()

let window_content_scale_callback _window xscale yscale =
  let open CallbackLogger in
  let data = Printf.sprintf "%.2f×%.2f" xscale yscale in
  log_call WindowContentScale data ()

let framebuffer_size_callback _window width height =
  let open CallbackLogger in
  let data = Printf.sprintf "%d×%d" width height in
  log_call FramebufferSize data ()

let scroll_callback _window xoffset yoffset =
  let open CallbackLogger in
  let data = Printf.sprintf "(%.2f,%.2f)" xoffset yoffset in
  log_call Scroll data ~acc_x:xoffset ~acc_y:yoffset ()

let key_callback _window key scancode action mods =
  let open CallbackLogger in
  let key_name = Option.value (GLFW.getKeyName key scancode) ~default:"Unknown" in
  let action_str = key_action_to_string action in
  let mods_str = mods_to_string mods in
  let data = Printf.sprintf "%s scan=%d %s mods=%s" key_name scancode action_str mods_str in
  log_call Key data ()

let codepoint_to_utf8 codepoint =
  if Uchar.is_valid codepoint then
    let buf = Buffer.create 4 in
    Buffer.add_utf_8_uchar buf (Uchar.of_int codepoint);
    Buffer.contents buf
  else "<invalid>"

let char_callback _window codepoint =
  let open CallbackLogger in
  let char_str = codepoint_to_utf8 codepoint in
  let data = Printf.sprintf "%s (U+%04X)" char_str codepoint in
  log_call Char data ()

let char_mods_callback _window codepoint mods =
  let open CallbackLogger in
  let char_str = codepoint_to_utf8 codepoint in
  let data = Printf.sprintf "%s (U+%04X) mods=%s" char_str codepoint (mods_to_string mods) in
  log_call CharMods data ()

let mouse_button_callback _window button action mods =
  let open CallbackLogger in
  let action_str = if action then "Press" else "Release" in
  let data = Printf.sprintf "btn=%d %s mods=%s" button action_str (mods_to_string mods) in
  log_call MouseButton data ()

let cursor_pos_callback _window xpos ypos =
  let open CallbackLogger in
  let data = Printf.sprintf "(%.1f,%.1f)" xpos ypos in
  log_call CursorPos data ()

let cursor_enter_callback _window entered =
  let open CallbackLogger in
  let state = if entered then "entered" else "left" in
  log_call CursorEnter state ()

let drop_callback _window paths =
  let open CallbackLogger in
  let data = Printf.sprintf "[%s]" (String.concat "," paths) in
  log_call Drop data ()

let monitor_callback monitor event =
  let open CallbackLogger in
  let event_str = match event with
    | GLFW.Connected -> "connected"
    | GLFW.Disconnected -> "disconnected"
  in
  let data = Printf.sprintf "%s %s" (GLFW.getMonitorName monitor) event_str in
  log_call Monitor data ()

let joystick_callback joystick event =
  let open CallbackLogger in
  let data = match event with
    | GLFW.Connected ->
       let name = Option.value (GLFW.getJoystickName joystick) ~default:"(no name)" in
       Printf.sprintf "#%d %s connected" joystick name
    | GLFW.Disconnected -> Printf.sprintf "#%d disconnected" joystick
  in
  log_call Joystick data ()

let () =
  GLFW.init ();
  at_exit (fun () ->
    CallbackLogger.finalize ();
    GLFW.terminate ());
  let window = GLFW.createWindow 640 480 "GLFW-OCaml Test App" () in

  (* Window callbacks *)
  ignore (GLFW.setWindowPosCallback window (Some window_pos_callback));
  ignore (GLFW.setWindowSizeCallback window (Some window_size_callback));
  ignore (GLFW.setWindowCloseCallback window (Some window_close_callback));
  ignore (GLFW.setWindowRefreshCallback window (Some window_refresh_callback));
  ignore (GLFW.setWindowFocusCallback window (Some window_focus_callback));
  ignore (GLFW.setWindowIconifyCallback window (Some window_iconify_callback));
  ignore (GLFW.setWindowMaximizeCallback window (Some window_maximize_callback));
  ignore (GLFW.setWindowContentScaleCallback window (Some window_content_scale_callback));
  ignore (GLFW.setFramebufferSizeCallback window (Some framebuffer_size_callback));

  (* Input callbacks *)
  ignore (GLFW.setKeyCallback window (Some key_callback));
  ignore (GLFW.setCharCallback window (Some char_callback));
  ignore (GLFW.setCharModsCallback window (Some char_mods_callback) [@warning "-3"]);
  ignore (GLFW.setMouseButtonCallback window (Some mouse_button_callback));
  ignore (GLFW.setCursorPosCallback window (Some cursor_pos_callback));
  ignore (GLFW.setCursorEnterCallback window (Some cursor_enter_callback));
  ignore (GLFW.setScrollCallback window (Some scroll_callback));
  ignore (GLFW.setDropCallback window (Some drop_callback));

  ignore (GLFW.setMonitorCallback (Some monitor_callback));
  ignore (GLFW.setJoystickCallback (Some joystick_callback));
  GLFW.makeContextCurrent (Some window);
  while not (GLFW.windowShouldClose window) do
    glClearColorBufferBit ();
    GLFW.swapBuffers window;
    GLFW.pollEvents ()
  done
