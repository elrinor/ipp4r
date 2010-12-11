#include "ipp4r.h"


// -------------------------------------------------------------------------- //
// define_enum_type
// -------------------------------------------------------------------------- //
VALUE define_enum_type(char *tag) {
  VALUE klass;

  klass = rb_define_class_under(rb_Ipp, tag, rb_Enum);
  rb_define_singleton_method(klass, "values", rb_Enum_type_values, 0);
  rb_define_method(klass, "initialize", rb_Enum_type_initialize, 2);
  rb_define_method(klass, "inspect", rb_Enum_type_inspect, 0);

  return klass;
}


// -------------------------------------------------------------------------- //
// enum_new
// -------------------------------------------------------------------------- //
VALUE enum_new(VALUE klass, VALUE sym, VALUE value) {
  VALUE argv[2];

  argv[0] = sym;
  argv[1] = value;
  return rb_class_new_instance(2, argv, klass);
}


// -------------------------------------------------------------------------- //
// rb_Enum_alloc
// -------------------------------------------------------------------------- //
VALUE rb_Enum_alloc(VALUE klass) {
  Enum *enum_;
  VALUE rb_enum;
  
  /* Note that we doesn't assign a free function here.
   * We don't need it because we are not going to deallocate enums. */
  rb_enum = Data_Make_Struct(klass, Enum, NULL, NULL, enum_); 
  
  OBJ_FREEZE(rb_enum);
  return rb_enum;
}


// -------------------------------------------------------------------------- //
// rb_Enum_initialize
// -------------------------------------------------------------------------- //
VALUE rb_Enum_initialize(VALUE self, VALUE sym, VALUE val) {
  Enum *enum_;

  Data_Get_Struct(self, Enum, enum_);

  enum_->id = rb_to_id(sym); /* symbol to ID */
  enum_->value = NUM2INT(val);

  return self;
}


// -------------------------------------------------------------------------- //
// rb_Enum_to_s
// -------------------------------------------------------------------------- //
VALUE rb_Enum_to_s(VALUE self) {
  Enum *enum_;

  Data_Get_Struct(self, Enum, enum_);

  return rb_str_new2(rb_id2name(enum_->id));
}


// -------------------------------------------------------------------------- //
// rb_Enum_to_i
// -------------------------------------------------------------------------- //
VALUE rb_Enum_to_i(VALUE self) {
  Enum *enum_;

  Data_Get_Struct(self, Enum, enum_);

  return INT2NUM(enum_->value);
}


// -------------------------------------------------------------------------- //
// rb_Enum_spaceship
// -------------------------------------------------------------------------- //
VALUE rb_Enum_spaceship(VALUE self, VALUE other) {
  Enum *self_enum, *other_enum;

  Data_Get_Struct(self, Enum, self_enum);
  Data_Get_Struct(other, Enum, other_enum);

  if(self_enum->value > other_enum->value)
    return INT2FIX(1);
  else if(self_enum->value < other_enum->value)
    return INT2FIX(-1);

  // Values are equal, check class.
  return rb_funcall(CLASS_OF(self), rb_ID_spaceship, 1, CLASS_OF(other));
}


// -------------------------------------------------------------------------- //
// rb_Enum_case_eq
// -------------------------------------------------------------------------- //
VALUE rb_Enum_case_eq(VALUE self, VALUE other) {
  Enum *self_enum, *other_enum;

  if(CLASS_OF(self) == CLASS_OF(other)) {
    Data_Get_Struct(self, Enum, self_enum);
    Data_Get_Struct(other, Enum, other_enum);
    return C2R_BOOL(self_enum->value == other_enum->value);
  } else
    return Qfalse;
}


// -------------------------------------------------------------------------- //
// rb_Enum_type_initialize
// -------------------------------------------------------------------------- //
VALUE rb_Enum_type_initialize(VALUE self, VALUE sym, VALUE val) {
  VALUE super_argv[2];

  super_argv[0] = sym;
  super_argv[1] = val;
  rb_call_super(2, (VALUE*) super_argv);

  if(rb_cvar_defined(CLASS_OF(self), rb_ID_enumerators) != Qtrue)
    rb_cvar_set(CLASS_OF(self), rb_ID_enumerators, rb_ary_new(), 0);

  if(rb_cvar_defined(CLASS_OF(self), rb_ID_enumerators_by_value) != Qtrue)
    rb_cvar_set(CLASS_OF(self), rb_ID_enumerators_by_value, rb_hash_new(), 0);

  rb_ary_push(rb_cvar_get(CLASS_OF(self), rb_ID_enumerators), self);
  rb_hash_aset(rb_cvar_get(CLASS_OF(self), rb_ID_enumerators_by_value), rb_Enum_to_i(self), self);

  return self;
}


// -------------------------------------------------------------------------- //
// rb_Enum_type_inspect
// -------------------------------------------------------------------------- //
VALUE rb_Enum_type_inspect(VALUE self) {
  char buf[100]; // this'll be enough
  Enum *enum_;

  Data_Get_Struct(self, Enum, enum_);
  sprintf(buf, "%.32s=%d", rb_id2name(enum_->id), enum_->value); // TODO: kangaenakya

  return rb_str_new2(buf);
}


// -------------------------------------------------------------------------- //
// rb_Enum_type_values
// -------------------------------------------------------------------------- //
VALUE rb_Enum_type_values(VALUE klass) {
  VALUE enumerators;
  VALUE result;
  int i;

  enumerators = rb_cvar_get(klass, rb_ID_enumerators);

  if(rb_block_given_p()) {
    for(i = 0; i < RARRAY_LEN(enumerators); i++)
      rb_yield(rb_ary_entry(enumerators, i));
    result = klass;
  } else {
    result = rb_ary_new2(RARRAY_LEN(enumerators));
    for(i = 0; i < RARRAY_LEN(enumerators); i++) 
      rb_ary_push(result, rb_ary_entry(enumerators, i));
    OBJ_FREEZE(result);
  }

  return result;
}



