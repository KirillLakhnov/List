all: main.cpp list.cpp
	g++ main.cpp list.cpp -o all_list

release: main.cpp list.cpp
	g++ main.cpp list.cpp -o list_release

debug: main.cpp list.cpp
	g++ main.cpp list.cpp -DNDEBUG -o list_debug

