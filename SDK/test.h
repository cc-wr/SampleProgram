#define WLR_ATTRIBUTE

typedef int mint;

typedef void* wlr_expr;

typedef enum wlr_error_type
{
	WLR_SUCCESS = 0,
	WLR_ALLOCATION_ERROR = 1,
	WLR_UNEXPECTED_TYPE = 2,
	WLR_ERROR_EXPRESSION = 3,
	WLR_MISCELLANEOUS_ERROR = 4,
	WLR_OUT_OF_BOUNDS = 5,
	WLR_SIGNING_ERROR = 6,
	WLR_UNSAFE_EXPRESSION = 7,
	WLR_MALFORMED = 8,
	WLR_RUNTIME_NOT_STARTED = 9
} wlr_err_t;

typedef enum wlr_license_type
{
	WLR_SIGNED_CODE_MODE = 0,
	WLR_LICENSE_OR_SIGNED_CODE_MODE = 1
} wlr_license_t;

typedef enum wlr_version_type
{
	WLR_VERSION_1 = 0
} wlr_version_t;

typedef enum wlr_application_type
{
	WLR_EXECUTABLE = 0,
	WLR_DYNAMIC_LIBRARY = 1
} wlr_application_t;

typedef enum wlr_containment_type
{
	WLR_CONTAINED = 0,
	WLR_UNCONTAINED = 1
} wlr_containment_t;

typedef struct wlr_runtime_configuration
{
	mint argumentCount;
	char** arguments;
	wlr_containment_t containmentSetting;
} wlr_runtime_conf;

WLR_ATTRIBUTE wlr_err_t wlr_sdk_StartRuntime(wlr_application_t applicationType, wlr_version_t version, wlr_license_t licenseType, const char *layoutDirectory, const wlr_runtime_conf *configuration);

WLR_ATTRIBUTE wlr_expr wlr_String(const char* string);

WLR_ATTRIBUTE wlr_expr wlr_Eval(wlr_expr expression);

WLR_ATTRIBUTE void wlr_ReleaseExpression(wlr_expr detachedExpression);

WLR_ATTRIBUTE wlr_err_t wlr_StringData(wlr_expr expression, char** resultData, mint* resultLength);

WLR_ATTRIBUTE wlr_expr wlr_ParseExpression(wlr_expr inputString);
