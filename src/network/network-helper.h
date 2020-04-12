#pragma once

/**
 * File for utility functions used by network code
 * authors: welch.da@husky.neu.edu, lanotte.m@husky.neu.edu
 */

/**
 * Creates a serializer object for an message.
 * @param kind Kind of message
 * @param sender Sender node.
 * @param target Target node.
 * @param id Message Id.
 * @return returns the serializer object for the Message.
 */
Serializer *get_message_serializer(MsgKind kind, size_t sender, size_t target, size_t id) {

    // Create the Message
    Message *mes = new Message(kind, sender, target, id);

    // Add the message to the serializer
    Serializer *mes_ser = new Serializer();
    mes_ser->serialize(mes);

    delete mes;

    // Return the serializer.
    return mes_ser;
}

/**
 * Creates a serializer object for an Ack message.
 * @param sender Sender node.
 * @param target Target node.
 * @param id Message Id.
 * @return returns the serializer object for the Ack.
 */
Serializer *get_ack_serializer(size_t sender, size_t target, size_t id) {

    // Create the ack
    Ack *ack = new Ack(sender, target, id);

    // Add the ack to the serializer
    Serializer *ack_ser = new Serializer();
    ack_ser->serialize(ack);

    delete ack;

    // Return the serializer.
    return ack_ser;
}

/**
 * Creates a serializer object for a Directory Message.
 * @param sender Sender node.
 * @param target Target node.
 * @param id Message id.
 * @param client Client (same as sender).
 * @param ports_count Count of ports.
 * @param ports List of other nodes ports.
 * @param addresses_count Count of addresses.
 * @param addresses List of other nodes addresses.
 * @param nodes_count Count of nodes.
 * @param nodes List of other node ids.
 * @return Returns the serializer for the directory.
 */
Serializer *get_directory_serializer(size_t sender, size_t target, size_t id, size_t client,
                                     size_t ports_count, size_t *ports, size_t addresses_count,
                                     String **addresses, size_t nodes_count, size_t *nodes) {

    // Create a directory to send to the new client.
    Directory *dir = new Directory(node_, target, 9999, node_, IS_total_socket_count_, IC_port_list_,
                                   IS_total_socket_count_, IC_ip_list_, IS_total_socket_count_, IC_nodes_list_);

    // Add the directory to the serializer.
    Serializer *directory_ser = new Serializer();
    directory_ser->serialize(dir);

    delete dir;

    // Return the serializer.
    return directory_ser;
}

/**
 * Creates a serializer object for an Ack message.
 * @param sender Sender node.
 * @param target Target node.
 * @param id Message Id.
 * @param client_ip Ip of client that is attempting to register.
 * @param port Port of the client that is attempting to register.
 * @return returns the serializer object for the register.
 */
Serializer *get_register_serializer(size_t sender, size_t target, size_t id,
                                    char *client_ip, size_t port) {

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
 * Creates a serializer object for an Ack message.
 * @param sender Sender node.
 * @param target Target node.
 * @param id Message Id.
 * @param msg Status message.
 * @param port Port of the client that is attempting to register.
 * @return returns the serializer object for the status.
 */
Serializer *get_status_serializer(size_t sender, size_t target, size_t id, String *msg) {

    // Create the status object.
    Status *stat = new Status(sender, target, id, msg);

    // Add the status to the serializer
    Serializer *stat_ser = new Serializer();
    stat_ser->serialize(stat);

    delete stat;

    // Return the serializer.
    return stat_ser;

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
