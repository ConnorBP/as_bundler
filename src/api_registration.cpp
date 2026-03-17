// AUTO-GENERATED Perception.cx AngelScript API Registration
// Based on official documentation at https://docs.perception.cx/
// This file registers stubs for all Perception.cx APIs to enable validation

#include "../include/angelscript/include/angelscript.h"
#include <stdio.h>
#include <cassert>
#include <string>

typedef unsigned int uint;

// Stub function for registration (actual implementation not needed for validation)
void StubFunction() {}

void RegisterCustomAPIs(asIScriptEngine* engine) {
	int r;

	// =====================================================
	// FUNCDEFS (must be registered before functions using them)
	// =====================================================

	r = engine->RegisterFuncdef("void __Internal_CallbackFn(int callback_id, int data_index)");
	if (r < 0) printf("WARNING: Failed to register __Internal_CallbackFn funcdef (code: %d)\n", r);

	r = engine->RegisterFuncdef("bool UcHookFn(uint64 uc, uint64 addr)");
	if (r < 0) printf("WARNING: Failed to register UcHookFn funcdef (code: %d)\n", r);

	r = engine->RegisterFuncdef("void button_callback_t()");
	if (r < 0) printf("WARNING: Failed to register button_callback_t funcdef (code: %d)\n", r);

	// =====================================================
	// OPAQUE HANDLE VALUE TYPES - must be registered FIRST before value types use them
	// =====================================================

	// proc_t - process handle (opaque value type, not reference)
	r = engine->RegisterObjectType("proc_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	if (r < 0) printf("WARNING: Failed to register proc_t type (code: %d)\n", r);

	// uproc_t - unprotected process handle (opaque value type)
	r = engine->RegisterObjectType("uproc_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	if (r < 0) printf("WARNING: Failed to register uproc_t type (code: %d)\n", r);

	// uproc_t methods - same as proc_t
	r = engine->RegisterObjectMethod("uproc_t", "void deref()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("uproc_t", "uint64 base_address() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("uproc_t", "bool alive() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("uproc_t", "uint8 ru8(uint64) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// ws_t - websocket handle (opaque value type)
	r = engine->RegisterObjectType("ws_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	if (r < 0) printf("WARNING: Failed to register ws_t type (code: %d)\n", r);

	// mutex_t - mutex handle (opaque value type)
	r = engine->RegisterObjectType("mutex_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	if (r < 0) printf("WARNING: Failed to register mutex_t type (code: %d)\n", r);

	// ZydisBuilder
	r = engine->RegisterObjectType("ZydisBuilder", 0, asOBJ_REF | asOBJ_NOCOUNT);
	if (r < 0) printf("WARNING: Failed to register ZydisBuilder type (code: %d)\n", r);

	// GUI types - opaque handle value types (owned by engine, like proc_t)
	r = engine->RegisterObjectType("subtab_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	if (r < 0) printf("WARNING: Failed to register subtab_t type (code: %d)\n", r);

	r = engine->RegisterObjectType("panel_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	if (r < 0) printf("WARNING: Failed to register panel_t type (code: %d)\n", r);

	r = engine->RegisterObjectType("checkbox_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	if (r < 0) printf("WARNING: Failed to register checkbox_t type (code: %d)\n", r);

	r = engine->RegisterObjectType("slider_int_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	if (r < 0) printf("WARNING: Failed to register slider_int_t type (code: %d)\n", r);

	r = engine->RegisterObjectType("slider_double_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	if (r < 0) printf("WARNING: Failed to register slider_double_t type (code: %d)\n", r);

	r = engine->RegisterObjectType("keybind_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	if (r < 0) printf("WARNING: Failed to register keybind_t type (code: %d)\n", r);

	r = engine->RegisterObjectType("color_picker_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	if (r < 0) printf("WARNING: Failed to register color_picker_t type (code: %d)\n", r);

	r = engine->RegisterObjectType("input_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	if (r < 0) printf("WARNING: Failed to register input_t type (code: %d)\n", r);

	r = engine->RegisterObjectType("list_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	if (r < 0) printf("WARNING: Failed to register list_t type (code: %d)\n", r);

	r = engine->RegisterObjectType("multi_select_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	if (r < 0) printf("WARNING: Failed to register multi_select_t type (code: %d)\n", r);

	r = engine->RegisterObjectType("single_select_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	if (r < 0) printf("WARNING: Failed to register single_select_t type (code: %d)\n", r);

	r = engine->RegisterObjectType("button_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	if (r < 0) printf("WARNING: Failed to register button_t type (code: %d)\n", r);

	// =====================================================
	// VALUE TYPES (POD types with members)
	// =====================================================

	// vector2 - 2D vector with x, y
	r = engine->RegisterObjectType("vector2", sizeof(double)*2, asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	if (r < 0) printf("WARNING: Failed to register vector2 type (code: %d)\n", r);

	r = engine->RegisterObjectProperty("vector2", "double x", 0);
	if (r < 0) printf("WARNING: Failed to register vector2.x (code: %d)\n", r);

	r = engine->RegisterObjectProperty("vector2", "double y", sizeof(double));
	if (r < 0) printf("WARNING: Failed to register vector2.y (code: %d)\n", r);

	r = engine->RegisterObjectBehaviour("vector2", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectBehaviour("vector2", asBEHAVE_CONSTRUCT, "void f(double, double)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector2", "vector2 opAdd(const vector2 &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector2", "vector2 opSub(const vector2 &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector2", "vector2 opMul(double) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector2", "vector2 opDiv(double) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector2", "vector2 opNeg() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector2", "bool opEquals(const vector2 &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector2", "double length() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector2", "double distance(const vector2 &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector2", "double distance_to(const vector2 &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector2", "vector2 lerp(const vector2 &in, double) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector2", "vector2 min(const vector2 &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector2", "vector2 max(const vector2 &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector2", "void readas_double(proc_t &in, uint64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector2", "void readas_float(proc_t &in, uint64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector2", "bool writeas_double(proc_t &in, uint64) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector2", "bool writeas_float(proc_t &in, uint64) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// vector3 - 3D vector with x, y, z
	r = engine->RegisterObjectType("vector3", sizeof(double)*3, asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	if (r < 0) printf("WARNING: Failed to register vector3 type (code: %d)\n", r);

	r = engine->RegisterObjectProperty("vector3", "double x", 0);
	if (r < 0) printf("WARNING: Failed to register vector3.x (code: %d)\n", r);

	r = engine->RegisterObjectProperty("vector3", "double y", sizeof(double));
	if (r < 0) printf("WARNING: Failed to register vector3.y (code: %d)\n", r);

	r = engine->RegisterObjectProperty("vector3", "double z", sizeof(double)*2);
	if (r < 0) printf("WARNING: Failed to register vector3.z (code: %d)\n", r);

	r = engine->RegisterObjectBehaviour("vector3", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectBehaviour("vector3", asBEHAVE_CONSTRUCT, "void f(double, double, double)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector3", "vector3 opAdd(const vector3 &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector3", "vector3 opSub(const vector3 &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector3", "vector3 opMul(double) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector3", "vector3 opDiv(double) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector3", "vector3 opNeg() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector3", "bool opEquals(const vector3 &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector3", "double length() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector3", "double length2d() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector3", "double distance(const vector3 &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector3", "double distance_to(const vector3 &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector3", "double distance2d(const vector3 &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector3", "double distance2d_to(const vector3 &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector3", "vector3 lerp(const vector3 &in, double) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector3", "vector3 min(const vector3 &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector3", "vector3 max(const vector3 &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector3", "double dot_product(const vector3 &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector3", "vector3 cross_product(const vector3 &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector3", "void readas_double(proc_t &in, uint64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector3", "void readas_float(proc_t &in, uint64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector3", "bool writeas_double(proc_t &in, uint64) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("vector3", "bool writeas_float(proc_t &in, uint64) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// quaternion - quaternion with x, y, z, w
	r = engine->RegisterObjectType("quaternion", sizeof(double)*4, asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	if (r < 0) printf("WARNING: Failed to register quaternion type (code: %d)\n", r);

	r = engine->RegisterObjectProperty("quaternion", "double x", 0);
	r = engine->RegisterObjectProperty("quaternion", "double y", sizeof(double));
	r = engine->RegisterObjectProperty("quaternion", "double z", sizeof(double)*2);
	r = engine->RegisterObjectProperty("quaternion", "double w", sizeof(double)*3);

	r = engine->RegisterObjectBehaviour("quaternion", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectBehaviour("quaternion", asBEHAVE_CONSTRUCT, "void f(double, double, double, double)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("quaternion", "quaternion opMul(const quaternion &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("quaternion", "quaternion opMul(double) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("quaternion", "quaternion opDiv(double) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("quaternion", "quaternion opAdd(const quaternion &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("quaternion", "quaternion opSub(const quaternion &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("quaternion", "quaternion opNeg() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("quaternion", "bool opEquals(const quaternion &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("quaternion", "double length() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("quaternion", "quaternion normalized() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("quaternion", "quaternion conjugate() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("quaternion", "quaternion inverse() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("quaternion", "double dot(const quaternion &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("quaternion", "vector3 rotate(const vector3 &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("quaternion", "void to_euler(double &out, double &out, double &out) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// matrix4x4
	r = engine->RegisterObjectType("matrix4x4", sizeof(double)*16, asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	if (r < 0) printf("WARNING: Failed to register matrix4x4 type (code: %d)\n", r);

	r = engine->RegisterObjectBehaviour("matrix4x4", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("matrix4x4", "matrix4x4 opMul(const matrix4x4 &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("matrix4x4", "vector3 transform(const vector3 &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("matrix4x4", "void read(proc_t &in, uint64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("matrix4x4", "void readas_float(proc_t &in, uint64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("matrix4x4", "void readas_double(proc_t &in, uint64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("matrix4x4", "bool writeas_float(proc_t &in, uint64) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("matrix4x4", "bool writeas_double(proc_t &in, uint64) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("matrix4x4", "double opIndex(int) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register matrix4x4::opIndex (code: %d)\n", r);

	// atomic_int32
	r = engine->RegisterObjectType("atomic_int32", 4, asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	if (r < 0) printf("WARNING: Failed to register atomic_int32 type (code: %d)\n", r);

	r = engine->RegisterObjectBehaviour("atomic_int32", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectBehaviour("atomic_int32", asBEHAVE_CONSTRUCT, "void f(int32)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectBehaviour("atomic_int32", asBEHAVE_CONSTRUCT, "void f(const atomic_int32 &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int32", "int32 load() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int32", "void store(int32)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int32", "int32 exchange(int32)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int32", "bool compare_exchange(int32, int32)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int32", "int32 add(int32)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int32", "int32 sub(int32)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int32", "int32 increment()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int32", "int32 decrement()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int32", "atomic_int32 &opAssign(const atomic_int32 &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int32", "int32 and_op(int32)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int32", "int32 or_op(int32)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int32", "int32 xor_op(int32)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// atomic_int64
	r = engine->RegisterObjectType("atomic_int64", 8, asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	if (r < 0) printf("WARNING: Failed to register atomic_int64 type (code: %d)\n", r);

	r = engine->RegisterObjectBehaviour("atomic_int64", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectBehaviour("atomic_int64", asBEHAVE_CONSTRUCT, "void f(int64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectBehaviour("atomic_int64", asBEHAVE_CONSTRUCT, "void f(const atomic_int64 &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int64", "int64 load() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int64", "void store(int64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int64", "int64 exchange(int64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int64", "bool compare_exchange(int64, int64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int64", "atomic_int64 &opAssign(const atomic_int64 &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int64", "int64 and_op(int64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int64", "int64 or_op(int64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int64", "int64 xor_op(int64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// WindowInfo - value type for window information
	// Note: This is a simplified registration - actual implementation would need proper storage
	r = engine->RegisterObjectType("WindowInfo", sizeof(void*)*8, asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	if (r < 0) printf("WARNING: Failed to register WindowInfo type (code: %d)\n", r);

	// WindowInfo properties
	r = engine->RegisterObjectProperty("WindowInfo", "uint64 hwnd", 0);
	if (r < 0) printf("WARNING: Failed to register WindowInfo.hwnd (code: %d)\n", r);

	r = engine->RegisterObjectProperty("WindowInfo", "uint pid", sizeof(uint64_t));
	if (r < 0) printf("WARNING: Failed to register WindowInfo.pid (code: %d)\n", r);

	r = engine->RegisterObjectProperty("WindowInfo", "uint tid", sizeof(uint64_t) + sizeof(unsigned int));
	if (r < 0) printf("WARNING: Failed to register WindowInfo.tid (code: %d)\n", r);

	// String members would need special handling in actual implementation
	// For stub registration, we'll skip them since they complicate POD structure

	// __m128 - 128-bit SIMD type (4 floats)
	r = engine->RegisterObjectType("__m128", sizeof(float)*4, asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	if (r < 0) printf("WARNING: Failed to register __m128 type (code: %d)\n", r);

	r = engine->RegisterObjectBehaviour("__m128", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectBehaviour("__m128", asBEHAVE_CONSTRUCT, "void f(float, float, float, float)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// __m256 - 256-bit SIMD type (8 floats)
	r = engine->RegisterObjectType("__m256", sizeof(float)*8, asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	if (r < 0) printf("WARNING: Failed to register __m256 type (code: %d)\n", r);

	r = engine->RegisterObjectBehaviour("__m256", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectBehaviour("__m256", asBEHAVE_CONSTRUCT, "void f(float, float, float, float, float, float, float, float)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// ZydisEncoderRequest
	r = engine->RegisterObjectType("ZydisEncoderRequest", sizeof(void*)*16, asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	if (r < 0) printf("WARNING: Failed to register ZydisEncoderRequest type (code: %d)\n", r);

	r = engine->RegisterObjectBehaviour("ZydisEncoderRequest", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ZydisEncoderRequest", "void set_mnemonic(int)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ZydisEncoderRequest", "void set_operand_count(int)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ZydisEncoderRequest", "void set_operand_reg(int, int)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ZydisEncoderRequest", "void set_operand_imm(int, uint64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ZydisEncoderRequest", "void set_operand_mem(int, int, int, int, int64, int)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	// ZydisEncoderRequest getters
	r = engine->RegisterObjectMethod("ZydisEncoderRequest", "int get_mnemonic()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ZydisEncoderRequest", "int get_machine_mode()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ZydisEncoderRequest", "int get_operand_count()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	// ZydisEncoderRequest setters
	r = engine->RegisterObjectMethod("ZydisEncoderRequest", "void set_machine_mode(int)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ZydisEncoderRequest", "void set_branch_type(int)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ZydisEncoderRequest", "void set_branch_width(int)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ZydisEncoderRequest", "void set_operand_ptr(int, uint16, uint32)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// =====================================================
	// VALUE TYPE METHODS (opaque handles)
	// =====================================================

	// proc_t methods
	r = engine->RegisterObjectMethod("proc_t", "void deref()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "bool read(uint64, uint64 &out)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "uint64 base_address() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "uint64 peb() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "uint pid() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "bool alive() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// proc_t read methods
	r = engine->RegisterObjectMethod("proc_t", "uint8 ru8(uint64) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "uint16 ru16(uint64) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "uint32 ru32(uint64) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "uint64 ru64(uint64) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "int8 r8(uint64) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "int16 r16(uint64) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "int32 r32(uint64) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "int64 r64(uint64) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "float rf32(uint64) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "double rf64(uint64) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// proc_t write methods
	r = engine->RegisterObjectMethod("proc_t", "bool wu8(uint64, uint8)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "bool wu16(uint64, uint16)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "bool wu32(uint64, uint32)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "bool wu64(uint64, uint64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "bool w8(uint64, int8)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "bool w16(uint64, int16)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "bool w32(uint64, int32)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "bool w64(uint64, int64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "bool wf32(uint64, float)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "bool wf64(uint64, double)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// proc_t string methods
	r = engine->RegisterObjectMethod("proc_t", "string rs(uint64, uint) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "string rws(uint64, uint) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "bool ws(uint64, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "bool wws(uint64, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// proc_t memory methods
	r = engine->RegisterObjectMethod("proc_t", "void rvm(uint64, uint, array<uint8> &out) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "bool wvm(uint64, const array<uint8> &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// proc_t module/pattern methods
	r = engine->RegisterObjectMethod("proc_t", "bool get_module(const string &in, uint64 &out, uint64 &out) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "uint64 find_code_pattern(uint64, uint64, const string &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "void find_all_code_patterns(uint64, uint64, const string &in, array<uint64> &out) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// proc_t advanced methods
	r = engine->RegisterObjectMethod("proc_t", "uint64 get_proc_address(uint64, const string &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "array<uint64>@ get_all_tebs() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "array<dictionary@>@ cs2_get_schema_dump() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "uint64 cs2_get_interface(const string &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// proc_t SIMD helpers
	r = engine->RegisterObjectMethod("proc_t", "void r128(uint64, array<uint8> &out) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "void r256(uint64, array<uint8> &out) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "void r512(uint64, array<uint8> &out) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "bool w128(uint64, const array<uint8> &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "bool w256(uint64, const array<uint8> &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "bool w512(uint64, const array<uint8> &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// proc_t struct helpers
	r = engine->RegisterObjectMethod("proc_t", "bool read_struct(uint64, dictionary &out, const dictionary &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "bool read_struct_array(uint64, uint, uint, array<dictionary>@ &out, const dictionary &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// proc_t virtual memory functions
	r = engine->RegisterObjectMethod("proc_t", "uint64 alloc_vm(uint)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "bool free_vm(uint64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// proc_t import/export functions
	r = engine->RegisterObjectMethod("proc_t", "uint64 get_import_rdata_address(uint64, const string &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// proc_t pointer array helper
	r = engine->RegisterObjectMethod("proc_t", "array<uint64>@ read_pointer_array(uint64, uint, int) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// proc_t virtual memory analysis
	r = engine->RegisterObjectMethod("proc_t", "bool virtual_query(uint64, uint64 &out, uint64 &out, uint32 &out, bool &out) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "void get_vad_snapshot(bool, array<dictionary>@ &out) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// proc_t memory scan helpers
	r = engine->RegisterObjectMethod("proc_t", "void scan_bytes(const array<uint8> &in, array<uint64> &out, bool) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "void scan_all_bytes(array<uint64> &out, bool) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "void scan_u32(uint32, array<uint64> &out, bool) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "void scan_u64(uint64, array<uint64> &out, bool) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "void scan_all_u32(array<uint64> &out, bool) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "void scan_all_u64(array<uint64> &out, bool) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// ws_t methods
	r = engine->RegisterObjectMethod("ws_t", "bool send_text(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ws_t", "bool send_binary(const array<uint8> &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ws_t", "bool send_json(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ws_t", "bool is_open() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ws_t", "void close(int = 1000)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ws_t", "void recv(string &out, bool &out)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ws_t", "bool poll(string &out, bool &out, bool &out)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// mutex_t methods
	r = engine->RegisterObjectMethod("mutex_t", "void lock()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("mutex_t", "void unlock()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("mutex_t", "bool try_lock()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("mutex_t", "void lock_shared()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("mutex_t", "void unlock_shared()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("mutex_t", "bool try_lock_shared()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("mutex_t", "void destroy()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// ZydisBuilder methods
	r = engine->RegisterObjectMethod("ZydisBuilder", "void set_base_address(uint64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ZydisBuilder", "void set_machine_mode(int)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ZydisBuilder", "void push(const ZydisEncoderRequest &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ZydisBuilder", "void push_nop(int)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ZydisBuilder", "void push_int3()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ZydisBuilder", "void push_ret()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ZydisBuilder", "void clear()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ZydisBuilder", "void push_bytes(const array<uint8> &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ZydisBuilder", "void push_byte(uint8)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ZydisBuilder", "void push_u16(uint16)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ZydisBuilder", "void push_u32(uint32)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ZydisBuilder", "void push_u64(uint64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ZydisBuilder", "int get_instruction_count()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ZydisBuilder", "bool build(array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// input_t methods
	r = engine->RegisterObjectMethod("input_t", "string get() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register input_t::get (code: %d)\n", r);

	r = engine->RegisterObjectMethod("input_t", "void set(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register input_t::set (code: %d)\n", r);

	r = engine->RegisterObjectMethod("input_t", "void set_active(bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register input_t::set_active (code: %d)\n", r);

	// =====================================================
	// GUI TYPE METHODS
	// =====================================================

	// subtab_t methods
	r = engine->RegisterObjectMethod("subtab_t", "panel_t add_panel(const string &in, bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register subtab_t::add_panel (code: %d)\n", r);

	r = engine->RegisterObjectMethod("subtab_t", "bool is_valid() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register subtab_t::is_valid (code: %d)\n", r);

	r = engine->RegisterObjectMethod("subtab_t", "void set_active(bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register subtab_t::set_active (code: %d)\n", r);

	// panel_t methods
	r = engine->RegisterObjectMethod("panel_t", "void set_active(bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register panel_t::set_active (code: %d)\n", r);

	// panel_t::add_checkbox
	r = engine->RegisterObjectMethod("panel_t", "checkbox_t add_checkbox(const string &in, bool, bool = true, bool = false, bool = false)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register panel_t::add_checkbox (code: %d)\n", r);

	// panel_t::add_slider_int
	r = engine->RegisterObjectMethod("panel_t", "slider_int_t add_slider_int(const string &in, const string &in, int, int, int, int, bool = true, bool = false)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register panel_t::add_slider_int (code: %d)\n", r);

	// panel_t::add_slider_double
	r = engine->RegisterObjectMethod("panel_t", "slider_double_t add_slider_double(const string &in, const string &in, double, double, double, double, bool = true, bool = false)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register panel_t::add_slider_double (code: %d)\n", r);

	// panel_t::add_keybind
	r = engine->RegisterObjectMethod("panel_t", "keybind_t add_keybind(const string &in, int, const string &in, bool = true, bool = false)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register panel_t::add_keybind (code: %d)\n", r);

	// panel_t::add_color
	r = engine->RegisterObjectMethod("panel_t", "color_picker_t add_color(const string &in, const array<float> &in, bool = false)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register panel_t::add_color (code: %d)\n", r);

	// panel_t::add_input
	r = engine->RegisterObjectMethod("panel_t", "input_t add_input(const string &in, const string &in, bool = true, bool = false)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register panel_t::add_input (code: %d)\n", r);

	// panel_t::add_list
	r = engine->RegisterObjectMethod("panel_t", "list_t add_list(const string &in, const array<dictionary@> &in, bool = true, bool = false)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register panel_t::add_list (code: %d)\n", r);

	// panel_t::add_multi_select
	r = engine->RegisterObjectMethod("panel_t", "multi_select_t add_multi_select(const string &in, const array<dictionary@> &in, bool, bool = true, bool = false)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register panel_t::add_multi_select (code: %d)\n", r);

	// panel_t::add_single_select
	r = engine->RegisterObjectMethod("panel_t", "single_select_t add_single_select(const string &in, const array<string> &in, int, bool, bool = true, bool = false)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register panel_t::add_single_select (code: %d)\n", r);

	// panel_t::add_button
	r = engine->RegisterObjectMethod("panel_t", "button_t add_button(const string &in, const button_callback_t@, bool = false)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register panel_t::add_button (code: %d)\n", r);

	// checkbox_t methods
	r = engine->RegisterObjectMethod("checkbox_t", "bool get() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register checkbox_t::get (code: %d)\n", r);

	r = engine->RegisterObjectMethod("checkbox_t", "void set(bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register checkbox_t::set (code: %d)\n", r);

	r = engine->RegisterObjectMethod("checkbox_t", "void set_active(bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register checkbox_t::set_active (code: %d)\n", r);

	// slider_int_t methods
	r = engine->RegisterObjectMethod("slider_int_t", "int get() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register slider_int_t::get (code: %d)\n", r);

	r = engine->RegisterObjectMethod("slider_int_t", "void set(int)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register slider_int_t::set (code: %d)\n", r);

	r = engine->RegisterObjectMethod("slider_int_t", "void set_active(bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register slider_int_t::set_active (code: %d)\n", r);

	// slider_double_t methods
	r = engine->RegisterObjectMethod("slider_double_t", "double get() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register slider_double_t::get (code: %d)\n", r);

	r = engine->RegisterObjectMethod("slider_double_t", "void set(double)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register slider_double_t::set (code: %d)\n", r);

	r = engine->RegisterObjectMethod("slider_double_t", "void set_active(bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register slider_double_t::set_active (code: %d)\n", r);

	// keybind_t methods
	r = engine->RegisterObjectMethod("keybind_t", "void get(int &out, string &out) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register keybind_t::get (code: %d)\n", r);

	r = engine->RegisterObjectMethod("keybind_t", "void set(int, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register keybind_t::set (code: %d)\n", r);

	r = engine->RegisterObjectMethod("keybind_t", "void set_active(bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register keybind_t::set_active (code: %d)\n", r);

	r = engine->RegisterObjectMethod("keybind_t", "bool is_pressed() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register keybind_t::is_pressed (code: %d)\n", r);

	// color_picker_t methods
	r = engine->RegisterObjectMethod("color_picker_t", "void get(array<float> &out) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register color_picker_t::get (code: %d)\n", r);

	r = engine->RegisterObjectMethod("color_picker_t", "void set(const array<float> &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register color_picker_t::set (code: %d)\n", r);

	r = engine->RegisterObjectMethod("color_picker_t", "void set_active(bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register color_picker_t::set_active (code: %d)\n", r);

	// list_t methods
	r = engine->RegisterObjectMethod("list_t", "int get() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register list_t::get (code: %d)\n", r);

	r = engine->RegisterObjectMethod("list_t", "int get_count() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register list_t::get_count (code: %d)\n", r);

	r = engine->RegisterObjectMethod("list_t", "void clear()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register list_t::clear (code: %d)\n", r);

	r = engine->RegisterObjectMethod("list_t", "void append(const string &in, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register list_t::append (code: %d)\n", r);

	r = engine->RegisterObjectMethod("list_t", "void remove(int) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register list_t::remove (code: %d)\n", r);

	r = engine->RegisterObjectMethod("list_t", "void highlight(int) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register list_t::highlight (code: %d)\n", r);

	r = engine->RegisterObjectMethod("list_t", "void remove_highlight(int) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register list_t::remove_highlight (code: %d)\n", r);

	r = engine->RegisterObjectMethod("list_t", "void hide(int) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register list_t::hide (code: %d)\n", r);

	r = engine->RegisterObjectMethod("list_t", "void show(int) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register list_t::show (code: %d)\n", r);

	r = engine->RegisterObjectMethod("list_t", "void set_active(bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register list_t::set_active (code: %d)\n", r);

	// multi_select_t methods
	r = engine->RegisterObjectMethod("multi_select_t", "void get(array<bool> &out) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register multi_select_t::get (code: %d)\n", r);

	r = engine->RegisterObjectMethod("multi_select_t", "void set(int, bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register multi_select_t::set (code: %d)\n", r);

	r = engine->RegisterObjectMethod("multi_select_t", "void set_active(bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register multi_select_t::set_active (code: %d)\n", r);

	// single_select_t methods
	r = engine->RegisterObjectMethod("single_select_t", "int get() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register single_select_t::get (code: %d)\n", r);

	r = engine->RegisterObjectMethod("single_select_t", "void set(int)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register single_select_t::set (code: %d)\n", r);

	r = engine->RegisterObjectMethod("single_select_t", "void set_active(bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register single_select_t::set_active (code: %d)\n", r);

	// button_t methods
	r = engine->RegisterObjectMethod("button_t", "void set_active(bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register button_t::set_active (code: %d)\n", r);

	// =====================================================
	// STRING METHODS
	// =====================================================

	r = engine->RegisterObjectMethod("string", "string toUpper() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJFIRST);
	if (r < 0) printf("WARNING: Failed to register string::toUpper (code: %d)\n", r);

	r = engine->RegisterObjectMethod("string", "string toLower() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJFIRST);
	if (r < 0) printf("WARNING: Failed to register string::toLower (code: %d)\n", r);

	r = engine->RegisterObjectMethod("string", "array<string>@ split(const string &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJFIRST);
	if (r < 0) printf("WARNING: Failed to register string::split (code: %d)\n", r);

	r = engine->RegisterObjectMethod("string", "string trim() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJFIRST);
	if (r < 0) printf("WARNING: Failed to register string::trim (code: %d)\n", r);

	// =====================================================
	// DICTIONARY METHODS
	// =====================================================

	r = engine->RegisterObjectMethod("dictionary", "array<string>@ getValues() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJFIRST);
	if (r < 0) printf("WARNING: Failed to register dictionary::getValues (code: %d)\n", r);

	// Note: exists, delete, clear are already registered by AngelScript's dictionary addon

	// =====================================================
	// HASH_MAP TYPE
	// =====================================================

	// hash_map is a reference type (factory constructor returns handle)
	r = engine->RegisterObjectType("hash_map", 0, asOBJ_REF | asOBJ_NOCOUNT);
	if (r < 0) printf("WARNING: Failed to register hash_map type (code: %d)\n", r);

	// Factory constructor
	r = engine->RegisterObjectBehaviour("hash_map", asBEHAVE_FACTORY, "hash_map@ f()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register hash_map factory (code: %d)\n", r);

	// Core methods
	r = engine->RegisterObjectMethod("hash_map", "void set(uint64, ?&in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register hash_map::set (code: %d)\n", r);

	r = engine->RegisterObjectMethod("hash_map", "bool get(uint64, ?&out)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register hash_map::get (code: %d)\n", r);

	r = engine->RegisterObjectMethod("hash_map", "bool contains(uint64) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register hash_map::contains (code: %d)\n", r);

	r = engine->RegisterObjectMethod("hash_map", "bool erase(uint64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register hash_map::erase (code: %d)\n", r);

	r = engine->RegisterObjectMethod("hash_map", "void clear()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register hash_map::clear (code: %d)\n", r);

	r = engine->RegisterObjectMethod("hash_map", "uint size() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register hash_map::size (code: %d)\n", r);

	r = engine->RegisterObjectMethod("hash_map", "bool empty() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register hash_map::empty (code: %d)\n", r);

	// Iteration methods
	r = engine->RegisterObjectMethod("hash_map", "void iter_begin()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register hash_map::iter_begin (code: %d)\n", r);

	r = engine->RegisterObjectMethod("hash_map", "bool iter_next_key(uint64 &out)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register hash_map::iter_next_key (code: %d)\n", r);

	r = engine->RegisterObjectMethod("hash_map", "bool iter_next(uint64 &out, ?&out)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register hash_map::iter_next (code: %d)\n", r);

	// =====================================================
	// HASH_SET TYPE
	// =====================================================

	// hash_set is a value type (not a reference type)
	r = engine->RegisterObjectType("hash_set", sizeof(void*), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);
	if (r < 0) printf("WARNING: Failed to register hash_set type (code: %d)\n", r);

	// Constructor
	r = engine->RegisterObjectBehaviour("hash_set", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register hash_set constructor (code: %d)\n", r);

	// Destructor
	r = engine->RegisterObjectBehaviour("hash_set", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register hash_set destructor (code: %d)\n", r);

	// Core methods
	r = engine->RegisterObjectMethod("hash_set", "bool contains(uint64) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register hash_set::contains (code: %d)\n", r);

	r = engine->RegisterObjectMethod("hash_set", "bool insert(uint64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register hash_set::insert (code: %d)\n", r);

	r = engine->RegisterObjectMethod("hash_set", "bool erase(uint64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register hash_set::erase (code: %d)\n", r);

	r = engine->RegisterObjectMethod("hash_set", "void clear()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register hash_set::clear (code: %d)\n", r);

	r = engine->RegisterObjectMethod("hash_set", "uint size() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register hash_set::size (code: %d)\n", r);

	r = engine->RegisterObjectMethod("hash_set", "bool empty() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register hash_set::empty (code: %d)\n", r);

	// Convenience methods
	r = engine->RegisterObjectMethod("hash_set", "void set(uint64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register hash_set::set (code: %d)\n", r);

	r = engine->RegisterObjectMethod("hash_set", "bool get(uint64, uint64 &out) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register hash_set::get (code: %d)\n", r);

	// Iteration methods
	r = engine->RegisterObjectMethod("hash_set", "void iter_begin()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register hash_set::iter_begin (code: %d)\n", r);

	r = engine->RegisterObjectMethod("hash_set", "bool iter_next(uint64 &out)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register hash_set::iter_next (code: %d)\n", r);

	// Assignment operator
	r = engine->RegisterObjectMethod("hash_set", "hash_set &opAssign(const hash_set &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	if (r < 0) printf("WARNING: Failed to register hash_set::opAssign (code: %d)\n", r);

	// =====================================================
	// STRING GLOBAL FUNCTIONS
	// =====================================================

	r = engine->RegisterGlobalFunction("string join(const array<string> &in, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register join (code: %d)\n", r);

	// Note: formatFloat and formatInt are already registered by AngelScript addon

	// =====================================================
	// UTILITY FUNCTIONS (Base64, Hex, URL encoding)
	// =====================================================

	// Base64 functions
	r = engine->RegisterGlobalFunction("string util_base64_encode(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register util_base64_encode (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("bool util_base64_decode(const string &in, string &out, string &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register util_base64_decode (code: %d)\n", r);

	// Hex functions
	r = engine->RegisterGlobalFunction("string util_hex_encode(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register util_hex_encode (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("bool util_hex_decode(const string &in, string &out, string &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register util_hex_decode (code: %d)\n", r);

	// URL encoding functions
	r = engine->RegisterGlobalFunction("string util_url_encode(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register util_url_encode (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("string util_url_decode(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register util_url_decode (code: %d)\n", r);

	// =====================================================
	// GLOBAL FUNCTIONS
	// =====================================================

	// Process API
	r = engine->RegisterGlobalFunction("proc_t ref_process(uint)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register ref_process(uint) (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("proc_t ref_process(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register ref_process(string) (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("uproc_t ref_unprotected_process(uint)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register ref_unprotected_process (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("uproc_t ref_unprotected_process(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register ref_unprotected_process (code: %d)\n", r);

	// Callback API
	r = engine->RegisterGlobalFunction("int register_callback(const __Internal_CallbackFn@, int, int)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register register_callback (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void unregister_callback(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register unregister_callback (code: %d)\n", r);

	// Mutex API
	r = engine->RegisterGlobalFunction("mutex_t create_mutex()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register create_mutex (code: %d)\n", r);

	// Net API
	r = engine->RegisterGlobalFunction("ws_t ws_connect(const string &in, uint = 0)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register ws_connect (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("bool net_http_get(const string &in, uint &out, string &out, uint = 0)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register net_http_get (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("bool net_http_post(const string &in, const string &in, const string &in, uint &out, string &out, uint = 0)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register net_http_post (code: %d)\n", r);

	// Render API - Viewport
	r = engine->RegisterGlobalFunction("void get_view(float &out, float &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("float get_view_scale()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double get_fps()", asFUNCTION(StubFunction), asCALL_CDECL);

	// Render API - Shapes
	r = engine->RegisterGlobalFunction("void draw_rect(float, float, float, float, uint8, uint8, uint8, uint8, float, float, uint8)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register draw_rect (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void draw_rect_filled(float, float, float, float, uint8, uint8, uint8, uint8, float, uint8)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register draw_rect_filled (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void draw_line(float, float, float, float, uint8, uint8, uint8, uint8, float)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register draw_line (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void draw_circle(float, float, float, uint8, uint8, uint8, uint8, float, bool)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register draw_circle (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void draw_triangle(float, float, float, float, float, float, uint8, uint8, uint8, uint8, float, bool)", asFUNCTION(StubFunction), asCALL_CDECL);

	// Render API - Additional drawing functions
	r = engine->RegisterGlobalFunction("void draw_arc(float, float, float, float, float, float, uint8, uint8, uint8, uint8, float, bool)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void draw_polygon(const array<float> &in, uint, uint8, uint8, uint8, uint8, float, bool)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void draw_four_corner_gradient(float, float, float, float, uint8, uint8, uint8, uint8, uint8, uint8, uint8, uint8, uint8, uint8, uint8, uint8, uint8, uint8, uint8, uint8, float)", asFUNCTION(StubFunction), asCALL_CDECL);

	// Render API - Fonts and Text
	r = engine->RegisterGlobalFunction("uint64 create_font(const string &in, float, bool, bool, array<uint>@ = null)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register create_font (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("uint64 create_font_mem(const string &in, float, const array<uint8> &in, bool, bool, array<uint>@ = null)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register create_font_mem (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("uint64 get_font18()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register get_font18 (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("uint64 get_font20()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register get_font20 (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("uint64 get_font24()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register get_font24 (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("uint64 get_font28()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register get_font28 (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void draw_text(const string &in, float, float, uint8, uint8, uint8, uint8, uint64, int, uint8, uint8, uint8, uint8, float)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register draw_text (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void get_text_size(uint64, const string &in, int, int, float &out, float &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register get_text_size (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("int get_char_advance(uint64, uint)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register get_char_advance (code: %d)\n", r);

	// Render API - Clipping
	r = engine->RegisterGlobalFunction("void clip_push(float, float, float, float)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void clip_pop()", asFUNCTION(StubFunction), asCALL_CDECL);

	// Render API - Bitmaps
	r = engine->RegisterGlobalFunction("uint64 create_bitmap(const array<uint8> &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void draw_bitmap(uint64, float, float, float, float, uint8, uint8, uint8, uint8, bool)", asFUNCTION(StubFunction), asCALL_CDECL);

	// Render API - Direct/Custom Draw (D3D11 pipeline access)
	// -- Shaders
	r = engine->RegisterGlobalFunction("uint64 create_shader(const string &in, const string &in, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void destroy_shader(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	// -- Vertex Buffers
	r = engine->RegisterGlobalFunction("uint64 create_vertex_buffer(uint, uint, bool)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void destroy_vertex_buffer(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	// -- Constant Buffers
	r = engine->RegisterGlobalFunction("uint64 create_constant_buffer(uint)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void destroy_constant_buffer(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	// -- Blend States
	r = engine->RegisterGlobalFunction("uint64 create_blend_state(int, int, int, int, int, int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void destroy_blend_state(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	// -- Samplers
	r = engine->RegisterGlobalFunction("uint64 create_sampler(int, int, int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void destroy_sampler(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	// -- Textures
	r = engine->RegisterGlobalFunction("uint64 create_texture(uint, uint, const array<uint8> &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void destroy_texture(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	// -- Render Targets
	r = engine->RegisterGlobalFunction("uint64 create_render_target(uint, uint)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void destroy_render_target(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	// -- Draw Call
	r = engine->RegisterGlobalFunction("void custom_draw(uint64, uint64, const array<uint8> &in, uint, int, uint64, uint64, uint64, int, uint64, const array<uint8> @, int)", asFUNCTION(StubFunction), asCALL_CDECL);
	// -- Render Target Control
	r = engine->RegisterGlobalFunction("void custom_set_render_target(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void custom_reset_render_target()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void custom_bind_rt_as_texture(uint64, int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void custom_restore_state()", asFUNCTION(StubFunction), asCALL_CDECL);

	// Extended Math API - Global functions
	r = engine->RegisterGlobalFunction("double clamp(double, double, double)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double saturate(double)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("int sign(double)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double round(double)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double round_up(double)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double round_down(double)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double fract(double)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double lerp(double, double, double)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double wrap(double, double, double)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double inverse_lerp(double, double, double)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double remap(double, double, double, double, double)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double smoothstep(double, double, double)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool is_nan(double)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool is_inf(double)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double random()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register random (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void random_seed(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double random_range(double, double)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("int64 random_int(int64, int64)", asFUNCTION(StubFunction), asCALL_CDECL);

	// Extended Math API - Random functions
	r = engine->RegisterGlobalFunction("bool random_bool()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double random_gaussian(double, double)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("vector2 random_unit_vec2()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("vector3 random_unit_vec3()", asFUNCTION(StubFunction), asCALL_CDECL);

	r = engine->RegisterGlobalFunction("quaternion quat_from_euler(double, double, double)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register quat_from_euler (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("matrix4x4 mat4_identity()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("matrix4x4 mat4_zero()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("matrix4x4 mat4_translate(double, double, double)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("matrix4x4 mat4_scale(double, double, double)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("matrix4x4 mat4_rotate_euler(double, double, double)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("matrix4x4 mat4_from_quaternion(const quaternion &in)", asFUNCTION(StubFunction), asCALL_CDECL);

	// Engine Specific API
	r = engine->RegisterGlobalFunction("bool world_to_screen_rowmajor(const vector3 &in, const matrix4x4 &in, vector2 &out, const vector2 &in = vector2(0, 0))", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool world_to_screen_transposed(const vector3 &in, const matrix4x4 &in, vector2 &out, const vector2 &in = vector2(0, 0))", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool source2_world_to_screen(const vector3 &in, const matrix4x4 &in, vector2 &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register source2_world_to_screen (code: %d)\n", r);

	// Logging
	r = engine->RegisterGlobalFunction("void log(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register log (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void log_error(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);

	r = engine->RegisterGlobalFunction("void log_console(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register log_console (code: %d)\n", r);

	// =====================================================
	// SYSTEM API (CPU info, timing, datetime)
	// =====================================================

	// CPU Information
	r = engine->RegisterGlobalFunction("string cpu_vendor()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register cpu_vendor (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("string cpu_brand()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register cpu_brand (code: %d)\n", r);

	// Timing functions
	r = engine->RegisterGlobalFunction("uint64 rdtsc()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register rdtsc (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("int64 perf_time()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register perf_time (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("int64 perf_frequency()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register perf_frequency (code: %d)\n", r);

	// DateTime functions
	r = engine->RegisterGlobalFunction("dictionary@ get_datetime()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register get_datetime (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("uint64 get_timestamp()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register get_timestamp (code: %d)\n", r);

	// Thread priority functions
	r = engine->RegisterGlobalFunction("bool set_thread_to_highest_priority()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register set_thread_to_highest_priority (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("bool set_thread_to_lowest_priority()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register set_thread_to_lowest_priority (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("bool set_thread_to_normal_priority()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register set_thread_to_normal_priority (code: %d)\n", r);

	// Zydis disassembly functions
	r = engine->RegisterGlobalFunction("void zydis_disasm(const array<uint8>& in, array<dictionary@>& out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void zydis_disasm(const array<uint8>& in, uint64, array<dictionary@>& out)", asFUNCTION(StubFunction), asCALL_CDECL);

	// =====================================================
	// INPUT API
	// =====================================================

	// Mouse functions
	r = engine->RegisterGlobalFunction("void get_mouse_pos(float &out, float &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register get_mouse_pos (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void get_mouse_pos_desktop(float &out, float &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register get_mouse_pos_desktop (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void get_mouse_delta(float &out, float &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register get_mouse_delta (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void get_mouse_delta_desktop(float &out, float &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register get_mouse_delta_desktop (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("float get_scroll_delta()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register get_scroll_delta (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("bool mouse_movement_received()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register mouse_movement_received (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("bool is_hovered(float, float, float, float)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register is_hovered (code: %d)\n", r);

	// Keyboard functions
	r = engine->RegisterGlobalFunction("bool key_down(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register key_down (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("bool key_raw_down(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register key_raw_down (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("bool key_fired(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register key_fired (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("bool key_toggle(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register key_toggle (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("bool key_singlepress(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register key_singlepress (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("bool key_prev_down(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register key_prev_down (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void get_key_state(int, bool &out, bool &out, bool &out, bool &out, bool &out, bool &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("string get_key_name(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register get_key_name (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("string get_recent_key_input()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register get_recent_key_input (code: %d)\n", r);

	// =====================================================
	// WIN API
	// =====================================================
	// Note: Most Window API functions are registered below in the original code section
	// with uint64 handles. Adding is_foreground_window and clipboard functions here
	// since they're not in the original.

	r = engine->RegisterGlobalFunction("bool is_foreground_window(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register is_foreground_window (code: %d)\n", r);

	// Clipboard functions
	r = engine->RegisterGlobalFunction("bool copy_to_clipboard(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register copy_to_clipboard (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("bool copy_from_clipboard(string &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register copy_from_clipboard (code: %d)\n", r);

	// Util function
	r = engine->RegisterGlobalFunction("uint64 get_tickcount64()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register get_tickcount64 (code: %d)\n", r);

	// Engine API
	r = engine->RegisterGlobalFunction("string get_username()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register get_username (code: %d)\n", r);

	// Game-specific functions
	r = engine->RegisterGlobalFunction("string fortnite_get_player_name(proc_t &in, uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("vector3 rust_get_transform_position(proc_t &in, uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void set_bf6_globals(uint64, uint64, uint64, uint64, uint64, uint64)", asFUNCTION(StubFunction), asCALL_CDECL);

	// GUI API
	r = engine->RegisterGlobalFunction("void get_gui_position(float &out, float &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register get_gui_position (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void get_gui_size(float &out, float &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register get_gui_size (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("bool gui_active()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register gui_active (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("subtab_t create_subtab(int, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register create_subtab (code: %d)\n", r);

	// GUI find_* functions
	r = engine->RegisterGlobalFunction("checkbox_t find_checkbox(int, const string &in, const string &in, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register find_checkbox (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("slider_double_t find_slider_double(int, const string &in, const string &in, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register find_slider_double (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("slider_int_t find_slider_int(int, const string &in, const string &in, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register find_slider_int (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("input_t find_input(int, const string &in, const string &in, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register find_input (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("multi_select_t find_multi_select(int, const string &in, const string &in, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING:Failed to register find_multi_select (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("single_select_t find_single_select(int, const string &in, const string &in, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register find_single_select (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("keybind_t find_keybind(int, const string &in, const string &in, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register find_keybind (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("button_t find_button(int, const string &in, const string &in, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register find_button (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("color_picker_t find_color(int, const string &in, const string &in, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register find_color (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("list_t find_list(int, const string &in, const string &in, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register find_list (code: %d)\n", r);

	// GUI config helpers
	r = engine->RegisterGlobalFunction("string construct_config()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register construct_config (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void apply_config(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register apply_config (code: %d)\n", r);

	// Zydis API - use factory function name that doesn't conflict with type
	r = engine->RegisterGlobalFunction("ZydisBuilder@ CreateZydisBuilder()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool zydis_encode(ZydisEncoderRequest &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool zydis_encode_absolute(ZydisEncoderRequest &in, uint64, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool zydis_nop_fill(array<uint8> &out, uint32)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool zydis_decoded_to_request(const array<uint8> &in, uint64, ZydisEncoderRequest &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("int zydis_mnemonic_from_string(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("int zydis_register_from_string(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("string zydis_mnemonic_to_string(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("string zydis_register_to_string(int)", asFUNCTION(StubFunction), asCALL_CDECL);

	// Win API
	r = engine->RegisterGlobalFunction("array<WindowInfo>@ get_all_hwnds()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool set_clipboard_text(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool get_clipboard_text(string &out)", asFUNCTION(StubFunction), asCALL_CDECL);

	// System API
	r = engine->RegisterGlobalFunction("uint64 query_performance_frequency()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register query_performance_frequency (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("uint64 query_performance_counter()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register query_performance_counter (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void sleep(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register sleep (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("uint cpuid(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register cpuid (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("uint64 xgetbv(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register xgetbv (code: %d)\n", r);

	// Unicorn API
	r = engine->RegisterGlobalFunction("uint64 uc_create()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register uc_create (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("uint64 uc_create_process(proc_t, bool)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register uc_create_process (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("bool uc_mem_map(uint64, uint64, uint64, uint)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register uc_mem_map (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("bool uc_mem_write(uint64, uint64, const array<uint8> &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register uc_mem_write (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("bool uc_mem_read(uint64, uint64, const int, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register uc_mem_read (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void uc_reg_write64(uint64, int, uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register uc_reg_write64 (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("uint64 uc_reg_read64(uint64, int)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register uc_reg_read64 (code: %d)\n", r);

	// Unicorn 128-bit and 256-bit register functions
	r = engine->RegisterGlobalFunction("bool uc_reg_write128(uint64, int, const array<uint8> &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool uc_reg_read128(uint64, int, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool uc_reg_write256(uint64, int, const array<uint8> &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool uc_reg_read256(uint64, int, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);

	r = engine->RegisterGlobalFunction("bool uc_setup_stack(uint64, uint64, uint64, uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register uc_setup_stack (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("int uc_start(uint64, uint64, uint64, uint64, uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register uc_start (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void uc_close(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register uc_close (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void uc_flush_code(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register uc_flush_code (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("bool uc_hook_add(uint64, int, UcHookFn@)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register uc_hook_add (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void uc_emu_stop(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register uc_emu_stop (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void uc_del_hook(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register uc_del_hook (code: %d)\n", r);

	// Unicorn constants
	static const uint UC_PROT_NONE = 0;
	static const uint UC_PROT_READ = 1;
	static const uint UC_PROT_WRITE = 2;
	static const uint UC_PROT_EXEC = 4;
	static const uint UC_PROT_ALL = 7;

	static const int UC_HOOK_INTR = 1;
	static const int UC_HOOK_CODE = 2;
	static const int UC_HOOK_BLOCK = 4;
	static const int UC_HOOK_MEM_READ = 8;
	static const int UC_HOOK_MEM_WRITE = 16;
	static const int UC_HOOK_MEM_FETCH = 32;
	static const int UC_HOOK_MEM_READ_WRITE = 48;
	static const int UC_HOOK_MEM_UNMAPPED = 64;

	// Unicorn X86 register constants
	static const int UC_X86_REG_RAX = 0;
	static const int UC_X86_REG_RBX = 1;
	static const int UC_X86_REG_RCX = 2;
	static const int UC_X86_REG_RDX = 3;
	static const int UC_X86_REG_RSP = 4;
	static const int UC_X86_REG_RBP = 5;
	static const int UC_X86_REG_RSI = 6;
	static const int UC_X86_REG_RDI = 7;
	static const int UC_X86_REG_R8 = 8;
	static const int UC_X86_REG_R9 = 9;
	static const int UC_X86_REG_R10 = 10;
	static const int UC_X86_REG_R11 = 11;
	static const int UC_X86_REG_R12 = 12;
	static const int UC_X86_REG_R13 = 13;
	static const int UC_X86_REG_R14 = 14;
	static const int UC_X86_REG_R15 = 15;
	static const int UC_X86_REG_RIP = 16;
	static const int UC_X86_REG_EFLAGS = 17;
	static const int UC_X86_REG_GS_BASE = 18;
	static const int UC_X86_REG_FS_BASE = 19;
	static const int UC_X86_REG_XMM0  = 20;
	static const int UC_X86_REG_XMM1  = 21;
	static const int UC_X86_REG_XMM2  = 22;
	static const int UC_X86_REG_XMM3  = 23;
	static const int UC_X86_REG_XMM4  = 24;
	static const int UC_X86_REG_XMM5  = 25;
	static const int UC_X86_REG_XMM6  = 26;
	static const int UC_X86_REG_XMM7  = 27;
	static const int UC_X86_REG_XMM8  = 28;
	static const int UC_X86_REG_XMM9  = 29;
	static const int UC_X86_REG_XMM10 = 30;
	static const int UC_X86_REG_XMM11 = 31;
	static const int UC_X86_REG_XMM12 = 32;
	static const int UC_X86_REG_XMM13 = 33;
	static const int UC_X86_REG_XMM14 = 34;
	static const int UC_X86_REG_XMM15 = 35;

	r = engine->RegisterGlobalProperty("const uint UC_PROT_NONE", (void*)&UC_PROT_NONE);
	r = engine->RegisterGlobalProperty("const uint UC_PROT_READ", (void*)&UC_PROT_READ);
	r = engine->RegisterGlobalProperty("const uint UC_PROT_WRITE", (void*)&UC_PROT_WRITE);
	r = engine->RegisterGlobalProperty("const uint UC_PROT_EXEC", (void*)&UC_PROT_EXEC);
	r = engine->RegisterGlobalProperty("const uint UC_PROT_ALL", (void*)&UC_PROT_ALL);

	r = engine->RegisterGlobalProperty("const int UC_HOOK_INTR", (void*)&UC_HOOK_INTR);
	r = engine->RegisterGlobalProperty("const int UC_HOOK_CODE", (void*)&UC_HOOK_CODE);
	r = engine->RegisterGlobalProperty("const int UC_HOOK_BLOCK", (void*)&UC_HOOK_BLOCK);
	r = engine->RegisterGlobalProperty("const int UC_HOOK_MEM_READ", (void*)&UC_HOOK_MEM_READ);
	r = engine->RegisterGlobalProperty("const int UC_HOOK_MEM_WRITE", (void*)&UC_HOOK_MEM_WRITE);
	r = engine->RegisterGlobalProperty("const int UC_HOOK_MEM_FETCH", (void*)&UC_HOOK_MEM_FETCH);
	r = engine->RegisterGlobalProperty("const int UC_HOOK_MEM_READ_WRITE", (void*)&UC_HOOK_MEM_READ_WRITE);
	r = engine->RegisterGlobalProperty("const int UC_HOOK_MEM_UNMAPPED", (void*)&UC_HOOK_MEM_UNMAPPED);

	r = engine->RegisterGlobalProperty("const int UC_X86_REG_RAX", (void*)&UC_X86_REG_RAX);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_RBX", (void*)&UC_X86_REG_RBX);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_RCX", (void*)&UC_X86_REG_RCX);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_RDX", (void*)&UC_X86_REG_RDX);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_RSP", (void*)&UC_X86_REG_RSP);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_RBP", (void*)&UC_X86_REG_RBP);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_RSI", (void*)&UC_X86_REG_RSI);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_RDI", (void*)&UC_X86_REG_RDI);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_R8", (void*)&UC_X86_REG_R8);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_R9", (void*)&UC_X86_REG_R9);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_R10", (void*)&UC_X86_REG_R10);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_R11", (void*)&UC_X86_REG_R11);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_R12", (void*)&UC_X86_REG_R12);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_R13", (void*)&UC_X86_REG_R13);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_R14", (void*)&UC_X86_REG_R14);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_R15", (void*)&UC_X86_REG_R15);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_RIP", (void*)&UC_X86_REG_RIP);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_EFLAGS", (void*)&UC_X86_REG_EFLAGS);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_GS_BASE", (void*)&UC_X86_REG_GS_BASE);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_FS_BASE", (void*)&UC_X86_REG_FS_BASE);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_XMM0",  (void*)&UC_X86_REG_XMM0);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_XMM1",  (void*)&UC_X86_REG_XMM1);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_XMM2",  (void*)&UC_X86_REG_XMM2);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_XMM3",  (void*)&UC_X86_REG_XMM3);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_XMM4",  (void*)&UC_X86_REG_XMM4);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_XMM5",  (void*)&UC_X86_REG_XMM5);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_XMM6",  (void*)&UC_X86_REG_XMM6);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_XMM7",  (void*)&UC_X86_REG_XMM7);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_XMM8",  (void*)&UC_X86_REG_XMM8);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_XMM9",  (void*)&UC_X86_REG_XMM9);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_XMM10", (void*)&UC_X86_REG_XMM10);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_XMM11", (void*)&UC_X86_REG_XMM11);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_XMM12", (void*)&UC_X86_REG_XMM12);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_XMM13", (void*)&UC_X86_REG_XMM13);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_XMM14", (void*)&UC_X86_REG_XMM14);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_XMM15", (void*)&UC_X86_REG_XMM15);

	// Window operations
	r = engine->RegisterGlobalFunction("uint64 find_window(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register find_window (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("uint64 find_window_ex(const string &in, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register find_window_ex (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("bool get_window_title(uint64, string &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register get_window_title (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("bool get_window_class(uint64, string &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register get_window_class (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("bool get_window_rect(uint64, int &out, int &out, int &out, int &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register get_window_rect (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("bool get_client_rect(uint64, int &out, int &out, int &out, int &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register get_client_rect (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("bool get_window_size(uint64, int &out, int &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool is_window_active(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);

	r = engine->RegisterGlobalFunction("bool get_window_thread_process_id(uint64, uint &out, uint &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register get_window_thread_process_id (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("bool set_foreground_window(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register set_foreground_window (code: %d)\n", r);

	// Mouse operations
	r = engine->RegisterGlobalFunction("void mouse_move(int, int)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register mouse_move (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void mouse_move_relative(int, int)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register mouse_move_relative (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void mouse_left_click()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register mouse_left_click (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void mouse_right_click()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register mouse_right_click (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void mouse_middle_click()", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register mouse_middle_click (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void mouse_scroll(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register mouse_scroll (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void send_mouse_input(int64, int64, uint, uint)", asFUNCTION(StubFunction), asCALL_CDECL);

	// Key operations
	r = engine->RegisterGlobalFunction("bool send_char(uint64, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register send_char (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("bool send_key(uint64, int)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register send_key (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void win_key_down(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register win_key_down (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void win_key_up(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register win_key_up (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("void win_key_press(int, int = 30)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register win_key_press (code: %d)\n", r);

	// Message operations
	r = engine->RegisterGlobalFunction("bool post_message(uint64, uint, uint64, uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register post_message (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("uint64 send_message(uint64, uint, uint64, uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register send_message (code: %d)\n", r);

	// Input API
	r = engine->RegisterGlobalFunction("bool is_key_down(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool is_key_pressed(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool is_key_released(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void get_keys_down(array<int> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register get_keys_down (code: %d)\n", r);

	// File System API
	r = engine->RegisterGlobalFunction("bool file_exists(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool does_file_exist(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool create_directory(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool create_file(const string &in, const string &in = \"\")", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool read_file(const string &in, string &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool read_file(const string &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool write_file(const string &in, const array<uint8> &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool query_directory(const string &in, bool, bool, const array<string> &in, array<string> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool delete_file(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool delete_directory(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool write_file_binary(const string &in, const array<uint8> &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool read_file_binary(const string &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool append_file_binary(const string &in, const array<uint8> &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint64 get_file_size(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);

	// Bit Reinterpret Helpers
	r = engine->RegisterGlobalFunction("uint float_to_bits(float)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register float_to_bits (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("float bits_to_float(uint)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register bits_to_float (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("uint64 double_to_bits(double)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register double_to_bits (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("double bits_to_double(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register bits_to_double (code: %d)\n", r);

	// Additional bit conversion utilities
	r = engine->RegisterGlobalFunction("uint32 f_to_u32(float)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint32 u8_to_u32(uint8)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint32 u16_to_u32(uint16)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint32 u64_to_u32(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint32 i8_to_u32(int8)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint32 i16_to_u32(int16)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint32 i32_to_u32(int32)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint32 i64_to_u32(int64)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint32 d_to_u32(double)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("float u32_to_f(uint)", asFUNCTION(StubFunction), asCALL_CDECL);

	// JSON API
	r = engine->RegisterGlobalFunction("dictionary@ json_parse(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register json_parse (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("string json_stringify(const dictionary &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING: Failed to register json_stringify (code: %d)\n", r);

	// JSON API alternative function names (aliases)
	r = engine->RegisterGlobalFunction("bool json_decode(const string &in, dictionary &out, string &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool json_encode(const dictionary &in, string &out, string &out)", asFUNCTION(StubFunction), asCALL_CDECL);

	// =====================================================
	// INTRINSICS API - Bit Rotation
	// =====================================================

	r = engine->RegisterGlobalFunction("uint8 rol8(uint8, int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint8 ror8(uint8, int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint16 rol16(uint16, int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint16 ror16(uint16, int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint32 rol32(uint32, int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint32 ror32(uint32, int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint64 rol64(uint64, int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint64 ror64(uint64, int)", asFUNCTION(StubFunction), asCALL_CDECL);

	// Byte Swap
	r = engine->RegisterGlobalFunction("uint16 bswap16(uint16)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint32 bswap32(uint32)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint64 bswap64(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);

	// Bit Manipulation
	r = engine->RegisterGlobalFunction("int popcnt32(uint32)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("int popcnt64(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("int lzcnt32(uint32)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("int lzcnt64(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("int tzcnt32(uint32)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("int tzcnt64(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);

	// SSE Logical Operations
	r = engine->RegisterGlobalFunction("void mm_xor_si128(const array<uint8> &in, const array<uint8> &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_or_si128(const array<uint8> &in, const array<uint8> &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_and_si128(const array<uint8> &in, const array<uint8> &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_andnot_si128(const array<uint8> &in, const array<uint8> &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);

	// SSE Shift Operations
	r = engine->RegisterGlobalFunction("void mm_slli_epi16(const array<uint8> &in, int, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_srli_epi16(const array<uint8> &in, int, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_slli_epi32(const array<uint8> &in, int, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_srli_epi32(const array<uint8> &in, int, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_slli_epi64(const array<uint8> &in, int, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_srli_epi64(const array<uint8> &in, int, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_slli_si128(const array<uint8> &in, int, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_srli_si128(const array<uint8> &in, int, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);

	// SSE Shuffle Operations
	r = engine->RegisterGlobalFunction("void mm_shuffle_epi8(const array<uint8> &in, const array<uint8> &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_shuffle_epi32(const array<uint8> &in, int, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_shufflehi_epi16(const array<uint8> &in, int, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_shufflelo_epi16(const array<uint8> &in, int, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);

	// SSE Unpack Operations
	r = engine->RegisterGlobalFunction("void mm_unpackhi_epi8(const array<uint8> &in, const array<uint8> &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_unpackhi_epi16(const array<uint8> &in, const array<uint8> &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_unpackhi_epi32(const array<uint8> &in, const array<uint8> &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_unpackhi_epi64(const array<uint8> &in, const array<uint8> &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_unpacklo_epi8(const array<uint8> &in, const array<uint8> &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_unpacklo_epi16(const array<uint8> &in, const array<uint8> &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_unpacklo_epi32(const array<uint8> &in, const array<uint8> &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_unpacklo_epi64(const array<uint8> &in, const array<uint8> &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);

	// SSE Arithmetic
	r = engine->RegisterGlobalFunction("void mm_add_epi8(const array<uint8> &in, const array<uint8> &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_add_epi16(const array<uint8> &in, const array<uint8> &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_add_epi32(const array<uint8> &in, const array<uint8> &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_add_epi64(const array<uint8> &in, const array<uint8> &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_sub_epi8(const array<uint8> &in, const array<uint8> &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_sub_epi16(const array<uint8> &in, const array<uint8> &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_sub_epi32(const array<uint8> &in, const array<uint8> &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_sub_epi64(const array<uint8> &in, const array<uint8> &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_mullo_epi16(const array<uint8> &in, const array<uint8> &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_mullo_epi32(const array<uint8> &in, const array<uint8> &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);

	// SSE Set / Broadcast
	r = engine->RegisterGlobalFunction("void mm_set_epi64x(int64, int64, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_set_epi32(int32, int32, int32, int32, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_set1_epi64x(int64, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_set1_epi32(int32, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_set1_epi16(int16, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_set1_epi8(int8, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_setzero_si128(array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void broadcast_qword(uint64, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void broadcast_dword(uint32, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);

	// SSE Extract
	r = engine->RegisterGlobalFunction("int64 mm_extract_epi64(const array<uint8> &in, int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("int32 mm_extract_epi32(const array<uint8> &in, int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("int32 mm_extract_epi16(const array<uint8> &in, int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("int32 mm_extract_epi8(const array<uint8> &in, int)", asFUNCTION(StubFunction), asCALL_CDECL);

	// SSE Compare
	r = engine->RegisterGlobalFunction("void mm_cmpeq_epi8(const array<uint8> &in, const array<uint8> &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_cmpeq_epi16(const array<uint8> &in, const array<uint8> &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mm_cmpeq_epi32(const array<uint8> &in, const array<uint8> &in, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);

	// =====================================================
	// CONSTANTS
	// =====================================================

	// Math constants
	static const double M_ZERO = 0.0;
	static const double M_ONE = 1.0;
	static const double M_EPSILON = 1e-6;
	static const double M_PI = 3.14159265358979323846;
	static const double M_TAU = 6.28318530717958647692;
	static const double M_PI_2 = 1.57079632679489661923;
	static const double M_PI_4 = 0.78539816339744830962;
	static const double RAD2DEG = 57.2957795130823208768;
	static const double DEG2RAD = 0.01745329251994329577;

	r = engine->RegisterGlobalProperty("const double M_ZERO", (void*)&M_ZERO);
	r = engine->RegisterGlobalProperty("const double M_ONE", (void*)&M_ONE);
	r = engine->RegisterGlobalProperty("const double M_EPSILON", (void*)&M_EPSILON);
	r = engine->RegisterGlobalProperty("const double M_PI", (void*)&M_PI);
	r = engine->RegisterGlobalProperty("const double M_TAU", (void*)&M_TAU);
	r = engine->RegisterGlobalProperty("const double M_PI_2", (void*)&M_PI_2);
	r = engine->RegisterGlobalProperty("const double M_PI_4", (void*)&M_PI_4);
	r = engine->RegisterGlobalProperty("const double RAD2DEG", (void*)&RAD2DEG);
	r = engine->RegisterGlobalProperty("const double DEG2RAD", (void*)&DEG2RAD);

	// Direct render API constants - Topology
	static const int TOPO_TRIANGLE_LIST  = 0;
	static const int TOPO_TRIANGLE_STRIP = 1;
	static const int TOPO_LINE_LIST      = 2;
	static const int TOPO_LINE_STRIP     = 3;
	static const int TOPO_POINT_LIST     = 4;

	r = engine->RegisterGlobalProperty("const int TOPO_TRIANGLE_LIST",  (void*)&TOPO_TRIANGLE_LIST);
	r = engine->RegisterGlobalProperty("const int TOPO_TRIANGLE_STRIP", (void*)&TOPO_TRIANGLE_STRIP);
	r = engine->RegisterGlobalProperty("const int TOPO_LINE_LIST",      (void*)&TOPO_LINE_LIST);
	r = engine->RegisterGlobalProperty("const int TOPO_LINE_STRIP",     (void*)&TOPO_LINE_STRIP);
	r = engine->RegisterGlobalProperty("const int TOPO_POINT_LIST",     (void*)&TOPO_POINT_LIST);

	// Direct render API constants - Blend Factors
	static const int BLEND_ZERO          = 0;
	static const int BLEND_ONE           = 1;
	static const int BLEND_SRC_ALPHA     = 2;
	static const int BLEND_INV_SRC_ALPHA = 3;
	static const int BLEND_DEST_ALPHA    = 4;
	static const int BLEND_INV_DEST_ALPHA= 5;
	static const int BLEND_SRC_COLOR     = 6;
	static const int BLEND_INV_SRC_COLOR = 7;
	static const int BLEND_DEST_COLOR    = 8;
	static const int BLEND_INV_DEST_COLOR= 9;

	r = engine->RegisterGlobalProperty("const int BLEND_ZERO",           (void*)&BLEND_ZERO);
	r = engine->RegisterGlobalProperty("const int BLEND_ONE",            (void*)&BLEND_ONE);
	r = engine->RegisterGlobalProperty("const int BLEND_SRC_ALPHA",      (void*)&BLEND_SRC_ALPHA);
	r = engine->RegisterGlobalProperty("const int BLEND_INV_SRC_ALPHA",  (void*)&BLEND_INV_SRC_ALPHA);
	r = engine->RegisterGlobalProperty("const int BLEND_DEST_ALPHA",     (void*)&BLEND_DEST_ALPHA);
	r = engine->RegisterGlobalProperty("const int BLEND_INV_DEST_ALPHA", (void*)&BLEND_INV_DEST_ALPHA);
	r = engine->RegisterGlobalProperty("const int BLEND_SRC_COLOR",      (void*)&BLEND_SRC_COLOR);
	r = engine->RegisterGlobalProperty("const int BLEND_INV_SRC_COLOR",  (void*)&BLEND_INV_SRC_COLOR);
	r = engine->RegisterGlobalProperty("const int BLEND_DEST_COLOR",     (void*)&BLEND_DEST_COLOR);
	r = engine->RegisterGlobalProperty("const int BLEND_INV_DEST_COLOR", (void*)&BLEND_INV_DEST_COLOR);

	// Direct render API constants - Blend Operations
	static const int BLEND_OP_ADD         = 0;
	static const int BLEND_OP_SUBTRACT    = 1;
	static const int BLEND_OP_REV_SUBTRACT= 2;
	static const int BLEND_OP_MIN         = 3;
	static const int BLEND_OP_MAX         = 4;

	r = engine->RegisterGlobalProperty("const int BLEND_OP_ADD",          (void*)&BLEND_OP_ADD);
	r = engine->RegisterGlobalProperty("const int BLEND_OP_SUBTRACT",     (void*)&BLEND_OP_SUBTRACT);
	r = engine->RegisterGlobalProperty("const int BLEND_OP_REV_SUBTRACT", (void*)&BLEND_OP_REV_SUBTRACT);
	r = engine->RegisterGlobalProperty("const int BLEND_OP_MIN",          (void*)&BLEND_OP_MIN);
	r = engine->RegisterGlobalProperty("const int BLEND_OP_MAX",          (void*)&BLEND_OP_MAX);

	// Direct render API constants - Vertex Layout Element Types
	static const int ELEM_FLOAT1      = 0;
	static const int ELEM_FLOAT2      = 1;
	static const int ELEM_FLOAT3      = 2;
	static const int ELEM_FLOAT4      = 3;
	static const int ELEM_BYTE4_UNORM = 4;
	static const int ELEM_UINT1       = 5;

	r = engine->RegisterGlobalProperty("const int ELEM_FLOAT1",      (void*)&ELEM_FLOAT1);
	r = engine->RegisterGlobalProperty("const int ELEM_FLOAT2",      (void*)&ELEM_FLOAT2);
	r = engine->RegisterGlobalProperty("const int ELEM_FLOAT3",      (void*)&ELEM_FLOAT3);
	r = engine->RegisterGlobalProperty("const int ELEM_FLOAT4",      (void*)&ELEM_FLOAT4);
	r = engine->RegisterGlobalProperty("const int ELEM_BYTE4_UNORM", (void*)&ELEM_BYTE4_UNORM);
	r = engine->RegisterGlobalProperty("const int ELEM_UINT1",       (void*)&ELEM_UINT1);

	// Direct render API constants - Texture Filter Modes
	static const int FILTER_POINT       = 0;
	static const int FILTER_LINEAR      = 1;
	static const int FILTER_ANISOTROPIC = 2;

	r = engine->RegisterGlobalProperty("const int FILTER_POINT",       (void*)&FILTER_POINT);
	r = engine->RegisterGlobalProperty("const int FILTER_LINEAR",      (void*)&FILTER_LINEAR);
	r = engine->RegisterGlobalProperty("const int FILTER_ANISOTROPIC", (void*)&FILTER_ANISOTROPIC);

	// Direct render API constants - Texture Address Modes
	static const int ADDRESS_WRAP   = 0;
	static const int ADDRESS_CLAMP  = 1;
	static const int ADDRESS_MIRROR = 2;
	static const int ADDRESS_BORDER = 3;

	r = engine->RegisterGlobalProperty("const int ADDRESS_WRAP",   (void*)&ADDRESS_WRAP);
	r = engine->RegisterGlobalProperty("const int ADDRESS_CLAMP",  (void*)&ADDRESS_CLAMP);
	r = engine->RegisterGlobalProperty("const int ADDRESS_MIRROR", (void*)&ADDRESS_MIRROR);
	r = engine->RegisterGlobalProperty("const int ADDRESS_BORDER", (void*)&ADDRESS_BORDER);

	// Text effect constants
	static const int TE_NONE = 0;
	static const int TE_OUTLINE = 1;
	static const int TE_SHADOW = 2;
	static const int TE_GLOW = 3;

	r = engine->RegisterGlobalProperty("const int TE_NONE", (void*)&TE_NONE);
	r = engine->RegisterGlobalProperty("const int TE_OUTLINE", (void*)&TE_OUTLINE);
	r = engine->RegisterGlobalProperty("const int TE_SHADOW", (void*)&TE_SHADOW);
	r = engine->RegisterGlobalProperty("const int TE_GLOW", (void*)&TE_GLOW);

	// Rectangle rounding flags
	static const unsigned char RR_TOP_LEFT = 0x01;
	static const unsigned char RR_TOP_RIGHT = 0x02;
	static const unsigned char RR_BOTTOM_LEFT = 0x04;
	static const unsigned char RR_BOTTOM_RIGHT = 0x08;

	r = engine->RegisterGlobalProperty("const uint8 RR_TOP_LEFT", (void*)&RR_TOP_LEFT);
	r = engine->RegisterGlobalProperty("const uint8 RR_TOP_RIGHT", (void*)&RR_TOP_RIGHT);
	r = engine->RegisterGlobalProperty("const uint8 RR_BOTTOM_LEFT", (void*)&RR_BOTTOM_LEFT);
	r = engine->RegisterGlobalProperty("const uint8 RR_BOTTOM_RIGHT", (void*)&RR_BOTTOM_RIGHT);

	// Zydis mode constants
	static const int MODE_LONG_64 = 0;
	static const int MODE_LONG_COMPAT_32 = 1;
	static const int MODE_LONG_COMPAT_16 = 3;
	static const int MODE_LEGACY_32 = 2;
	static const int MODE_LEGACY_16 = 4;
	static const int MODE_REAL_16 = 5;

	r = engine->RegisterGlobalProperty("const int MODE_LONG_64", (void*)&MODE_LONG_64);
	r = engine->RegisterGlobalProperty("const int MODE_LONG_COMPAT_32", (void*)&MODE_LONG_COMPAT_32);
	r = engine->RegisterGlobalProperty("const int MODE_LONG_COMPAT_16", (void*)&MODE_LONG_COMPAT_16);
	r = engine->RegisterGlobalProperty("const int MODE_LEGACY_32", (void*)&MODE_LEGACY_32);
	r = engine->RegisterGlobalProperty("const int MODE_LEGACY_16", (void*)&MODE_LEGACY_16);
	r = engine->RegisterGlobalProperty("const int MODE_REAL_16", (void*)&MODE_REAL_16);

	// Zydis branch type constants
	static const int BRANCH_NONE = 0;
	static const int BRANCH_SHORT = 1;
	static const int BRANCH_NEAR = 2;
	static const int BRANCH_FAR = 3;

	r = engine->RegisterGlobalProperty("const int BRANCH_NONE", (void*)&BRANCH_NONE);
	r = engine->RegisterGlobalProperty("const int BRANCH_SHORT", (void*)&BRANCH_SHORT);
	r = engine->RegisterGlobalProperty("const int BRANCH_NEAR", (void*)&BRANCH_NEAR);
	r = engine->RegisterGlobalProperty("const int BRANCH_FAR", (void*)&BRANCH_FAR);

	// Zydis branch width constants
	static const int WIDTH_NONE = 0;
	static const int WIDTH_8 = 1;
	static const int WIDTH_16 = 2;
	static const int WIDTH_32 = 3;
	static const int WIDTH_64 = 4;

	r = engine->RegisterGlobalProperty("const int WIDTH_NONE", (void*)&WIDTH_NONE);
	r = engine->RegisterGlobalProperty("const int WIDTH_8", (void*)&WIDTH_8);
	r = engine->RegisterGlobalProperty("const int WIDTH_16", (void*)&WIDTH_16);
	r = engine->RegisterGlobalProperty("const int WIDTH_32", (void*)&WIDTH_32);
	r = engine->RegisterGlobalProperty("const int WIDTH_64", (void*)&WIDTH_64);

	// Zydis register constants
	static const int ZYDIS_REGISTER_RAX = 0;
	static const int ZYDIS_REGISTER_RBX = 1;
	static const int ZYDIS_REGISTER_RCX = 2;
	static const int ZYDIS_REGISTER_RDX = 3;
	static const int ZYDIS_REGISTER_RSP = 4;
	static const int ZYDIS_REGISTER_RBP = 5;
	static const int ZYDIS_REGISTER_RIP = 6;
	static const int ZYDIS_REGISTER_R8 = 7;
	static const int ZYDIS_REGISTER_R9 = 8;
	static const int ZYDIS_REGISTER_R10 = 9;
	static const int ZYDIS_REGISTER_R11 = 10;
	static const int ZYDIS_REGISTER_R12 = 11;
	static const int ZYDIS_REGISTER_R13 = 12;
	static const int ZYDIS_REGISTER_R14 = 13;
	static const int ZYDIS_REGISTER_R15 = 14;
	static const int ZYDIS_REGISTER_XMM0 = 16;
	static const int ZYDIS_REGISTER_XMM1 = 17;
	static const int ZYDIS_REGISTER_XMM2 = 18;
	static const int ZYDIS_REGISTER_XMM3 = 19;
	static const int ZYDIS_REGISTER_XMM4 = 20;
	static const int ZYDIS_REGISTER_XMM5 = 21;
	static const int ZYDIS_REGISTER_XMM6 = 22;
	static const int ZYDIS_REGISTER_XMM7 = 23;
	static const int ZYDIS_REGISTER_YMM0 = 24;
	static const int ZYDIS_REGISTER_YMM1 = 25;
	static const int ZYDIS_REGISTER_YMM2 = 26;
	static const int ZYDIS_REGISTER_YMM3 = 27;
	static const int ZYDIS_REGISTER_YMM4 = 28;
	static const int ZYDIS_REGISTER_YMM5 = 29;
	static const int ZYDIS_REGISTER_YMM6 = 30;
	static const int ZYDIS_REGISTER_YMM7 = 31;

	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_RAX", (void*)&ZYDIS_REGISTER_RAX);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_RBX", (void*)&ZYDIS_REGISTER_RBX);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_RCX", (void*)&ZYDIS_REGISTER_RCX);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_RDX", (void*)&ZYDIS_REGISTER_RDX);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_RSP", (void*)&ZYDIS_REGISTER_RSP);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_RBP", (void*)&ZYDIS_REGISTER_RBP);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_RIP", (void*)&ZYDIS_REGISTER_RIP);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_R8", (void*)&ZYDIS_REGISTER_R8);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_R9", (void*)&ZYDIS_REGISTER_R9);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_R10", (void*)&ZYDIS_REGISTER_R10);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_R11", (void*)&ZYDIS_REGISTER_R11);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_R12", (void*)&ZYDIS_REGISTER_R12);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_R13", (void*)&ZYDIS_REGISTER_R13);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_R14", (void*)&ZYDIS_REGISTER_R14);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_R15", (void*)&ZYDIS_REGISTER_R15);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_XMM0", (void*)&ZYDIS_REGISTER_XMM0);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_XMM1", (void*)&ZYDIS_REGISTER_XMM1);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_XMM2", (void*)&ZYDIS_REGISTER_XMM2);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_XMM3", (void*)&ZYDIS_REGISTER_XMM3);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_XMM4", (void*)&ZYDIS_REGISTER_XMM4);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_XMM5", (void*)&ZYDIS_REGISTER_XMM5);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_XMM6", (void*)&ZYDIS_REGISTER_XMM6);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_XMM7", (void*)&ZYDIS_REGISTER_XMM7);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_YMM0", (void*)&ZYDIS_REGISTER_YMM0);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_YMM1", (void*)&ZYDIS_REGISTER_YMM1);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_YMM2", (void*)&ZYDIS_REGISTER_YMM2);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_YMM3", (void*)&ZYDIS_REGISTER_YMM3);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_YMM4", (void*)&ZYDIS_REGISTER_YMM4);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_YMM5", (void*)&ZYDIS_REGISTER_YMM5);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_YMM6", (void*)&ZYDIS_REGISTER_YMM6);
	r = engine->RegisterGlobalProperty("const int ZYDIS_REGISTER_YMM7", (void*)&ZYDIS_REGISTER_YMM7);

	// Virtual Key Code constants
	static const int VK_LBUTTON = 0x01;
	static const int VK_RBUTTON = 0x02;
	static const int VK_CANCEL = 0x03;
	static const int VK_MBUTTON = 0x04;
	static const int VK_BACK = 0x08;
	static const int VK_TAB = 0x09;
	static const int VK_CLEAR = 0x0C;
	static const int VK_RETURN = 0x0D;
	static const int VK_SHIFT = 0x10;
	static const int VK_CONTROL = 0x11;
	static const int VK_MENU = 0x12;
	static const int VK_PAUSE = 0x13;
	static const int VK_CAPITAL = 0x14;
	static const int VK_ESCAPE = 0x1B;
	static const int VK_SPACE = 0x20;
	static const int VK_PRIOR = 0x21;
	static const int VK_NEXT = 0x22;
	static const int VK_END = 0x23;
	static const int VK_HOME = 0x24;
	static const int VK_LEFT = 0x25;
	static const int VK_UP = 0x26;
	static const int VK_RIGHT = 0x27;
	static const int VK_DOWN = 0x28;
	static const int VK_SELECT = 0x29;
	static const int VK_PRINT = 0x2A;
	static const int VK_EXECUTE = 0x2B;
	static const int VK_SNAPSHOT = 0x2C;
	static const int VK_INSERT = 0x2D;
	static const int VK_DELETE = 0x2E;
	static const int VK_HELP = 0x2F;
	static const int VK_LWIN = 0x5B;
	static const int VK_RWIN = 0x5C;
	static const int VK_APPS = 0x5D;
	static const int VK_NUMPAD0 = 0x60;
	static const int VK_NUMPAD1 = 0x61;
	static const int VK_NUMPAD2 = 0x62;
	static const int VK_NUMPAD3 = 0x63;
	static const int VK_NUMPAD4 = 0x64;
	static const int VK_NUMPAD5 = 0x65;
	static const int VK_NUMPAD6 = 0x66;
	static const int VK_NUMPAD7 = 0x67;
	static const int VK_NUMPAD8 = 0x68;
	static const int VK_NUMPAD9 = 0x69;
	static const int VK_MULTIPLY = 0x6A;
	static const int VK_ADD = 0x6B;
	static const int VK_SEPARATOR = 0x6C;
	static const int VK_SUBTRACT = 0x6D;
	static const int VK_DECIMAL = 0x6E;
	static const int VK_DIVIDE = 0x6F;
	static const int VK_F1 = 0x70;
	static const int VK_F2 = 0x71;
	static const int VK_F3 = 0x72;
	static const int VK_F4 = 0x73;
	static const int VK_F5 = 0x74;
	static const int VK_F6 = 0x75;
	static const int VK_F7 = 0x76;
	static const int VK_F8 = 0x77;
	static const int VK_F9 = 0x78;
	static const int VK_F10 = 0x79;
	static const int VK_F11 = 0x7A;
	static const int VK_F12 = 0x7B;
	static const int VK_NUMLOCK = 0x90;
	static const int VK_SCROLL = 0x91;
	static const int VK_LSHIFT = 0xA0;
	static const int VK_RSHIFT = 0xA1;
	static const int VK_LCONTROL = 0xA2;
	static const int VK_RCONTROL = 0xA3;
	static const int VK_LMENU = 0xA4;
	static const int VK_RMENU = 0xA5;
	static const int VK_OEM_1 = 0xBA;
	static const int VK_OEM_PLUS = 0xBB;
	static const int VK_OEM_COMMA = 0xBC;
	static const int VK_OEM_MINUS = 0xBD;
	static const int VK_OEM_PERIOD = 0xBE;
	static const int VK_OEM_2 = 0xBF;
	static const int VK_OEM_3 = 0xC0;
	static const int VK_OEM_4 = 0xDB;
	static const int VK_OEM_5 = 0xDC;
	static const int VK_OEM_6 = 0xDD;
	static const int VK_OEM_7 = 0xDE;

	r = engine->RegisterGlobalProperty("const int VK_LBUTTON", (void*)&VK_LBUTTON);
	r = engine->RegisterGlobalProperty("const int VK_RBUTTON", (void*)&VK_RBUTTON);
	r = engine->RegisterGlobalProperty("const int VK_CANCEL", (void*)&VK_CANCEL);
	r = engine->RegisterGlobalProperty("const int VK_MBUTTON", (void*)&VK_MBUTTON);
	r = engine->RegisterGlobalProperty("const int VK_BACK", (void*)&VK_BACK);
	r = engine->RegisterGlobalProperty("const int VK_TAB", (void*)&VK_TAB);
	r = engine->RegisterGlobalProperty("const int VK_CLEAR", (void*)&VK_CLEAR);
	r = engine->RegisterGlobalProperty("const int VK_RETURN", (void*)&VK_RETURN);
	r = engine->RegisterGlobalProperty("const int VK_SHIFT", (void*)&VK_SHIFT);
	r = engine->RegisterGlobalProperty("const int VK_CONTROL", (void*)&VK_CONTROL);
	r = engine->RegisterGlobalProperty("const int VK_MENU", (void*)&VK_MENU);
	r = engine->RegisterGlobalProperty("const int VK_PAUSE", (void*)&VK_PAUSE);
	r = engine->RegisterGlobalProperty("const int VK_CAPITAL", (void*)&VK_CAPITAL);
	r = engine->RegisterGlobalProperty("const int VK_ESCAPE", (void*)&VK_ESCAPE);
	r = engine->RegisterGlobalProperty("const int VK_SPACE", (void*)&VK_SPACE);
	r = engine->RegisterGlobalProperty("const int VK_PRIOR", (void*)&VK_PRIOR);
	r = engine->RegisterGlobalProperty("const int VK_NEXT", (void*)&VK_NEXT);
	r = engine->RegisterGlobalProperty("const int VK_END", (void*)&VK_END);
	r = engine->RegisterGlobalProperty("const int VK_HOME", (void*)&VK_HOME);
	r = engine->RegisterGlobalProperty("const int VK_LEFT", (void*)&VK_LEFT);
	r = engine->RegisterGlobalProperty("const int VK_UP", (void*)&VK_UP);
	r = engine->RegisterGlobalProperty("const int VK_RIGHT", (void*)&VK_RIGHT);
	r = engine->RegisterGlobalProperty("const int VK_DOWN", (void*)&VK_DOWN);
	r = engine->RegisterGlobalProperty("const int VK_SELECT", (void*)&VK_SELECT);
	r = engine->RegisterGlobalProperty("const int VK_PRINT", (void*)&VK_PRINT);
	r = engine->RegisterGlobalProperty("const int VK_EXECUTE", (void*)&VK_EXECUTE);
	r = engine->RegisterGlobalProperty("const int VK_SNAPSHOT", (void*)&VK_SNAPSHOT);
	r = engine->RegisterGlobalProperty("const int VK_INSERT", (void*)&VK_INSERT);
	r = engine->RegisterGlobalProperty("const int VK_DELETE", (void*)&VK_DELETE);
	r = engine->RegisterGlobalProperty("const int VK_HELP", (void*)&VK_HELP);
	r = engine->RegisterGlobalProperty("const int VK_LWIN", (void*)&VK_LWIN);
	r = engine->RegisterGlobalProperty("const int VK_RWIN", (void*)&VK_RWIN);
	r = engine->RegisterGlobalProperty("const int VK_APPS", (void*)&VK_APPS);
	r = engine->RegisterGlobalProperty("const int VK_NUMPAD0", (void*)&VK_NUMPAD0);
	r = engine->RegisterGlobalProperty("const int VK_NUMPAD1", (void*)&VK_NUMPAD1);
	r = engine->RegisterGlobalProperty("const int VK_NUMPAD2", (void*)&VK_NUMPAD2);
	r = engine->RegisterGlobalProperty("const int VK_NUMPAD3", (void*)&VK_NUMPAD3);
	r = engine->RegisterGlobalProperty("const int VK_NUMPAD4", (void*)&VK_NUMPAD4);
	r = engine->RegisterGlobalProperty("const int VK_NUMPAD5", (void*)&VK_NUMPAD5);
	r = engine->RegisterGlobalProperty("const int VK_NUMPAD6", (void*)&VK_NUMPAD6);
	r = engine->RegisterGlobalProperty("const int VK_NUMPAD7", (void*)&VK_NUMPAD7);
	r = engine->RegisterGlobalProperty("const int VK_NUMPAD8", (void*)&VK_NUMPAD8);
	r = engine->RegisterGlobalProperty("const int VK_NUMPAD9", (void*)&VK_NUMPAD9);
	r = engine->RegisterGlobalProperty("const int VK_MULTIPLY", (void*)&VK_MULTIPLY);
	r = engine->RegisterGlobalProperty("const int VK_ADD", (void*)&VK_ADD);
	r = engine->RegisterGlobalProperty("const int VK_SEPARATOR", (void*)&VK_SEPARATOR);
	r = engine->RegisterGlobalProperty("const int VK_SUBTRACT", (void*)&VK_SUBTRACT);
	r = engine->RegisterGlobalProperty("const int VK_DECIMAL", (void*)&VK_DECIMAL);
	r = engine->RegisterGlobalProperty("const int VK_DIVIDE", (void*)&VK_DIVIDE);
	r = engine->RegisterGlobalProperty("const int VK_F1", (void*)&VK_F1);
	r = engine->RegisterGlobalProperty("const int VK_F2", (void*)&VK_F2);
	r = engine->RegisterGlobalProperty("const int VK_F3", (void*)&VK_F3);
	r = engine->RegisterGlobalProperty("const int VK_F4", (void*)&VK_F4);
	r = engine->RegisterGlobalProperty("const int VK_F5", (void*)&VK_F5);
	r = engine->RegisterGlobalProperty("const int VK_F6", (void*)&VK_F6);
	r = engine->RegisterGlobalProperty("const int VK_F7", (void*)&VK_F7);
	r = engine->RegisterGlobalProperty("const int VK_F8", (void*)&VK_F8);
	r = engine->RegisterGlobalProperty("const int VK_F9", (void*)&VK_F9);
	r = engine->RegisterGlobalProperty("const int VK_F10", (void*)&VK_F10);
	r = engine->RegisterGlobalProperty("const int VK_F11", (void*)&VK_F11);
	r = engine->RegisterGlobalProperty("const int VK_F12", (void*)&VK_F12);
	r = engine->RegisterGlobalProperty("const int VK_NUMLOCK", (void*)&VK_NUMLOCK);
	r = engine->RegisterGlobalProperty("const int VK_SCROLL", (void*)&VK_SCROLL);
	r = engine->RegisterGlobalProperty("const int VK_LSHIFT", (void*)&VK_LSHIFT);
	r = engine->RegisterGlobalProperty("const int VK_RSHIFT", (void*)&VK_RSHIFT);
	r = engine->RegisterGlobalProperty("const int VK_LCONTROL", (void*)&VK_LCONTROL);
	r = engine->RegisterGlobalProperty("const int VK_RCONTROL", (void*)&VK_RCONTROL);
	r = engine->RegisterGlobalProperty("const int VK_LMENU", (void*)&VK_LMENU);
	r = engine->RegisterGlobalProperty("const int VK_RMENU", (void*)&VK_RMENU);
	r = engine->RegisterGlobalProperty("const int VK_OEM_1", (void*)&VK_OEM_1);
	r = engine->RegisterGlobalProperty("const int VK_OEM_PLUS", (void*)&VK_OEM_PLUS);
	r = engine->RegisterGlobalProperty("const int VK_OEM_COMMA", (void*)&VK_OEM_COMMA);
	r = engine->RegisterGlobalProperty("const int VK_OEM_MINUS", (void*)&VK_OEM_MINUS);
	r = engine->RegisterGlobalProperty("const int VK_OEM_PERIOD", (void*)&VK_OEM_PERIOD);
	r = engine->RegisterGlobalProperty("const int VK_OEM_2", (void*)&VK_OEM_2);
	r = engine->RegisterGlobalProperty("const int VK_OEM_3", (void*)&VK_OEM_3);
	r = engine->RegisterGlobalProperty("const int VK_OEM_4", (void*)&VK_OEM_4);
	r = engine->RegisterGlobalProperty("const int VK_OEM_5", (void*)&VK_OEM_5);
	r = engine->RegisterGlobalProperty("const int VK_OEM_6", (void*)&VK_OEM_6);
	r = engine->RegisterGlobalProperty("const int VK_OEM_7", (void*)&VK_OEM_7);
}
