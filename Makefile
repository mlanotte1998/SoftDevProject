main: build
	- make test
	- make valgrind

build:
	g++ -std=c++11 -o main ./src/main.cpp

test:
	# Should print out 424452
	./main -f ./data/megabyte.sor -from 0

valgrind:
	docker build -t cs4500:0.1 .
	docker run -ti -v `pwd`:/src cs4500:0.1 bash -c "cd /src ;  g++ -std=c++11 -o main src/main.cpp ; valgrind --leak-check=full ./main -f ./data/data.sor -from 0"
clean:
	rm -rf main

cleanQueue:
	rm queue


