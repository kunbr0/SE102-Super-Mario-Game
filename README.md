Super Mario Bros. 3 (NES) with Direct
=======
* OS: Windows 10
* Graphic Libary: DirectX 9
* Language(s): C/C++ (main), Javascript (NodeJS) for tool building

Introduction
-----------
* My project in the subject "Introduction to Game Development" at my university, using DirectX 9 library. The requirement is to manually build a **Game Framework** and create SMB3 based on a self-made Framework instead of using Game Engine.

Product Demo
-----------
* [PRODUCT DEMO - CLICK HERE](https://drive.google.com/drive/folders/15Hyued_DWmdUoh8md8bRiikCG31ySOwg)

Instruction
-----------
* [DETAIL INSTRUCTION - CLICK HERE](https://github.com/kunbr0/SE102-Game/blob/master/Instruction.pdf)

Tools
-----------
* Map editor: Tiled Map Editor
* Data reader: TinyXML library for reading XML
* Data generation for Grid: Excel

Technique
-----------
* SweptAABB: for detecting collisions of physical movement
* AABB: for detecting overlapped collisions
* Grid: for space partitioning

Control
-----------
**Main Menu**
* Select options: Up/Down arrow key
* Decision: "W" key

**Overworld Map**
* Movement (select level): Arrow keys (left, right, up, down)
* Enter level: "S" key

**Play Scene**
* Ground movement: Left/Right arrow
* Run: Hold "A" key and move
* Jump: Press "X" for press jump, Press "S" for long-press jump 
* Duck (crouch): Down arrow
* Warp to Pipes: An Arrow key on a respective direction (left, right, up, down) 
