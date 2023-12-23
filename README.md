# CS2-Final-Project
Code Profiler to Count Function and Line Execution Amounts

This project takes as an argument a .xml representation of source code and turns it into a version that includes profiling information at the end.

Note: Only works in a Unix/Linux environment
# To Run:

A Makefile is included with the project. To build the profiler, use the command: ```make profiler```

From there, a new profiling source file, which will add functions to count line and function execution,  will need to be made. This is done using the srcML class, which is an .xml representation of a source code file. If there are multiple .cpp files, the first argument will need to be the file with the main function, and the rest will need to be added as aditional arguments. To do this, use the command: ```./profiler example1.cpp.xml example2.cpp.xml```

This will create a new file or files of the format: ```p-example1.cpp``` and ```p-example2.cpp```

From here, use the Makefile to compile, with the command: ```make p-example1```

This will create an executable with the format: ```p-example1```

Running this executable will run the original source code, as well as the profiling functions attached to it. At the end of the execution of the original code, the profiling information will be output.

The output will categorized by source file, and will include the line, the function if that line is the function definition, and the amount of times executed.

Two programs and their .xml files are included with this project: simple.cpp, and sort.cpp, which includes header file sort_lib.h and definition file sort_lib.cpp

# Project Information:

This project has 5 main files:

- 1 and 2: AST.cpp and AST.hpp
  - Defines the srcML class to represent the source code of a file, as well as the Abstract Sytax Tree within the srcML class that is used to hold the data itself.
- 3 and 4: profile.cpp and profile.hpp
  - Defines the profile class, one object of which will be created for each definition file, and it includes functions to count the execution of lines, the counts of which are stored in a std::map, which uses the string representation of the line number as a key.
- 5: main.cpp
  - Takes in the srcML files as command line arguments, and outputs the p-***.cpp files that correlate to them. To simplify, at the beginning of each file, it inserts:
 ```C++
    #include "profile.hpp"
    profile example1_cpp;
 ```
  - and then before every conditional statement, it inserts:
```C++
    profileName + ".conditionCount(__LINE__)" + " && "

```
  - and after every other statement that ends with a semicolon, it inserts:
```C++
    " " + profileName + ".count(__LINE__);"
```

As mentioned before, this all results in the creation of another program, which will run the original code as well as the profiling functions.

Here is an example of output from this program, which was run on the included sort program:  

```
Data Before: 

         1          7          0          7          5 



Data After: 

         0          1          5          7          7


File: sort.cpp

101 10

102 2

104 10

107 2

115 process_command_line 1

118 1

122 6

128 5

129 6

130 5

131 5

132 6

133 6

134 5

136 3

139 5

144 5

146 3

149 5

36 main 1

42 1

46 1

49 1

50 2

53 2

54 1

55 1

56 1

62 1

63 2

73 generate_random_data 1

76 1

79 1

82 6

84 10

91 output_data 2

98 12



File: sort_lib.cpp

131 SWAP 4

133 4

139 LESS_THAN 5

147 GREATER_THAN 10

31 quick_sort 1

34 1

38 1

45 quick_sort 3

53 8

54 5

57 10

58 5

61 5

63 4

64 4

65 4

70 3

73 3

74 2

```






