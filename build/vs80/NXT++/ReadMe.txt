========================================================================
    STATIC LIBRARY : NXT++ Project Overview
========================================================================

Todo:

- use example.cpp for the example project
- Debug the getNXTFileByType error
- Make sure the include file are the good one
- remove precompiled header for test and example
- Add comment and info to the project
- Take test code from example and put it in test.cpp

Done:

- Put the NXT++ lib project as a dependency for the example and test project
- Add a test project
- Build the lib
- Add ../inlcude in the seach path for header file in the project properties ( relative )
- Add cutom post-build event to copy the library to /bin directory
- Add link to fantom.lib