#include <ruby.h>
#include <semaphore.h>

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


typedef struct sys_stuff_posix_namedsemaphore {
  sem_t *sem;
  int a;
} sys_stuff_posix_namedsemaphore;


static VALUE
sys_stuff_posix_namedsemaphore_alloc(VALUE klass) {
  VALUE obj;
  sys_stuff_posix_namedsemaphore *ptr;

  ptr = ruby_xmalloc(sizeof(sys_stuff_posix_namedsemaphore));
  obj = Data_Wrap_Struct(klass, NULL, ruby_xfree, ptr);
  ptr->sem = NULL;
  ptr->a = 77;

  return obj;
}


static VALUE
sys_stuff_posix_hello(VALUE self) {
 
  sys_stuff_posix_namedsemaphore *ptr;
  Data_Get_Struct(self, sys_stuff_posix_namedsemaphore, ptr);
  printf("Hello from C POSIX sem is %d\n", ptr->a);
 
  return self;
}

void
Init_sys_stuff(void) {

  VALUE cSysStuffPosix = rb_define_module_under(rb_define_module("SysStuff"), "Posix");
  
  VALUE cSysStuff = rb_define_class_under(cSysStuffPosix, "Malloc", rb_cObject);

  rb_define_alloc_func(cSysStuff, sys_stuff_alloc);
  rb_define_method(cSysStuff, "alloc", sys_stuff_init, 1);
  rb_define_method(cSysStuff, "free", sys_stuff_release, 0);

  VALUE cNamedSemaphore = rb_define_class_under(cSysStuffPosix, "NamedSemaphore", rb_cObject);

  rb_define_alloc_func(cNamedSemaphore, sys_stuff_posix_namedsemaphore_alloc);

  rb_define_method(cNamedSemaphore, "hello", sys_stuff_posix_hello, 0);
}
