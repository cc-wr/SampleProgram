/*************************************************************************

						Mathematica source file

		Copyright 1986 through 2024 by Wolfram Research Inc.

This material contains trade secrets and may be registered with the
U.S. Copyright Office as an unpublished work, pursuant to Title 17,
U.S. Code, Section 408.  Unauthorized copying, adaptation, distribution
or display is prohibited.

*************************************************************************/

#pragma once

#include <stdarg.h>

#include "WolframLibrary.h"
#include "WolframNumericArrayLibrary.h"
#include "dllexport.h"

/* If the macro WLR_EXPORT_LINKING is defined, the expression API functions are declared with DLLEXPORT.
	If the macro WLR_STATIC_LINKING is defined, the expression API functions are declared without attributes.
	If neither of these macros are defined, the expression API functions are declared with DLLIMPORT. */
#if defined(WLR_EXPORT_LINKING)
#define WLR_ATTRIBUTE DLLEXPORT
#elif defined(WLR_STATIC_LINKING)
#define WLR_ATTRIBUTE
#else
#define WLR_ATTRIBUTE DLLIMPORT
#endif

/* Expression API macros */

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define WLR_HEADER_VERSION WLR_VERSION_1

#define WLR_START_RUNTIME(licenseType, layoutDirectory, configuration) \
	wlr_StartRuntime(WLR_HEADER_VERSION, licenseType, layoutDirectory, configuration)

/**
	An error code that will be returned by certain expression API functions if the runtime has not been started
	@remarks This error code will only be returned by wlr_MNumericArray_* functions that have a return type of errcode_t
*/
#define RUNTIME_NOT_STARTED 1000

/**
	The size in bytes of a signature in the expression API (see shim_RegisterSignature and shim_RegisterSymbols)
*/
#define WLR_SIGNATURE_SIZE 256

/* Expression API types */

/**
	Type of an expression in the expression API
*/
typedef void* wlr_expr;

/**
	Type of an expression bag in the expression API
	@remarks See the documentation for the function wlr_ExpressionBag for more information
*/
typedef void* wlr_exprbag;

/**
	Type of a stdout handler function pointer
	@remarks See the documentation for the function wlr_AddStdoutHandler for more information
*/
typedef void (*wlr_stdout_handler_t)(char*, mint, void*);

/**
	Type of a message handler function pointer
	@remarks See the documentation for the function wlr_AddMessageHandler for more information
*/
typedef void (*wlr_message_handler_t)(wlr_expr, wlr_expr, wlr_expr, void*);

/**
	Type of a signature in the expression API
	@remarks See the documentation for the functions wlr_RegisterSignature and wlr_RegisterSymbols for more information
*/
typedef unsigned char wlr_signature_t[WLR_SIGNATURE_SIZE];

/**
	Enum for the different types of number-type expressions in the expression API
	@remarks A value of this type is returned by the function wlr_NumberType
	@remarks "Number-type expressions" are those expressions for which wlr_ExpressionType returns WLR_NUMBER
*/
typedef enum wlr_number_type
{
	WLR_MACHINE_INTEGER = 0,
	WLR_BIG_INTEGER = 1,
	WLR_MACHINE_REAL = 2,
	WLR_BIG_REAL = 3,
	WLR_COMPLEX = 4,
	WLR_RATIONAL = 5,
	WLR_OVERFLOW = 6,
	WLR_UNDERFLOW = 7,
	WLR_NOT_A_NUMBER = 8
} wlr_num_t;

/**
	Enum for the different types of expressions in the expression API
	@remarks A value of this type is returned by the function wlr_ExpressionType
*/
typedef enum wlr_expression_type
{
	WLR_NUMBER = 0,
	WLR_STRING = 1,
	WLR_SYMBOL = 2,
	WLR_NORMAL = 3,
	WLR_ERROR = 4,
	WLR_PACKED_ARRAY = 5,
	WLR_NUMERIC_ARRAY = 6,
	WLR_BOOLEAN_FUNCTION = 7,
	WLR_GRAPH = 8,
	WLR_ASSOCIATION = 9,
	WLR_DISPATCH = 10,
	WLR_REGION = 11,
	WLR_OTHER = 12
} wlr_expr_t;

