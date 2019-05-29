#include <GL/gl.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>

#define CAMLvoid CAMLunused_start value unit CAMLunused_end

CAMLprim value caml_glClearColorBufferBit(CAMLvoid)
{
    glClear(GL_COLOR_BUFFER_BIT);
    return Val_unit;
}
