message(STATUS "Accessing ryu folder")

add_library(ryu STATIC
common.h
d2fixed.c
d2fixed_full_table.h
d2s.c
d2s_full_table.h
d2s_intrinsics.h
d2s_small_table.h
digit_table.h
f2s.c
f2s_full_table.h
# generic_128.c
# generic_128.h
ryu.h
# ryu_generic_128.h
ryu_parse.h
s2d.c
)

create_target_directory_groups(ryu)