/**
	Enum for the different types of errors in the expression API
	@remarks Some expression API functions return a value of this type directly to indicate their status
	@remarks There is a sub-type of expression in the expression API called an "error expression." An error expression
   is returned when an error occurs during the execution of an expression API function that returns an expression. Every
   error expression corresponds to a single error type.
	@remarks The function wlr_ErrorType returns the error type of an error expression.
	@remarks WLR_SUCCESS is the lowest value, and WLR_RUNTIME_NOT_STARTED is the highest value. The internal
   implementation of the expression API depends on this fact.
*/
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

/**
	Enum for whether the runtime should prefer consuming a license, if possible, to using code signing
	@remarks The function wlr_StartRuntime takes an argument of this type. See the documentation for that function for
   more information.
*/
typedef enum wlr_license_type
{
	WLR_SIGNED_CODE_MODE = 0,
	WLR_LICENSE_OR_SIGNED_CODE_MODE = 1
} wlr_license_t;

/**
	Enum for different expression API versions
	@remarks The function wlr_StartRuntime takes an argument of this type. See the documentation for that function for
   more information.
*/
typedef enum wlr_version_type
{
	WLR_VERSION_1 = 0
} wlr_version_t;

/**
	Enum for different code signing modes
	@remarks The function wlr_ConfigureCodeSigning takes an argument of this type. See the documentation for that
   function for more information.
*/
typedef enum wlr_signing_configuration_type
{
	WLR_ENABLE_CODE_SIGNING = 0,
	WLR_ENABLE_CODE_SIGNING_EXCEPT_EXPRESSION_API = 1,
	WLR_DISABLE_CODE_SIGNING = 2
} wlr_signing_conf_t;

/**
	Enum for whether the runtime should be started in a "contained" mode or not
	@remarks A member of this type is present in the wlr_runtime_conf struct, which is one of the arguments to the
   function wlr_StartRuntime. See the documentation for that function for more information.
*/
typedef enum wlr_containment_type
{
	WLR_CONTAINED = 0,
	WLR_UNCONTAINED = 1
} wlr_containment_t;

/**
	Struct containing various aspects of how a runtime can be configured
	@remarks The function wlr_StartRuntime takes an argument of this type. See the documentation for that function for
   more information.
*/
typedef struct wlr_runtime_configuration
{
	mint argumentCount;
	char** arguments;
	wlr_containment_t containmentSetting;
} wlr_runtime_conf;

/* Expression API functions */

