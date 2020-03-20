# EAU2 System Design Document 

## Introduction
The EAU2 system is an application that can be booted up and create a cluster to hold data. 
Data will be read using a Sorer (schema on read) based data reading system from text files. The data
for this system is read only so once data is inputted it cannot be overwritten. Queries can be made to access
the data across multiple nodes in the cluster. Being able to have fast/efficient queries is especially important
for this system. 


## Architecture 
At a high level, there are three main parts of the this system. First, there is a 
key value store at the bottom that handles management/retrieval of data. This layer also includes
networking code so that this system can be distributed across multiple nodes that each have their own memory.
Concurrency of the system will also be handled in the bottom layer. Key's will have information on not only
the key to the data that is needed, but also which node the value is stored on so that when a query is made, 
the node on which it is made knows which node it needs to communicate with to read the data. Having this home
node be known for keys speeds up the system in the long run because then if a node does not have the key in it's
own KV store then it would have to wait to hear back from every other node to see if they have the data. 

Above that is abstraction of data in the form of DataFrames and distributed arrays. 
These data structures build up their data by calling for data from the bottom layer which then accesses it's KV store. 

The top layer above that is where the application code lies so that their can be interactions with the users. What this
layer will have to do more specifically remains to be seen. 

To go further on how the system will be distributed, there will be one main node that handles the 
registration of the system. Other nodes will be made and they will register with the main node and be given 
information on how to access the other nodes in the system. This allows each node to be able to have direct 
communication with the other nodes. 

## Implementation 

#### Application:  
The main application class. Running this class will handle launching the entire application and setting up all other classes necessary to set up and run the application.  
  
#### RendezvousServer and Node:  
These two classes have the network code needed for the application. The Rendezvous Server is essentially a server meant
to sync up all Nodes in the system. Another way of describing it is by calling it a Registrar. 
When a Node is created, it needs to know the IP of the Registrar to connect to
so that the Node can learn the IP's of other Nodes in the system. When the Registrar has a new Node connect to it, 
it sends a list of the IP addresses of the other Nodes to the new one along with sending the received new IP address 
to all of the other Nodes in the system. This allows all of the Nodes to be fully aware of each other. Once a Node is
integrated in the system, it will have the IP addresses of the other Nodes so it can then send and receive messages
to/from them. 
There are also two lower level classes for abstracting the actual network behavior even more, these are the Server 
and Client classes. A Server object is used by the Registrar so that the Nodes can connect to it. The Nodes use both
Clients to send a message to the Registrar along with sending messages to other Nodes. For Nodes to be able to 
receive their messages from other Nodes they also need their own Server object also listening. Since the Nodes 
need to be simultaneously connected to the Registrar along with being able to receive/send messages, threading is 
needed to run both of these functions at the same time. 
  
#### DataFrame:  
The top level class for data storage in the application. The DataFrame will store data in row/column format. 
There are column classes specific to being used for the DataFrame, one for each of the main four data types (Boolean,
Float, Integer, and String). A DataFrame also has a Schema object inside of it that is meant to know the format
of the data along with the name of any of the rows/columns if they exist. Data can be added to a DataFrame by
inserting entire Rows or entire Columns but the data being added must stay consistent with the format of the
current data. 
  
#### Parser and Sorer Columns:  
These classes make up the Sorer functionality used to read in data from .sor files. A parser object
is created with the file to be read from, starting location, byte length, and the total file size. This object
then has functions for guessing the schema of the given file and for handling the full parsing. Once these two 
are used, the Parser object will be holding Sorer Columns that contain all of the data. These columns are aware of
missing data and are different from the columns used in the DataFrame. The data can be accessed by reading through
the columns. For this application, the data is read by first seeing how long the columns are by checking the length
of the first one (the length of all columns should be the same). Once this is found, it can be used along with the 
length of the set of columns to read the data row by row. Each column can be casted to figure out whether the data
inside is a Boolean, Float, Integer, or String. The data that is read can then be added to a DataFrame.  


## Use Cases  
  
Creation of Dataframe:  
```
Schema builder("IFFSB");  
builder.add_row(nullptr);  
builder.add_row(nullptr);  
  
DataFrame dframe(builder);  
  
Row r(builder);  
r.set(0, 1);  
r.set(1, (float)1.0);  
r.set(2, (float)2.0);  
r.set(3, new String("Hi"));  
r.set(4, true);  
dframe.add_row(r);  
  
r.set(0, 2);  
r.set(1, (float)2.0);  
r.set(2, (float)4.0);  
r.set(3, new String("Hey"));  
r.set(4, false);  
dframe.add_row(r);  

```

## Open Questions
What are the sum-dataframes used for?  
Are entire dataframes supposed to be serialized and stored in the KV store?  
If dataframes cannot be updated then why would we possibly allow K/V pairs to be overwritten?  
What did the assignment mean by saying the distributed array is a list of keys and a cache?  
What does the fromScalar function do?  
What are some example queries?  


## Status
All of the code to be reused has been brought over into the directory along with the tests for all of them.
The tests are all together in one file and can be run together with the Makefile in the directory. The code
to be reused can be split up into a few different types of classes. The first is utility classes; these include
Object, String, Array, and the other common data structure classes. The next group of classes are the classes
that are used by the DataFrame; this folder has all of the classes that were new for the DataFrame assignment.
Lastly there is a folder for sorer code that came from the group 45000NE. There is one main.cpp file in the top 
level project directory that tests out reading in a file and creating a DataFrame with it along with running
a simple map function on it. The tests have been run with valgrind to ensure that any memory issues that
were in the code for the previous assignments have been fixed. When running the main executable with valgrind
and the megabyte.sor file there is no memory leaks or memory errors. 

#TODO ADD IN ASSIGNMENT 2 Status Update and Implementation Update
