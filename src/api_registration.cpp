// AUTO-GENERATED Perception.cx AngelScript API Registration
// Based on official documentation at https://docs.perception.cx/
// This file registers stubs for all Perception.cx APIs to enable validation

#include "../include/angelscript/include/angelscript.h"
#include <stdio.h>
#include <cassert>

typedef unsigned int uint;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

// Stub function for registration (actual implementation not needed for validation)
void StubFunction() {}

void RegisterCustomAPIs(asIScriptEngine* engine) {
	int r;

	// =====================================================
	// FUNCDEFS (must be registered before functions using them)
	// =====================================================

	r = engine->RegisterFuncdef("void __Internal_CallbackFn(int callback_id, int data_index)");
	r = engine->RegisterFuncdef("bool UcHookFn(uint64 uc, uint64 addr)");
	r = engine->RegisterFuncdef("void button_callback_t()");
	// =====================================================
	// OPAQUE HANDLE VALUE TYPES - must be registered FIRST before value types use them
	// =====================================================

	// proc_t - process handle (opaque value type, not reference)
	r = engine->RegisterObjectType("proc_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	// uproc_t - unprotected process handle (opaque value type)
	r = engine->RegisterObjectType("uproc_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	// uproc_t methods - same as proc_t
	r = engine->RegisterObjectMethod("uproc_t", "void deref()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("uproc_t", "uint64 base_address() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("uproc_t", "bool alive() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("uproc_t", "uint8 ru8(uint64) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// ws_t - websocket handle (opaque value type)
	r = engine->RegisterObjectType("ws_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	// mutex_t - mutex handle (opaque value type)
	r = engine->RegisterObjectType("mutex_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	// ZydisBuilder
	r = engine->RegisterObjectType("ZydisBuilder", 0, asOBJ_REF | asOBJ_NOCOUNT);
	// GUI types - opaque handle value types (owned by engine, like proc_t)
	r = engine->RegisterObjectType("subtab_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	r = engine->RegisterObjectType("panel_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	r = engine->RegisterObjectType("checkbox_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	r = engine->RegisterObjectType("slider_int_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	r = engine->RegisterObjectType("slider_double_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	r = engine->RegisterObjectType("keybind_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	r = engine->RegisterObjectType("color_picker_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	r = engine->RegisterObjectType("input_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	r = engine->RegisterObjectType("list_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	r = engine->RegisterObjectType("multi_select_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	r = engine->RegisterObjectType("single_select_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	r = engine->RegisterObjectType("button_t", sizeof(void*), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	// =====================================================
	// VALUE TYPES (POD types with members)
	// =====================================================

	// vector2 - 2D vector with x, y
	r = engine->RegisterObjectType("vector2", sizeof(double)*2, asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	r = engine->RegisterObjectProperty("vector2", "double x", 0);
	r = engine->RegisterObjectProperty("vector2", "double y", sizeof(double));
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
	r = engine->RegisterObjectProperty("vector3", "double x", 0);
	r = engine->RegisterObjectProperty("vector3", "double y", sizeof(double));
	r = engine->RegisterObjectProperty("vector3", "double z", sizeof(double)*2);
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
	r = engine->RegisterObjectMethod("quaternion", "bool writeas_float(uproc_t& in, uint64 addr) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// matrix4x4
	r = engine->RegisterObjectType("matrix4x4", sizeof(double)*16, asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	r = engine->RegisterObjectBehaviour("matrix4x4", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("matrix4x4", "matrix4x4 opMul(const matrix4x4 &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("matrix4x4", "vector3 transform(const vector3 &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("matrix4x4", "void read(proc_t &in, uint64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("matrix4x4", "void readas_float(proc_t &in, uint64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("matrix4x4", "void readas_double(proc_t &in, uint64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("matrix4x4", "bool writeas_float(proc_t &in, uint64) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("matrix4x4", "bool writeas_double(proc_t &in, uint64) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("matrix4x4", "double &opIndex(int)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	// atomic_int32
	r = engine->RegisterObjectType("atomic_int32", 4, asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
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
	r = engine->RegisterObjectBehaviour("atomic_int64", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectBehaviour("atomic_int64", asBEHAVE_CONSTRUCT, "void f(int64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectBehaviour("atomic_int64", asBEHAVE_CONSTRUCT, "void f(const atomic_int64 &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int64", "atomic_int64 &opAssign(const atomic_int64 &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int64", "int64 load() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int64", "void store(int64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int64", "int64 exchange(int64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int64", "bool compare_exchange(int64, int64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int64", "int64 add(int64 v)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int64", "int64 sub(int64 v)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int64", "int64 and_op(int64 v)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod ("atomic_int64", "int64 or_op(int64 v)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int64", "int64 xor_op(int64 v)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int64", "int64 increment()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("atomic_int64", "int64 decrement()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);


	// WindowInfo - value type for window information
	// Note: This is a simplified registration - actual implementation would need proper storage
	r = engine->RegisterObjectType("WindowInfo", sizeof(void*)*8, asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	// WindowInfo properties
	r = engine->RegisterObjectProperty("WindowInfo", "uint64 hwnd", 0);
	r = engine->RegisterObjectProperty("WindowInfo", "uint pid", sizeof(uint64_t));
	r = engine->RegisterObjectProperty("WindowInfo", "uint tid", sizeof(uint64_t) + sizeof(unsigned int));
	// String members would need special handling in actual implementation
	// For stub registration, we'll skip them since they complicate POD structure

	// __m128 - 128-bit SIMD type (4 floats)
	r = engine->RegisterObjectType("__m128", sizeof(float)*4, asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	r = engine->RegisterObjectBehaviour("__m128", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectBehaviour("__m128", asBEHAVE_CONSTRUCT, "void f(float, float, float, float)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// __m256 - 256-bit SIMD type (8 floats)
	r = engine->RegisterObjectType("__m256", sizeof(float)*8, asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	r = engine->RegisterObjectBehaviour("__m256", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectBehaviour("__m256", asBEHAVE_CONSTRUCT, "void f(float, float, float, float, float, float, float, float)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// ZydisEncoderRequest
	r = engine->RegisterObjectType("ZydisEncoderRequest", sizeof(void*)*16, asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	r = engine->RegisterObjectBehaviour("ZydisEncoderRequest", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ZydisEncoderRequest", "void set_mnemonic(int)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ZydisEncoderRequest", "void set_operand_count(int)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ZydisEncoderRequest", "void set_operand_reg(int, int)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ZydisEncoderRequest", "void set_operand_imm(int, int64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
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
	r = engine->RegisterObjectMethod("proc_t", "bool is_valid_address(uint64 address) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

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
	r = engine->RegisterObjectMethod("proc_t", "string rs (uint64 addr, int max_chars) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "string rws(uint64 addr, int max_chars) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "bool ws(uint64, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "bool wws(uint64, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// proc_t memory methods
	r = engine->RegisterObjectMethod("proc_t", "void rvm(uint64 addr, uint size, array<uint8> &out out_buf)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "bool wvm(uint64, const array<uint8> &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// proc_t module/pattern methods
	r = engine->RegisterObjectMethod("proc_t", "bool get_module(const string &in name, uint64 &out module_base, uint64 &out module_size)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "uint64 find_code_pattern(uint64 search_start, uint64 search_size, const string &in signature)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "void find_all_code_patterns(uint64 search_start, uint64 search_size, const string &in signature, array<uint64> &out result)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// proc_t advanced methods
	r = engine->RegisterObjectMethod("proc_t", "uint64 get_proc_address(uint64 module_base, const string &in export_name)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "array<uint64>@ get_all_tebs() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "array<dictionary@>@ cs2_get_schema_dump() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "uint64 cs2_get_interface(uint64 module_base, const string &in name) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// proc_t SIMD helpers
	r = engine->RegisterObjectMethod("proc_t", "void r128(uint64, array<uint8> &out) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "void r256(uint64, array<uint8> &out) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "void r512(uint64, array<uint8> &out) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "bool w128(uint64, const array<uint8> &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "bool w256(uint64, const array<uint8> &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "bool w512(uint64, const array<uint8> &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// proc_t struct helpers
	r = engine->RegisterObjectMethod("proc_t", "bool read_struct(uint64 addr, dictionary &out result, const dictionary &in desc)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "bool read_struct_array(uint64 base, uint count, uint size, array<dictionary>@ &out result, const dictionary &in desc)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// proc_t virtual memory functions
	r = engine->RegisterObjectMethod("proc_t", "uint64 alloc_vm(uint)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "bool free_vm(uint64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// proc_t import/export functions
	r = engine->RegisterObjectMethod("proc_t", "uint64 get_import_rdata_address(uint64 module_base, const string &in import_name)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// proc_t pointer array helper
	r = engine->RegisterObjectMethod("proc_t", "array<uint64>@ read_pointer_array(uint64 base, uint count, int offset_delta) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// proc_t virtual memory analysis
	r = engine->RegisterObjectMethod("proc_t", "bool virtual_query(uint64 address, uint64 &out region_start, uint64 &out region_size, uint &out protection, bool &out heap_likely) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "array<dictionary@>@ get_vad_snapshot(bool heap_likely_only = false) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// proc_t memory scan helpers
	r = engine->RegisterObjectMethod("proc_t", "array<uint64>@ scan_u32(uint value, bool heap_only = false) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "array<uint64>@ scan_u64(uint64 value, bool heap_only = false) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "array<uint64>@ scan_float(float value, bool heap_only = false) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "array<uint64>@ scan_double(double value, bool heap_only = false) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "array<uint64>@ scan_string(const string &in text, bool heap_only = false) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "array<uint64>@ scan_wstring(const string &in text, bool heap_only = false) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("proc_t", "array<uint64>@ scan_pointer(uint64 target, bool heap_only = false) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// ws_t methods
	r = engine->RegisterObjectMethod("ws_t", "bool send_text(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ws_t", "bool send_binary(const array<uint8> &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ws_t", "bool send_json(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ws_t", "bool is_open() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ws_t", "void close(uint16 code = 1000)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("ws_t", "bool recv(string &out msg, bool &out is_text)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
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
	r = engine->RegisterObjectMethod("ZydisBuilder", "void push_nop(int count = 1)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
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
	r = engine->RegisterObjectMethod("input_t", "void set(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("input_t", "void set_active(bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	// =====================================================
	// GUI TYPE METHODS
	// =====================================================

	// subtab_t methods
	r = engine->RegisterObjectMethod("subtab_t", "panel_t add_panel(const string &in, bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("subtab_t", "bool is_valid() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("subtab_t", "void set_active(bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("panel_t", "void set_active(bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("panel_t", "checkbox_t add_checkbox(const string &in name, bool initial, bool draw_title = true, bool find_protect = false, bool draw_just_label = false)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("panel_t", "slider_int_t add_slider_int(const string &in name, const string &in postfix, int value, int minv, int maxv, int step, bool draw_title = true, bool find_protect = false)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("panel_t", "slider_double_t add_slider_double(const string &in name, const string &in postfix, double value, double minv, double maxv, double step, bool draw_title = true, bool find_protect = false)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("panel_t", "keybind_t add_keybind(const string &in name, int key, const string &in mode, bool draw_title = true, bool find_protect = false)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("panel_t", "color_picker_t add_color(const string &in name, const array<float> &in rgba, bool find_protect = false)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("panel_t", "input_t add_input(const string &in name, const string &in initial, bool draw_title = true, bool find_protect = false)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("panel_t", "list_t add_list(const string &in name, const array<dictionary@> &in members, bool draw_title = true, bool find_protect = false)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("panel_t", "multi_select_t add_multi_select(const string &in name, const array<dictionary@> &in options, bool is_expandable, bool draw_title = true, bool find_protect = false)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("panel_t", "single_select_t add_single_select(const string &in name, const array<string> &in options, int initial_index, bool is_expandable, bool draw_title = true, bool find_protect = false)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("panel_t", "button_t add_button(const string &in name, button_callback_t@ cb, bool find_protect = false)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("checkbox_t", "bool get() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("checkbox_t", "void set(bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("checkbox_t", "void set_active(bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("slider_int_t", "int get() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("slider_int_t", "void set(int)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("slider_int_t", "void set_active(bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("slider_double_t", "double get() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("slider_double_t", "void set(double)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("slider_double_t", "void set_active(bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("keybind_t", "void get(int &out, string &out) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("keybind_t", "void set(int, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("keybind_t", "void set_active(bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("keybind_t", "bool is_pressed() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("color_picker_t", "void get(array<float> &out) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("color_picker_t", "void set(const array<float> &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("color_picker_t", "void set_active(bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("list_t", "int get() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("list_t", "int get_count() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("list_t", "void clear()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("list_t", "void append(const string &in, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("list_t", "void remove(int) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("list_t", "void highlight(int) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("list_t", "void remove_highlight(int) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("list_t", "void hide(int) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("list_t", "void show(int) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("list_t", "void set_active(bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("multi_select_t", "void get(array<bool> &out) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("multi_select_t", "void set(int, bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("multi_select_t", "void set_active(bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("single_select_t", "int get() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("single_select_t", "void set(int)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("single_select_t", "void set_active(bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("button_t", "void set_active(bool)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);

	// =====================================================
	// STRING METHODS
	// =====================================================

	r = engine->RegisterObjectMethod("string", "string toUpper() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJFIRST);
	r = engine->RegisterObjectMethod("string", "string toLower() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJFIRST);
	r = engine->RegisterObjectMethod("string", "array<string>@ split(const string &in) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJFIRST);
	r = engine->RegisterObjectMethod("string", "string trim() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJFIRST);
	// =====================================================
	// DICTIONARY METHODS
	// =====================================================

	r = engine->RegisterObjectMethod("dictionary", "array<string>@ getValues() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJFIRST);
	// Note: exists, delete, clear are already registered by AngelScript's dictionary addon

	// =====================================================
	// HASH_MAP TYPE
	// =====================================================

	// hash_map is a reference type (factory constructor returns handle)
	r = engine->RegisterObjectType("hash_map", 0, asOBJ_REF | asOBJ_NOCOUNT);
	// Factory constructor
	r = engine->RegisterObjectBehaviour("hash_map", asBEHAVE_FACTORY, "hash_map@ f()", asFUNCTION(StubFunction), asCALL_CDECL);
	// Core methods
	r = engine->RegisterObjectMethod("hash_map", "void set(uint64, ?&in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("hash_map", "bool get(uint64, ?&out)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("hash_map", "bool contains(uint64 key) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("hash_map", "bool erase(uint64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("hash_map", "void clear()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("hash_map", "uint size() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("hash_map", "bool empty() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	// Iteration methods
	r = engine->RegisterObjectMethod("hash_map", "void iter_begin()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("hash_map", "bool iter_next_key(uint64 &out)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("hash_map", "bool iter_next(uint64 &out, ?&out)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	// =====================================================
	// HASH_SET TYPE
	// =====================================================

	// hash_set is a value type (not a reference type)
	r = engine->RegisterObjectType("hash_set", sizeof(void*), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);
	// Constructor
	r = engine->RegisterObjectBehaviour("hash_set", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	// Destructor
	r = engine->RegisterObjectBehaviour("hash_set", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	// Core methods
	r = engine->RegisterObjectMethod("hash_set", "bool contains(uint64) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("hash_set", "bool insert(uint64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("hash_set", "bool erase(uint64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("hash_set", "void clear()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("hash_set", "uint size() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("hash_set", "bool empty() const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	// Convenience methods
	r = engine->RegisterObjectMethod("hash_set", "void set(uint64)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("hash_set", "bool get(uint64 v, uint64 &out value) const", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	// Iteration methods
	r = engine->RegisterObjectMethod("hash_set", "void iter_begin()", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	r = engine->RegisterObjectMethod("hash_set", "bool iter_next(uint64 &out)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	// Assignment operator
	r = engine->RegisterObjectMethod("hash_set", "hash_set &opAssign(const hash_set &in)", asFUNCTION(StubFunction), asCALL_CDECL_OBJLAST);
	// =====================================================
	// STRING GLOBAL FUNCTIONS
	// =====================================================

	r = engine->RegisterGlobalFunction("string join(const array<string> &in, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	// Note: formatFloat and formatInt are already registered by AngelScript addon

	// =====================================================
	// UTILITY FUNCTIONS (Base64, Hex, URL encoding)
	// =====================================================

	// Base64 functions
	r = engine->RegisterGlobalFunction("string util_base64_encode(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool util_base64_decode(const string &in, string &out, string &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	// Hex functions
	r = engine->RegisterGlobalFunction("string util_hex_encode(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool util_hex_decode(const string &in, string &out, string &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	// URL encoding functions
	r = engine->RegisterGlobalFunction("string util_url_encode(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("string util_url_decode(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	// =====================================================
	// GLOBAL FUNCTIONS
	// =====================================================

	// Process API
	r = engine->RegisterGlobalFunction("proc_t ref_process(uint)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("proc_t ref_process(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uproc_t ref_unprotected_process(uint)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uproc_t ref_unprotected_process(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	// Callback API
	r = engine->RegisterGlobalFunction("int register_callback(const __Internal_CallbackFn@ fn, int every_ms, int data_index, bool render_on_top = false)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void unregister_callback(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	// Mutex API
	r = engine->RegisterGlobalFunction("mutex_t create_mutex()", asFUNCTION(StubFunction), asCALL_CDECL);
	// Net API
	r = engine->RegisterGlobalFunction("ws_t ws_connect(const string &in url, uint timeout_ms = 0)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool net_http_get(const string &in, uint &out, string &out, uint = 0)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool net_http_post(const string &in, const string &in, const string &in, uint &out, string &out, uint = 0)", asFUNCTION(StubFunction), asCALL_CDECL);
	// Render API - Viewport
	r = engine->RegisterGlobalFunction("void get_view(float &out, float &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("float get_view_scale()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double get_fps()", asFUNCTION(StubFunction), asCALL_CDECL);

	// Render API - Shapes
	r = engine->RegisterGlobalFunction("void draw_rect(float, float, float, float, uint8, uint8, uint8, uint8, float, float, uint8)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void draw_rect_filled(float, float, float, float, uint8, uint8, uint8, uint8, float, uint8)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void draw_line(float, float, float, float, uint8, uint8, uint8, uint8, float)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void draw_circle(float, float, float, uint8, uint8, uint8, uint8, float, bool)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void draw_triangle(float, float, float, float, float, float, uint8, uint8, uint8, uint8, float, bool)", asFUNCTION(StubFunction), asCALL_CDECL);

	// Render API - Additional drawing functions
	r = engine->RegisterGlobalFunction("void draw_arc(float, float, float, float, float, float, uint8, uint8, uint8, uint8, float, bool)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void draw_polygon(const array<float> &in, uint, uint8, uint8, uint8, uint8, float, bool)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void draw_four_corner_gradient(float, float, float, float, uint8, uint8, uint8, uint8, uint8, uint8, uint8, uint8, uint8, uint8, uint8, uint8, uint8, uint8, uint8, uint8, float)", asFUNCTION(StubFunction), asCALL_CDECL);

	// Render API - Fonts and Text
	r = engine->RegisterGlobalFunction("uint64 create_font(const string &in path, float size, bool antialias, bool load_color, array<uint> @glyph_ranges = null)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint64 create_font_mem(const string &in label, float size, const array<uint8> &in buf, bool antialias, bool load_color, array<uint> @glyph_ranges = null)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint64 get_font18()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint64 get_font20()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint64 get_font24()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint64 get_font28()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void draw_text(const string &in, float, float, uint8, uint8, uint8, uint8, uint64, int, uint8, uint8, uint8, uint8, float)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void get_text_size(uint64, const string &in, int, int, float &out, float &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("int get_char_advance(uint64, uint)", asFUNCTION(StubFunction), asCALL_CDECL);
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
	r = engine->RegisterGlobalFunction("void custom_draw(uint64 shader, uint64 vb, const array<uint8> &in vertex_data, uint vertex_count, int topology, uint64 blend, uint64 sampler, uint64 texture, int tex_slot, uint64 cb, const array<uint8> @cb_data, int cb_slot)", asFUNCTION(StubFunction), asCALL_CDECL);
	// -- Render Target Control
	r = engine->RegisterGlobalFunction("void custom_set_render_target(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void custom_reset_render_target()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void custom_bind_rt_as_texture(uint64, int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void custom_restore_state()", asFUNCTION(StubFunction), asCALL_CDECL);

	// -- Index Buffers
	r = engine->RegisterGlobalFunction("uint64 create_index_buffer(uint, bool, bool)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void destroy_index_buffer(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void custom_draw_indexed(uint64 shader, uint64 vb, const array<uint8> &in vertex_data, uint vertex_count, uint64 ib, const array<uint8> &in index_data, uint index_count, int topology, uint64 blend, uint64 sampler, uint64 texture, int tex_slot, uint64 cb, const array<uint8> @cb_data, int cb_slot)", asFUNCTION(StubFunction), asCALL_CDECL);

	// -- Depth / Stencil
	r = engine->RegisterGlobalFunction("uint64 create_depth_stencil_state(bool, bool, int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void destroy_depth_stencil_state(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void custom_set_depth_stencil_state(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint64 create_depth_buffer(uint, uint)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void destroy_depth_buffer(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void custom_set_render_target_ext(uint64, uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void custom_clear_render_target(uint64, float, float, float, float)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void custom_clear_depth_buffer(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);

	// -- Rasterizer State
	r = engine->RegisterGlobalFunction("uint64 create_rasterizer_state(int, int, bool)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void destroy_rasterizer_state(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void custom_set_rasterizer_state(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);

	// -- Viewport
	r = engine->RegisterGlobalFunction("void custom_set_viewport(float, float, float, float)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void custom_reset_viewport()", asFUNCTION(StubFunction), asCALL_CDECL);

	// -- Multi-Texture Binding
	r = engine->RegisterGlobalFunction("void custom_bind_texture(uint64, uint64, int)", asFUNCTION(StubFunction), asCALL_CDECL);

	// -- Compute Shaders
	r = engine->RegisterGlobalFunction("uint64 create_compute_shader(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void destroy_compute_shader(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void dispatch_compute(uint64, uint, uint, uint)", asFUNCTION(StubFunction), asCALL_CDECL);

	// -- Structured Buffers
	r = engine->RegisterGlobalFunction("uint64 create_structured_buffer(uint, uint, bool, bool)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void destroy_structured_buffer(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void update_structured_buffer(uint64, const array<uint8> &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void bind_structured_buffer(uint64, int, int)", asFUNCTION(StubFunction), asCALL_CDECL);

	// -- Backbuffer Capture
	r = engine->RegisterGlobalFunction("void capture_backbuffer(int)", asFUNCTION(StubFunction), asCALL_CDECL);

	// -- Texture Loading
	r = engine->RegisterGlobalFunction("uint64 load_texture(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint64 load_texture_mem(const array<uint8> &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void get_texture_info(uint64, float &out, float &out)", asFUNCTION(StubFunction), asCALL_CDECL);

	// -- Mesh Loading
	r = engine->RegisterGlobalFunction("uint64 load_mesh(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint64 load_mesh_mem(const array<uint8> &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void get_mesh_info(uint64, float &out, float &out, float &out, float &out, float &out, float &out, float &out, float &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void destroy_mesh(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void draw_mesh(uint64 mesh, uint64 shader, int topology, uint64 blend, uint64 sampler, uint64 texture, int tex_slot, uint64 cb, const array<uint8> @cb_data, int cb_slot)", asFUNCTION(StubFunction), asCALL_CDECL);

	// -- Procedural Mesh
	r = engine->RegisterGlobalFunction("uint64 create_mesh_raw(const array<uint8> &in, uint, uint, const array<uint8> &in, uint, bool)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("float get_mesh_stride(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);

	// -- Multi Constant Buffer Binding
	r = engine->RegisterGlobalFunction("void custom_bind_constant_buffer(uint64 cb, const array<uint8> &in data, int slot, int stage)", asFUNCTION(StubFunction), asCALL_CDECL);

	// -- Dynamic Texture Updates
	r = engine->RegisterGlobalFunction("void custom_update_texture(uint64, uint, uint, uint, uint, const array<uint8> &in)", asFUNCTION(StubFunction), asCALL_CDECL);

	// Extended Math API - Global functions
	r = engine->RegisterGlobalFunction("double clamp(double x, double a, double b)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double saturate(double x)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("int sign(double x)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double round(double x)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double round_up(double x)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double round_down(double x)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double fract(double x)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double lerp(double a, double b, double t)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double wrap(double x, double min, double max)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double inverse_lerp(double a, double b, double v)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double remap(double a1, double b1, double a2, double b2, double v)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double step(double edge, double x)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double smoothstep(double edge0, double edge1, double x)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool is_nan(double x)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool is_inf(double x)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double random()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void random_seed(uint64 x)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double random_range(double a, double b)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("int64 random_int(int64 a, int64 b)", asFUNCTION(StubFunction), asCALL_CDECL);

	// Extended Math API - Random functions
	r = engine->RegisterGlobalFunction("bool random_bool()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double random_gaussian(double a, double b)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("vector2 random_unit_vec2()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("vector3 random_unit_vec3()", asFUNCTION(StubFunction), asCALL_CDECL);

	r = engine->RegisterGlobalFunction("quaternion quat_from_euler(double, double, double)", asFUNCTION(StubFunction), asCALL_CDECL);
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
	// Logging
	r = engine->RegisterGlobalFunction("void log(const string &in message)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void log_error(const string &in message)", asFUNCTION(StubFunction), asCALL_CDECL);

	r = engine->RegisterGlobalFunction("void log_console(const string &in message)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void log_console_error(const string &in message)", asFUNCTION(StubFunction), asCALL_CDECL);
	// =====================================================
	// SYSTEM API (CPU info, timing, datetime)
	// =====================================================

	// CPU Information
	r = engine->RegisterGlobalFunction("string cpu_vendor()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("string cpu_brand()", asFUNCTION(StubFunction), asCALL_CDECL);
	// Timing functions
	r = engine->RegisterGlobalFunction("uint64 rdtsc()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("int64 perf_time()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("int64 perf_frequency()", asFUNCTION(StubFunction), asCALL_CDECL);
	// DateTime functions
	r = engine->RegisterGlobalFunction("dictionary@ get_datetime()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint64 get_timestamp()", asFUNCTION(StubFunction), asCALL_CDECL);
	// Thread priority functions
	r = engine->RegisterGlobalFunction("bool set_thread_to_highest_priority()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool set_thread_to_lowest_priority()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool set_thread_to_normal_priority()", asFUNCTION(StubFunction), asCALL_CDECL);
	// Zydis disassembly functions
	r = engine->RegisterGlobalFunction("void zydis_disasm(const array<uint8>& in, array<dictionary@>& out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void zydis_disasm(const array<uint8>& in, uint64, array<dictionary@>& out)", asFUNCTION(StubFunction), asCALL_CDECL);

	// =====================================================
	// INPUT API
	// =====================================================

	// Mouse functions
	r = engine->RegisterGlobalFunction("void get_mouse_pos(float &out, float &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void get_mouse_pos_desktop(float &out, float &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void get_mouse_delta(float &out, float &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void get_mouse_delta_desktop(float &out, float &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("float get_scroll_delta()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool mouse_movement_received()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool is_hovered(float, float, float, float)", asFUNCTION(StubFunction), asCALL_CDECL);
	// Keyboard functions
	r = engine->RegisterGlobalFunction("bool key_down(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool key_raw_down(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool key_fired(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool key_toggle(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool key_singlepress(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool key_prev_down(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void get_key_state(int, bool &out, bool &out, bool &out, bool &out, bool &out, bool &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("string get_key_name(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("string get_recent_key_input()", asFUNCTION(StubFunction), asCALL_CDECL);
	// =====================================================
	// WIN API
	// =====================================================
	// Note: Most Window API functions are registered below in the original code section
	// with uint64 handles. Adding is_foreground_window and clipboard functions here
	// since they're not in the original.

	r = engine->RegisterGlobalFunction("bool is_foreground_window(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	// Clipboard functions
	r = engine->RegisterGlobalFunction("bool copy_to_clipboard(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool copy_from_clipboard(string &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	// Util function
	r = engine->RegisterGlobalFunction("int64 get_tickcount64()", asFUNCTION(StubFunction), asCALL_CDECL);
	// Engine API
	r = engine->RegisterGlobalFunction("string get_username()", asFUNCTION(StubFunction), asCALL_CDECL);
	// Game-specific functions
	r = engine->RegisterGlobalFunction("string fortnite_get_player_name(proc_t &in, uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("vector3 rust_get_transform_position(proc_t &in, uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void set_bf6_globals(uint64, uint64, uint64, uint64, uint64, uint64)", asFUNCTION(StubFunction), asCALL_CDECL);

	// GUI API
	r = engine->RegisterGlobalFunction("void get_gui_position(float &out, float &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void get_gui_size(float &out, float &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool gui_active()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("subtab_t create_subtab(int, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	// GUI find_* functions
	r = engine->RegisterGlobalFunction("checkbox_t find_checkbox(int, const string &in, const string &in, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("slider_double_t find_slider_double(int, const string &in, const string &in, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("slider_int_t find_slider_int(int, const string &in, const string &in, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("input_t find_input(int, const string &in, const string &in, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("multi_select_t find_multi_select(int, const string &in, const string &in, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	if (r < 0) printf("WARNING:Failed to register find_multi_select (code: %d)\n", r);

	r = engine->RegisterGlobalFunction("single_select_t find_single_select(int, const string &in, const string &in, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("keybind_t find_keybind(int, const string &in, const string &in, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("button_t find_button(int, const string &in, const string &in, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("color_picker_t find_color(int, const string &in, const string &in, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("list_t find_list(int, const string &in, const string &in, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	// GUI config helpers
	r = engine->RegisterGlobalFunction("string construct_config()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void apply_config(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	// Zydis API - use factory function name that doesn't conflict with type
	r = engine->RegisterGlobalFunction("ZydisBuilder@ ZydisBuilder()", asFUNCTION(StubFunction), asCALL_CDECL);
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
	r = engine->RegisterGlobalFunction("uint64 query_performance_counter()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void sleep(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint cpuid(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint64 xgetbv(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	// Unicorn API
	r = engine->RegisterGlobalFunction("uint64 uc_create()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint64 uc_create_process(proc_t proc, bool allow_writes = true)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool uc_mem_map(uint64 handle, uint64 addr, uint64 size, uint32 perms)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool uc_mem_write(uint64, uint64, const array<uint8> &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool uc_mem_read(uint64 handle, uint64 addr, uint32 size, array<uint8> &out data)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool uc_reg_write64(uint64 handle, int reg, uint64 value)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint64 uc_reg_read64(uint64, int)", asFUNCTION(StubFunction), asCALL_CDECL);
	// Unicorn 128-bit and 256-bit register functions
	r = engine->RegisterGlobalFunction("bool uc_reg_write128(uint64, int, const array<uint8> &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool uc_reg_read128(uint64, int, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool uc_reg_write256(uint64, int, const array<uint8> &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool uc_reg_read256(uint64, int, array<uint8> &out)", asFUNCTION(StubFunction), asCALL_CDECL);

	r = engine->RegisterGlobalFunction("bool uc_setup_stack(uint64, uint64, uint64, uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("int uc_start(uint64 handle, uint64 begin, uint64 end, uint64 timeout = 0, uint64 count = 0)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void uc_close(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool uc_flush_code(uint64 handle)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool uc_hook_add(uint64, int, UcHookFn@)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void uc_emu_stop(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void uc_del_hook(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("int uc_get_last_exception(uint64 handle)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint64 uc_get_exception_address(uint64 handle)", asFUNCTION(StubFunction), asCALL_CDECL);
	// Unicorn constants
	static const uint UC_PROT_NONE = 0;
	static const uint32 UC_PROT_READ = 1;
	static const uint32 UC_PROT_WRITE = 2;
	static const uint32 UC_PROT_EXEC = 4;
	static const uint32 UC_PROT_ALL = 7;

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

	static const int UC_X86_REG_CS = 36;
	static const int UC_X86_REG_DS = 37;
	static const int UC_X86_REG_ES = 38;
	static const int UC_X86_REG_FS = 39;
	static const int UC_X86_REG_GS = 40;
	static const int UC_X86_REG_SS = 41;
	static const int UC_X86_REG_MXCSR = 42;
	static const int UC_X86_REG_YMM0 = 43;
	static const int UC_X86_REG_YMM1 = 44;
	static const int UC_X86_REG_YMM2 = 45;
	static const int UC_X86_REG_YMM3 = 46;
	static const int UC_X86_REG_YMM4 = 47;
	static const int UC_X86_REG_YMM5 = 48;
	static const int UC_X86_REG_YMM6 = 49;
	static const int UC_X86_REG_YMM7 = 50;
	static const int UC_X86_REG_YMM8 = 51;
	static const int UC_X86_REG_YMM9 = 52;
	static const int UC_X86_REG_YMM10 = 53;
	static const int UC_X86_REG_YMM11 = 54;
	static const int UC_X86_REG_YMM12 = 55;
	static const int UC_X86_REG_YMM13 = 56;
	static const int UC_X86_REG_YMM14 = 57;
	static const int UC_X86_REG_YMM15 = 58;

	r = engine->RegisterGlobalProperty("const uint UC_PROT_NONE", (void*)&UC_PROT_NONE);
	r = engine->RegisterGlobalProperty("const uint32 UC_PROT_READ", (void*)&UC_PROT_READ);
	r = engine->RegisterGlobalProperty("const uint32 UC_PROT_WRITE", (void*)&UC_PROT_WRITE);
	r = engine->RegisterGlobalProperty("const uint32 UC_PROT_EXEC", (void*)&UC_PROT_EXEC);
	r = engine->RegisterGlobalProperty("const uint32 UC_PROT_ALL", (void*)&UC_PROT_ALL);

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

	r = engine->RegisterGlobalProperty("const int UC_X86_REG_CS;", (void*)&UC_X86_REG_CS);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_DS;", (void*)&UC_X86_REG_DS);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_ES;", (void*)&UC_X86_REG_ES);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_FS;", (void*)&UC_X86_REG_FS);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_GS;", (void*)&UC_X86_REG_GS);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_SS;", (void*)&UC_X86_REG_SS);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_MXCSR;", (void*)&UC_X86_REG_MXCSR);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_YMM0;", (void*)&UC_X86_REG_YMM0);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_YMM1;", (void*)&UC_X86_REG_YMM1);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_YMM2;", (void*)&UC_X86_REG_YMM2);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_YMM3;", (void*)&UC_X86_REG_YMM3);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_YMM4;", (void*)&UC_X86_REG_YMM4);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_YMM5;", (void*)&UC_X86_REG_YMM5);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_YMM6;", (void*)&UC_X86_REG_YMM6);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_YMM7;", (void*)&UC_X86_REG_YMM7);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_YMM8;", (void*)&UC_X86_REG_YMM8);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_YMM9;", (void*)&UC_X86_REG_YMM9);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_YMM10;", (void*)&UC_X86_REG_YMM10);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_YMM11;", (void*)&UC_X86_REG_YMM11);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_YMM12;", (void*)&UC_X86_REG_YMM12);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_YMM13;", (void*)&UC_X86_REG_YMM13);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_YMM14;", (void*)&UC_X86_REG_YMM14);
	r = engine->RegisterGlobalProperty("const int UC_X86_REG_YMM15;", (void*)&UC_X86_REG_YMM15);

	// Window operations
	r = engine->RegisterGlobalFunction("uint64 find_window(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);	r = engine->RegisterGlobalFunction("uint64 find_window(const string &in title, const string &in className)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint64 find_window_ex(const string &in, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool get_window_title(uint64, string &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool get_window_class(uint64, string &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool get_window_rect(uint64, int &out, int &out, int &out, int &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool get_client_rect(uint64, int &out, int &out, int &out, int &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool get_window_size(uint64, int &out, int &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool is_window_active(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);

	r = engine->RegisterGlobalFunction("bool get_window_thread_process_id(uint64, uint &out, uint &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool set_foreground_window(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	// Mouse operations
	r = engine->RegisterGlobalFunction("void mouse_move(int, int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mouse_move_relative(int, int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mouse_left_click()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mouse_right_click()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mouse_middle_click()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void mouse_scroll(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void send_mouse_input(int64, int64, uint, uint)", asFUNCTION(StubFunction), asCALL_CDECL);

	// Key operations
	r = engine->RegisterGlobalFunction("bool send_char(uint64 hwnd, const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool send_key(uint64 hwnd, uint vk)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void win_key_down(uint vk)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void win_key_up(uint vk)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void win_key_press(uint vk, uint delay_ms = 30)", asFUNCTION(StubFunction), asCALL_CDECL);
	// Message operations
	r = engine->RegisterGlobalFunction("bool post_message(uint64, uint, uint64, uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint64 send_message(uint64, uint, uint64, uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
	// Input API
	r = engine->RegisterGlobalFunction("bool is_key_down(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool is_key_pressed(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool is_key_released(int)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void get_keys_down(array<int> &out)", asFUNCTION(StubFunction), asCALL_CDECL);
	// File System API
	r = engine->RegisterGlobalFunction("bool file_exists(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool does_file_exist(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool create_directory(const string &in)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool create_file(const string &in path, const string &in data = \"\")", asFUNCTION(StubFunction), asCALL_CDECL);
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
	r = engine->RegisterGlobalFunction("float bits_to_float(uint)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint64 double_to_bits(double)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("double bits_to_double(uint64)", asFUNCTION(StubFunction), asCALL_CDECL);
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
	r = engine->RegisterGlobalFunction("bool json_parse(const string &in text, dictionary &out result, string &out error)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool json_stringify(const dictionary &in value, string &out json, string &out error)", asFUNCTION(StubFunction), asCALL_CDECL);
	// JSON API alternative function names (aliases)
	r = engine->RegisterGlobalFunction("bool json_decode(const string &in text, dictionary &out result, string &out error)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool json_encode(const dictionary &in value, string &out json, string &out error)", asFUNCTION(StubFunction), asCALL_CDECL);

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
	// Unreal Engine specific functions
	// =====================================================

	r = engine->RegisterGlobalFunction("bool unreal_world_to_screen(const vector3 &in world_pos, const vector3 &in cam_location, const vector3 &in cam_rotation, double fov_deg, vector2 &out screen_pos)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool unreal_read_tarray(proc_t &in proc, uint64 tarray_addr, array<uint64> &out result, uint max_count = 4096)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool unreal_read_minimal_view_info(proc_t &in proc, uint64 pov_addr, vector3 &out location, vector3 &out rotation, double &out fov)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool unreal_read_minimal_view_info_f64(proc_t &in proc, uint64 pov_addr, vector3 &out location, vector3 &out rotation, double &out fov)", asFUNCTION(StubFunction), asCALL_CDECL);

	// =====================================================
	// Sound API
	// =====================================================

	r = engine->RegisterGlobalFunction("uint64 load_sound(const string &in path)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void free_sound(uint64 handle)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("uint64 play_sound(uint64 sound, float volume = 1.0, float pan = 0.0, bool loop = false)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void stop_sound(uint64 instance)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void stop_all_sounds()", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("bool is_sound_playing(uint64 instance)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void set_sound_volume(uint64 instance, float volume)", asFUNCTION(StubFunction), asCALL_CDECL);
	r = engine->RegisterGlobalFunction("void set_sound_pan(uint64 instance, float pan)", asFUNCTION(StubFunction), asCALL_CDECL);

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

	// Direct render API constants - Compare Functions (depth/stencil)
	static const int CMP_NEVER         = 0;
	static const int CMP_LESS          = 1;
	static const int CMP_EQUAL         = 2;
	static const int CMP_LESS_EQUAL    = 3;
	static const int CMP_GREATER       = 4;
	static const int CMP_NOT_EQUAL     = 5;
	static const int CMP_GREATER_EQUAL = 6;
	static const int CMP_ALWAYS        = 7;

	r = engine->RegisterGlobalProperty("const int CMP_NEVER",         (void*)&CMP_NEVER);
	r = engine->RegisterGlobalProperty("const int CMP_LESS",          (void*)&CMP_LESS);
	r = engine->RegisterGlobalProperty("const int CMP_EQUAL",         (void*)&CMP_EQUAL);
	r = engine->RegisterGlobalProperty("const int CMP_LESS_EQUAL",    (void*)&CMP_LESS_EQUAL);
	r = engine->RegisterGlobalProperty("const int CMP_GREATER",       (void*)&CMP_GREATER);
	r = engine->RegisterGlobalProperty("const int CMP_NOT_EQUAL",     (void*)&CMP_NOT_EQUAL);
	r = engine->RegisterGlobalProperty("const int CMP_GREATER_EQUAL", (void*)&CMP_GREATER_EQUAL);
	r = engine->RegisterGlobalProperty("const int CMP_ALWAYS",        (void*)&CMP_ALWAYS);

	// Direct render API constants - Cull Modes
	static const int CULL_NONE  = 0;
	static const int CULL_FRONT = 1;
	static const int CULL_BACK  = 2;

	r = engine->RegisterGlobalProperty("const int CULL_NONE",  (void*)&CULL_NONE);
	r = engine->RegisterGlobalProperty("const int CULL_FRONT", (void*)&CULL_FRONT);
	r = engine->RegisterGlobalProperty("const int CULL_BACK",  (void*)&CULL_BACK);

	// Direct render API constants - Fill Modes
	static const int FILL_SOLID     = 0;
	static const int FILL_WIREFRAME = 1;

	r = engine->RegisterGlobalProperty("const int FILL_SOLID",     (void*)&FILL_SOLID);
	r = engine->RegisterGlobalProperty("const int FILL_WIREFRAME", (void*)&FILL_WIREFRAME);

	// Direct render API constants - Shader Stages
	static const int STAGE_VS = 0;
	static const int STAGE_PS = 1;
	static const int STAGE_CS = 2;

	r = engine->RegisterGlobalProperty("const int STAGE_VS", (void*)&STAGE_VS);
	r = engine->RegisterGlobalProperty("const int STAGE_PS", (void*)&STAGE_PS);
	r = engine->RegisterGlobalProperty("const int STAGE_CS", (void*)&STAGE_CS);

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
