#pragma once
class PacketHandler {
public:
	PacketHandler();
	~PacketHandler();

	enum class Type {TEXT, GALAXY, COMMAND, ENTITY};
	enum class Entity {CREATE, MODIFY, REMOVE};
};

