external glClearColorBufferBit : unit -> unit = "caml_glClearColorBufferBit"

let () =
  (* Initialize the library *)
  GLFW.init ();
  at_exit GLFW.terminate;
  (* Create a windowed mode window and its OpenGL context *)
  let window = GLFW.createWindow 640 480 "Hello World" () in
  (* Make the window's context current *)
  GLFW.makeContextCurrent (Some window);
  (* Loop until the user closes the window *)
  while not (GLFW.windowShouldClose window) do
    (* Render here *)
    glClearColorBufferBit ();
    (* Swap front and back buffers *)
    GLFW.swapBuffers window;
    (* Poll for and process events *)
    GLFW.pollEvents ()
  done
