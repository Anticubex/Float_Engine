from typing import Generator, List, Tuple, Set
from functools import cache
from os import path

from globalsettings import Dynamic_Includes, Static_Includes

""" Check the last modified time of C/C++ file and its included headers, chained """

@cache
def findIncludes(file: str) -> Set[str]:
        """     Tree-searches through files to find all includes, using #include directives

        Args:
            file (str): file to tree-search

        Yields:
            List[str]: List of included files found 
        """
        found_searched: Set[str] = set()
        curr_searches: Set[str] = {file}
        found_unsearched: Set[str] = set()

        while curr_searches:
                just_found: Set[Tuple[str, bool]] = set()
                for file in curr_searches:
                        just_found.update(checkFile(file))
                for include, dynamic in just_found:
                        if dynamic:
                                found_unsearched.add(include)
                        else:
                                found_searched.add(include)

                found_searched.update(curr_searches)
                curr_searches = found_unsearched
                found_unsearched = set()

        return found_searched

@cache
def checkFile(absFile: str) -> Set[Tuple[str, bool]]:
        """Finds all files included from the file

        Args:
            absFile (str): The absolute name of the file to find includes in

        Returns:
            List[str]: The included files detected
        """

        root, file = path.split(absFile)
        found: Set[str] = set()
        with open(absFile) as c_file:
                for line_number, line in enumerate(c_file, 1):
                        includeArg = getInclude(line)
                        if not includeArg:
                                continue
                        included = parseInclude(root, includeArg)
                        # if isinstance(included, NameError):
                        #         raise NameError(f"Error with #include directive in {absFile}: {included}: \n {line_number} | {line}")
                        if included:
                                # print("Found", included)
                                found.add(included)
        return found

@cache  
def getInclude(line: str) -> str:
        len_include = 7 # len("include")

        stripped = line.strip()
        # Check if it is at least a preprocessor directive
        if line[0] != "#":
                return None

        ptr = 1
        # Walk until found some whitespace (because some people don't follow proper formatting)
        while line[ptr] == " ":
                ptr += 1
        
        if len(line) - ptr < len_include:
                return None
        
        if line[ptr:ptr+len_include] != "include":
                return None
        
        ptr = ptr+len_include

        # Walk until found some whitespace (because some people don't follow proper formatting)
        while line[ptr] == " ":
                ptr += 1

        return line[ptr::1].strip()

@cache
def parseInclude(root: str, arg: str) -> Tuple[str, bool]:
        """     Parses an `#include` preprocessor directive, from just the argument ()
                Uses functools cache
                The use of the `Static/Dynamic_Includes` global has the side effect of ignoring standard library, looking only through the -I included libraries
                the `Includes` are the -I option of the compiler. The `Static_Includes` includes shouldn't be searched further, i.e. in the case of an external library, but the `Dynamic_Includes` should 

        Args:
                arg (str): the "<included>" or ""included"" argument of the #include directive
        Returns:
                None: If include not found, or include directive has invalid syntax
                str: the absolute path of the file, with precedence of in root first, then by order of `I` directories
                bool: if the path is a dynamic include
        """        

        global Dynamic_Includes
        global Static_Includes

        # Extract naked include directory from within
        include = arg[1:-1].strip()

        # fileFound = None
        hasQuotes = False
        
        # If brackets (<>) are used, than the root is not searched.
        # print(f"Parsing {arg}")

        if (arg[0], arg[-1]) == ('"', '"'):
                # print("Has \" quotes")
                hasQuotes = True
                # Search root for file
                absPath = path.join(root, include)
                if path.exists(absPath):
                        return (absPath, True)
        
        if hasQuotes or (arg[0], arg[-1]) == ('<', '>'):
                # print("Has \" or <> quotes")
                hasQuotes = True

                for directory in Dynamic_Includes:
                        absPath = path.join(directory, include)
                        # print("Searching directory", directory, "for path", absPath)
                        if path.exists(absPath):
                                # if fileFound:
                                #         return NameError(f"Found multiple files of name {include}")
                                return (absPath, True)
                
                for directory in Static_Includes:
                        absPath = path.join(directory, include)
                        if path.exists(absPath):
                                return (absPath, False)
                
        return None
        # if fileFound:
        #         return fileFound
        # if hasQuotes:
        #         return NameError("File not found")
        # else:
        #         return NameError("Invalid Directive")
        
        
def lastTimeMod(file: str) -> float:
        """Find the most recent modified time of any file in the entire include path

        Args:
            file (str): file to search time of

        Returns:
            float: Most Recently Modified Time
        """
        includes = findIncludes(file);

        lastTime = path.getmtime(file)

        for include in includes:
                lastTime = max(lastTime, path.getmtime(include))

        return lastTime