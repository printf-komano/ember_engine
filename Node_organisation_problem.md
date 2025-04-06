Ahah, development stumbled upon a problem:
There's `prim` (contains primitive data) and `prim_inst`(also contains instanced primitive data) structures. `prim_inst` is the instance of the primitive. So, we can create instance from the template and then easily change in in runtime.
Makes sence? Kinda.

Next, we get the `nodes` system which works EXACTLY with instances `prim_inst` - NOT with templates. Nodes and all the tools for managing them are made for runtime so they are not desined for templates (`prim`).

Then, we get some model-loader (for example, gltf format, it doesn't actually matter). What this loader... loads? `prim`, exactly. We load the template to instance it later. But model made of difficult hierarchy (primitive-mesh-model) which can b replaced with... `node`-s. But nodes doesn't work with `prim` templates. 


Solutions:
1. `node` is bound to `node_pool`. Pools are independent. So, we can allow `prim` to be bound to `node` and then return the "small" `node_pool` when the model was loaded from file. In that case, we have to implement pretty difficult (imo) function to copy data from "small" `node_pool` to the `bhandler`.
2. completely rewrite data structures (instead of just `prim`) - maybe some `model_template`. And then create an algorythm to "push" data correctly into all buffets form template.
3. create some data structure over `prim` which contains all the pimitives in array and instructions how to create the `node` tree properly.
