//
// Created by Andrik Meneses on 13/03/24.
//
#include "Combat.h"
#include <string>
#include <iostream>
#include <utility>
#include <algorithm>
#include <cstdlib>

using namespace std;

bool compareSpeed(Character *a, Character *b) {
    return a->getSpeed() > b->getSpeed();
}

Combat::Combat(vector<Character *> _participants) {
    participants = std::move(_participants);
    for(auto participant : participants) {
        if (participant->getIsPlayer()) {
            partyMembers.push_back((Player *) participant);
        } else {
            enemies.push_back((Enemy *) participant);
        }
    }
}

Combat::Combat(vector<Player*> _partyMembers, vector<Enemy*> _enemies) {
    partyMembers = std::move(_partyMembers);
    enemies = std::move(_enemies);
    participants = vector<Character*>();
    participants.insert(participants.end(), partyMembers.begin(), partyMembers.end());
    participants.insert(participants.end(), enemies.begin(), enemies.end());
}

Combat::Combat() {
    participants = vector<Character*>();
}

void Combat::addParticipant(Character *participant) {
    participants.push_back(participant);
    if(participant->getIsPlayer()){
        partyMembers.push_back((Player*) participant);
    } else {
        enemies.push_back((Enemy*) participant);
    }
}

void Combat::combatPrep() {
    // Sort participants by speed
    sort(participants.begin(), participants.end(), compareSpeed);
}

string Combat::toString() {
    string result = "";
    vector<Character*>::iterator it;
    for(it = participants.begin(); it != participants.end(); it++){
        result += (*it)->toString() + "\n";
    }
    cout<<"===================="<<endl;
    return result;
}

Character* Combat::getTarget(Character* attacker) {
    vector<Character*>::iterator it;
    for(it = participants.begin(); it != participants.end(); it++){
        if((*it)->getIsPlayer() != attacker->getIsPlayer()){
            return *it;
        }
    }
    //TODO: Handle this exception
    return nullptr;
}

void Combat::chooseEnemy() {
    cout << "Elige con qué enemigo deseas pelear:" << endl;
    for (int i = 0; i < enemies.size(); ++i) {
        cout << i+1 << ". " << enemies[i]->getName() << endl;
    }
    int choice;
    cin >> choice;
    if (choice > 0 && choice <= enemies.size()) {
        // Establecer el enemigo seleccionado para el jugador
        partyMembers[0]->setSelectedEnemy(enemies[choice-1]);
        // Actualizar la lista de participantes solo con el jugador y el enemigo seleccionado
        participants.clear();  // Limpiar la lista de participantes
        participants.push_back(partyMembers[0]);  // Agregar al jugador
        selectedEnemy = enemies[choice-1];  // Almacenar el enemigo seleccionado
        participants.push_back(selectedEnemy);  // Agregar al enemigo seleccionado
    } else {
        cout << "Opción inválida. Se iniciará el combate con todos los participantes." << endl;
        // No se ha seleccionado ningún enemigo, por lo que se peleará con todos los enemigos
        selectedEnemy = nullptr;
    }
}

void Combat::doCombat() {
    cout << "Inicio del combate" << endl;
    chooseEnemy();

    // Mostrar los parámetros de vida, defensa y ataque de todos los participantes al inicio del combate
    cout << "=== Participant Status: ===" << endl;
    for (auto participant : participants) {
        cout << participant->toString() << endl;
    }
    cout << "" << endl;

    combatPrep();
    int round = 1;
    //Este while representa las rondas del combate
    while(enemies.size() > 0 && partyMembers.size() > 0) {
        cout<<"Round " << round << endl;
        vector<Character*>::iterator it = participants.begin();
        registerActions(it);
        executeActions(it);

        // Mostrar los parámetros de vida, defensa y ataque de todos los participantes al final de cada ronda
        cout << "=== Participant Status: ===" << endl;
        for (auto participant : participants) {
            cout << participant->toString() << endl;
        }
        cout << "" << endl;

        round++;

        // Si se ha derrotado al enemigo seleccionado, salir del bucle de combate
        if (selectedEnemy && selectedEnemy->getHealth() <= 0) {
            break;
        }
    }

    if (selectedEnemy && selectedEnemy->getHealth() <= 0) {
        cout << "You win!" << endl;
    } else {
        cout << "You lose!" << endl;
    }
}

void Combat::executeActions(vector<Character*>::iterator participant) {
    while(!actionQueue.empty()) {
        Action currentAction = actionQueue.top();
        currentAction.action();
        actionQueue.pop();

        //Check if there are any dead characters
        checkParticipantStatus(*participant);
        checkParticipantStatus(currentAction.target);
    }
}

void Combat::checkParticipantStatus(Character *participant) {
    if(participant->getHealth() <= 0) {
        if(participant->getIsPlayer()) {
            partyMembers.erase(remove(partyMembers.begin(), partyMembers.end(), participant), partyMembers.end());
        } else {
            enemies.erase(remove(enemies.begin(), enemies.end(), participant), enemies.end());
        }
        participants.erase(remove(participants.begin(), participants.end(), participant), participants.end());
    }
}

void Combat::registerActions(vector<Character*>::iterator participantIterator) {
    //Este while representa el turno de cada participante
    //La eleccion que cada personaje elije en su turno
    while(participantIterator != participants.end()) {
        if((*participantIterator)->getIsPlayer()) {
            Action playerAction = ((Player*) *participantIterator)->takeAction(enemies);
            // Si el jugador eligió atacar, establecer al enemigo seleccionado como objetivo
            if (playerAction.action != nullptr && selectedEnemy != nullptr) {
                playerAction.target = selectedEnemy;
            }
            actionQueue.push(playerAction);
        } else {
            // Lógica para que el enemigo pueda defenderse con probabilidad del 40% si tiene menos del 15% de vida
            Enemy *enemyParticipant = dynamic_cast<Enemy*>(*participantIterator);
            double fifteenPercentMaxHealth = 0.15 * enemyParticipant->getMaxHealth();
            if (enemyParticipant->getHealth() < fifteenPercentMaxHealth) {
                // Calcular la probabilidad de defensa
                double defenseProbability = static_cast<double>(rand()) / RAND_MAX;
                // Calcular el límite
                int limite = static_cast<int>(defenseProbability * RAND_MAX);
                cout << "Tu probabilidad de defenderte fue: " << defenseProbability << endl;
                if (defenseProbability <= 0.4) {
                    enemyParticipant->defend();
                    cout << enemyParticipant->getName() << " decide defenderse al tener menos del 15% de vida." << endl;
                    participantIterator++;
                    continue; // Saltar al siguiente participante sin registrar acción de ataque
                }
            }

            Action enemyAction = enemyParticipant->takeAction(partyMembers);
            actionQueue.push(enemyAction);
        }

        participantIterator++;
    }
}