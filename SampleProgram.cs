using System;
using System.Runtime.InteropServices;
using System.Text;

using WolframLanguageRuntime;

/*
	Documentation links for SDK functions used in this file:

		wlr_sdk_StartRuntime - https://wolfr.am/1wl9JDTah
		wlr_CreateExpressionPool - https://wolfr.am/1wl9MzvmX
		wlr_Eval - https://wolfr.am/1wl9OeJOl
		wlr_VariadicE - https://wolfr.am/1wl9PYmJe
		wlr_ParseExpression - https://wolfr.am/1wl9SBSNH
		wlr_ReleaseExpressionPool - https://wolfr.am/1wl9VbykU
		wlr_StringData - https://wolfr.am/1wl9XfJgQ
		wlr_String - https://wolfr.am/1wla043ht
		wlr_Symbol - https://wolfr.am/1wla3FzQg
*/

unsafe class SampleProgram
{
	// Start the kernel runtime. Evaluate and print sample expression.
	static void Main()
	{
		if(!StartRuntime())
		{
			Console.Error.WriteLine("Failed to start kernel runtime.");
			return;
		}

		Console.Error.WriteLine("Started kernel runtime.");

		Console.WriteLine("WL Evaluation: 2+2 => " + EvaluateToOutputForm("2 + 2"));
	}

	// Start the kernel runtime. Return false on error.
	static bool StartRuntime()
	{
		string layoutDirectory = @"C:\Program Files\Wolfram Research\Wolfram\14.3";

		// Get unmanaged copy of string for use in SDK function
		sbyte *layoutDirectoryPointer = (sbyte *) Marshal.StringToHGlobalAnsi(layoutDirectory);

		WLR.wlr_error_type result =
			WLR.Methods.wlr_sdk_StartRuntime(
				WLR.wlr_application_type.WLR_EXECUTABLE,
				WLR.wlr_version_type.WLR_VERSION_1,
				WLR.wlr_license_type.WLR_LICENSE_OR_SIGNED_CODE_MODE,
				layoutDirectoryPointer,
				null
			);

		Marshal.FreeHGlobal((IntPtr) layoutDirectoryPointer);

		return result == WLR.wlr_error_type.WLR_SUCCESS;
	}

	// Evaluate an input string, returning the result as a string in OutputForm. Return empty string on error.
	static string EvaluateToOutputForm(string input)
	{
		WLR.Methods.wlr_CreateExpressionPool();

		// Evaluate ToString[<expression parsed from input string>, OutputForm]
		void *evaluatedExpression =
			WLR.Methods.wlr_Eval(
				WLR.Methods.wlr_VariadicE(
					ExpressionFromString(ExpressionResultType.SYMBOL_RESULT, "ToString"),
					2,
					// This function accepts a variable number of arguments in __arglist(...). The previous argument is the number of
					// arguments in the __arglist.
					__arglist(
						// Parse input string into unevaluated expression
						WLR.Methods.wlr_ParseExpression(
							// Get an expression corresponding to the input string
							ExpressionFromString(ExpressionResultType.STRING_RESULT, input)
						),
						// Get an expression corresponding to the symbol OutputForm
						ExpressionFromString(ExpressionResultType.SYMBOL_RESULT, "OutputForm")
					)
				)
			);

		// Get a C# string from evaluated expression, which will be our result
		string result = StringFromExpression(evaluatedExpression);

		// Release all of the expressions generated during this function
		WLR.Methods.wlr_ReleaseExpressionPool();

		return result;
	}

	// Get a C# string from a string expression
	static string StringFromExpression(void *stringExpression)
	{
		sbyte *stringData;

		int stringDataLength;

		// Get unmanaged data corresponding to the string of a string expression
		WLR.wlr_error_type error =
			WLR.Methods.wlr_StringData(
				stringExpression,
				&stringData,
				&stringDataLength
			);

		if(
			(error != WLR.wlr_error_type.WLR_SUCCESS) ||
				(stringDataLength == 0)
		)
		{
			return "";
		}

		var memorySpan = new ReadOnlySpan<byte>(stringData, stringDataLength);

		// Get C# string from unmanaged data allocated by wlr_StringData
		string result = Encoding.UTF8.GetString(memorySpan);

		// Free the unmanaged data allocated by wlr_StringData
		WLR.Methods.wlr_Release(stringData);

		return result;
	}

	// Get a string expression or symbol expression from C# string.
	static void *ExpressionFromString(ExpressionResultType type, string input)
	{
		void *result;

		fixed(byte *inputBytePointer = Encoding.UTF8.GetBytes(input))
		{
			sbyte *inputSignedBytePointer = (sbyte *) inputBytePointer;

			if(type == ExpressionResultType.STRING_RESULT)
			{
				result = WLR.Methods.wlr_String(inputSignedBytePointer);
			}
			else
			{
				result = WLR.Methods.wlr_Symbol(inputSignedBytePointer);
			}
		}

		return result;
	}

	// Enum for first argument to function ExpressionFromString.
	enum ExpressionResultType
	{
		STRING_RESULT,
		SYMBOL_RESULT
	}
}