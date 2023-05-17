This observer exports spatial domain graph to Dot files and can be post-processed with GraphViz  

Parameters can be

* `title` : the main title of the graph
* `basefilename` : base of the generated files names
* `show.pcsord` : set to 1 to include the process order of the spatial unit in graph nodes
* `show.attributes` : set to 1 to include the attributes of the spatial unit in graph nodes
* `show.variables` : set to 1 to include the variables of the spatial unit in graph nodes
* `when.init` : set to 1 to export the graph at initialization time
* `when.everytime` : set to 1 to export the graph at every simulation time point
* `when.final` : set to 1 to export the graph at finalization time
* `style.<unitsclass>.<attribute>` : the value of a Dot attribute for all nodes of a unit class
