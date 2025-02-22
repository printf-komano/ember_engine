# Ember engine 

3d Engine written on C + OpenGL.

This engine adds some tools to work comfortably with buffers, adding and deleting the models in real time.
The goal is to create a simple and optimised engine on C.

## Implementing scenes and subscenes
Work is still in progress. For now, all models are stored in th batch handler (need to fix this)

## Batch
Used to reduce draw calls. For now each model is drawn individually, but all vertices and elements are already stored in a single buffer. After setting up materials, I want to reduce draw calls as much as possible - implementing static scenes. (WIP)
Also I have the idea to introduce few independent batches for overlays.

## Colorful lighting
Implement lighting (at least directional). 

## Reflections
LMAO


