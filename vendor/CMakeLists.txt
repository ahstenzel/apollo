add_library(liblz4 STATIC "lz4.c")
add_library(libaes STATIC "aes.c")

add_library(vendor INTERFACE)
target_link_libraries(vendor INTERFACE liblz4 libaes)