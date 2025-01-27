# Usage of random in ware : setup and example {#apdx_seedableware}

[TOC]

## Setup stochasticity {#apdx_seedableware_setup}

OpenFLUID provides tools to use stochasticity inside your wares.
To do so, a parameter with the key *seed* must be specified inside a ware tags in the *model.fluidx* file:

```.xml
<simulator ID="SimulatorTest">
    <param name="seed" value="1" />
</simulator>
```

A seed is a value used to initialise a random number generator. It is used to produce a pseudo-random sequence of numbers, which will always be identical if the same seed is used again. It allows the reproduction of random results in simulations, for example.
If a negative value is specified for this parameter,  it will use a random seed value. If it is a positive value, it will use this value as the seed. 
In both cases, you can track the seed value in the simulation output logs.

Once the parameter *seed* has been added, go to your ware source code and add the include line: `#include <openfluid/ware/WareRNG.hpp>`
Create a class variable of type `openfluid::ware::WareRNG RNG`. 
This variable will handle the randomness and provide few utility functions.
To construct this variable, call its constructor by passing as argument the ware object

```.cpp
SimulatorTest() : PluggableSimulator(), Rng(this)
{
	// constructor
}
```

In order to use the seed value provided in the *model.fluidx*, we need to initialize the variable in the `initParams(const openfluid::ware::WareParams_t& Params)` function by adding the line : `Rng.init(Params);
`
```.cpp
void initParams(const openfluid::ware::WareParams_t& Params)
{
	Rng.init(Params);
}
```

@note If not initialized, the object will use a random seed.

Utility functions can now be used in order to generate random numbers.

## Example {#apdx_seedableware_example}

The example below shows the use of stochasticity in ware.

```.cpp
// HPP

#include <openfluid/ware/WareRNG.hpp>

class SimulatorTest : public openfluid::ware::PluggableSimulator
{
	public:
		SimulatorTest();
	
	private:
		openfluid::ware::WareRNG Rng;
};


// CPP

SimulatorTest() : PluggableSimulator(), Rng(this)
{
	// constructor
}


// =====================================================================
// =====================================================================


void initParams(const openfluid::ware::WareParams_t& Params)
{
	Rng.init(Params);
}

openfluid::base::SchedulingRequest runStep()
{
	// example of uniform distribution with integer numbers
	std::vector<int> Vector = Rng.runif(5, 0, 10);
} 
```

To see all available utility functions, check **openfluid::tools::RandomNumberGenerator** class.