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
  | None -> []
  | Some config ->
     match C.Pkg_config.query ~package:"glfw3" config with
     | None -> []
     | Some deps -> deps.cflags
  end
  |> find_header
  |> Printf.printf "(%s)\n"

let () =
  C.main ~name:"generate_header_dep" main
