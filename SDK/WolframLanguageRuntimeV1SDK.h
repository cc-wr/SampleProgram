/*************************************************************************

						Mathematica source file

		Copyright 1986 through 2024 by Wolfram Research Inc.

This material contains trade secrets and may be registered with the
U.S. Copyright Office as an unpublished work, pursuant to Title 17,
U.S. Code, Section 408.  Unauthorized copying, adaptation, distribution
or display is prohibited.

*************************************************************************/

#ifndef WOLFRAM_RUNTIME_SDK_H
#define WOLFRAM_RUNTIME_SDK_H

#if !defined(WLR_STATIC_LINKING) && !defined(WLR_EXPORT_LINKING)
#define WLR_STATIC_LINKING
#endif
#include "WolframLanguageRuntimeV1.h"

#define WLR_SDK_START_RUNTIME(applicationType, licenseType, layoutDirectory, configuration) wlr_sdk_StartRuntime(applicationType, WLR_HEADER_VERSION, licenseType, layoutDirectory, configuration)

typedef enum wlr_application_type
{
	WLR_EXECUTABLE = 0,
	WLR_DYNAMIC_LIBRARY = 1
} wlr_application_t;

#ifdef __cplusplus
extern "C" {
#endif

WLR_ATTRIBUTE wlr_err_t wlr_sdk_StartRuntime(wlr_application_t applicationType, wlr_version_t version, wlr_license_t licenseType, const char *layoutDirectory, const wlr_runtime_conf *configuration);

WLR_ATTRIBUTE wlr_err_t wlr_sdk_RegisterSignatureFile(wlr_application_t applicationType, const char *fileName);

WLR_ATTRIBUTE mbool wlr_sdk_CodeSignatureModuleDefined(void);

#ifdef __cplusplus
}
#endif

#endif /* WOLFRAM_RUNTIME_SDK_H */
