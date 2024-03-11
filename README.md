# Installation

To successfully compile the code, you need to install **visual studio 2022** (not 2019) with C++ development tools.
Then, the application can be compiled.

This app can only be compiled for the **x86** platform (or **Win32**). Because the Dobot library is only available for x86 build. So before building it, change the platform from x64 to x86.

This app can be compiled in both Debug or release mode.

To run the app, you need to copy all the .dll files from the [./opencv/x86/vc17/bin](./opencv/x86/vc17/bin/) folder and from the [./Dobot/bin](./Dobot/bin/)
 to the folder where is your compiled .exe file.

For example, if the app is compiled using the Debug configuration, you need to copy the .dll files to the [./Debug](./Debug/) folder.