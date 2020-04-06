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


runNode0:
	docker run -ti -v `pwd`:/src cs4500:0.1 bash -c "cd /src ; valgrind --leak-check=full ./main -node 0"

runNode1:
	docker run -ti -v `pwd`:/src cs4500:0.1 bash -c "cd /src ; valgrind --leak-check=full ./main -node 1"

test:
	cd tests && make test

valgrind:
	docker build -t cs4500:0.1 .
	docker run -ti -v `pwd`:/src cs4500:0.1 bash -c "cd /src ;  g++ -std=c++11 -o main src/main.cpp ; valgrind --leak-check=full ./main"

p1test:
	# Should print out 424452
	./main -f ./data/megabyte.sor -from 0

p2test:
	cd src && g++ -std=c++11 -pthread -o main p2main.cpp && ./main && rm main

p1valgrind:
	docker build -t cs4500:0.1 .
	docker run -ti -v `pwd`:/src cs4500:0.1 bash -c "cd /src ;  g++ -std=c++11 -o main src/main.cpp ; valgrind --leak-check=full ./main -f ./data/data.sor -from 0"

clean:
	rm -rf main


