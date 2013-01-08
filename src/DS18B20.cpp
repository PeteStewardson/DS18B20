/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * Name        : DS18B20.cpp
 * Author      : Georgi Todorov <terahz@geodar.com>
 * Version     : 
 * Created on  : Jan 3, 2013
 *
 * Copyright © 2013 Georgi Todorov <terahz@geodar.com>
 */

#include "DS18B20.h"

DS18B20::DS18B20(char* address) {
	address_ = strdup(address);
	unit_ = CELCIUS;
	char path[47]; // path should be 46 chars
	snprintf(path, 46, "%s%s%s", BUS, address_, TEMPFILE);
	devFile = fopen(path, "r");
	if (devFile == NULL) {
		printf("Count not open %s\n", path);
		throw errno;
	}
}

DS18B20::~DS18B20() {
	fclose(devFile);
}

float DS18B20::getTemp() {
	float temp = -1;
	if (devFile != NULL) {
		  rewind (devFile);
		if (!ferror(devFile)) {
			unsigned int tempInt;
			char crcConf[5];
			fscanf(devFile, "%*x %*x %*x %*x %*x %*x %*x %*x %*x : crc=%*x %s", crcConf);
			if (strncmp(crcConf, "YES", 3) == 0) {
				fscanf(devFile, "%*x %*x %*x %*x %*x %*x %*x %*x %*x t=%5d", &tempInt);
				temp = (float) tempInt / 1000.0;
			}
		}
	}
	if (unit_ == CELCIUS) {
		return temp;
	} else
		return CtoF(temp);
}

uint8_t DS18B20::getUnits() {
	return unit_;
}

void DS18B20::setUnits(uint8_t u) {
	unit_ = u;
}

float DS18B20::CtoF(float temp) {
	return temp * 1.8 + 32;
}
