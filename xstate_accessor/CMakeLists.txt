aux_source_directory(./src xsa_src)

add_library(xstate_accessor STATIC ${xsa_src})
