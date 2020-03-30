test:
	@ make runDataStructure
	@ make runDataframe
	@ make runSorer
	@ make runTrivial

#===========================================================================================================
#Data structure tests

runDataStructure:
	@ make testArray
	@ make cleanArray
	@ make testMap
	@ make cleanMap
	@ make testAdditionalMap
	@ make cleanMap

testArray:
	@ g++ -w -std=c++11 -o array test-array.cpp
	#----------------------Array Tests------------------------#
	@ ./array
	#--------------------End Array Tests----------------------#
	@ echo ""

testMap:
	@ g++ -std=c++11 -o map test-map.cpp
	#-----------------------Map Tests-------------------------#
	@ ./map
	#---------------------End Map Tests-----------------------#
	@ echo ""

testAdditionalMap:
	@ g++ -std=c++11 -o map test-additional-map.cpp
	#------------------Additional Map Tests-------------------#
	@ ./map
	#----------------End Additional Map Tests-----------------#
	@ echo ""

cleanArray:
	@ rm array

cleanMap:
	@ rm map

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
	@ make testDataframe
	@ make cleanDataframe

buildDataframe:
	@ g++ -w dataframe-test-suite.cpp -o dataframe -std=c++11

testDataframe:
	#--------------------Dataframe Tests----------------------#
	@ ./dataframe
	#------------------End Dataframe Tests--------------------#
	@ echo ""

cleanDataframe:
	@ rm -rf dataframe

#=================================================================================================================

runSorer: buildSorer
	@ make testSorer
	@ make testSorerUnitTests
	@ make cleanSorer

buildSorer:
	@ g++ sorer.cpp -o sorer -std=c++11

testSorer:
	#----------------------Sorer Tests------------------------#
	@ printf "Bool Col Test " && ./test_sor.sh "BOOL" -f "../data/data.sor" -from 0 -len 100 -print_col_type 0
	@ printf "String Col Test " && ./test_sor.sh "STRING" -f "../data/data.sor" -from 0 -len 100 -print_col_type 2
	@ printf "Is Missing Test 1 " && ./test_sor.sh "0" -f "../data/data.sor" -from 0 -len 100 -is_missing_idx 2 0
	@ printf "Is Missing Test 2 " && ./test_sor.sh "1" -f "../data/data.sor" -from 0 -len 100 -is_missing_idx 2 1
	@ printf "Is Missing Test 3 " && ./test_sor.sh "1" -f "../data/data.sor" -from 0 -len 100 -is_missing_idx 2 2
	@ printf "Print Col String Test " && ./test_sor.sh '"hi"' -f "../data/data.sor" -from 0 -len 100 -print_col_idx 2 0
	@ printf "Print Col Int Test " && ./test_sor.sh "12" -f "../data/data.sor" -from 5 -len 100 -print_col_idx 1 0
	@ printf "Print Col Float Test " && ./test_sor.sh '2.200000e+00' -f "../data/data1.sor" -print_col_idx 2 0
	@ printf "No Such Column Test " && ./test_sor.sh 'No such column' -f "../data/data1.sor" -print_col_idx 6 0
	@ printf "No Such Entry Test " && ./test_sor.sh 'No such entry' -f "../data/data1.sor" -print_col_idx 1 999
	@ printf "Non-Trivial Is Missing Test " && ./test_sor.sh '1' -f "../data/data1.sor" -is_missing_idx 1 1
	@ printf "Non-Trivial Print Index Test " && ./test_sor.sh "100" -f "../data/data2.sor" -print_col_idx 2 1
	@ printf "From Test 1 " && ./test_sor.sh "2" -f "../data/data2.sor" -from 5 -print_col_idx 2 1
	@ printf "From Test 2 " && ./test_sor.sh "2" -f "../data/data2.sor" -from 5 -print_col_idx 2 1
	@ printf "From Test 3 " && ./test_sor.sh "3" -f "../data/data2.sor" -from 5 -print_col_idx 2 2
	@ printf "Len Test " && ./test_sor.sh "No such entry" -f "../data/data2.sor" -from 5 -len 60 -print_col_idx 2 2
	@ printf "Parse String Test 1 " && ./test_sor.sh "4" -f "../data/data3.sor" -print_col_idx 3 0
	@ printf "Parse String Test 2 " && ./test_sor.sh '"string with > inside "' -f "../data/data3.sor" -print_col_idx 2 0
	#--------------------End Sorer Tests----------------------#
	@ echo ""

testSorerUnitTests:
	@ g++ test-sorer.cpp -o test-sorer -std=c++11
	#-----------------Additional Sorer Tests------------------#
	@ ./test-sorer
	#---------------End Additional Sorer Tests----------------#
	@ echo ""

cleanSorer:
	@ rm -rf sorer
	@ rm -rf test-sorer

#===================================================================================================================

runNetwork: buildNetwork
	- make testNetwork
	- make cleanNetwork

buildNetwork:
	g++ -std=c++11 -pthread -o server server.cpp
	g++ -std=c++11 -pthread -o client client.cpp

testNetwork: server client

	# start the server and the clients
	./server -ip 127.0.0.1 &
	(./client -ip 127.0.0.2 &) > file1.txt
	(./client -ip 127.0.0.3 &) > file2.txt
	pkill -f ./server


cleanNetwork:
	-rm *.o $(objects) *.txt server client

#===================================================================================================================

runTrivial:
	@ g++ -w -std=c++11 -o trivial ../src/p2main.cpp
	#------------------Trivial Application--------------------#
	@ ./trivial
	#----------------End Trivial Application------------------#
	@ echo ""
	
	@ rm trivial
