#ifndef AS_WRAPPER_H
#define AS_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct {
	char message[4096];
	int line;
	int column;
	char* file;
} ASError;

typedef struct {
	ASError* errors;
	int error_count;
	int capacity;
} ASErrorList;

// Initialize the validator with all custom APIs registered
void* as_validator_create(void);

// Validate a single script
int as_validate_script(void* validator, const char* script,
					   const char* filename, ASErrorList* errors);

// Add a script section (for multi-file bundling)
int as_add_section(void* validator, const char* script, const char* section_name);

// Build/compile all added sections
int as_build(void* validator, ASErrorList* errors);

// Clean up
void as_validator_destroy(void* validator);

// Error list management
ASErrorList* as_error_list_create(void);
void as_error_list_destroy(ASErrorList* list);

// Flat list of all identifier names registered with the engine.
// Used by the obfuscator to build its protected-name set.
typedef struct {
	char **names;
	int    count;
} ASNameList;

ASNameList *as_get_registered_names(void *validator);
void        as_name_list_destroy(ASNameList *list);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // AS_WRAPPER_H
