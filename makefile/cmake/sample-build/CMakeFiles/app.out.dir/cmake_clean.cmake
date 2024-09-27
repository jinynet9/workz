FILE(REMOVE_RECURSE
  "CMakeFiles/app.out.dir/foo.c.o"
  "CMakeFiles/app.out.dir/main.c.o"
  "CMakeFiles/app.out.dir/bar.c.o"
  "app.out.pdb"
  "app.out"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang C)
  INCLUDE(CMakeFiles/app.out.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
