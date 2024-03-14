//
// Created by Victor Navarro on 15/02/24.
//

#ifndef RPG_CHARACTER_H
#define RPG_CHARACTER_H
#include <string>
#include <cmath>

using namespace std;

class Character {
protected:
    string name;
    int health;
    int maxHealth; // Nuevo miembro para la salud máxima
    int attack;
    int defense;
    int speed;
    bool isPlayer;
public:
    Character(string _name, int _health, int _maxHealth, int _attack, int _defense, int _speed, bool _isPlayer); // Declaración del constructor

    virtual void doAttack(Character *target) = 0;
    virtual void takeDamage(int damage) = 0;

    //TODO: Implementar metodo de defensa
    //Incrementar la defensa un 20% solo por el turno actual
    void defend() {
        defense = std::ceil(defense*1.2);
    }

    bool flee(Character* target);
    string getName();
    int getHealth();
    int getAttack();
    int getDefense();
    bool getIsPlayer();
    int getSpeed();
    string toString();

    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
};


#endif //
