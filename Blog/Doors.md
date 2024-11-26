


# NPCs interacting with Doors

This series dives into how we can create NPCs that interact with common gameplay features. The first feature we are going to look at is doors. We will start with simple sliding doors, and build up the complexity piece by piece from there.

This series will be broken into separate parts, and roughly follow the following outline:
1. A simple sliding door that blocks an NPC when closed.
2. Allowing an NPC to open a closed door.
3. Having the NPC play an animation to open the door.
4. Closing the door behind an NPC, and handling multiple NPCs passing through a door.
5. Only allowing NPCs to pass through a doorway in one direction.
6. Allowing only some NPCs to pass through a door.
7. Preventing NPCs from blocking doorways.
8. Handling the space required for swinging doors.

At each step, we will discuss several ways that doors can be made to interact with NPCs and the navmesh.

I will be working predominantly in Unreal, since it has many of the pieces that we need already in the shipping engine. I'm using Unreal 5.5, but nearly all of the engine features that we will be using have existed for some time in the engine. The example project is available for download at https://github.com/phildunstan/Doors.




## Part One - A simple sliding door

This is the first part of a series on creating doors that can be interacted with by NPCs.

On the last game I worked, Star Wars Outlaws, we were fortunate enough to have a lot of sliding doors. These simplify the problem a little since we don't have to worry about the space a swinging door takes up when it is open. So, let's start there.

In Example 0, I've created a sliding door that can be toggled open or closed. The door itself is implemented as a simple prop blueprint BP_SimpleDoor. The door opening and closing is controlled by a timeline set up in the blueprint graph that animates the horizontal position of the door.

To start with, the door doesn't effect navigation at all. The door is set up with collision, but since *Can Ever Affect Navigation* has been set to false this collision has no impact on the navmesh.

As you can see in the Example 0 animation, the door doesn't block the navmesh through the doorway and the NPC can path through the closed door.

### Blocking the navmesh with the door collision geometry

As a first step towards preventing the NPC from pathing through the doorway we can modify the door collision to affect the navmesh. We'll start by setting *Can Ever Affect Navigation* and *Is Dynamic Obstacle* on the door to true so that the door collision affects the navmesh generation.

If we want the navmesh to be updated whenever the door opens or closes we will also need to modify the Runtime Generation option in the RecastNavMesh asset to Dynamic. Without this change, the navmesh will be blocked at the door position during the initial generation but won't be updated as the door opens and closes.

We will use the NavArea_Obstacle instead of NavArea_Null to block the navmesh under door, with a corresponding change to exclude this nav area from our NPC's NavQueryFilter. Both options would be equally effective in preventing NPCs from pathing through the closed door right now. However, as we will see there are several advantages to having navmesh through the doorway.

The Example 1 image shows the navmesh updating as we open the door. We can make a couple of observations from this image.

First, the navmesh is being updated several times during the opening or closing of the door. Navmesh updates are typically very expensive. We'd be better to only update the navmesh once the door is in its fully opened position.

Second, the doorway becomes passable before the door is completely open. This could be an issue if an NPC is very close to the door when it starts to open and clips through the door as it passes through. Similarly, the door remains passable until just before the door is closed. Again an NPC may clip through the door as it closes.

Both of these issues can be mitigated by keeping the navmesh blocked until the door is fully open, or blocking the navmesh immediately as the door starts to close.

### Blocking the navmesh with a NavModifierVolume

So far we've used the door collision to modifier the navmesh as the door moves. Alternatively, Unreal has a dedicated volume called the NavModifierVolume to control the navmesh generation in an area. We will disable the Can Ever Affect Navigation setting on the door, and rely on this volume to update the navigation in the doorway.

As an intermediate step, we could parent the NavModifierVolume to the door so that the modifier volume moves as the door opens. When this volume is moved the navmesh is locally regenerated in the same way that it was regenerated when the door collision moved. As shown in Example 2, this has a similar effect to using the door collision to block the navmesh.

Our next step is to decouple the NavModifierVolume from the door so that it no longer moves as the door moves. Instead we will trigger the modifier volume to change the state of the navmesh only when the door is in its fully open position.

In Example 3 I've exposed a NavModifierVolume variable on the door and bound that to the volume that we've added to the doorway. Within the door blueprint, I can change the NavAreaType of the modifier volume when the door is in its fully open position.

Fortunately, changing the NavAreaType of a modifier volume is a very quick operation. It doesn't cause a regeneration or the navmesh faces. It simply changes the nav area data of the existing faces. Combined with the need to only update the NavModifierVolume when the door becomes fully open, this is a much cleaner solution than our previous attempts.

We now have a working sliding door, albeit a simple one. We can toggle the door to open or close, and NPCs will only try to path through the door when it is in its open state. In the next part of this series, we will allow NPCs to path through a closed door and to trigger the door to open as they approach it.









## Part Two - Allowing NPCs to path through a closed door








## Door solutions

1. a custom navlink/user edge
2. dynamic rebuilding of the navmesh after adding/removing blocking collision
3. dynamic rebuilding of the navmesh after adding/removing/enabling/disabling a volume that paints a value onto the navmesh (eg Nav Modifier Volumes in Unreal)
4. changing the value of a volume that always exists (eg the Nav Modifier Volume) without requiring rebuilding navmesh
5. navmesh patches (ie. swapping out one pre-built version of the navmesh for another pre-built version)
6. something else
