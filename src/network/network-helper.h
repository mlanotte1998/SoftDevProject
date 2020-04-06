#pragma once

//=============================================================
// Common helper function



Serializer* get_ack_serializer(size_t sender, size_t target, size_t id) {

  Ack *ack = new Ack(sender, target, 0);

  Serializer *ackSer = new Serializer();
  ackSer->serialize(ack);

  delete ack;

  return ackSer;

}

Object* deserialize_buffer(char* buffer) {
  char *serial_string = new char[1024];
  strcpy(serial_string, buffer);

  Serializer *ser = new Serializer(reinterpret_cast<unsigned char *>(serial_string));
  Object *obj = ser->deserialize();
  delete ser;

  return obj;
}

/**
 * Gets the ip from the command line arguments.
 * @param argc Argument count.
 * @param argv List of arguments.
 * @param ip_buffer String buffer to add ip to.
 * @return Return -1 if ip not found so it can error.
 */
int get_ip_from_arguments(int argc, char **argv, char *ip_buffer) {

    // Flags for making sure the ip was found.
    bool ip_flag_found = false;
    bool ip_found = false;

    // Loop through arguments looking for the -ip and then the string
    // after that is the ip you want.
    for (int i = 1; i < argc; i++) {

        if (argv[i][0] == '-') {
            if (strcmp(argv[i], "-ip") == 0) {
                ip_flag_found = true;
            }
        } else if (ip_flag_found) {
            strcpy(ip_buffer, argv[i]);
            ip_found = true;
            ip_flag_found = false;
        }
    }

    // If no ip found than return error int.
    if (!ip_found) {
        return -1;
    } else {
        return 0;
    }
}

/**
 * Function for storing an ip of a client in a list of ip char*'s.
 * @param ip_buffer Buffer char* with the ip
 * @param ip_list The list to move the ip over to.
 * @param index The index that the ip should be added to the list.
 * @return Returns the new char* for the ip.
 */
char *store_client_ip(char *ip_buffer, char** ip_list, int index) {
    // Create a new string for this ip and store it.
    char *new_ip = new char[30];
    strcpy(new_ip, ip_buffer);
    ip_list[index] = new_ip;
    return new_ip;
}

/**
 * Removes the -ip from the front of a char*
 * @param ip_string The char* with a -ip and an ip address after
 * @return A new char* without the -ip
 */
char *remove_ip_tag(char *ip_string) {
    char *new_ip_string = new char[1024];
    // Start looping after the -ip and copy elements into new char*.
    for (int i = 4; i < strlen(ip_string); i++) {
        new_ip_string[i - 4] = ip_string[i];
    }
    new_ip_string[strlen(ip_string) - 4] = '\0';
    return new_ip_string;
}

/**
 * Pulls the ip from a client to client message.
 * @param message The client to client message
 * @return A new char* without the -ip or the message after the ip.
 */
char *get_ip_from_client_message(char *message) {
    char *new_ip_string = new char[1024];
    int count = 4;
    // Start looping after the -ip and copy elements into new char*.
    for (count; count < strlen(message); count++) {
        if (message[count] != ' ') {
            new_ip_string[count - 4] = message[count];
        } else {
          break;
        }
    }
    new_ip_string[count + 1] = '\0';
    return new_ip_string;
}

//===============================================================================
// Server helper functions

/**
 * Creates a string of ips from a list of ips.
 * @param list_of_ips The list of known ips to the server.
 * @param size The size of the list.
 * @return Returns the string of all ips added separated by spaces.
 */
char *get_ip_list_string(char **list_of_ips, int size) {
    char *string_list = new char[1024];
    // Add -list tag to front of string.
    strcpy(string_list, "-list ");
    // Loop through list and add every ip with a space after.
    for (int i = 0; i < size; i++) {
        // Create a char* without the ip tag, add it to the main string, then delete this temp string.
        char *ip_with_no_tag = remove_ip_tag(list_of_ips[i]);
        strcat(string_list, ip_with_no_tag);
        strcat(string_list, " ");
        delete[] ip_with_no_tag;
    }
    // Finally return the string of ips.
    return string_list;
}

/**
 * Finds the given ip_to_locate in a list of ip strings
 * @param ip_list List of ips.
 * @param ip_count Number of ips aka the size of the ip list.
 * @param ip_to_locate The ip that is being searched for.
 * @return Returns the index in the list that the ip was found at.
 */
int locate_ip_in_list(char **ip_list, int ip_count, char *ip_to_locate) {
    for (int i = 0; i < ip_count; i++) {
        char* current = remove_ip_tag(ip_list[i]);
        if (strcmp(current, ip_to_locate) == 0) {
            delete [] current;
            return i;
        }
        delete [] current;
    }
    return -1;
}

//====================================================================================
// Client functions

/**
 * Takes in a string with ips seperared by spaces and adds them to a list.
 * @param ip_list_string The string from server that has all known ips.
 * @param ip_list The list to be appended to.
 * @return Returns the total count of ips in the string.
 */
int build_ip_list(char *ip_list_string, char **ip_list) {
    // Create new char* to be added to, then it is added to the list.
    char *current_ip = new char[20];
    // Keep track of count to return.
    int current_ip_count = 0;
    int character_count = 0;
    // Loop through the string
    for (int i = 6; i < strlen(ip_list_string); i++) {
        // If not a space then add to the current string.
        if (ip_list_string[i] != ' ') {
            current_ip[i - 6 - character_count] = ip_list_string[i];
        } else {
            // If a space found, end the string,
            // add it to the list
            // increase the character count of the total characters already processed.
            // and create a new current_ip.
            current_ip[i - 6 - character_count] = '\0';
            ip_list[current_ip_count] = current_ip;
            character_count += strlen(current_ip) + 1;
            current_ip = new char[20];
            current_ip_count++;
        }
    }

    // Messages always end in a space, so the final current ip char*
    // will be empty so delete it.
    delete [] current_ip;

    return current_ip_count;
}

/**
 * Handles receiving one new ip in the buffer and adding it to the known ip_list.
 * @param ip_list List of known ips.
 * @param other_ip_count Count of known ips aka the length of the ip list.
 * @param buffer A string buffer that is being passed around for handling most messages.
 */
void new_ip_received_by_client(char **ip_list, int other_ip_count, char *buffer) {
    // remove the -ip tag and add this ip to the list of ips known.
    char *ip_no_tag = remove_ip_tag(buffer);
    ip_list[other_ip_count] = ip_no_tag;
}
