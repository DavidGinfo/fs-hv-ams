# ECU-Software

This is the monorepository for our entire ECU software.
It uses a set of base modules that can be dynamically integrated into the independent ECU projects.

## Structure
Each ECU has its own Subproject located in the ECUs directory and is configured automatically using CMake. 

Each ECU has its own `README.md` file in its  Subprojects's "root" directory explaining the requirements and tasks of the ECU.

>TODO: Add Link to your ECU's README here.

## Guides
Here is a list of Guides that might help if you are stuck or lost or both.
- [Setup a Development Environment](docs/Setup.md)
- [Integrate new ECU](docs/IntegrateECU.md)


