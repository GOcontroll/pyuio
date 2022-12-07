# pyuio
A package to expose the Linux Userspace IO to python. Currently implements process_vm_readv and process_vm_writev from sys/uio.h.

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

There are two functions process_read and process write, read takes two parameters (pid, asap_element) and write takes three (pid, asap_element, data).\
asap_element is a class contained in the module that takes 3 parameters to initialize (address, dataType, arraySize).\
The different kinds of datatypes are in the asap_datatypes class.

A very simple implementation would look like this:

```
from pyuio import asap_element, asap_datatypes, process_read, process_write

address = 0x422540                  #the memory address to read from
dataType = asap_datatypes.uint16    #the value to read is a unsigned 16 bit integer
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