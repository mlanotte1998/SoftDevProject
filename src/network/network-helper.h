#pragma once

/**
 * Creates a serializer object for an Ack message.
 * @param sender Sender node.
 * @param target Target node.
 * @param id Message Id.
 * @return returns the serializer object for the Ack.
 */
Serializer *get_ack_serializer(size_t sender, size_t target, size_t id) {

    // Create the ack
    Ack *ack = new Ack(sender, target, 0);

    // Add the ack to the serializer
    Serializer *ack_ser = new Serializer();
    ack_ser->serialize(ack);

    delete ack;

    // Return the serializer.
    return ack_ser;

}

/**
 * Creates a serializer object for an Ack message.
 * @param sender Sender node.
 * @param target Target node.
 * @param id Message Id.
 * @param client_ip Ip of client that is attempting to register.
 * @param port Port of the client that is attempting to register.
 * @return returns the serializer object for the Ack.
 */
Serializer *get_register_serializer(size_t sender, size_t target, size_t id,
        char* client_ip, size_t port ) {

    // Create the register object.
    Register *reg = new Register(sender, target, id, client_ip, port);

    // Add the register to the serializer
    Serializer *reg_ser = new Serializer();
    reg_ser->serialize(reg);

    delete reg;

    // Return the serializer.
    return reg_ser;

}

/**
 * Deserialize a buffer and return the object.
 * @param buffer buffer with message to deserialize.
 * @return deserialized object.
 */
Object *deserialize_buffer(char *buffer) {

    // Create a string to copy the buffer to and then pass to the Serializer.
    char *serial_string = new char[1024];
    strcpy(serial_string, buffer);

    // Create the serializer object and deserialize it.
    Serializer *ser = new Serializer(reinterpret_cast<unsigned char *>(serial_string));
    Object *obj = ser->deserialize();
    delete ser;

    return obj;
}

