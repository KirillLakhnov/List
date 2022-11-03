all: List/main.cpp List/list.cpp
	g++ List/main.cpp List/list.cpp -o all_list

debug: List/main.cpp List/list.cpp
	g++ List/main.cpp List/list.cpp -DNDEBUG -o list_debug

list_p: List_pointer/main.cpp List_pointer/list_pointer.cpp
	g++ List_pointer/main.cpp List_pointer/list_pointer.cpp -o list_p

