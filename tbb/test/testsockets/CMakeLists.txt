file(GLOB SRCS *.cpp)

ADD_EXECUTABLE(testsockets ${SRCS})

TARGET_LINK_LIBRARIES(testsockets
    libgtest
    libgmock
)

add_test(NAME testsockets
         COMMAND testsockets)
