rmdir -Recurse build
cmake -S. -Bbuild
cmake --build build
rmdir -Recurse .\search_engine