# Master server
add_executable(master 
    master/main.cpp
    master/master_server.cpp
)

target_link_libraries(master 
    Threads::Threads
    httplib
)

# Slave 1 - Letters counter
add_executable(slave_letras 
    slave_letras/main.cpp
    slave_letras/slave_letras.cpp
)

target_link_libraries(slave_letras 
    Threads::Threads
    httplib
)

# Slave 2 - Numbers counter
add_executable(slave_numeros 
    slave_numeros/main.cpp
    slave_numeros/slave_numeros.cpp
)

target_link_libraries(slave_numeros 
    Threads::Threads
    httplib
)
