let read_keys header_location =
  let glfw_header = open_in header_location in
  let delim_regexp = Str.regexp "[ \t]+" in
  let prefix_regexp = Str.regexp "GLFW_KEY_" in
  let rec loop acc i =
    let line = String.trim (input_line glfw_header) in
    match Str.split delim_regexp line with
    | "#define" :: "GLFW_KEY_LAST" :: _ ->
       close_in_noerr glfw_header;
       List.rev acc
    | "#define" :: name :: value :: _
         when Str.string_partial_match prefix_regexp name 0 ->
       loop ((i, name, int_of_string value) :: acc) (succ i)
    | _ -> loop acc i
  in
  let keys = loop [] 0 in
  close_in_noerr glfw_header;
  keys

let generate keys =
  (* Unknown being used as the default value, we can omit the corresponding
     GLFW key when converting from C to OCaml, saving memory for that table. *)
  let min_key, max_key =
    List.fold_left (fun (min_key, max_key) (_, name, v) ->
        if name = "GLFW_KEY_UNKNOWN"
        then min_key, max_key
        else min min_key v, max max_key v
      ) (max_int, min_int) keys
  in
  let open Printf in
  printf "#define GLFW_KEY_FIRST %d\n\n" min_key;
  printf "static const int ml_to_glfw_key[] = {\n";
  List.iter (fun (_, name, _) -> printf "    %s,\n" name) keys;
  printf "};\n\n";
  printf "static const int glfw_to_ml_key[] = {\n";
  let rec loop i = function
    | _ when i > max_key -> ()
    | (_, "GLFW_KEY_UNKNOWN", _) :: tl -> loop i tl
    | (ml_val, _, glfw_val) :: tl when i = glfw_val ->
       printf "    %d,\n" ml_val;
       loop (i + 1) tl
    | keys ->
       printf "    0,\n"; (* Unknown *)
       loop (i + 1) keys
  in
  loop min_key keys;
  printf "};\n"

let () =
  let header_location =
    if Array.length Sys.argv < 2
    then "/usr/include/GLFW/glfw3.h"
    else Sys.argv.(1)
  in
  generate (read_keys header_location)
