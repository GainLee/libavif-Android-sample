cmake_minimum_required(VERSION 3.7)

project(avif_sample)

include_directories(include)

#导入静态库
add_library(libgav1 SHARED IMPORTED)
set_target_properties(libgav1 PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/jniLibs/${ANDROID_ABI}/libgav1.a )

add_library(libaom SHARED IMPORTED)
set_target_properties(libaom PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/jniLibs/${ANDROID_ABI}/libaom.a )

add_library(libavif SHARED IMPORTED)
set_target_properties(libavif PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/jniLibs/${ANDROID_ABI}/libavif.a )

add_library("avif_sample" SHARED "libavif_jni.cc")

target_link_libraries(avif_sample jnigraphics log)

target_link_libraries(avif_sample
        "-Wl,--whole-archive"
        libgav1
        "-Wl,--no-whole-archive"
        )

target_link_libraries(avif_sample
        "-Wl,--whole-archive"
        libaom
        "-Wl,--no-whole-archive"
        )

target_link_libraries(avif_sample
        "-Wl,--whole-archive"
        libavif
        "-Wl,--no-whole-archive"
        )