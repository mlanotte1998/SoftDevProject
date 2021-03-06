#include <string.h>
#include <ctype.h>
#include <time.h>
#include "../src/application/application.h"
#include "../src/dataframe/dataframe.h"
#include "../src/seriazation_and_messages/serial.h"

// test serializing and deserializing a bool array
void serializeBoolArray() {
	// create bool column
	BoolColumn* boolCol = new BoolColumn();
	boolCol->push_back(0);
	boolCol->push_back(1);
	boolCol->push_back(0);
	boolCol->push_back(1);
	boolCol->push_back(1);

	// serialize bool column
	Serializer* boolSer = new Serializer();
	char* serialized_bool_col = boolSer->serialize(boolCol);

	// // deserialize bool column into second bool column object
	BoolColumn* boolCol2 = dynamic_cast<BoolColumn*>(boolSer->deserialize());

	// two bool columns should be equal
	assert(boolCol->array_length_ == boolCol2->array_length_);
	assert(boolCol->size_ == boolCol2->size_);
	for (size_t i = 0; i < boolCol->size_; i++) {
		assert(boolCol->get(i) == boolCol2->get(i));
	}

	printf("Passed Serialized Bool Column Test\n");

	delete boolCol;
	delete boolCol2;
	delete boolSer;
}

// test serializing and deserializing an int array
void serializeIntArray() {
	// create int column
	IntColumn* intCol = new IntColumn();
	intCol->push_back(1);
	intCol->push_back(22);
	intCol->push_back(333);
	intCol->push_back(4444);
	intCol->push_back(55555);

	// serialize int column
	Serializer* intSer = new Serializer();
	char* serialized_int_col = intSer->serialize(intCol);

	// deserialize int column into second int column object
	IntColumn* intCol2 = dynamic_cast<IntColumn*>(intSer->deserialize());

	// two int columns should be equal
	assert(intCol->array_length_ == intCol2->array_length_);
	assert(intCol->size_ == intCol2->size_);
	for (size_t i = 0; i < intCol->size_; i++) {
		assert(intCol->get(i) == intCol2->get(i));
	}

	printf("Passed Serialized Int Column Test\n");

	delete intCol;
	delete intCol2;
	delete intSer;
}

// test serializing and deserializing a float array
void serializeFloatArray() {
	// create float column
	FloatColumn* floatCol = new FloatColumn();
	floatCol->push_back(1.1);
	floatCol->push_back(22.22);
	floatCol->push_back(333.3);
	floatCol->push_back(4444.44);
	floatCol->push_back(55555.5);

	// serialize float column
	Serializer* floatSer = new Serializer();
	char* serialized_float_col = floatSer->serialize(floatCol);

	// deserialize float column into second float column object
	FloatColumn* floatCol2 = dynamic_cast<FloatColumn*>(floatSer->deserialize());

	// two float columns should be equal
	assert(floatCol->array_length_ == floatCol2->array_length_);
	assert(floatCol->size_ == floatCol2->size_);
	for (size_t i = 0; i < floatCol->size_; i++) {
		assert(floatCol->get(i) == floatCol2->get(i));
	}

	printf("Passed Serialized Float Column Test\n");

	delete floatCol;
	delete floatCol2;
	delete floatSer;
}

// test serializing and deserializing a double array
void serializeDoubleArray() {
	// create double column
	DoubleColumn* doubleCol = new DoubleColumn();
	doubleCol->push_back(1.100000);
	doubleCol->push_back(22.220000);
	doubleCol->push_back(333.300000);
	doubleCol->push_back(4444.440000);
	doubleCol->push_back(55555.500000);

	// serialize double column
	Serializer* doubleSer = new Serializer();
	char* serialized_double_col = doubleSer->serialize(doubleCol);

	// deserialize double column into second double column object
	DoubleColumn* doubleCol2 = dynamic_cast<DoubleColumn*>(doubleSer->deserialize());

	// two double columns should be equal
	assert(doubleCol->array_length_ == doubleCol2->array_length_);
	assert(doubleCol->size_ == doubleCol2->size_);
	for (size_t i = 0; i < doubleCol->size_; i++) {
		assert(doubleCol->get(i) == doubleCol2->get(i));
	}

	printf("Passed Serialized Double Column Test\n");

	delete doubleCol;
	delete doubleCol2;
	delete doubleSer;
}

