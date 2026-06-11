<h1> 1 - INTRO </h1>
Single producer Single consumer Queue is implemented in .hpp file so to use it, all that needs to be done is to place it in conveniet location.

<h1> 2 - TESTS </h1>
To build tets follow code sequence below

```bash
git submodule update --init --recursive
mkdir build
cd build
cmake ..
make
```

<h1> 3 - DESCRIPTION </h1>
Queue specification didnt specify any particular performance characteristics or data types. Based on that I assumed: <br>
- Queue should be flexible with data types and applications. <br>
   Based on that I decided to use templates for type safety and optimization possibilities <br>
- Latency is less of an issue than wasting cpu cycles on busy waiting. <br>
   I decided to use condition variable as it seems to be decent synchronization mechanism for general case. For low latency system though it might be limiting factor that can be replaced by busy waiting or some type of lock free mechanism(for a lot of parallel threads)<br>
- There should be mechanism for registering producer and consumer threads <br>
  I decided that introducing mechanism of ownership can make managing queue more ordered 
  and will help in cases where someone for some reason, may want to reuse this queue by different threads at different times. <br>
- For tests queue code should be separated from testing code <br>
  That is rule that I try to apply to my projects to avoid any way in which testing code messes with tested code during optimized builds(UB problems or some unexpected optimizations). To enforce that rule I use shared libraries on tested code, that is also wy I introduced queue_wrapper.cpp
<br> 
Known limitation: <br>
- Calling shutdown makes queue unusable and discards all remaining elements.
- If either producer or consumer exits without calling unregister remaining producer or consumer will be stuck in condition variable until third thread calls shutdown<br>
- Calling resize in queue constructor may not always work. The idea is for queue to allocated everything up-front and then reuse that memory. If this behaviour becomes crucial, different data structure might be needed.