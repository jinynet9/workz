FILE(REMOVE_RECURSE
  "CMakeFiles/program.dir/main.o"
  "CMakeFiles/program.dir/foo.o"
  "program.pdb"
  "program"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang CXX)
  INCLUDE(CMakeFiles/program.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
