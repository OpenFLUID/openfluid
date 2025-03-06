# Fragment for Hayami Kernel operations
For wave propagation

By Moussa R., Fabre J.-C., Louchart X., Lalauze S.


## Usage
Simple use example: 
```cpp
fragments::math::numerical::ComputeHayamiKernel(...);
float QOutput = fragments::math::numerical::DoHayamiPropagation(...);
```


## Requirements
- OpenFLUID 2.2
  - binaries directory
  - `core` component (library)
- Catch2 (v3) is optional, used for unit testing


## Tests
Tests can be triggered as follows:
```
mkdir _build
cd _build
cmake ..
make
ctest
```