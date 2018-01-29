set(Main_Sockets_SRCS
	SafeQueue.cpp
	BufferPool.cpp
	MemWorker.cpp
	run_mythread.cpp
)

add_executable(apheleia-memthread
    ${Main_Sockets_SRCS}
)

