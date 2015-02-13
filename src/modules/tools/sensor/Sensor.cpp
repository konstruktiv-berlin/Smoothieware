/*
 * Sensor.cpp
 *
 *  Created on: 19.12.2014
 *      Author: rf
 */

#include "Sensor.h"
#include "libs/Module.h"
#include "libs/Kernel.h"
#include "modules/robot/Conveyor.h"
#include "modules/communication/utils/Gcode.h"
#include "StreamOutputPool.h"


Sensor::Sensor() {
}

Sensor::~Sensor() {
}

void Sensor::on_module_loaded() {
	// THEKERNEL->streams->printf("DEBUG:SENSOR: Module loaded\n");

	/* bind to pin, configure as input*/
	//FIXME: make pin configurable
	this->sensor_pin.from_string("1.22")->as_input()->pull_down();

	this->register_for_event(ON_GCODE_RECEIVED);
	this->register_for_event(ON_GCODE_EXECUTE);
}


void Sensor::on_gcode_received(void* argument)
{
	//THEKERNEL->streams->printf("DEBUG:SENSOR:Event on_gcode_received\n\r");
	Gcode *gcode = static_cast<Gcode *>(argument);
	if (gcode->has_m)
	{
		if (gcode->m == 700)
			THEKERNEL->conveyor->append_gcode(gcode); /* add to queue + process later */
	}
}


void Sensor::on_gcode_execute(void* argument){

	//THEKERNEL->streams->printf("DEBUG:SENSOR:Event on_gcode_execute\n\r");
	Gcode *gcode = static_cast<Gcode *>(argument);
	if (gcode->has_m)
	{
		if (gcode->m == 700)
		{
			if(this->sensor_pin.get())
				THEKERNEL->streams->puts("M700=1\n\r");
			else
				THEKERNEL->streams->puts("M700=0\n\r");
		}
	}
}