// test serializing and deserializing a string array
void serializeStringArray() {
	// create string column
	StringColumn* stringCol = new StringColumn();
	String* hello = new String("Hello");
	String* world = new String("World");
	String* my_name = new String("My name is: N/A");
	String* how_are_you = new String("Hi, how are you");
	String* quoted = new String("\"Quoted\"");
	stringCol->push_back(hello);
	stringCol->push_back(world);
	stringCol->push_back(my_name);
	stringCol->push_back(how_are_you);
	stringCol->push_back(quoted);

	// serialize string column
	Serializer* stringSer = new Serializer();
	char* serialized_string_col = stringSer->serialize(stringCol);

	// deserialize string column into second string column object
	StringColumn* stringCol2 = dynamic_cast<StringColumn*>(stringSer->deserialize());

	// two string columns should be equal
	assert(stringCol->array_length_ == stringCol2->array_length_);
	assert(stringCol->size_ == stringCol2->size_);
	assert(stringCol->get(0)->equals(stringCol2->get(0)));
	for (size_t i = 0; i < stringCol->size_; i++) {
		assert(stringCol->get(i)->equals(stringCol2->get(i)));
	}

	printf("Passed Serialized String Column Test\n");

	delete stringCol;
	delete stringCol2;
	delete stringSer;
	delete hello;
	delete world;
	delete my_name;
	delete how_are_you;
	delete quoted;
}

// test serializing and deserializing a message object
void serializeMessage() {
	// create message
	Message* message1 = new Message(ACK, 10, 11, 1234);

	// serialize message
	Serializer* messageSer = new Serializer();
	char* serialized_message = messageSer->serialize(message1);

	// deserialize message into second message object
	Message* message2 = dynamic_cast<Message*>(messageSer->deserialize());

	// two messages should be equal
	assert(message1->kind_ == message2->kind_);
	assert(message1->sender_ == message2->sender_);
	assert(message1->target_ == message2->target_);
	assert(message1->id_ == message2->id_);

	printf("Passed Serialized Message Test\n");

	delete message1;
	delete message2;
	delete messageSer;
}

// test serializing and deserializing an ack message
void serializeAck() {
	// create ack message
	Ack* ack1 = new Ack(8, 9, 1111);

	// serialize ack message
	Serializer* ackSer = new Serializer();
	char* serialized_ack = ackSer->serialize(ack1);

	// deserialize ack message into second ack message object
	Ack* ack2 = dynamic_cast<Ack*>(ackSer->deserialize());

	// two ack messages should be equal
	assert(ack1->sender_ == ack2->sender_);
	assert(ack1->target_ == ack2->target_);
	assert(ack1->id_ == ack2->id_);

	printf("Passed Serialized Ack Test\n");

	delete ack1;
	delete ack2;
	delete ackSer;
}

// test serializing and deserializing a status message
void serializeStatus() {
	// create status message
	Status* status1 = new Status(8, 9, 1111, new String("Marco Polo"));

	// serialize status message
	Serializer* statusSer = new Serializer();
	char* serialized_status = statusSer->serialize(status1);

	// deserialize status message into second ack message object
	Status* status2 = dynamic_cast<Status*>(statusSer->deserialize());

	// two status messages should be equal
	assert(status1->sender_ == status2->sender_);
	assert(status1->target_ == status2->target_);
	assert(status1->id_ == status2->id_);
	assert(status1->msg_->equals(status2->msg_));

	printf("Passed Serialized Status Test\n");

	delete status1;
	delete status2;
	delete statusSer;
}

