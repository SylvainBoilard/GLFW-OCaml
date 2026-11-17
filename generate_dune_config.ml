module C = Configurator.V1

let rec find_header = function
  | [] -> "/usr/include/GLFW/glfw3.h"
  | hd :: tl when String.length hd < 2 || Str.first_chars hd 2 <> "-I" ->
     find_header tl
  | hd :: tl ->
     let header_location = Str.string_after hd 2 ^ "/GLFW/glfw3.h" in
     if Sys.file_exists header_location
     then header_location
     else find_header tl

let main c =
  begin match C.Pkg_config.get c with
  | None -> None
  | Some config -> C.Pkg_config.query ~package:"glfw3" config
  end |> function
  | None -> failwith "pkg-config query failed"
  | Some glfw_config ->
     let open_out_truncate path =
       open_out_gen [Open_creat; Open_trunc; Open_wronly] 0o664 path
     in
     let cflags_sexp = open_out_truncate "glfw_cflags.sexp" in
     let libs_sexp = open_out_truncate "glfw_libs.sexp" in
     let header_sexp = open_out_truncate "glfw_header.sexp" in
     Printf.fprintf cflags_sexp "(%s)\n" (String.concat " " glfw_config.cflags);
     Printf.fprintf libs_sexp "(%s)\n" (String.concat " " glfw_config.libs);
     Printf.fprintf header_sexp "(%s)\n" (find_header glfw_config.cflags);
     close_out_noerr cflags_sexp;
     close_out_noerr libs_sexp;
     close_out_noerr header_sexp

let () =
  C.main ~name:"generate_dune_config" main
