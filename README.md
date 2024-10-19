# DPS - dma_buf publisher & subscriber

DPS - is a small library that implements message transport based on the publisher/subscriber pattern, 
where dma_buf is used as a transport, allowing to directly map data structures to memory 
and to implement publishers with only one write access to memory and 
subscribers with only one read access to memory per subscriber.

Selecting dma_buf allows this memory to be used by external hardware modules without the overhead of memory bandwidth and latency:
- GPU:
- - Vulkan
- - OpenGL
- - Direct Rendering Manager (DRM)
- Hardware Video Encoders/Decoders
- Image Signal Processors (e.g. ISP in raspberry pi SoC)

The public headers of the linux kernel are used as the transport layer, 
so the library is extremely low-level and its transport has no dependencies other than linux itself.

# Build Dep

Включить камеру в /boot/firmware/config.txt:

```bash
sudo apt-get install -y build-essential gdb cmake

pip3 install posix-ipc
```

https://docs.oracle.com/cd/E19476-01/821-0505/file-descriptor-requirements.html#:~:text=Linux%20systems%20limit%20the%20number,worker%20threads%20will%20be%20blocked.
