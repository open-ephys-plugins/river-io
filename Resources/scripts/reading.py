import river

# Create the Reader and then initialize it with the stream we want to read from.
r = river.StreamReader(river.RedisConnection("127.0.0.1", 6379))

# The initialize() call accepts a timeout in milliseconds for the maximum amount
# of time to wait for the stream to be created, if it is not already.
r.initialize("Red-769", 10000)

# Here, we'll read one sample at a time, and print it out. The number of samples
# read per invocation is decided by the size of the buffer passed in, so in this
# case, we create a new empty buffer with `new_buffer` (again, syntactic sugar for
# creating a numpy array with appropriate dtype). In real use cases, you should
# read as many samples per call as your latency/system tolerates to amoritze overhead
# of each call, i.e. you should use a buffer with many more samples.
data = r.new_buffer(1)

# Like with the writer, you can use a context manager to auto-stop the reader after
# you're done.
with r:
  while True:
    # Similar to the style of many I/O streams, we pass in a buffer that will be
    # filled with read data when available. In this case, since `data` is of size
    # 1, at most 1 sample will be read from the stream at a time. The second parameter
    # is the timeout in milliseconds: the max amount of time this call will block until
    # the given number of samples is available. In this case, it's 100ms max.
    #
    # The return value returns the number of samples read into the buffer. It should always
    # be checked. -1 is returned once EOF is encountered.
    num_read = r.read(data, 100)
    if num_read > 0:
      print(f"Spike on electrode {data['channel_index']} unit {data['unit_index']} at {data['sample_number']}")
    elif num_read == 0:
      continue
    else:
      print('EOF encountered for stream', r.stream_name)
      break
