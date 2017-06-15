/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.33
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package edu.uvic.marsyas;

class marsyasJNI {
  public final static native long new_Map_String_MarControlPtr__SWIG_0();
  public final static native long new_Map_String_MarControlPtr__SWIG_1(long jarg1, Map_String_MarControlPtr jarg1_);
  public final static native long Map_String_MarControlPtr_size(long jarg1, Map_String_MarControlPtr jarg1_);
  public final static native boolean Map_String_MarControlPtr_empty(long jarg1, Map_String_MarControlPtr jarg1_);
  public final static native void Map_String_MarControlPtr_clear(long jarg1, Map_String_MarControlPtr jarg1_);
  public final static native long Map_String_MarControlPtr_get(long jarg1, Map_String_MarControlPtr jarg1_, String jarg2);
  public final static native void Map_String_MarControlPtr_set(long jarg1, Map_String_MarControlPtr jarg1_, String jarg2, long jarg3, MarControlPtr jarg3_);
  public final static native void Map_String_MarControlPtr_del(long jarg1, Map_String_MarControlPtr jarg1_, String jarg2);
  public final static native boolean Map_String_MarControlPtr_has_key(long jarg1, Map_String_MarControlPtr jarg1_, String jarg2);
  public final static native void delete_Map_String_MarControlPtr(long jarg1);
  public final static native void MarSystem_tick(long jarg1, MarSystem jarg1_);
  public final static native void MarSystem_update(long jarg1, MarSystem jarg1_);
  public final static native void MarSystem_addMarSystem(long jarg1, MarSystem jarg1_, long jarg2, MarSystem jarg2_);
  public final static native String MarSystem_getType(long jarg1, MarSystem jarg1_);
  public final static native String MarSystem_getName(long jarg1, MarSystem jarg1_);
  public final static native String MarSystem_getPrefix(long jarg1, MarSystem jarg1_);
  public final static native void MarSystem_setControl(long jarg1, MarSystem jarg1_, String jarg2, long jarg3, MarControlPtr jarg3_);
  public final static native void MarSystem_updControl(long jarg1, MarSystem jarg1_, String jarg2, long jarg3, MarControlPtr jarg3_);
  public final static native long MarSystem_getControl(long jarg1, MarSystem jarg1_, String jarg2);
  public final static native boolean MarSystem_hasControl(long jarg1, MarSystem jarg1_, String jarg2);
  public final static native void MarSystem_linkControl(long jarg1, MarSystem jarg1_, String jarg2, String jarg3);
  public final static native long MarSystem_getControls(long jarg1, MarSystem jarg1_);
  public final static native void delete_MarSystem(long jarg1);
  public final static native long new_Vector_String__SWIG_0();
  public final static native long new_Vector_String__SWIG_1(long jarg1);
  public final static native long Vector_String_size(long jarg1, Vector_String jarg1_);
  public final static native long Vector_String_capacity(long jarg1, Vector_String jarg1_);
  public final static native void Vector_String_reserve(long jarg1, Vector_String jarg1_, long jarg2);
  public final static native boolean Vector_String_isEmpty(long jarg1, Vector_String jarg1_);
  public final static native void Vector_String_clear(long jarg1, Vector_String jarg1_);
  public final static native void Vector_String_add(long jarg1, Vector_String jarg1_, String jarg2);
  public final static native String Vector_String_get(long jarg1, Vector_String jarg1_, int jarg2);
  public final static native void Vector_String_set(long jarg1, Vector_String jarg1_, int jarg2, String jarg3);
  public final static native void delete_Vector_String(long jarg1);
  public final static native long new_MarSystemManager();
  public final static native long MarSystemManager_create(long jarg1, MarSystemManager jarg1_, String jarg2, String jarg3);
  public final static native long MarSystemManager_registeredPrototypes(long jarg1, MarSystemManager jarg1_);
  public final static native void delete_MarSystemManager(long jarg1);
  public final static native long new_MarControlPtr__SWIG_0();
  public final static native long new_MarControlPtr__SWIG_1(long jarg1, MarControlPtr jarg1_);
  public final static native void delete_MarControlPtr(long jarg1);
  public final static native boolean MarControlPtr_isInvalid(long jarg1, MarControlPtr jarg1_);
  public final static native long MarControlPtr_from_natural(int jarg1);
  public final static native long MarControlPtr_from_real(double jarg1);
  public final static native long MarControlPtr_from_bool(boolean jarg1);
  public final static native long MarControlPtr_from_string(String jarg1);
  public final static native String MarControlPtr_getType(long jarg1, MarControlPtr jarg1_);
  public final static native String MarControlPtr_getName(long jarg1, MarControlPtr jarg1_);
  public final static native boolean MarControlPtr_setValue_natural(long jarg1, MarControlPtr jarg1_, int jarg2);
  public final static native boolean MarControlPtr_setValue_real(long jarg1, MarControlPtr jarg1_, double jarg2);
  public final static native boolean MarControlPtr_setValue_bool(long jarg1, MarControlPtr jarg1_, boolean jarg2);
  public final static native boolean MarControlPtr_setValue_string(long jarg1, MarControlPtr jarg1_, String jarg2);
  public final static native int MarControlPtr_to_natural(long jarg1, MarControlPtr jarg1_);
  public final static native double MarControlPtr_to_real(long jarg1, MarControlPtr jarg1_);
  public final static native String MarControlPtr_to_string(long jarg1, MarControlPtr jarg1_);
  public final static native boolean MarControlPtr_to_bool(long jarg1, MarControlPtr jarg1_);
}
