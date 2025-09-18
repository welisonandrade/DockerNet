# Client application
add_executable(client 
    main.cpp
    client_app.cpp
)

target_link_libraries(client 
    Threads::Threads
    httplib
)
