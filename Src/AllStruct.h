
#ifndef _AllStruct
#define _AllStruct

#ifdef MESH_DIRECTORY
#define ROBOT_PARTS_DIRECTORY(current_path) MESH_DIRECTORY("RobotsParts/") current_path
#define WEAPOIN_DIRECTORY(current_path) ROBOT_PARTS_DIRECTORY("Weapon/") current_path
#endif


#endif