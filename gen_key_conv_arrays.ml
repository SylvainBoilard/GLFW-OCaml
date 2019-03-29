module C = Configurator.V1

let read_keys headers_location =
  let glfw_header = open_in (headers_location ^ "/GLFW/glfw3.h") in
  let delim_regexp = Str.regexp " +" in
  let prefix_regexp = Str.regexp "GLFW_KEY_" in
  let rec loop acc i =
    let line = input_line glfw_header in
    match Str.split delim_regexp line with
    | "#define" :: "GLFW_KEY_LAST" :: _ ->
       close_in_noerr glfw_header;
       List.rev acc
    | "#define" :: name :: value :: _
         when Str.string_partial_match prefix_regexp name 0 ->
       loop ((i, name, int_of_string value) :: acc) (succ i)
    | _ -> loop acc i
  in
  loop [] 0

let rec find_header_read_keys = function
  | [] -> read_keys "/usr/include"
  | hd :: tl when Str.first_chars hd 2 <> "-I" -> find_header_read_keys tl
  | hd :: tl ->
     try read_keys (Str.string_after hd 2)
     with Sys_error _ -> find_header_read_keys tl

let generate keys =
  let key_stub =
    open_out_gen
      [Open_creat; Open_trunc; Open_wronly] 0o664 "GLFW_key_conv_arrays.inl"
  in
  let min_key, max_key =
    List.fold_left (fun (min_key, max_key) (_, _, v) ->
        min min_key v, max max_key v
      ) (max_int, min_int) keys
  in
  let open Buffer in
  let open Printf in
  let buffer = create 8192 in
  add_string buffer (sprintf "#define GLFW_KEY_FIRST %d\n" min_key);
  add_string buffer "\nstatic const int ml_to_glfw_key[] = {\n";
  List.iter (fun (_, name, _) ->
      add_string buffer (sprintf "    %s,\n" name)
    ) keys;
  add_string buffer "};\n";
  add_string buffer "\nstatic const int glfw_to_ml_key[] = {\n";
  let keys = ref keys in
  for i = min_key to max_key do
    match !keys with
    | (ml_val, _, glfw_val) :: tl when i = glfw_val ->
       add_string buffer (sprintf "    %d,\n" ml_val);
       keys := tl
    | _ -> add_string buffer "    -1,\n"
  done;
  add_string buffer "};\n";
  output_buffer key_stub buffer;
  close_out_noerr key_stub

let main c =
  begin match C.Pkg_config.get c with
  | None -> []
  | Some config ->
     match C.Pkg_config.query ~package:"glfw3" config with
     | None -> []
     | Some deps -> deps.cflags
  end
  |> find_header_read_keys
  |> generate

let () =
  C.main ~name:"gen_key_conv_arrays" main
