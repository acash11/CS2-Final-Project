# CS2-Final-Project
Code Profiler to Count Function and Line Execution Amounts

This project takes as an argument an .xml representation of source code and turns it into a version that includes profiling information at the end.

Note: Only works in a Unix/Linux environment
# To Run:

A Makefile is included with the project. To build the profiler, use the command: ```make profiler```

From there, a new profiling source file, which will add functions to count line and function execution,  will need to be made. This is created from the .xml files of the source code. If there are multiple .cpp files, the first will need to be the file with the mainfunction, and the rest will need to be added as adiitonal arguments. To do this, use the command: ```./profiler example1.cpp.xml example2.cpp.xml```

This will create a new file or files of the format ```p-example1.cpp``` and ```p-example2.cpp```

From here, use the Makefile to compile and run, with the command ```make p-example1```

This will create an executable with the format: ```p-example1```

Running this executable will run the original source code, as well as the profiling functions attached to it. At the end of the execution of the original code, the profiling information will be output.

The output will categorized by source file, and will include the line, the function if that line is the function definition, and the amount of times executed.
