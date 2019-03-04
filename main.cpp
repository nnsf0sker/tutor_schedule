/*
 * main.cpp
 *
 *  Created on: 26 февр. 2019 г.
 *      Author: nnsf0sker
 */

#include <iostream>
#include <string>
#include "schedule.h"

using namespace std;

int main() {

	Schedule schedule;

	schedule.fileInput("input.txt");

	schedule.consoleOutput();

	schedule.computeSchedule();

	schedule.consoleOutput();

	schedule.answersOutput();
}
