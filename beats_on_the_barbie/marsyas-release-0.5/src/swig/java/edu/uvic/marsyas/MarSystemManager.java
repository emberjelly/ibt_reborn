/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.33
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package edu.uvic.marsyas;

public class MarSystemManager {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected MarSystemManager(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(MarSystemManager obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if(swigCPtr != 0 && swigCMemOwn) {
      swigCMemOwn = false;
      marsyasJNI.delete_MarSystemManager(swigCPtr);
    }
    swigCPtr = 0;
  }

  public MarSystemManager() {
    this(marsyasJNI.new_MarSystemManager(), true);
  }

  public MarSystem create(String arg0, String arg1) {
    long cPtr = marsyasJNI.MarSystemManager_create(swigCPtr, this, arg0, arg1);
    return (cPtr == 0) ? null : new MarSystem(cPtr, false);
  }

  public Vector_String registeredPrototypes() {
    return new Vector_String(marsyasJNI.MarSystemManager_registeredPrototypes(swigCPtr, this), true);
  }

}
