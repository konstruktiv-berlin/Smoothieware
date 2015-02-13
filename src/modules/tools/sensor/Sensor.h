/*
 * Sensor.h
 *
 *  Created on: 19.12.2014
 *      Author: rf
 */

#ifndef SENSOR_MODULE_H
#define SENSOR_MODULE_H

#include "libs/Module.h"
#include "libs/Pin.h"
#include "libs/gpio.h"


class Sensor : public Module {
public:
	Sensor();
	virtual ~Sensor();

    void on_module_loaded();
    void on_config_reload(void* argument);
    void on_gcode_received(void *argument);
    void on_gcode_execute(void* argument);

private:
    Pin sensor_pin;
    int m_code;
	std::string answer_if_high;
	std::string answer_if_low;
};

#endif /* SRC_MODULES_TOOLS_SENSOR_SENSOR_H_ */
