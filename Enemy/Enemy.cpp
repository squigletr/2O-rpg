//
// Created by Victor Navarro on 13/02/24.
//
#include "Enemy.h"
#include <iostream>
#include <climits>

using namespace std;

//TODO: Check the circular dependency
int getRolledAttack(int attack) {
    int lowerLimit = attack * .80;
    return (rand() % (attack - lowerLimit)) + lowerLimit;
}

Enemy::Enemy(string name, int health, int attack, int defense, int speed) : Character(name, health, attack, defense, speed, false) {
}

void Enemy::doAttack(Character *target) {
    int rolledAttack = getRolledAttack(getAttack());
    int trueDamage = target->getDefense() > rolledAttack ? 0 : rolledAttack - target->getDefense();
    target->takeDamage(trueDamage);
}

void Enemy::takeDamage(int damage) {
    setHealth(getHealth() - damage);
    if(getHealth() <= 0) {
        cout<<getName()<<" has died"<<endl;
    }
    else {
        cout<<getName()<<" has taken " << damage << " damage" << endl;
    }
}

Character* Enemy::getTarget(vector<Player *> teamMembers) {
    // Obtiene el miembro del equipo con menos vida
    int targetIndex = 0;
    int lowestHealth = INT_MAX;
    for(int i=0; i < teamMembers.size(); i++) {
        if(teamMembers[i]->getHealth() < lowestHealth) {
            lowestHealth = teamMembers[i]->getHealth();
            targetIndex = i;
        }
    }

    return teamMembers[targetIndex];
}

Action Enemy::takeAction(vector<Player*> teamMembers) {
    Action myAction;
    myAction.speed = getSpeed();
    Character* target = nullptr;

    if (getHealth() < 0.15 * getMaxHealth()) {
	if (rand() % 100 < 5) {
	    myAction.action = [this, teamMembers](){
		    vector<Character*> participants (teamMembers.begin(), teamMembers.end());
		    bool fleed = flee(participants);
		    if(fleed) {
			cout<<"Enemy has fled"<<endl;
		    }
		    else {
			cout<<"Enemy tried to flee but couldn't"<<endl;
		    }
	    };
	}
    }
    if (myAction.action == nullptr){
        target = getTarget(teamMembers);
        myAction.action = [this, target](){
            doAttack(target);
        };
    }
    return myAction;
}

