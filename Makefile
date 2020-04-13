main:
	# make valgrind
	# make clean
	- make test

demo: build
	- make runNode0
	- make clean

# Right now we do not have a good way at running the Demo with one command.
# The way it can be done is to run these commands:
# g++ -std=c++11 -pthread -o main src/main.cpp
# ./main -node 3
# ./main -node 0
# ./main -node 1
# Then wait for the node 1 and node 0 to finish their back and forth
# ./main -node 2
# More details of this in the memo
build:
	docker build -t cs4500:0.1 .
	docker run -ti -v `pwd`:/src cs4500:0.1 bash -c "cd /src ;  g++ -std=c++11 -pthread -o main src/main.cpp ;"

test:
	cd tests && make test

test-valgrind:
	cd tests && make test-valgrind

clean:
	rm -rf main


