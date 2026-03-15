#include "as_wrapper.h"
#include "../include/angelscript/include/angelscript.h"
#include "../include/add_on/scriptstdstring/scriptstdstring.h"
#include "../include/add_on/scriptarray/scriptarray.h"
#include "../include/add_on/scriptdictionary/scriptdictionary.h"
#include "../include/add_on/scriptmath/scriptmath.h"
#include "../include/add_on/scriptany/scriptany.h"
#include "../include/add_on/scriptgrid/scriptgrid.h"
#include "../include/add_on/scripthelper/scripthelper.h"
#include <cstring>
#include <string>
#include <vector>

// Forward declaration for API registration
void RegisterCustomAPIs(asIScriptEngine* engine);

struct ValidatorContext {
	asIScriptEngine* engine;
	asIScriptModule* module;
	std::vector<std::string> error_messages;
};

void MessageCallback(const asSMessageInfo* msg, void* param) {
	ValidatorContext* ctx = static_cast<ValidatorContext*>(param);

	const char* type = "ERR ";
	if (msg->type == asMSGTYPE_WARNING)
		type = "WARN";
	else if (msg->type == asMSGTYPE_INFORMATION)
		type = "INFO";

	char buffer[4096];
	snprintf(buffer, sizeof(buffer), "%s (%d, %d) : %s : %s",
			 msg->section, msg->row, msg->col, type, msg->message);

	ctx->error_messages.push_back(buffer);
}