#ifdef __cplusplus
extern "C" {
#endif

WLR_ATTRIBUTE void wlr_InitializeRuntimeConfiguration(wlr_runtime_conf* configuration);
WLR_ATTRIBUTE wlr_err_t wlr_StartRuntime(wlr_version_t version, wlr_license_t licenseType, const char* layoutDirectory,
										 const wlr_runtime_conf* configuration);
WLR_ATTRIBUTE void wlr_CloseRuntime(void);

WLR_ATTRIBUTE wlr_err_t wlr_RegisterSignature(const wlr_signature_t signature);
WLR_ATTRIBUTE wlr_err_t wlr_RegisterSignatureFile(const char* fileName);
WLR_ATTRIBUTE wlr_err_t wlr_RegisterSymbols(const wlr_signature_t signature, mint symbolNameCount, const char** symbolNames);
WLR_ATTRIBUTE void wlr_ConfigureCodeSigning(wlr_signing_conf_t signingConfiguration);
WLR_ATTRIBUTE mint wlr_MemoryInUse(void);

WLR_ATTRIBUTE wlr_expr wlr_Eval(wlr_expr expression);
WLR_ATTRIBUTE wlr_expr wlr_EvalData(wlr_expr expression);
WLR_ATTRIBUTE wlr_expr wlr_EvalString(wlr_expr inputString);
WLR_ATTRIBUTE wlr_expr wlr_Get(const char* fileName);
WLR_ATTRIBUTE void wlr_Abort(void);
WLR_ATTRIBUTE void wlr_ClearAbort(void);

WLR_ATTRIBUTE wlr_err_t wlr_AddStdoutHandler(wlr_stdout_handler_t handlerFunction, const void* contextData);
WLR_ATTRIBUTE wlr_err_t wlr_AddMessageHandler(wlr_message_handler_t handlerFunction, const void* contextData);
WLR_ATTRIBUTE void wlr_RemoveStdoutHandler(wlr_stdout_handler_t handlerFunction);
WLR_ATTRIBUTE void wlr_RemoveMessageHandler(wlr_message_handler_t handlerFunction);
WLR_ATTRIBUTE void wlr_DefaultStdoutHandler(char* stringData, mint stringDataLength, void* contextData);
WLR_ATTRIBUTE void wlr_DefaultMessageHandler(wlr_expr messageName, wlr_expr heldMessage, wlr_expr messageText,
											 void* contextData);

WLR_ATTRIBUTE void wlr_CreateExpressionPool(void);
WLR_ATTRIBUTE void wlr_ReleaseExpressionPool(void);
WLR_ATTRIBUTE void wlr_ReleaseExpression(wlr_expr detachedExpression);
WLR_ATTRIBUTE void wlr_MoveExpressionToParentPool(wlr_expr expression);
WLR_ATTRIBUTE void wlr_DetachExpression(wlr_expr expression);
WLR_ATTRIBUTE wlr_expr wlr_Clone(wlr_expr expression);
WLR_ATTRIBUTE void wlr_ReleaseAll(void);
WLR_ATTRIBUTE void wlr_Release(void* data);

WLR_ATTRIBUTE wlr_expr wlr_Error(wlr_err_t errorType);
WLR_ATTRIBUTE mbool wlr_ErrorQ(wlr_expr expression);
WLR_ATTRIBUTE wlr_err_t wlr_ErrorType(wlr_expr errorExpression);
WLR_ATTRIBUTE mbool wlr_NumberQ(wlr_expr expression);
WLR_ATTRIBUTE wlr_num_t wlr_NumberType(wlr_expr numberExpression);
WLR_ATTRIBUTE wlr_expr wlr_Integer(mint value);
WLR_ATTRIBUTE wlr_err_t wlr_IntegerConvert(wlr_expr numberExpression, mint* result);
WLR_ATTRIBUTE wlr_expr wlr_Real(mreal value);
WLR_ATTRIBUTE wlr_err_t wlr_RealConvert(wlr_expr numberExpression, mreal* result);
WLR_ATTRIBUTE wlr_expr wlr_Complex(wlr_expr realPart, wlr_expr imaginaryPart);
WLR_ATTRIBUTE wlr_expr wlr_RealPart(wlr_expr complexExpression);
WLR_ATTRIBUTE wlr_expr wlr_ImaginaryPart(wlr_expr complexExpression);
WLR_ATTRIBUTE wlr_expr wlr_Rational(wlr_expr numerator, wlr_expr denominator);
WLR_ATTRIBUTE wlr_expr wlr_Numerator(wlr_expr rationalExpression);
WLR_ATTRIBUTE wlr_expr wlr_Denominator(wlr_expr rationalExpression);
WLR_ATTRIBUTE wlr_expr wlr_NumberFromString(const char* numberString);
WLR_ATTRIBUTE wlr_err_t wlr_StringFromNumber(wlr_expr numberExpression, char** result);
WLR_ATTRIBUTE wlr_expr wlr_Rule(wlr_expr leftHandSide, wlr_expr rightHandSide);
WLR_ATTRIBUTE mbool wlr_RuleQ(wlr_expr expression);
WLR_ATTRIBUTE mbool wlr_ListQ(wlr_expr expression);
WLR_ATTRIBUTE mbool wlr_AssociationQ(wlr_expr expression);
WLR_ATTRIBUTE mint wlr_Length(wlr_expr expression);
WLR_ATTRIBUTE wlr_expr wlr_Part(wlr_expr expression, mint index);
WLR_ATTRIBUTE wlr_expr wlr_First(wlr_expr expression);
WLR_ATTRIBUTE wlr_expr wlr_Last(wlr_expr expression);
WLR_ATTRIBUTE wlr_expr wlr_Rest(wlr_expr expression);
WLR_ATTRIBUTE wlr_expr wlr_Head(wlr_expr expression);
WLR_ATTRIBUTE wlr_expr wlr_ReplacePart(wlr_expr expression, mint index, wlr_expr newPart);
WLR_ATTRIBUTE wlr_expr wlr_Symbol(const char* symbolName);
WLR_ATTRIBUTE wlr_expr wlr_GlobalSymbol(const char* baseSymbolName);
WLR_ATTRIBUTE wlr_expr wlr_SystemSymbol(const char* baseSymbolName);
WLR_ATTRIBUTE wlr_expr wlr_ContextSymbol(const char* symbolContext, const char* baseSymbolName);
WLR_ATTRIBUTE wlr_expr wlr_SymbolName(wlr_expr symbol);
WLR_ATTRIBUTE wlr_expr wlr_SymbolContext(wlr_expr symbol);
WLR_ATTRIBUTE wlr_exprbag wlr_ExpressionBag(void);
WLR_ATTRIBUTE wlr_err_t wlr_AddExpression(wlr_exprbag expressionBag, wlr_expr expression);
WLR_ATTRIBUTE mint wlr_ExpressionBagLength(wlr_exprbag expressionBag);
WLR_ATTRIBUTE wlr_expr wlr_ExpressionBagToExpression(wlr_exprbag expressionBag, wlr_expr expressionHead);
WLR_ATTRIBUTE void wlr_ReleaseExpressionBag(wlr_exprbag expressionBag);
WLR_ATTRIBUTE wlr_expr wlr_String(const char* string);
WLR_ATTRIBUTE wlr_expr wlr_StringFromData(const char* utf8Data, mint utf8DataLength);
WLR_ATTRIBUTE wlr_expr wlr_RawString(const char* string);
WLR_ATTRIBUTE wlr_expr wlr_RawStringFromData(const char* utf8Data, mint utf8DataLength);
WLR_ATTRIBUTE wlr_err_t wlr_StringData(wlr_expr expression, char** resultData, mint* resultLength);
WLR_ATTRIBUTE wlr_expr wlr_ExpressionFromNumericArray(const MNumericArray numericArray, wlr_expr expressionHead);
WLR_ATTRIBUTE wlr_err_t wlr_NumericArrayData(wlr_expr expression, MNumericArray* result);
WLR_ATTRIBUTE wlr_expr_t wlr_ExpressionType(wlr_expr expression);
WLR_ATTRIBUTE wlr_expr wlr_Normalize(wlr_expr expression);
WLR_ATTRIBUTE wlr_expr wlr_ExpressionFromIntegerArray(mint arrayLength, const mint* array, wlr_expr expressionHead);
WLR_ATTRIBUTE wlr_expr wlr_ExpressionFromRealArray(mint arrayLength, const mreal* array, wlr_expr expressionHead);
WLR_ATTRIBUTE wlr_err_t wlr_IntegerArrayData(wlr_expr expression, mint* resultLength, mint** resultArray);
WLR_ATTRIBUTE wlr_err_t wlr_RealArrayData(wlr_expr expression, mint* resultLength, mreal** resultArray);
WLR_ATTRIBUTE mbool wlr_TrueQ(wlr_expr expression);
WLR_ATTRIBUTE mbool wlr_SameQ(wlr_expr firstExpression, wlr_expr secondExpression);
WLR_ATTRIBUTE wlr_expr wlr_GetValueFromKey(wlr_expr association, wlr_expr key);
WLR_ATTRIBUTE wlr_expr wlr_GetKeys(wlr_expr association);
WLR_ATTRIBUTE wlr_expr wlr_GetValues(wlr_expr association);
WLR_ATTRIBUTE wlr_expr wlr_ParseExpression(wlr_expr inputString);
WLR_ATTRIBUTE wlr_err_t wlr_Serialize(const char* fileName, wlr_expr expression);
WLR_ATTRIBUTE wlr_expr wlr_Deserialize(const char* fileName);
WLR_ATTRIBUTE wlr_err_t wlr_IntegerData(wlr_expr machineIntegerExpression, mint* result);
WLR_ATTRIBUTE wlr_err_t wlr_RealData(wlr_expr machineRealExpression, mreal* result);
WLR_ATTRIBUTE mbool wlr_SignedCodeMode(void);

WLR_ATTRIBUTE errcode_t wlr_MNumericArray_new(const numericarray_data_t type, const mint rank, const mint* dims,
											  MNumericArray* res);
WLR_ATTRIBUTE errcode_t wlr_MNumericArray_clone(const MNumericArray from, MNumericArray* to);
WLR_ATTRIBUTE void wlr_MNumericArray_free(MNumericArray narray);
WLR_ATTRIBUTE void wlr_MNumericArray_disown(MNumericArray narray);
WLR_ATTRIBUTE void wlr_MNumericArray_disownAll(MNumericArray narray);
WLR_ATTRIBUTE mint wlr_MNumericArray_shareCount(const MNumericArray narray);
WLR_ATTRIBUTE numericarray_data_t wlr_MNumericArray_getType(const MNumericArray narray);
WLR_ATTRIBUTE mint wlr_MNumericArray_getRank(const MNumericArray narray);
WLR_ATTRIBUTE mint wlr_MNumericArray_getFlattenedLength(const MNumericArray narray);
WLR_ATTRIBUTE errcode_t wlr_MNumericArray_convertType(MNumericArray* outP, const MNumericArray narray,
													  const numericarray_data_t result_type,
													  const numericarray_convert_method_t method, const mreal tolerance);
WLR_ATTRIBUTE mint* wlr_MNumericArray_getDimensions(const MNumericArray narray);
WLR_ATTRIBUTE void* wlr_MNumericArray_getData(const MNumericArray narray);

/* Expression API variadic function machinery */

WLR_ATTRIBUTE wlr_expr wlr_VariadicE(void* expressionHead, mint childElementNumber, ...);
WLR_ATTRIBUTE wlr_expr wlr_iVariadicE(wlr_expr expressionHead, mint childElementNumber, va_list childElements);
WLR_ATTRIBUTE wlr_expr wlr_VariadicList(mint childElementNumber, ...);
WLR_ATTRIBUTE wlr_expr wlr_iVariadicList(mint childElementNumber, va_list childElements);
WLR_ATTRIBUTE wlr_expr wlr_VariadicAssociation(mint keyValuePairNumber, ...);
WLR_ATTRIBUTE wlr_expr wlr_iVariadicAssociation(mint keyValuePairNumber, va_list keyValuePairs);

#ifdef __cplusplus
}
#endif

