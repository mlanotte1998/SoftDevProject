main:
	# make valgrind
	# make clean
	- make test

demo: build
	- make test1
	- make clean

build:
	docker build -t cs4500:0.1 .
	docker run -ti -v `pwd`:/src cs4500:0.1 bash -c "cd /src ;  g++ -std=c++11 -pthread -o main src/main.cpp ;"

test1:
	docker run -ti -v `pwd`:/src cs4500:0.1 bash -c "cd /src ; valgrind --leak-check=full ./main -node 0"

test2:
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


