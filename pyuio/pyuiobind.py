from pyuiolib import _process_read, _process_write
import ctypes
import errno
from struct import pack, unpack

class asap_datatypes:
    uint8 = 0
    int8 = 1
    uint16 = 2
    int16 = 3
    uint32 = 4
    int32 = 5
    uint64 = 6
    int64 = 7
    single = 8
    double = 9
    dataSizes = [1,1,2,2,4,4,8,8,4,8]

class asap_element:

    address = 0
    size_element = 0
    size_t =  0
    dataType = 0

    def __init__(self, address:int, dataType: int, arraySize:int = 1):
        self.address = address
        self.dataType = dataType
        self.size_element = asap_datatypes.dataSizes[dataType]
        self.size_t = asap_datatypes.dataSizes[dataType]*arraySize

def chunks(lst, n):
    for i in range(0, len(lst), n):
        yield lst[i:i + n]

def convert_to_bytes(data, dataSize, dataType):
    array = []
    if not isinstance(data, list):
        data = [data]
    if dataType == asap_datatypes.int8:
        for unit in data:
            array += pack("b", unit)
    elif dataType == asap_datatypes.int16:
        for unit in data:
            array += pack("h", unit)
    elif dataType == asap_datatypes.int32:
        for unit in data:
            array += pack("i", unit)
    elif dataType == asap_datatypes.int64:
        for unit in data:
            array += pack("q", unit)
    elif dataType == asap_datatypes.single:
        for unit in data:
            array += pack("f", unit)
    elif dataType == asap_datatypes.double:
        for unit in data:
            array += pack("d", unit)
    else:
        for unit in data:
            array += int.to_bytes(unit, dataSize, 'little')
    return array

def convert_to_value(data, dataSize, dataType):
    dataBlocks = chunks(data, dataSize)
    array = []
    if dataType == asap_datatypes.int8 or dataType == asap_datatypes.int16 or dataType == asap_datatypes.int32 or dataType == asap_datatypes.int64:
        for unit in dataBlocks:
            array.append(int.from_bytes(bytearray(unit), 'little', signed=True))
    elif dataType == asap_datatypes.single:
        for unit in dataBlocks:
            array.append(unpack("f", bytearray(unit))[0])
    elif dataType == asap_datatypes.double:
        for unit in dataBlocks:
            array.append(unpack("d", bytearray(unit))[0])
    else:
        for unit in dataBlocks:
            array.append(int.from_bytes(bytearray(unit), 'little', signed=False))
    if len(array) == 1:
        array = array[0] 
    return array


def process_write(pid: int, asap_ele: asap_element, data):
    array = convert_to_bytes(data, asap_ele.size_element, asap_ele.dataType)
    array = (ctypes.c_uint8 * asap_ele.size_t)(*array)
    res = _process_write(pid, asap_ele.address, ctypes.addressof(array), asap_ele.size_t) #not terribly happy with how I'm passing this buffer if someone is reading this and knows a better way, please let me know.
    if res < 0:
        if res == -errno.EFAULT:
            raise Exception("The address or the buffer are not in an accessible memory location.")
        elif res == -errno.EINVAL:
            raise Exception("The required buffer size is too large.")
        elif res == -errno.ENOMEM:
            raise Exception("Could not allocate memory for the iovec structs.")
        elif res == -errno.EPERM:
            raise Exception("Insufficient permissions to access process memory space.")
        elif res == -errno.ESRCH:
            raise Exception(f"No process exists with the pid {pid}.")
        else:
            raise Exception("An exception occured of an unknown type, memory write likely failed.")

def process_read(pid: int, asap_ele: asap_element):
    array = [0]*asap_ele.size_t
    array = (ctypes.c_uint8 * asap_ele.size_t)(*array)
    res = _process_read(pid, asap_ele.address, ctypes.addressof(array), asap_ele.size_t)
    if res < 0:
        if res == -errno.EFAULT:
            raise Exception("The address or the buffer are not in an accessible memory location.")
        elif res == -errno.EINVAL:
            raise Exception("The required buffer size is too large.")
        elif res == -errno.ENOMEM:
            raise Exception("Could not allocate memory for the iovec structs.")
        elif res == -errno.EPERM:
            raise Exception("Insufficient permissions to access process memory space.")
        elif res == -errno.ESRCH:
            raise Exception(f"No process exists with the pid {pid}.")
        else:
            raise Exception("An exception occured of an unknown type, memory read likely failed.")
    return convert_to_value(array, asap_ele.size_element, asap_ele.dataType)