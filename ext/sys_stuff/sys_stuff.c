#include <ruby.h>

struct sys_stuff {
  size_t size;
  void *ptr;
};

static void
sys_stuff_free(void *p) {
  struct sys_stuff *ptr = p;

  if (ptr->size > 0)
    free(ptr->ptr);
}

static VALUE
sys_stuff_alloc(VALUE klass) {
  VALUE obj;
  struct sys_stuff *ptr;

  obj = Data_Make_Struct(klass, struct sys_stuff, NULL, sys_stuff_free, ptr);

  ptr->size = 0;
  ptr->ptr  = NULL;

  return obj;
}

static VALUE
sys_stuff_init(VALUE self, VALUE size) {
  struct sys_stuff *ptr;
  size_t requested = NUM2SIZET(size);

  if (0 == requested)
    rb_raise(rb_eArgError, "unable to allocate 0 bytes");

  Data_Get_Struct(self, struct sys_stuff, ptr);

  ptr->ptr = malloc(requested);

  if (NULL == ptr->ptr)
    rb_raise(rb_eNoMemError, "unable to allocate %ld bytes", requested);

  printf("allocating %ld bytes\n", requested);

  ptr->size = requested;

  return self;
}

static VALUE
sys_stuff_release(VALUE self) {
  struct sys_stuff *ptr;

  Data_Get_Struct(self, struct sys_stuff, ptr);

  printf("Freeing %ld\n", ptr->size);

  if (0 == ptr->size)
    return self;

  ptr->size = 0;
  free(ptr->ptr);

  

  return self;
}

void
Init_sys_stuff(void) {
  VALUE cSysStuff;

    // Get symbol for our module's name
    ID sym_mymodule = rb_intern("SysStuff");
    // Get the module
    VALUE mymodule = rb_const_get(rb_cObject, sym_mymodule);
    // Get symbol for our class' name
    ID sym_myclass = rb_intern("Malloc");
    // Get the class
    cSysStuff = rb_const_get(mymodule, sym_myclass);

  //cSysStuff = rb_const_get(rb_cObject, rb_intern("SysStuff"));

  rb_define_alloc_func(cSysStuff, sys_stuff_alloc);
  rb_define_method(cSysStuff, "alloc", sys_stuff_init, 1);
  rb_define_method(cSysStuff, "free", sys_stuff_release, 0);
}
