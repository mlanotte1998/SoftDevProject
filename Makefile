main: build
	# - make p1test
	# - make p1valgrind
	- make test
	# - make valgrind
	- make clean

build:
	docker build -t cs4500:0.1 .
	docker run -ti -v `pwd`:/src cs4500:0.1 bash -c "cd /src ;  g++ -std=c++11 -pthread -o main src/main.cpp ;"

test:
	docker run -ti -v `pwd`:/src cs4500:0.1 bash -c "cd /src ; valgrind --leak-check=full ./main"

valgrind:
	docker build -t cs4500:0.1 .
	docker run -ti -v `pwd`:/src cs4500:0.1 bash -c "cd /src ;  g++ -std=c++11 -o main src/main.cpp ; valgrind --leak-check=full ./main"

p1test:
	# Should print out 424452
	./main -f ./data/megabyte.sor -from 0

p1valgrind:
	docker build -t cs4500:0.1 .
	docker run -ti -v `pwd`:/src cs4500:0.1 bash -c "cd /src ;  g++ -std=c++11 -o main src/main.cpp ; valgrind --leak-check=full ./main -f ./data/data.sor -from 0"

clean:
	rm -rf main


