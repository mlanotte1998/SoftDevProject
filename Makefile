test:
	- make testArray
	- make cleanArray
	- make testQueue
	- make cleanQueue
	- make testMap
	- make cleanMap
	- make testAdditionalMap
	- make cleanMap

testArray:
	g++ -std=c++11 -o array test-array.cpp
	#---------------Array Tests----------------#
	./array
	#-------------End Array Tests--------------#

testQueue:
	g++ -std=c++11 -o queue test-queue.cpp
	#---------------Queue Tests----------------#
	./queue
	#-------------End Queue Tests--------------#

testMap:
	g++ -std=c++11 -o map test-map.cpp
	#----------------Map Tests-----------------#
	./map
	#--------------End Map Tests---------------#

testAdditionalMap:
	g++ -std=c++11 -o map test-additional-map.cpp
	#-----------Additional Map Tests-----------#
	./map
	#---------End Additional Map Tests---------#

cleanArray:
	rm array

cleanQueue:
	rm queue

cleanMap:
	rm map
