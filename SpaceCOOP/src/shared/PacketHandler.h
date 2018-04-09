#pragma once
class PacketHandler {
public:
	PacketHandler();
	~PacketHandler();

	enum class Type {TEXT, GALAXY, COMMAND, ENTITY, ASSIGN_SHIP};
	enum class Entity {CREATE, MODIFY, REMOVE, REMOVE_ALL};
};

