set(COMPONENT_SRCS 
		"src/jbutilities/Crc.cpp"
		"src/jbutilities/des.c"
		"src/jbutilities/Fifo.cpp"
		"src/jbutilities/InterfaceChecker.cpp"
		"src/jbutilities/md5.cpp"
		"src/jbutilities/RingBuffer.cpp"
		"src/jbutilities/SimpleFifo.cpp")
set(COMPONENT_ADD_INCLUDEDIRS 
		"include")
		
set(COMPONENT_REQUIRES jblib-platform-abstract-jbkernel)

register_component()