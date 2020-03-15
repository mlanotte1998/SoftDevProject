test:
	- make runDataStructure
	#- make runBench
	- make runDataframe
	- make runSorer

#===========================================================================================================
#Data structure tests

runDataStructure:
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

#===============================================================================================================
# Dataframe Time Map/PMap tests
REPO = https://github.com/mlanotte1998/SoftDevA5Public.git

runBench: pullBench buildBench
	- make thousandth
	- make hundredth
	- make tenth
	- make full
	- make cleanBench

buildBench:
	g++ bench.cpp -o bench -pthread -std=c++11

cleanBench:
	rm -rf bench
	rm -r __MACOSX
	rm -rf datafile.txt

pullBench:
	git clone $(REPO) ./temp
	cp temp/datafile.zip .
	rm -rf temp
	unzip datafile.zip
	rm datafile.zip

thousandth:
	# Running Benchmark on one Thousandth of File
	- make thousandth-sum-map
	- make thousandth-sum-pmap
	- make thousandth-sumall-map
	- make thousandth-sumall-pmap

hundredth:
	# Running Benchmark on one Hundredth of File
	- make hundredth-sum-map
	- make hundredth-sum-pmap
	- make hundredth-sumall-map
	- make hundredth-sumall-pmap

tenth:
	# Running Benchmark on one Tenth of File
	- make tenth-sum-map
	- make tenth-sum-pmap
	- make tenth-sumall-map
	- make tenth-sumall-pmap

full:
	# Running Benchmark on Full File
	- make full-sum-map
	- make full-sum-pmap
	- make full-sumall-map
	- make full-sumall-pmap

thousandth-sum-pmap:
	# Single Column Sum on one thousandth of File Using PMAP
	./bench datafile.txt sum pmap 1000

thousandth-sum-map:
	# Single Column Sum on one thousandth of File Using MAP
	./bench datafile.txt sum map 1000

thousandth-sumall-pmap:
	# All Column Sum on one thousandth of File Using PMAP
	./bench datafile.txt sumall pmap 1000

thousandth-sumall-map:
	# All Column Sum on one thousandth of File Using MAP
	./bench datafile.txt sumall map 1000

hundredth-sum-pmap:
	# Single Column Sum on one hundredth of File Using PMAP
	./bench datafile.txt sum pmap 10000

hundredth-sum-map:
	# Single Column Sum on one hundredth of File Using MAP
	./bench datafile.txt sum map 10000

hundredth-sumall-pmap:
	# All Column Sum on one hundredth of File Using PMAP
	./bench datafile.txt sumall pmap 10000

hundredth-sumall-map:
	# All Column Sum on one hundredth of File Using MAP
	./bench datafile.txt sumall map 10000

tenth-sum-pmap:
	# Single Column Sum on one tenth of File Using PMAP
	./bench datafile.txt sum pmap 100000

tenth-sum-map:
	# Single Column Sum on one tenth of File Using MAP
	./bench datafile.txt sum map 100000

tenth-sumall-pmap:
	# All Column Sum on one tenth of File Using PMAP
	./bench datafile.txt sumall pmap 100000

tenth-sumall-map:
	# All Column Sum on one tenth of File Using MAP
	./bench datafile.txt sumall map 100000

full-sum-pmap:
	# Single Column Sum on Full File Using PMAP
	./bench datafile.txt sum pmap

full-sum-map:
	# Single Column Sum on Full File Using MAP
	./bench datafile.txt sum map

full-sumall-pmap:
	# All Column Sum on Full File Using PMAP
	./bench datafile.txt sumall pmap

full-sumall-map:
	# All Column Sum on Full File Using MAP
	./bench datafile.txt sumall map

#===============================================================================================================
# Dataframe Accuracy Tests

runDataframe: buildDataframe
	- make testDataframe
	- make cleanDataframe

buildDataframe:
	g++ dataframe-test-suite.cpp -o dataframe -std=c++11

testDataframe:
	./dataframe

cleanDataframe:
	rm -rf dataframe

#=================================================================================================================

runSorer: buildSorer
	- make testSorer
	- make testSorerUnitTests
	- make cleanSorer

buildSorer:
	g++ sorer.cpp -o sorer -std=c++11

testSorer:
	# Basics tests
	./test_sor.sh "BOOL" -f "data.sor" -from 0 -len 100 -print_col_type 0
	./test_sor.sh "STRING" -f "data.sor" -from 0 -len 100 -print_col_type 2
	./test_sor.sh "0" -f "data.sor" -from 0 -len 100 -is_missing_idx 2 0
	./test_sor.sh "1" -f "data.sor" -from 0 -len 100 -is_missing_idx 2 1
	./test_sor.sh "1" -f "data.sor" -from 0 -len 100 -is_missing_idx 2 2
	./test_sor.sh '"hi"' -f "data.sor" -from 0 -len 100 -print_col_idx 2 0
	./test_sor.sh "12" -f "data.sor" -from 5 -len 100 -print_col_idx 1 0
	./test_sor.sh '2.200000' -f "data1.sor" -print_col_idx 2 0
	# Error conditions
	./test_sor.sh 'No such column' -f "data1.sor" -print_col_idx 6 0
	./test_sor.sh 'No such entry' -f "data1.sor" -print_col_idx 1 999
	# More complicated data
	./test_sor.sh '1' -f "data1.sor" -is_missing_idx 1 1
	./test_sor.sh "100" -f "data2.sor" -print_col_idx 2 1
	# Check that first line gets cut off with -from
	./test_sor.sh "2" -f "data2.sor" -from 5 -print_col_idx 2 1
	./test_sor.sh "2" -f "data2.sor" -from 5 -print_col_idx 2 1
	./test_sor.sh "3" -f "data2.sor" -from 5 -print_col_idx 2 2
	# Check last line cut off with -len
	./test_sor.sh "No such entry" -f "data2.sor" -from 5 -len 60 -print_col_idx 2 2
	# String parsing
	./test_sor.sh "4" -f "data3.sor" -print_col_idx 3 0
	./test_sor.sh '"string with > inside "' -f "data3.sor" -print_col_idx 2 0

testSorerUnitTests:
	g++ test-sorer.cpp -o test-sorer -std=c++11
	./test-sorer

cleanSorer:
	rm -rf sorer
	rm -rf test-sorer