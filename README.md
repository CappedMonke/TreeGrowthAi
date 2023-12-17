# TreeGrowthAi
Untrained tree-growth simulation controlled by reinforcement learning.
Used for this were Unreal Engine 5 and its experimental plugin "Learning Agents".


# Setup
Download Unreal Engine 5.3.2
Generate Visual Studio project files (right click on TreeGrowthAi.uproject)
Open generated solution in Visual Studio (C++ desktop devlopment workload must be installed)
Build project in Visual Studio


# Modification
To see what it does, build the project and hit the "play in editor" button.
We hadn't time to fully train the network, so the estimated time to grow an acceptable tree yourself with the settings as are, should be around 4 hours and more.

In the World Outliner you can find the Tree Agent and Learning Manager Actor.
The Tree Agent sets the rules for the tree.
The Manager sets properties for the learning aspect.

![image](https://github.com/CappedMonke/TreeGrowthAi/assets/100119945/2f8c8158-6926-49ca-add1-310b06d3a65f)
