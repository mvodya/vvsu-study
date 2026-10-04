add_executable(generate_lab_04_data
    main.cpp
)

target_compile_features(generate_lab_04_data PRIVATE cxx_std_23)

target_compile_definitions(generate_lab_04_data
    PRIVATE
        CPP_COURSE_LABS_DIR="${CPP_COURSE_LABS_DIR}"
)
