## Structure of the sample program

* `WolframLanguageRuntime.cs`
	* The standalone applications SDK is a C interface. We need to use [P/Invoke](https://learn.microsoft.com/en-us/dotnet/standard/native-interop/pinvoke) in order to access it from .NET. This file contains the P/Invoke machinery for representing the SDK in .NET.
	* This file only handles the parts of the SDK interface necessary for this sample program. For the entire interface, please see the [original C header in the SDK](SDK/WolframLanguageRuntimeV1.h) and [its documentation](https://www.wolframcloud.com/obj/ccooley/swadoc-current/runtime.html#reference-information).
* `SampleProgram.cs`
	* This file contains the entry point for the program. It contains example code for using the SDK to start the Wolfram Language kernel and evaluate an expression.
* `SDK/`
	* This folder contains the Wolfram standalone applications SDK. The only thing from this folder that this application actually needs is `bin/StandaloneApplicationsSDK_Shared.dll`. The SDK is also available in the Wolfram layout under `SystemFiles/Components/StandaloneApplicationsSDK`.
* `SampleProgram.csproj`
	* This is the project file for the sample program. This file does two important things.
	* The first is that it allows unsafe code via `<AllowUnsafeBlocks>true</AllowUnsafeBlocks>`. This is necessary for using the P/Invoke machinery for the SDK.
	* The second is that it copies `bin/StandaloneApplicationsSDK_Shared.dll` alongside `SampleProgram.exe` in the build directory. This is necessary because `SampleProgram.exe` has a dependency on `StandaloneApplicationsSDK_Shared.dll`.

## Prerequisites for trying out the sample program

* A git installation, such as [Git for Windows](https://gitforwindows.org/)
* Visual Studio, including the ".NET Desktop Environment" module
* .NET 8 Runtime
	* This is not required by the standalone applications SDK. This is just the target framework I used for this demo application.
* A Wolfram installation with an active license
	* Later in the development process, we will remove this requirement.

## Instructions for trying out the sample program

1. First, clone the repo:

```
C:\Users\ccooley\TestDirectory>git clone https://github.com/cc-wr/SampleProgram.git
Cloning into 'SampleProgram'...
remote: Enumerating objects: 7, done.
remote: Counting objects: 100% (7/7), done.
remote: Compressing objects: 100% (6/6), done.
Receiving objects: 100% (7/7)used 7 (delta 0), pack-reused 0 (from 0)
Receiving objects: 100% (7/7), 239.12 KiB | 6.13 MiB/s, done.

C:\Users\ccooley\TestDirectory>cd SampleProgram
```

2. Then, edit the path in the following line in `SampleProgram.cs`:

	[`string layoutDirectory = @"C:\Program Files\Wolfram Research\Wolfram\14.3";`](./SampleProgram.cs#L24)

The new path should be location of your Wolfram layout. This value should correspond with the result of evaluating `$InstallationDirectory` in a Wolfram notebook on your machine.

**Note:** Currently, the above code contains a hard-coded, absolute path. This will not work for distributing your application. Later in the development process, we will bundle the Wolfram layout with your application, and further change the above code.

3. Finally, build and run the program:

```
C:\Users\ccooley\TestDirectory\SampleProgram>dotnet build
  Determining projects to restore...
  Restored C:\Users\ccooley\TestDirectory\SampleProgram\SampleProgram.csproj (in 65 ms).
  SampleProgram -> C:\Users\ccooley\TestDirectory\SampleProgram\bin\x64\Debug\net8.0\SampleProgram.dll

Build succeeded.
    0 Warning(s)
    0 Error(s)

Time Elapsed 00:00:02.65

C:\Users\ccooley\TestDirectory\SampleProgram>dotnet run
Started kernel runtime.
WL Evaluation: 2+2 => 4
```