/* adapted from https://stackoverflow.com/a/66556553 and https://stackoverflow.com/a/11652569 */
#define WLR_INTERNAL_PRIMITIVE_CAT(a, ...) a##__VA_ARGS__
#define WLR_INTERNAL_IIF(c) WLR_INTERNAL_PRIMITIVE_CAT(WLR_INTERNAL_IIF_, c)
#define WLR_INTERNAL_IIF_0(t, ...) __VA_ARGS__
#define WLR_INTERNAL_IIF_1(t, ...) t
#define WLR_INTERNAL_EXPAND(x) x
#define WLR_INTERNAL_GET_ARGUMENT(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, \
								  _19, _20, _21, _22, _23, _24, _25, _26, ...)                                         \
	_26
#define WLR_HAS_COMMA(...)                                                                                           \
	WLR_INTERNAL_EXPAND(WLR_INTERNAL_GET_ARGUMENT(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
												  1, 1, 1, 1, 1, 1, 1, 0))
#define WLR_INTERNAL_PASTE_FIVE(_0, _1, _2, _3, _4) _0##_1##_2##_3##_4
#define WLR_INTERNAL_IS_EMPTY2_CASE_0001 ,
#define WLR_INTERNAL_IS_EMPTY2(_0, _1, _2, _3) \
	WLR_HAS_COMMA(WLR_INTERNAL_PASTE_FIVE(WLR_INTERNAL_IS_EMPTY2_CASE_, _0, _1, _2, _3))
