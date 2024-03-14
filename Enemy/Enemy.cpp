

#include "Enemy.h"
#include "../Utils.h"
#include <iostream>

using namespace std;
using namespace combat_utils;

// Constructor de Enemy inicializa un personaje con atributos específicos y experiencia.
Enemy::Enemy(string _name, int _health, int _attack, int _defense, int _speed, int _experience)
        : Character(_name, _health, _health, _attack, _defense, _speed, false), experience(_experience) {
}

// Realiza un ataque a un objetivo, calculando el daño con la ayuda de utilidades de combate.
void Enemy::doAttack(Character *target) {
    target->takeDamage(getRolledAttack(attack));
}

// Recibe daño, ajustando por la defensa del enemigo, y actualiza la salud correspondientemente.
void Enemy::takeDamage(int damage) {
    int trueDamage = max(0, damage - defense); // Asegura que el daño no sea negativo.
    health -= trueDamage;

    cout << name << " recibió " << trueDamage << " de daño!" << endl;

    if (health <= 0) {
        cout << name << " ha sido derrotado!" << endl;
    }
}

// Retorna la experiencia que proporciona este enemigo al ser derrotado.
int Enemy::getExperience() {
    return experience;
}

// Selecciona un objetivo de un vector de posibles objetivos basándose en quien tiene menos salud.
Character* Enemy::selectTarget(vector<Player*> possibleTargets) {
    int lessHealth = INT_MAX; // Utiliza el máximo valor entero para inicializar lessHealth.
    Character* target = nullptr;
    for (auto character : possibleTargets) {
        if (character->getHealth() < lessHealth) {
            lessHealth = character->getHealth();
            target = character;
        }
    }
    return target;
}

// Decide la acción a tomar contra los miembros del grupo de jugadores, seleccionando el ataque como acción.
Action Enemy::takeAction(vector<Player*> partyMembers) {
    Action currentAction;
    currentAction.speed = getSpeed();

    Character* target = selectTarget(partyMembers);
    currentAction.target = target;
    currentAction.action = [this, target]() {
        doAttack(target);
    };

    return currentAction;
}
