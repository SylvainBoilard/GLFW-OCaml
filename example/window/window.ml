external glClearColorBufferBit : unit -> unit = "caml_glClearColorBufferBit"

let () =
  GLFW.init ();
  at_exit GLFW.terminate;
  let window = GLFW.createWindow 640 480 "Hello World" () in
  GLFW.makeContextCurrent (Some window);
  while not (GLFW.windowShouldClose window) do
    glClearColorBufferBit ();
    GLFW.swapBuffers window;
    GLFW.pollEvents ()
  done
