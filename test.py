import sys, os
sys.path.append(os.path.join(".", "build"))

from includer import findIncludes

root_file = "./main.cpp"

for file in findIncludes(root_file):
        print(file)

# line = "#include <stdbool.h>"

# print(getInclude(line))