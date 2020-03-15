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

Above that is abstraction of data in the form of dataframes and distributed arrays. 
These data structures build up their data by calling for data from the bottom layer which then accesses it's KV store. 

The top layer above that is where the application code lies so that their can be interactions with the users. What this
layer will have to do more specifically remains to be seen. 

To go further on how the system will be distributed, there will be one main node that handles the 
registration of the system. Other nodes will be made and they will register with the main node and be given 
information on how to access the other nodes in the system. This allows each node to be able to have direct 
communication with the other nodes. 

## Implementation 
Classes:  
  
Application:  
The main application class. Running this class will handle launching the entire application and setting up all other classes necessary to set up and run the application.  
  
Registrar:  
The main node that all other nodes connect to and register their addresses along with learning the addresses of the other nodes in the system.  
  
Node:  
For connecting to the registrar so that it can learn about other nodes in its network and message directly to them in the network.  
  
Dataframe:  
The top level class for data storage in the application. The dataframe will store data in row/column format.  
  
Sorer:  
The class responsible for reading in data from a .sor file into a dataframe.  



## Use Cases  
  
Creation of Dataframe:  
```
Schema builder("IFFSB");  
builder.add_row(nullptr);  
builder.add_row(nullptr);  
  
DataFrame dframe(builder);  
dframe.print();  
  
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
So far the only implementation completed is setting up the code to reuse. This includes the Object,
String, Arrays, Dataframe classes, and another group's Sorer. All of these classes have tests brought over 
as well along with a test suite for testing everything working together. 
