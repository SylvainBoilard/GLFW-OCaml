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

all:	$(NAME_BYTE) $(NAME_OPT) $(NAME_TOPLEVEL)
byte:	$(NAME_BYTE)
opt:	$(NAME_OPT)
toplevel:	$(NAME_TOPLEVEL)

$(DEPENDS):	$(SRCS) $(INTS)
	ocamldep $^ > $@

-include $(DEPENDS)

$(OBJS):	GLFW_key_conv_arrays.inl
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
	$(OCAMLC) $(OCAMLCFLAGS) -custom $(OCAMLLDFLAGS) $^ -a -o $@
$(NAME_OPT):	$(CMXS) $(OBJS)
	$(OCAMLOPT) $(OCAMLOPTFLAGS) $(OCAMLLDFLAGS) $^ -a -o $@
	@rm GLFW.o GLFW.a
$(NAME_TOPLEVEL):	$(NAME_BYTE)
	$(OCAMLMKTOP) $^ -o $@

clean:
	rm -f $(DEPENDS) $(CMOS) $(CMXS) $(SRCS:%.ml=%.cmi) $(OBJS) \
		GLFW_key_conv_arrays.inl
fclean:	clean
	rm -f $(NAME_BYTE) $(NAME_OPT) $(NAME_TOPLEVEL)
re:
	@$(MAKE) $(MFLAGS) fclean
	@$(MAKE) $(MFLAGS) all
