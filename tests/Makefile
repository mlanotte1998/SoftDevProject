test:
	@ make runDataStructure
	@ make runDataFrame
	@ make runSerialize
	# cant run store tests now that network functionality is there
	# @ make runStore
	@ make runSorer

test-valgrind:
	@ make runDataStructureValgrind
	@ make runDataFrameValgrind
	@ make runSerializeValgrind
	# cant run store tests now that network functionality is there
    # @ make runStoreValgrind
	@ make runSorerValgrind

#===========================================================================================================
#Data structure tests

runDataStructure:
	@ make testArray
	@ make cleanArray
	@ make testMap
	@ make cleanMap
	@ make testAdditionalMap
	@ make cleanMap

runDataStructureValgrind:
	@ make testArrayValgrind
	@ make cleanArray
	@ make testMapValgrind
	@ make cleanMap
	@ make testAdditionalMapValgrind
	@ make cleanMap


testArray:
	@ g++ -w -std=c++11 -o array test-array.cpp
	#----------------------Array Tests------------------------#
	@ ./array
	#--------------------End Array Valgrind Tests----------------------#
	@ echo ""

testArrayValgrind :
	@ g++ -w -std=c++11 -o array test-array.cpp
	#----------------------Array Valgrind Tests------------------------#
	@ valgrind ./array
	#--------------------End Array Valgrind Tests----------------------#
	@ echo ""

testMap:
	@ g++ -std=c++11 -o map test-map.cpp
	#-----------------------Map Tests-------------------------#
	@ ./map
	#---------------------End Map Tests-----------------------#
	@ echo ""

testMapValgrind:
	@ g++ -std=c++11 -o map test-map.cpp
	#-----------------------Map Valgrind Tests-------------------------#
	@ valgrind ./map
	#---------------------End Map Valgrind Tests-----------------------#
	@ echo ""

testAdditionalMap:
	@ g++ -std=c++11 -o map test-additional-map.cpp
	#------------------Additional Map Tests-------------------#
	@ ./map
	#----------------End Additional Map Tests-----------------#
	@ echo ""

testAdditionalMapValgrind:
	@ g++ -std=c++11 -o map test-additional-map.cpp
	#------------------Additional Map Valgrind Tests-------------------#
	@ valgrind ./map
	#----------------End Additional Map Valgrind Tests-----------------#
	@ echo ""

cleanArray:
	@ rm array

cleanMap:
	@ rm map

#=======================================================================================================

# Dataframe Accuracy Tests

runDataFrame: buildDataframe
	@ make testDataframe
	@ make cleanDataframe

runDataFrameValgrind: buildDataframe
	@ make testDataframeValgrind
	@ make cleanDataframe

buildDataframe:
	@ g++ -w dataframe-test-suite.cpp -o dataframe -std=c++11

testDataframe:
	#--------------------Dataframe Tests----------------------#
	@ ./dataframe
	#------------------End Dataframe Tests--------------------#
	@ echo ""

testDataframeValgrind:
	#--------------------Dataframe Valgrind Tests----------------------#
	@ valgrind ./dataframe
	#------------------End Dataframe Valgrind Tests--------------------#
	@ echo ""

cleanDataframe:
	@ rm -rf dataframe

#===============================================================================================================
# Serialization Tests

runSerialize: buildSerialize
	@ make testSerialize
	@ make cleanSerialize

runSerializeValgrind: buildSerialize
	@ make testSerializeValgrind
	@ make cleanSerialize

buildSerialize:
	@ g++ -w serialize_tests.cpp -o serialize -std=c++11

testSerialize:
	#--------------------Serialize Tests----------------------#
	@ ./serialize
	#------------------End Serialize Tests--------------------#
	@ echo ""

testSerializeValgrind:
	#--------------------Serialize Valgrind Tests----------------------#
	@ valgrind ./serialize
	#------------------End Serialize Valgrind Tests--------------------#
	@ echo ""

cleanSerialize:
	@ rm -rf serialize

#===============================================================================================================
# Store Tests

runStore: buildStore
	@ make testStore
	@ make cleanStore

runStoreValgrind: buildStore
	@ make testStoreValgrind
	@ make cleanStore

buildStore:
	@ g++ -w test-store.cpp -o store -std=c++11

testStore:
	#--------------------Store Tests----------------------#
	@ ./store
	#------------------End Store Tests--------------------#
	@ echo ""

testStoreValgrind:
	#--------------------Store Valgrind Tests----------------------#
	@ valgrind ./store
	#------------------End Store Valgrind Tests--------------------#
	@ echo ""

cleanStore:
	@ rm -rf store

#=================================================================================================================

runSorer: buildSorer
	@ make testSorer
	@ make testSorerUnitTests
	@ make cleanSorer

runSorerValgrind: buildSorer
	@ make testSorerUnitTestsValgrind
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

testSorerUnitTestsValgrind :
	@ g++ test-sorer.cpp -o test-sorer -std=c++11
	#-----------------Additional Sorer Valgrind Tests------------------#
	@ valgrind ./test-sorer
	#---------------End Additional Sorer Valgrind Tests----------------#
	@ echo ""

cleanSorer:
	@ rm -rf sorer
	@ rm -rf test-sorer
