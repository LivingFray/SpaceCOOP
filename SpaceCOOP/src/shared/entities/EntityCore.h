#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <string>

//Set to the highest id a command may have, default is based on size of CommandID
#define MAX_ENTITIES 1<<(8*sizeof(EntityType))

typedef sf::Uint64 UUID;
typedef sf::Uint8 EntityType;
//TODO: Some kind of efficient update send that sends differences

class EntityCore:
	public sf::Transformable,
	public sf::Drawable{
public:
	EntityCore();
	virtual ~EntityCore();
	UUID id;
	EntityType type;
	sf::Vector2f getVelocity() const { return vel; }
	void setPosition(const sf::Vector2f& p);
	void setRotation(float angle);
	void setVelocity(sf::Vector2f v);
	friend sf::Packet& operator<<(sf::Packet& packet, const EntityCore& command);
	friend sf::Packet& operator>>(sf::Packet& packet, EntityCore& command);
	virtual void modify(sf::Packet& p);
	virtual void generateModifyPacket(sf::Packet& p); //Pass by reference should be faster
	virtual void update(double dt);
	virtual sf::Vector2f getFront();
	virtual sf::Vector2f getRight();
protected:
	virtual void packetIn(sf::Packet& packet);
	virtual void packetOut(sf::Packet& packet) const;
	//If the ID can be used by this instantation of Entity, use it to
	virtual bool applyModification(sf::Uint8, sf::Packet& p);
	const static int NUM_MODIFY_PROTOCOLS = 4;
	bool posChanged = false;
	sf::Vector2f vel;
	bool velChanged = false;
	sf::Sprite sprite;
	float width = 1.0f;
	float height = 1.0f;
	bool angleChanged = false;
	bool angMomentumChanged = false;
	float angMomentum = 0.0f;
	void loadSpriteAndResize(std::string texture);

private:
	//Could (probably should) use enum for this but for reasons I've forgotten I chose not to
	static const sf::Uint8 POS_MODID = 0;
	static const sf::Uint8 VEL_MODID = 1;
	static const sf::Uint8 ANG_MODID = 2;
	static const sf::Uint8 ANGMOM_MODID = 3;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

