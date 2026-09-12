add_executable(md_to_pdf
    document_converter.cpp
    main.cpp
    pdf_builder.cpp
)

target_compile_features(md_to_pdf PRIVATE cxx_std_23)
