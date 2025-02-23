# Ember engine 

__3d Engine written on C + OpenGL.__

![cubes](cubes.png "Cubes rendered using ember engine")

This engine adds some tools to work comfortably with buffers, adding and deleting the models in real time.
The goal is to create a simple and optimised engine on C.

## Node hierarchy 
Tngine implements `node` system (analogue of scenes in most engines). Each object can be bound to node and get transformed via this node. It can be useful to group meshes together (for example, player+sword mesh).

## Batch
Used to reduce draw calls. For now each model is drawn individually, but all vertices and elements are already stored in a single buffer. After setting up materials, I want to reduce draw calls as much as possible - implementing static scenes. (WIP)

## Static scenes
The idea is to group few models in one big vertex group. (WIP)

## Models
`vmodel` is unique model loaded from file or created directly. 
`vmodel_inst` is created instance of the model. _Gonna add some optimisation - possibility for re-use vertex data in memory._

## Colorful lighting
Implement lighting (at least directional). 

## Transparency
Wish me luck.

## Reflections
NO. PLEASE NO.

