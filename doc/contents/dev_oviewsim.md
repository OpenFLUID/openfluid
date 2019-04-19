@page dev_oviewsim Overview of an OpenFLUID simulator

[TOC]

Technically, an OpenFLUID simulator is made of two main parts: 
the signature and a C++ class containing the computational code.
These two parts have to be developped in a C++ file (.cpp). 
They must be compiled before using it in the OpenFLUID software environment.


# Simulator signature {#dev_oviewsim_signature}

The signature of a simulator contains meta-informations about the simulator.
These informations will be mainly used by the OpenFLUID framework for automatic coupling 
and consistency checking of the simulators of the coupled model.
To get more informations about the simulators signatures and how to declare them, see part @ref dev_signature.
  

#  Simulator C++ class {#dev_oviewsim_cppclass}

The computational part of a simulator is defined by a class, 
inherited from the **openfluid::ware::PluggableSimulator** class.
The simulation code have to be written into the different methods
provided by the **openfluid::ware::PluggableSimulator** class.
You can also develop other methods in order to organize your source code.  
To get more information about the C++ class of a simulator, see part @ref dev_createsim.


## Constructor and destructor {#dev_oviewsim_cppclass_cstrdstr}

The constructor of the simulator is called when the simulator is loaded.
You may put here the initialization of the private members of the simulator C++ class.  
The destructor of the simulator is called when the simulator is released after simulation.
You may put here instruction to free any dynamic memory allocated 
for the needs of the computational code (dynamic variables or objects, ...).


## Mandatory methods to be defined {#dev_oviewsim_cppclass_methods}

The class of a simulator must define the following methods:

* **initParams**
* **prepareData**
* **checkConsistency**
* **initializeRun**
* **runStep**
* **finalizeRun**


The @if DocIsLaTeX **initParams** @else @link openfluid::ware::PluggableSimulator::initParams initParams @endlink @endif 
 method is used to retreive the parameters of the simulator, 
read from the model.fluidx file or filled from the OpenFLUID-Builder interface (See @ref user_fluidx_sections_model). 
Once read, the values should be stored into private attributes to be accessed by other methods.  
<br/>


The @if DocIsLaTeX **prepareData** @else @link openfluid::ware::PluggableSimulator::prepareData prepareData @endlink @endif
 method can be used to perform data pre-processing before the consistency checking.  
<br/>


The @if DocIsLaTeX **checkConsistency** @else @link openfluid::ware::PluggableSimulator::checkConsistency checkConsistency @endlink @endif 
 method is called during the global consistency checking phase. It can be used to add specific consistency checking for the simulator.  
<br/>


The @if DocIsLaTeX **initializeRun** @else @link openfluid::ware::PluggableSimulator::initializeRun initializeRun @endlink @endif
 method is used for initialization of simulation variables, or to compute initialization data.  
<br/>


The @if DocIsLaTeX **runStep** @else @link openfluid::ware::PluggableSimulator::runStep runStep @endlink @endif
 method is called at each exchange time step. It should contain the main computational code.  
<br/>


The @if DocIsLaTeX **finalizeRun** @else @link openfluid::ware::PluggableSimulator::finalizeRun finalizeRun @endlink @endif
 method should be used to do post-processing after simulation. It is the last method ran.  
<br/>

@note Any of theses simulators methods can be empty if there is no relevant code to add for the method.

