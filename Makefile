main: test

buildDemo:
	g++ -std=c++11 -pthread -o demoNode src/demo.cpp && g++ -std=c++11 -pthread -o demoServer src/demo.cpp


buildWordCount:
	g++ -std=c++11 -pthread -o wordcount src/wordcount.cpp && g++ -std=c++11 -pthread -o server src/wordcount.cpp


# Should run fine in 10 seconds.
# Currently occasionaly (maybe 1 in 10 times) one of the processes get stuck
# and there is not an obvious valgrind issue coming up that seems to be leading to
# why and when trying to output stuff to standard out to nail down why it happens
# it would not happen even after 20 times of running it. It seems like
# node1 is getting stuck during a put but why I am unsure of.

# Recently ran the runDemo command 15 times in a row without fail after adding in
# a check of whether something is an ack or a register incase the threading caused
# issues with receiving messages in order and have not had a problem after doing that
# so hopeuflly that was the issue.
runDemo: buildDemo
	./demoServer -node 0 &
	(./demoNode -node 1 &) > node1.txt
	(./demoNode -node 2 &) > node2.txt

	echo "Wait 10 seconds for program to run before killing"
	sleep 10
	pkill -f ./demoServer
	echo "Cat both text files to show their outputs"
	cat node1.txt
	cat node2.txt
	- make cleanDemo

test:
	cd tests && make test

test-valgrind:
	cd tests && make test-valgrind

cleanDemo:
	rm -rf demoNode
	rm -rf demoServer
	rm -rf *.txt

cleanWordCount:
	rm -rf wordcount
	rm -rf server
	rm -rf *.txt
