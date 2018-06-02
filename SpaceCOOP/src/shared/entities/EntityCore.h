#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <string>

class SolarSystem;

//Set to the highest id a command may have, default is based on size of CommandID
#define MAX_ENTITIES 1<<(8*sizeof(EntityType))

typedef sf::Uint64 UUID;
typedef sf::Uint8 EntityType;

class EntityCore:
	public sf::Transformable,
	public sf::Drawable{
public:
	EntityCore();
	virtual ~EntityCore();

	void setSolarSystem(SolarSystem* sys);

	UUID id;
	EntityType type;
	sf::Vector2f getVelocity() const { return vel; }
	void setPosition(const sf::Vector2f& p);
	void setRotation(float angle);
	void setVelocity(sf::Vector2f v);
	friend sf::Packet& operator<<(sf::Packet& packet, const EntityCore& entity);
	friend sf::Packet& operator>>(sf::Packet& packet, EntityCore& entity);
	virtual void modify(sf::Packet& p);
	virtual void generateModifyPacket(sf::Packet& p); //Pass by reference should be faster
	virtual void update(double dt);
	virtual sf::Vector2f getFront();
	virtual sf::Vector2f getRight();
	virtual bool collidesWithLine(sf::Vector2f origin, sf::Vector2f direction);
	virtual void damage(float damage) {}

	virtual int getNumMods() const { return static_cast<int>(MODS::NUM_MODS); }
protected:
	virtual void packetIn(sf::Packet& packet);
	virtual void packetOut(sf::Packet& packet) const;
	//If the ID can be used by this instantation of Entity, use it to
	virtual bool applyModification(sf::Uint8 modId, sf::Packet& p);
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
	void resizeSprite();
	bool useShader = false;
	sf::Shader shader;

	SolarSystem* system;
	bool isServerSide;

	enum class MODS { POS, VEL, ANG, ANG_MOM, NUM_MODS };
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

