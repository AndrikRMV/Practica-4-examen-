//
// Created by Victor Navarro on 15/02/24.
//

#include "Player.h"
#include <iostream>

using namespace std;

Player::Player(string _name, int _health, int _attack, int _defense, int _speed) : Character(_name, _health, _health,_attack, _defense, _speed, true) {
    level = 1;
    experience = 0;
    originalDefense = _defense; // Almacena el valor original de la defensa
}

void Player::doAttack(Character *target) {
    target->takeDamage(attack);
}

void Player::takeDamage(int damage) {
    int trueDamage = damage - defense;
    if (trueDamage < 0) {
        trueDamage = 0; // El jugador no puede curarse debido a la defensa
    }

    health -= trueDamage;

    cout << name << " recibio " << trueDamage << " damage!" << endl;

    if(health <= 0) {
        cout << name << " has been defeated!" << endl;
    }

}

void Player::levelUp() {
    level++;
}

void Player::gainExperience(int exp) {
    experience += exp;
    if (experience >= 100) {
        levelUp();
        experience = 100-experience;
    }
}

Character* Player::selectTarget(vector<Enemy*> possibleTargets) {
    int selectedTarget = 0;
    cout << "Select a target: " << endl;
    for (int i = 0; i < possibleTargets.size(); i++) {
        cout << i + 1 << ". " << possibleTargets[i]->getName() << endl;  // Sumar 1 para mostrar el número de enemigo correctamente
    }

    // TODO: Add input validation
    cin >> selectedTarget;

    // Ajustar el índice seleccionado para que coincida con el índice del vector
    selectedTarget--;

    return possibleTargets[selectedTarget];
}

void Player::resetDefense() {
    // Restablecer la defensa al valor original
    defense = originalDefense;
}

Action Player::takeAction(vector<Enemy*> enemies) {
    int action;
    cout << "Select an action: " << endl
         << "1. Attack" << endl
         << "2. Defend" << endl; // Agregar la opción de defenderse
    cin >> action;
    Action currentAction;
    Character* target = nullptr;
    int originalDefense = defense; // Mover la declaración aquí
    switch(action) {
        case 1:
            // Utilizar al enemigo seleccionado al principio del combate como objetivo
            if (!enemies.empty()) {
                target = selectedEnemy;
            } else {
                cout << "No hay enemigos disponibles." << endl;
                currentAction.action = nullptr;
                return currentAction;
            }
            currentAction.target = target;
            currentAction.action = [this, target](){
                doAttack(target);
            };
            currentAction.speed = getSpeed();
            break;
        case 2:
            defend(); // Llamar al método defend si se elige defenderse

            if (!enemies.empty()) {
                target = enemies[0];
            }
            if (target) {
                currentAction.target = target;
                currentAction.action = [this, target, originalDefense](){
                    // Restablecer la defensa al valor original después de defenderse
                    defense = originalDefense;
                };
                currentAction.speed = target->getSpeed();
            } else {
                cout << "No hay enemigos disponibles." << endl;
                currentAction.action = nullptr;
            }
            break;
        default:
            cout << "Invalid action" << endl;
            currentAction.action = nullptr;
            break;
    }

    return currentAction;
}

