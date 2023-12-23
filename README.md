# CS2-Final-Project
Code Profiler to Count Function and Line Execution Amounts

This project takes as an argument an .xml representation of source code and turns it into a version that includes profiling information at the end.

Note: Only works in a Unix/Linux environment
# To Run:

A Makefile is included with the project. To build the profiler, use the command: ```make profiler```

From there, a new profiling source file, which will add functions to count line and function execution,  will need to be made. This is done using the srcML class, which is an .xml representation of a source code file. If there are multiple .cpp files, the first argument will need to be the file with the main function, and the rest will need to be added as adiitonal arguments. To do this, use the command: ```./profiler example1.cpp.xml example2.cpp.xml```

This will create a new file or files of the format: ```p-example1.cpp``` and ```p-example2.cpp```

From here, use the Makefile to compile and run, with the command: ```make p-example1```

This will create an executable with the format: ```p-example1```

Running this executable will run the original source code, as well as the profiling functions attached to it. At the end of the execution of the original code, the profiling information will be output.

The output will categorized by source file, and will include the line, the function if that line is the function definition, and the amount of times executed.

Two programs and their .xml files are included with this project: simple.cpp, and sort.cpp, which includes header file sort_lib.h and definition file sort_lib.cpp

This project has 5 main files:

- 1 and 2: AST.cpp and AST.hpp
 - Defines the srcML class to represent the source code of a file, as well as the Abstract Sytax Tree within the srcML class that is used to hold the data itself.
 - 
- 3 and 4:
 - Defines the profile class, one object wil be created for each definition file, and it includes functions to count the execution of lines, the counts of which are stored in a std::map, which uses the string representation of the line number as a key.
- 5: main.cpp
 - Takes in the srcML files as command line arguments, and outputs the p-***.cpp files that correlate to them. To simplify, at the beginning of each file, it inserts:
 ```C++
    #include "profile.hpp"
    profile example1_cpp;
 ```
 -  and then before every conditional statement, it inserts:
```C++
    profileName + ".conditionCount(__LINE__)" + " && "

```
 - and after every other statement that ends with a semicolon, it inserts:
```C++
    " " + profileName + ".count(__LINE__);"
```

As mentioned before, this all results in the creation of another program, which will run the original code as well as the profiling functions.






