# Target Practice
A simulation of finding anomalies in data sets related to player directional movement in FPS games (server-side perspective).

## What's featured?  
- A basic 3D-space setup with Entities (actors) and Shots (bullets). Entities can shoot to create bullets, which act as a single moving point in space.
- Tests on data sets: checking for linear functions created from player's aim, checking for "frame jumping/skips", etc.

## Findings  
- Datasets can contain 'anomalies' in respect to regular user behaviour, and occurances of anomalies increase the likelihood of a player cheating. Anomalies refer to 'non-human' behavior through the form of identifiable patterns.
- When a player is aiming, it's very unlikely that a linear function will be formed by the way the mouse drags. This would imply each point in the resulting vector is a magnitude of itself, and thus the mouse was dragged in a perfect diagonal line. Dragging the mouse in a perfect diagonal line in a fast-paced setting is very unlikely for humans, and repeated instances of this definitely suggests cheats are being used. Similarily, some cheats will draw 'arcs' instead of straight lines, and thus we can check for perfectly circular arcs.
- A player's mouse sensitivity can be used to detect if their aiming position 'skips/jumps', since aim direction can only move by some maximum of X amount every N frames. If there are consecutive points in the player's aiming vector which go beyond some distance over time threshold, it could imply that a cheat is being used.
- Multiple datasets from different aspects of the game should be combined: if there are anomalies with player aiming in combination with successful target hits (in particular, critical hits/headshots), the probability of cheating increases. If they were also reported by multiple other players in multiple matches, the probability goes even higher.
- If the player aims for the same relative position ('bone') repeatedly on a target while that target is moving, they are probably cheating. For example, the shooter always hits the target in exactly the middle of their hitbox despite the target jumping left to right repeatedly
- If the game features recoil when players shoot, firing to the exact same position repeatedly without any directional data changes in-between almost certainly implies a cheat. This creates a de-sync between client and server as the server expects the client's aim to be a specific offset of the previous shot as the recoil amount is static.
- Headshot to bodyshot ratios can be compared to statistical averages of known good/non-cheater datasets
- Line-of-sight can be utilized such that any players who aim at a target immediately when they become visible can be suspected (close to zero reaction times)
- Lack of randomness in aim data when compared to "good" datasets
  
More techniques will be added over time, thanks for taking an interest.






