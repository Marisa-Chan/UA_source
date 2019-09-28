CC = gcc
CXX = g++

lib2ndnet.a: client.o client_threads.o server.o server_session.o server_threads.o single.o single_threads.o zndNet.o
	ar rcs ../$@ $^

client.o: client.cpp
	$(CXX) -fPIC -c -o $@ $<

client_threads.o: client_threads.cpp
	$(CXX) -fPIC -c -o $@ $<

server.o: server.cpp
	$(CXX) -fPIC -c -o $@ $<

server_session.o: server_session.cpp
	$(CXX) -fPIC -c -o $@ $<

server_threads.o: server_threads.cpp
	$(CXX) -fPIC -c -o $@ $<

single.o: single.cpp
	$(CXX) -fPIC -c -o $@ $<

single_threads.o: single_threads.cpp
	$(CXX) -fPIC -c -o $@ $<

zndNet.o: zndNet.cpp
	$(CXX) -fPIC -c -o $@ $<

clean:
	rm *.o

all: lib2ndnet.a
	