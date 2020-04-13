main: test

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
	g++ -std=c++11 -pthread -o main src/main.cpp && g++ -std=c++11 -pthread -o server src/main.cpp

buildWordCount:
	g++ -std=c++11 -pthread -o wordcount src/wordcount.cpp && g++ -std=c++11 -pthread -o server src/wordcount.cpp

runDemo: build
	# can hit control c after seeing the 3 wait and get finished
	# and the put finished. Then run pkill -f ./server to kill all processes
	./server -node 0 &
	(./main -node 1 &) > node1.txt
	(./main -node 2 &) > node2.txt

test:
	cd tests && make test

test-valgrind:
	cd tests && make test-valgrind

clean:
	rm -rf main
	rm -rf wordcount
	rm -rf server
	rm -rf *.txt
