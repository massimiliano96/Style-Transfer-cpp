add_subdirectory(style-transfer)

include_directories(${onnxruntime_INCLUDE_DIRS}/onnxruntime/include)

add_executable(${EXE_NAME} 
    main.cpp
    ${STYLE_TRANSFER_SOURCES}
)

target_link_libraries(${EXE_NAME}
    ${onnxruntime_LIBRARY}
    ${OpenCV_LIBS}
)

