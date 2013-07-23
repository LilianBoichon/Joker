Joker
=====

This depot hosts the Joker projet components:

* applications
* libraries
* tests

Compilation information :
-------------------------

In order to compile the test project you will need :

### The SDL framework v1.2

Available [here](http://www.libsdl.org/download-1.2.php).


### The Leap SDK

Available [here](https://developer.leapmotion.com).

You must then define the following global variable in order to compile with the LEAP: 

	launchctl setenv LEAP_SDK /path/to/sdk/root
	

And add the following build steps:

* Command : `install_name_tool`
* Arguments : -change @loader_path/libLeap.dylib @executable_path/../Resources/libs/libLeap.dylib %{CurrentProject:Name}
* Working directory : %{buildDir}/%{CurrentProject:Name}.app/Contents/MacOS/



