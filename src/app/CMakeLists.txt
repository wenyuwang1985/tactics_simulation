add_executable(Client
    main.cpp
)

target_link_libraries(Client
    PRIVATE
        log
        config

        tactics_reader
        message_sender
        result_receiver
)