#define WLR_INTERNAL_TRIGGER_PARENTHESIS(...) ,
#define WLR_INTERNAL_IS_EMPTY(...)                                                               \
	WLR_INTERNAL_EXPAND(WLR_INTERNAL_IS_EMPTY2(                                                  \
		WLR_HAS_COMMA(__VA_ARGS__), WLR_HAS_COMMA(WLR_INTERNAL_TRIGGER_PARENTHESIS __VA_ARGS__), \
		WLR_HAS_COMMA(__VA_ARGS__()), WLR_HAS_COMMA(WLR_INTERNAL_TRIGGER_PARENTHESIS __VA_ARGS__())))
#define WLR_INTERNAL_MSVC_VA_ARGS_WORKAROUND(define, args) define args
#define WLR_INTERNAL_GET_VARIADIC_FUNCTION(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, \
										   _18, _19, _20, _21, _22, _23, _24, _25, _26, NAME, ...)                     \
	NAME
#define WLR_INTERNAL_CALL_VARIADIC_FUNCTION(functionName, ...)                                                         \
	WLR_INTERNAL_MSVC_VA_ARGS_WORKAROUND(                                                                              \
		WLR_INTERNAL_GET_VARIADIC_FUNCTION,                                                                            \
		(__VA_ARGS__, functionName##_26, functionName##_25, functionName##_24, functionName##_23, functionName##_22,   \
		 functionName##_21, functionName##_20, functionName##_19, functionName##_18, functionName##_17,                \
		 functionName##_16, functionName##_15, functionName##_14, functionName##_13, functionName##_12,                \
		 functionName##_11, functionName##_10, functionName##_9, functionName##_8, functionName##_7, functionName##_6, \
		 functionName##_5, functionName##_4, functionName##_3, functionName##_2,                                       \
		 WLR_INTERNAL_IIF(WLR_INTERNAL_IS_EMPTY(__VA_ARGS__))(functionName##_0, functionName##_1)))                    \
	(__VA_ARGS__)

#define wlr_E(...) WLR_INTERNAL_CALL_VARIADIC_FUNCTION(wlr_E, __VA_ARGS__)
#define wlr_E_1(expressionHead, ...) wlr_VariadicE(expressionHead, 0)
#define wlr_E_2(expressionHead, ...) wlr_VariadicE(expressionHead, 1, __VA_ARGS__)
#define wlr_E_3(expressionHead, ...) wlr_VariadicE(expressionHead, 2, __VA_ARGS__)
#define wlr_E_4(expressionHead, ...) wlr_VariadicE(expressionHead, 3, __VA_ARGS__)
#define wlr_E_5(expressionHead, ...) wlr_VariadicE(expressionHead, 4, __VA_ARGS__)
#define wlr_E_6(expressionHead, ...) wlr_VariadicE(expressionHead, 5, __VA_ARGS__)
#define wlr_E_7(expressionHead, ...) wlr_VariadicE(expressionHead, 6, __VA_ARGS__)
#define wlr_E_8(expressionHead, ...) wlr_VariadicE(expressionHead, 7, __VA_ARGS__)
#define wlr_E_9(expressionHead, ...) wlr_VariadicE(expressionHead, 8, __VA_ARGS__)
#define wlr_E_10(expressionHead, ...) wlr_VariadicE(expressionHead, 9, __VA_ARGS__)
#define wlr_E_11(expressionHead, ...) wlr_VariadicE(expressionHead, 10, __VA_ARGS__)
#define wlr_E_12(expressionHead, ...) wlr_VariadicE(expressionHead, 11, __VA_ARGS__)
#define wlr_E_13(expressionHead, ...) wlr_VariadicE(expressionHead, 12, __VA_ARGS__)
#define wlr_E_14(expressionHead, ...) wlr_VariadicE(expressionHead, 13, __VA_ARGS__)
#define wlr_E_15(expressionHead, ...) wlr_VariadicE(expressionHead, 14, __VA_ARGS__)
#define wlr_E_16(expressionHead, ...) wlr_VariadicE(expressionHead, 15, __VA_ARGS__)
#define wlr_E_17(expressionHead, ...) wlr_VariadicE(expressionHead, 16, __VA_ARGS__)
#define wlr_E_18(expressionHead, ...) wlr_VariadicE(expressionHead, 17, __VA_ARGS__)
#define wlr_E_19(expressionHead, ...) wlr_VariadicE(expressionHead, 18, __VA_ARGS__)
#define wlr_E_20(expressionHead, ...) wlr_VariadicE(expressionHead, 19, __VA_ARGS__)
#define wlr_E_21(expressionHead, ...) wlr_VariadicE(expressionHead, 20, __VA_ARGS__)
#define wlr_E_22(expressionHead, ...) wlr_VariadicE(expressionHead, 21, __VA_ARGS__)
#define wlr_E_23(expressionHead, ...) wlr_VariadicE(expressionHead, 22, __VA_ARGS__)
#define wlr_E_24(expressionHead, ...) wlr_VariadicE(expressionHead, 23, __VA_ARGS__)
#define wlr_E_25(expressionHead, ...) wlr_VariadicE(expressionHead, 24, __VA_ARGS__)
#define wlr_E_26(expressionHead, ...) wlr_VariadicE(expressionHead, 25, __VA_ARGS__)

#define wlr_List(...) WLR_INTERNAL_CALL_VARIADIC_FUNCTION(wlr_List, __VA_ARGS__)
#define wlr_List_0(...) wlr_VariadicList(0)
#define wlr_List_1(...) wlr_VariadicList(1, __VA_ARGS__)
#define wlr_List_2(...) wlr_VariadicList(2, __VA_ARGS__)
#define wlr_List_3(...) wlr_VariadicList(3, __VA_ARGS__)
#define wlr_List_4(...) wlr_VariadicList(4, __VA_ARGS__)
#define wlr_List_5(...) wlr_VariadicList(5, __VA_ARGS__)
#define wlr_List_6(...) wlr_VariadicList(6, __VA_ARGS__)
#define wlr_List_7(...) wlr_VariadicList(7, __VA_ARGS__)
#define wlr_List_8(...) wlr_VariadicList(8, __VA_ARGS__)
#define wlr_List_9(...) wlr_VariadicList(9, __VA_ARGS__)
#define wlr_List_10(...) wlr_VariadicList(10, __VA_ARGS__)
#define wlr_List_11(...) wlr_VariadicList(11, __VA_ARGS__)
#define wlr_List_12(...) wlr_VariadicList(12, __VA_ARGS__)
#define wlr_List_13(...) wlr_VariadicList(13, __VA_ARGS__)
#define wlr_List_14(...) wlr_VariadicList(14, __VA_ARGS__)
#define wlr_List_15(...) wlr_VariadicList(15, __VA_ARGS__)
#define wlr_List_16(...) wlr_VariadicList(16, __VA_ARGS__)
#define wlr_List_17(...) wlr_VariadicList(17, __VA_ARGS__)
#define wlr_List_18(...) wlr_VariadicList(18, __VA_ARGS__)
#define wlr_List_19(...) wlr_VariadicList(19, __VA_ARGS__)
#define wlr_List_20(...) wlr_VariadicList(20, __VA_ARGS__)
#define wlr_List_21(...) wlr_VariadicList(21, __VA_ARGS__)
#define wlr_List_22(...) wlr_VariadicList(22, __VA_ARGS__)
#define wlr_List_23(...) wlr_VariadicList(23, __VA_ARGS__)
#define wlr_List_24(...) wlr_VariadicList(24, __VA_ARGS__)
#define wlr_List_25(...) wlr_VariadicList(25, __VA_ARGS__)

#define wlr_Association(...) WLR_INTERNAL_CALL_VARIADIC_FUNCTION(wlr_Association, __VA_ARGS__)
#define wlr_Association_0(...) wlr_VariadicAssociation(0)
#define wlr_Association_1(...) wlr_VariadicAssociation(1, __VA_ARGS__)
#define wlr_Association_2(...) wlr_VariadicAssociation(2, __VA_ARGS__)
#define wlr_Association_3(...) wlr_VariadicAssociation(3, __VA_ARGS__)
#define wlr_Association_4(...) wlr_VariadicAssociation(4, __VA_ARGS__)
#define wlr_Association_5(...) wlr_VariadicAssociation(5, __VA_ARGS__)
#define wlr_Association_6(...) wlr_VariadicAssociation(6, __VA_ARGS__)
#define wlr_Association_7(...) wlr_VariadicAssociation(7, __VA_ARGS__)
#define wlr_Association_8(...) wlr_VariadicAssociation(8, __VA_ARGS__)
#define wlr_Association_9(...) wlr_VariadicAssociation(9, __VA_ARGS__)
#define wlr_Association_10(...) wlr_VariadicAssociation(10, __VA_ARGS__)
#define wlr_Association_11(...) wlr_VariadicAssociation(11, __VA_ARGS__)
#define wlr_Association_12(...) wlr_VariadicAssociation(12, __VA_ARGS__)
#define wlr_Association_13(...) wlr_VariadicAssociation(13, __VA_ARGS__)
#define wlr_Association_14(...) wlr_VariadicAssociation(14, __VA_ARGS__)
#define wlr_Association_15(...) wlr_VariadicAssociation(15, __VA_ARGS__)
#define wlr_Association_16(...) wlr_VariadicAssociation(16, __VA_ARGS__)
#define wlr_Association_17(...) wlr_VariadicAssociation(17, __VA_ARGS__)
#define wlr_Association_18(...) wlr_VariadicAssociation(18, __VA_ARGS__)
#define wlr_Association_19(...) wlr_VariadicAssociation(19, __VA_ARGS__)
#define wlr_Association_20(...) wlr_VariadicAssociation(20, __VA_ARGS__)
#define wlr_Association_21(...) wlr_VariadicAssociation(21, __VA_ARGS__)
#define wlr_Association_22(...) wlr_VariadicAssociation(22, __VA_ARGS__)
#define wlr_Association_23(...) wlr_VariadicAssociation(23, __VA_ARGS__)
#define wlr_Association_24(...) wlr_VariadicAssociation(24, __VA_ARGS__)
#define wlr_Association_25(...) wlr_VariadicAssociation(25, __VA_ARGS__)
