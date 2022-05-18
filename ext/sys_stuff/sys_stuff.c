#include <ruby.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

typedef struct posix_namedsemaphore {
  sem_t *sem;
} posix_namedsemaphore;


static VALUE posix_namedsemaphore_alloc(VALUE klass) {
  VALUE obj;
  posix_namedsemaphore *ptr;

  ptr = ruby_xmalloc(sizeof(posix_namedsemaphore));
  obj = Data_Wrap_Struct(klass, NULL, ruby_xfree, ptr);
  ptr->sem = NULL;

  return obj;
}

static VALUE posix_namedsemaphore_create(VALUE self, VALUE rb_initial_value) { 
  VALUE rb_name = rb_ivar_get(self, rb_intern("@name"));

  Check_Type(rb_name, T_STRING);
  Check_Type(rb_initial_value, T_FIXNUM);

  const char *name = StringValuePtr(rb_name);
  int initial_value = FIX2INT(rb_initial_value);

  posix_namedsemaphore *ptr;
  Data_Get_Struct(self, posix_namedsemaphore, ptr);

  ptr->sem = sem_open(name, O_CREAT | O_EXCL, 0600, initial_value);

  if(ptr->sem == SEM_FAILED) { 
    rb_raise(rb_eRuntimeError, strerror(errno)); 
  }

  return self;

}

static VALUE posix_namedsemaphore_open(VALUE self) {
  VALUE rb_name = rb_ivar_get(self, rb_intern("@name"));
  Check_Type(rb_name, T_STRING);
  const char* name = StringValuePtr(rb_name);

  posix_namedsemaphore *ptr;
  Data_Get_Struct(self, posix_namedsemaphore, ptr);

  ptr->sem = sem_open(name, 0);

  if(ptr->sem == SEM_FAILED) { 
    rb_raise(rb_eRuntimeError, strerror(errno)); 
  }

  return self;
}

static VALUE posix_namedsemaphore_close(VALUE self) { 

  posix_namedsemaphore *ptr;
  Data_Get_Struct(self, posix_namedsemaphore, ptr);

  if(sem_close(ptr->sem) == -1) { 
    rb_raise(rb_eRuntimeError, strerror(errno)); 
  }

  ptr->sem = NULL;
  
  return self;
}

static VALUE posix_namedsemaphore_unlink(VALUE self) { 
  VALUE rb_name = rb_ivar_get(self, rb_intern("@name"));
  Check_Type(rb_name, T_STRING);
  const char* name = StringValuePtr(rb_name);

  if(sem_unlink(name) == -1) { 
    rb_raise(rb_eRuntimeError, strerror(errno)); 
  }

  return self;
}

static VALUE posix_namedsemaphore_wait(VALUE self) { 
  posix_namedsemaphore *ptr;
  Data_Get_Struct(self, posix_namedsemaphore, ptr);

  if(sem_wait(ptr->sem) == -1) { 
    rb_raise(rb_eRuntimeError, strerror(errno)); 
  }

  return self;
}

static VALUE posix_namedsemaphore_post(VALUE self) { 
  posix_namedsemaphore *ptr;
  Data_Get_Struct(self, posix_namedsemaphore, ptr);

  if(sem_post(ptr->sem) == -1) { 
    rb_raise(rb_eRuntimeError, strerror(errno)); 
  }

  return self;
}



void Init_sys_stuff(void) {

  VALUE cSysStuffPosix = rb_define_module_under(rb_define_module("SysStuff"), "Posix");
  
  VALUE cNamedSemaphore = rb_define_class_under(cSysStuffPosix, "NamedSemaphore", rb_cObject);

  rb_define_alloc_func(cNamedSemaphore, posix_namedsemaphore_alloc);

  rb_define_method(cNamedSemaphore, "create", posix_namedsemaphore_create, 1);
  rb_define_method(cNamedSemaphore, "open", posix_namedsemaphore_open, 0);
  rb_define_method(cNamedSemaphore, "close", posix_namedsemaphore_close, 0);
  rb_define_method(cNamedSemaphore, "unlink!", posix_namedsemaphore_unlink, 0);
  rb_define_method(cNamedSemaphore, "post", posix_namedsemaphore_post, 0);
  rb_define_method(cNamedSemaphore, "wait", posix_namedsemaphore_wait, 0);

}