extern "C" {

void* as_validator_create(void) {
	ValidatorContext* ctx = new ValidatorContext();
	ctx->engine = asCreateScriptEngine();
	ctx->engine->SetEngineProperty(asEP_INIT_GLOBAL_VARS_AFTER_BUILD, false);

	if (!ctx->engine) {
		delete ctx;
		return nullptr;
	}

	// Set message callback
	ctx->engine->SetMessageCallback(asFUNCTION(MessageCallback), ctx, asCALL_CDECL);

	// Register standard add-ons
	RegisterStdString(ctx->engine);
	RegisterScriptArray(ctx->engine, true);
	RegisterScriptDictionary(ctx->engine);
	RegisterScriptMath(ctx->engine);
	RegisterScriptAny(ctx->engine);
	RegisterScriptGrid(ctx->engine);
	RegisterExceptionRoutines(ctx->engine);

	// Register all custom APIs
	RegisterCustomAPIs(ctx->engine);

	// Create module
	ctx->module = ctx->engine->GetModule("validator", asGM_ALWAYS_CREATE);

	if (!ctx->module) {
		ctx->engine->ShutDownAndRelease();
		delete ctx;
		return nullptr;
	}

	return ctx;
}

int as_validate_script(void* validator, const char* script,
					   const char* filename, ASErrorList* errors) {
	ValidatorContext* ctx = static_cast<ValidatorContext*>(validator);
	ctx->error_messages.clear();

	// Create fresh module
	ctx->module = ctx->engine->GetModule("validator", asGM_ALWAYS_CREATE);

	int r = ctx->module->AddScriptSection(filename, script);
	if (r < 0) {
		return -1;
	}

	r = ctx->module->Build();

	// Copy errors to C structure
	if (errors && !ctx->error_messages.empty()) {
		for (const auto& msg : ctx->error_messages) {
			if (errors->error_count >= errors->capacity) {
				// Resize
				errors->capacity = errors->capacity == 0 ? 16 : errors->capacity * 2;
				errors->errors = (ASError*)realloc(errors->errors,
												   errors->capacity * sizeof(ASError));
			}

			// Parse the error message to extract components
			// Format: "section (row, col) : TYPE : message"
			char section[256];
			int row, col;
			char type[16];
			char message[4096];
			
			if (sscanf(msg.c_str(), "%255s (%d,%d) : %5s : %[^\n]",
					   section, &row, &col, type, message) == 5) {
				strncpy(errors->errors[errors->error_count].message,
					   message, sizeof(errors->errors[0].message) - 1);
				errors->errors[errors->error_count].message[sizeof(errors->errors[0].message) - 1] = '\0';
				errors->errors[errors->error_count].line = row;
				errors->errors[errors->error_count].column = col;
				// Store the section name (file identifier)
				size_t len = strlen(section) + 1;
				errors->errors[errors->error_count].file = (char*)malloc(len);
				strncpy(errors->errors[errors->error_count].file, section, len - 1);
				errors->errors[errors->error_count].file[len - 1] = '\0';
			} else {
				strncpy(errors->errors[errors->error_count].message,
					   msg.c_str(), sizeof(errors->errors[0].message) - 1);
				errors->errors[errors->error_count].message[sizeof(errors->errors[0].message) - 1] = '\0';
				errors->errors[errors->error_count].line = 0;
				errors->errors[errors->error_count].column = 0;
				errors->errors[errors->error_count].file = nullptr;
			}
			errors->error_count++;
		}
	}

	return r < 0 ? -1 : 0;
}

int as_add_section(void* validator, const char* script, const char* section_name) {
	ValidatorContext* ctx = static_cast<ValidatorContext*>(validator);
	return ctx->module->AddScriptSection(section_name, script);
}

int as_build(void* validator, ASErrorList* errors) {
	ValidatorContext* ctx = static_cast<ValidatorContext*>(validator);
	ctx->error_messages.clear();

	int r = ctx->module->Build();

	// Copy errors
	if (errors && !ctx->error_messages.empty()) {
		for (const auto& msg : ctx->error_messages) {
			if (errors->error_count >= errors->capacity) {
				errors->capacity = errors->capacity == 0 ? 16 : errors->capacity * 2;
				errors->errors = (ASError*)realloc(errors->errors,
												   errors->capacity * sizeof(ASError));
			}

			// Parse the error message to extract components
			char section[256];
			int row, col;
			char type[16];
			char message[4096];
			
			if (sscanf(msg.c_str(), "%255s (%d,%d) : %5s : %[^\n]",
					   section, &row, &col, type, message) == 5) {
				strncpy(errors->errors[errors->error_count].message,
					   message, sizeof(errors->errors[0].message) - 1);
				errors->errors[errors->error_count].message[sizeof(errors->errors[0].message) - 1] = '\0';
				errors->errors[errors->error_count].line = row;
				errors->errors[errors->error_count].column = col;
				size_t len = strlen(section) + 1;
				errors->errors[errors->error_count].file = (char*)malloc(len);
				strncpy(errors->errors[errors->error_count].file, section, len - 1);
				errors->errors[errors->error_count].file[len - 1] = '\0';
			} else {
				strncpy(errors->errors[errors->error_count].message,
					   msg.c_str(), sizeof(errors->errors[0].message) - 1);
				errors->errors[errors->error_count].message[sizeof(errors->errors[0].message) - 1] = '\0';
				errors->errors[errors->error_count].line = 0;
				errors->errors[errors->error_count].column = 0;
				errors->errors[errors->error_count].file = nullptr;
			}
			errors->error_count++;
		}
	}

	return r < 0 ? -1 : 0;
}

ASNameList *as_get_registered_names(void *validator) {
	ValidatorContext *ctx = static_cast<ValidatorContext *>(validator);
	asIScriptEngine *engine = ctx->engine;

	std::vector<std::string> names;
	auto add = [&](const char *name) {
		if (name && *name) names.push_back(name);
	};

	// Global functions
	for (asUINT i = 0; i < engine->GetGlobalFunctionCount(); i++)
		add(engine->GetGlobalFunctionByIndex(i)->GetName());

	// Global properties
	for (asUINT i = 0; i < engine->GetGlobalPropertyCount(); i++) {
		const char *name = nullptr;
		engine->GetGlobalPropertyByIndex(i, &name);
		add(name);
	}

	// Object types (includes methods and properties)
	for (asUINT i = 0; i < engine->GetObjectTypeCount(); i++) {
		asITypeInfo *ti = engine->GetObjectTypeByIndex(i);
		add(ti->GetName());
		for (asUINT j = 0; j < ti->GetMethodCount(); j++)
			add(ti->GetMethodByIndex(j)->GetName());
		for (asUINT j = 0; j < ti->GetPropertyCount(); j++) {
			const char *pname = nullptr;
			ti->GetProperty(j, &pname);
			add(pname);
		}
	}

	// Enums and their values
	for (asUINT i = 0; i < engine->GetEnumCount(); i++) {
		asITypeInfo *ti = engine->GetEnumByIndex(i);
		add(ti->GetName());
		for (asUINT j = 0; j < ti->GetEnumValueCount(); j++) {
			int val;
			add(ti->GetEnumValueByIndex(j, &val));
		}
	}

	// Typedefs
	for (asUINT i = 0; i < engine->GetTypedefCount(); i++)
		add(engine->GetTypedefByIndex(i)->GetName());

	// Funcdefs
	for (asUINT i = 0; i < engine->GetFuncdefCount(); i++)
		add(engine->GetFuncdefByIndex(i)->GetName());

	ASNameList *list = (ASNameList *)malloc(sizeof(ASNameList));
	list->count = (int)names.size();
	list->names = (char **)malloc((size_t)list->count * sizeof(char *));
	for (int i = 0; i < list->count; i++) {
		size_t len = names[i].length() + 1;
		list->names[i] = (char *)malloc(len);
		memcpy(list->names[i], names[i].c_str(), len);
	}
	return list;
}

void as_name_list_destroy(ASNameList *list) {
	if (!list) return;
	for (int i = 0; i < list->count; i++) free(list->names[i]);
	free(list->names);
	free(list);
}

void as_validator_destroy(void* validator) {
	ValidatorContext* ctx = static_cast<ValidatorContext*>(validator);
	if (ctx->engine) {
		ctx->engine->ShutDownAndRelease();
	}
	delete ctx;
}

ASErrorList* as_error_list_create(void) {
	ASErrorList* list = (ASErrorList*)malloc(sizeof(ASErrorList));
	list->errors = nullptr;
	list->error_count = 0;
	list->capacity = 0;
	return list;
}

void as_error_list_destroy(ASErrorList* list) {
	if (list) {
		if (list->errors) {
			for (int i = 0; i < list->error_count; i++) {
				if (list->errors[i].file) {
					free(list->errors[i].file);
				}
			}
			free(list->errors);
		}
		free(list);
	}
}

} // extern "C"
