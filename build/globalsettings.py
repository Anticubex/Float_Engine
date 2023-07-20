from os import path
from typing import Tuple, List

sourceTypes = (".cpp", ".c")

# sourcedir = path.abspath(os.getcwd())
sourcedir: str = "."

objdir: str = path.join(sourcedir, "build", "obj")

out: str = "main"
cxx: str = "g++"
pflags: str = "-std=c++17"
flags: str = "-Wall -lmingw32 -lSDL2main -lSDL2"

Dynamic_Includes_Raw: List[str] = ["/src/core"]
Static_Includes_Raw: List[str] = ["/SDL2/include"] # Static includes remain unchecked

Dynamic_Includes_Raw = list(map(lambda d : f"{sourcedir}{d}", Dynamic_Includes_Raw))
Static_Includes_Raw = list(map(lambda d : f"{sourcedir}{d}", Static_Includes_Raw))

Dynamic_Includes: Tuple[str] = tuple(Dynamic_Includes_Raw)
Static_Includes: Tuple[str] = tuple(Static_Includes_Raw)

Includes = Dynamic_Includes + Static_Includes

Libs_Raw: List[str] = ["/SDL2/lib"]

Libs_Raw = list(map(lambda d : f"{sourcedir}{d}", Libs_Raw))

Libs: Tuple[str] = tuple(Libs_Raw)

lflags: str = ""     

for I in Includes:
        lflags += f"-I{I} "

for L in Libs:
        lflags += f"-L{L} "

# print(Libs, lflags)