// test serializing and deserializing a register message
void serializeRegister() {

	// create register message
	Register* register1 = new Register(8, 9, 1111, "127.0.0.1", 8000);

	// serialize register message
	Serializer* registerSer = new Serializer();
	char* serialized_register = registerSer->serialize(register1);

	// deserialize status message into second ack message object
	Register* register2 = dynamic_cast<Register*>(registerSer->deserialize());

	// two register messages should be equal
	assert(register1->kind_ == register2->kind_);
	assert(register1->sender_ == register2->sender_);
	assert(register1->target_ == register2->target_);
	assert(register1->id_ == register2->id_);
	assert(register1->client_ip_->equals(register2->client_ip_));
	assert(register1->port_ == register2->port_);

	printf("Passed Serialized Register Test\n");

	delete register1;
	delete register2;
	delete registerSer;
}

// test serializing and deserializing a directory message
void serializeDirectory() {
	size_t ports[3];
	ports[0] = 1;
	ports[1] = 22;
	ports[2] = 333333;

	String* addresses[4];
	String* one = new String("One");
	String* two = new String("Two");
	String* three = new String("Three");
	String* four = new String("Four");

	size_t nodes[5];
	nodes[0] = 0;
	nodes[1] = 1;
	nodes[2] = 2;
	nodes[3] = 3;
	nodes[4] = 6;


	addresses[0] = one;
	addresses[1] = two;
	addresses[2] = three;
	addresses[3] = four;

	// create directory message
	Directory* directory1 = new Directory(8, 9, 1111, 2222, 3, ports, 4, addresses, 5, nodes);

	// serialize directory message
	Serializer* directorySer = new Serializer();
	char* serialized_directory = directorySer->serialize(directory1);

	// deserialize directory message into second directory message object
	Directory* directory2 = dynamic_cast<Directory*>(directorySer->deserialize());

	// two directory messages should be equal
	assert(directory1->sender_ == directory2->sender_);
	assert(directory1->target_ == directory2->target_);
	assert(directory1->id_ == directory2->id_);
	assert(directory1->client_ == directory2->client_);
	assert(directory1->ports_count_ == directory2->ports_count_);
	assert(directory1->addresses_count_ == directory2->addresses_count_);
	for (size_t i = 0; i < 3; i++) {
		assert(directory1->ports_[i] == directory2->ports_[i]);
	}
	for (size_t j = 0; j < 4; j++) {
		assert(directory1->addresses_[j]->equals(directory2->addresses_[j]));
	}
	for (size_t i = 0; i < 5; i++) {
		assert(directory1->nodes_[i] == directory2->nodes_[i]);
	}

	printf("Passed Serialized Directory Test\n");

	delete one;
	delete two;
	delete three;
	delete four;
	delete directory1;
	delete directory2;
	delete directorySer;
}

// non-trivial test of serializing and deserializing bool array
void serializeDeserializeBoolArray() {
	// create bool column
	BoolColumn* boolCol = new BoolColumn();
	boolCol->push_back(0);
	boolCol->push_back(1);
	boolCol->push_back(0);
	boolCol->push_back(1);
	boolCol->push_back(1);

	// serialize bool column
	Serializer* boolSer = new Serializer();
	char* serialized_bool_col = boolSer->serialize(boolCol);

	// // deserialize bool column into second bool column object
	Serializer* boolSer2 = new Serializer();
	BoolColumn* boolCol2 = dynamic_cast<BoolColumn*>(boolSer->deserialize());
	char* serialized_bool_col2 = boolSer2->serialize(boolCol2);

	// two bool columns should be equal
	assert(boolCol->array_length_ == boolCol2->array_length_);
	assert(boolCol->size_ == boolCol2->size_);
	for (size_t i = 0; i < boolCol->size_; i++) {
		assert(boolCol->get(i) == boolCol2->get(i));
	}

	printf("Passed Serialize Deserialize Bool Array Test\n");

	delete boolCol;
	delete boolCol2;
	delete boolSer;
	delete boolSer2;
}

// main function
int main(int argc, char **argv) {
	serializeBoolArray();
	serializeIntArray();
	serializeFloatArray();
	serializeDoubleArray();
	serializeStringArray();
	serializeMessage();
	serializeAck();
	serializeStatus();
	serializeRegister();
	serializeDirectory();
	serializeDeserializeBoolArray();

	return 0;
}
