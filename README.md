# Ember engine 

__3d Engine written on C + OpenGL.__

![cubes](cubes.png "Cubes rendered using ember engine")

This engine adds some tools to work comfortably with buffers, adding and deleting the models in real time.
The goal is to create a simple and optimised engine on C.

## Models
`vmodel` is unique model loaded from file or created directly. 
`vmodel_inst` is created instance of the model. _Gonna add some optimisation - possibility for re-use vertex data in memory._

## Batch
Used to reduce draw calls. For now each model is drawn individually, but all vertices and elements are already stored in a single buffer. After setting up materials, I want to reduce draw calls as much as possible - implementing static scenes. (WIP)
`bhandler` is responsible for working with vertex buffer `vbo` and element buffer `vao`. Also `bhandler` has access to all existing `vmodel_inst` elements.


## Node hierarchy 
Tngine implements `node` system (analogue of scenes in most engines). Each object can be bound to node and get transformed via this node. It can be useful to group meshes together (for example, player+sword mesh).
The feature of working with nodes is that nodes do not contain references to child elements. Instead, each element can refer to the parent node. When data in parent node is being changed, it automatically applies to the child node as well.
Creating the node is simple. It can be bound with another `node` or `vmodel_inst`
```C
node n; node_init(&n); //creating new node
node n_child; node_init(&n_child); //creating new node
n_child->parent = &n; //binding the nodes
```
`node` elements are independent of `bhandler` or any OpenGL feature. They are being used only for transformations.

## Static scenes
The idea is to group few models in one big vertex group. (WIP)


## Colorful lighting
Implement lighting (at least directional). 

## Transparency
Wish me luck.

## Reflections
NO. PLEASE NO.

