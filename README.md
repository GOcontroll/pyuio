# pyuio
A package to expose the Linux Userspace IO to python. Currently implements process_vm_readv and process_vm_writev from sys/uio.h.\
https://man7.org/linux/man-pages/man2/process_vm_readv.2.html

## installing

Download the source from the github page and run: 
```
pip3 install .
```
In the rootfolder, the package is also available on PyPI
```
pip3 install pyuio
```

## usage

There are two functions process_read and process write, read takes two required arguments (pid, asap_element) and one optional return_bytes=True/False which defaults to False. When set to True the read will return the raw bytes from the read instead of a converted value. \
Write takes three arguments (pid, asap_element, data).\
asap_element is a class contained in the module that takes 2 required arguments to initialize (address, dataType) and one optional arraySize which defaults to 1.\
The different kinds of datatypes are in the asap_datatypes class, they are: \
uint8 \
int8 \
uint16 \
int16 \
uint32 \
int32 \
uint64 \
int64 \
single (32 bit float) \
double (64 bit float)

A very simple implementation would look like this:

```
from pyuio import asap_element, asap_datatypes, process_read, process_write

address = 0x422540                  #the memory address to read from
dataType = asap_datatypes.uint16    #the value to read is an unsigned 16 bit integer
arraySize = 1                       #it is a single value and not an array

asap_dutycycle = asap_element(address, dataType, arraySize)

pid = 2842                          #automate looking up the pid of the process you would like to influence, this is just a simple example

dutycycle = process_read(pid, asap_dutycycle)
# do some work
new_dutycycle = 900
process_write(pid, asap_dutycycle, new_dutycycle)

#process_read(pid, asap_dutycycle) == 900 now
```

To read and write arrays of data just set the arraySize to the desired size and then feed the process_write an array. or receive an array from the process_read function.

Matrices are an idea for future expansion. These could be usefull for modifying 2d lookup tables for example.