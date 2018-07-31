NAME_BYTE := GLFW.cma
NAME_OPT := GLFW.cmxa
NAME_TOPLEVEL := ocaml.GLFW

OCAML := ocaml
OCAMLC := ocamlc
OCAMLCFLAGS :=
OCAMLOPT := ocamlopt
OCAMLOPTFLAGS :=
OCAMLLDFLAGS := -cclib -lglfw
OCAMLMKTOP := ocamlmktop

DEPENDS := .depends
SRCS := GLFW.ml
INTS := GLFW.mli
STUBS := GLFW_stubs.c
CMOS := $(SRCS:%.ml=%.cmo)
CMXS := $(SRCS:%.ml=%.cmx)
CMIS := $(INTS:%.mli=%.cmi)
OBJS := $(STUBS:%.c=%.o)

GENERATED := GLFW_key_conv_arrays.inl

LIBS_BYTE := bigarray.cma
LIBS_OPT := bigarray.a

all:	$(NAME_BYTE) $(NAME_OPT) $(NAME_TOPLEVEL)
byte:	$(NAME_BYTE)
opt:	$(NAME_OPT)
toplevel:	$(NAME_TOPLEVEL)

$(DEPENDS):	$(SRCS) $(INTS)
	ocamldep $^ > $@

-include $(DEPENDS)

$(OBJS):	$(GENERATED)
GLFW_key_conv_arrays.inl:
	$(OCAML) str.cma gen_key_conv_arrays.ml

%.cmo:	%.ml
	$(OCAMLC) $(OCAMLCFLAGS) -c $< -o $@
%.cmx:	%.ml
	$(OCAMLOPT) $(OCAMLOPTFLAGS) -c $< -o $@
%.cmi:	%.mli
	$(OCAMLC) $(OCAMLCFLAGS) -c $< -o $@
%.o:	%.c
	$(OCAMLC) $(OCAMLCFLAGS) -c $< -o $@

$(NAME_BYTE):	$(CMOS) $(OBJS)
	$(OCAMLC) $(OCAMLCFLAGS) -custom $(OCAMLLDFLAGS) $(LIBS_BYTE) $^ -a -o $@
$(NAME_OPT):	$(CMXS) $(OBJS)
	$(OCAMLOPT) $(OCAMLOPTFLAGS) $(OCAMLLDFLAGS) $(LIBS_OPT) $^ -a -o $@
$(NAME_TOPLEVEL):	$(NAME_BYTE)
	$(OCAMLMKTOP) $^ -o $@

clean:
	rm -f $(CMOS) $(CMXS) $(OBJS) $(SRCS:%.ml=%.cmi) $(CMXS:%.cmx=%.o) \
		$(DEPENDS) $(GENERATED)
fclean:	clean
	rm -f $(NAME_BYTE) $(NAME_OPT) $(NAME_OPT:%.cmxa=%.a) $(NAME_TOPLEVEL)
re:
	@$(MAKE) $(MFLAGS) fclean
	@$(MAKE) $(MFLAGS) all
