I am setting out to implement a tiny userspace network stack as an engineering project. My goal is to write something that can receive raw Ethernet frames, parse the headers manually, maintain its own tiny state tables, then craft replies and send them back. 

This is my first commit so I only have a partial idea of how I'm going to go about this. But ideally, in addition to the core functionality, I'd like to also provide packet capture/architecture diagrams, as well as benchmarks.

Eventually I will update this README to reflect core features as they are implemented. 
