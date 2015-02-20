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
#include "libs/Config.h"
#include "libs/ConfigValue.h"
#include "libs/checksumm.h"

#define    sensor_checksum					CHECKSUM("sensor")
#define    sensor_input_pin_checksum		CHECKSUM("input_pin")
#define    sensor_m_code_checksum			CHECKSUM("m_code")
#define    sensor_answer_if_high_checksum	CHECKSUM("answer_if_high")
#define    sensor_answer_if_low_checksum	CHECKSUM("answer_if_low")


 
Sensor::Sensor() {
}
 

Sensor::Sensor(uint16_t name)
{
	this->name_checksum = name;
}


void Sensor::on_module_loaded() {
	// THEKERNEL->streams->printf("DEBUG:SENSOR: Module loaded\n");
	
	this->register_for_event(ON_GCODE_RECEIVED);
	this->register_for_event(ON_GCODE_EXECUTE);
	// load settings
	this->on_config_reload(this);
}


void Sensor::on_config_reload(void* argument)
{
	this->sensor_pin.from_string(  THEKERNEL->config->value( sensor_checksum, this->name_checksum, sensor_input_pin_checksum)->by_default("nc" )->as_string() )->as_input();
	this->m_code = THEKERNEL->config->value( sensor_checksum, this->name_checksum, sensor_m_code_checksum )->by_default(-1)->as_int();
	this->answer_if_high = THEKERNEL->config->value( sensor_checksum, this->name_checksum, sensor_answer_if_high_checksum )->by_default("")->as_string();
	this->answer_if_low = THEKERNEL->config->value( sensor_checksum, this->name_checksum, sensor_answer_if_low_checksum )->by_default("")->as_string();
}


void Sensor::on_gcode_received(void* argument)
{
	//THEKERNEL->streams->printf("DEBUG:SENSOR:Event on_gcode_received\n\r");
	Gcode *gcode = static_cast<Gcode *>(argument);
	if (gcode->has_m)
	{
		if (gcode->m == (unsigned int) this->m_code)
			THEKERNEL->conveyor->append_gcode(gcode); /* add to queue + process later */
	}
}


void Sensor::on_gcode_execute(void* argument){

	//THEKERNEL->streams->printf("DEBUG:SENSOR:Event on_gcode_execute\n\r");
	Gcode *gcode = static_cast<Gcode *>(argument);
	if (gcode->has_m)
	{
		if (gcode->m == (unsigned int) this->m_code)
		{
			if(this->sensor_pin.get())
			{
				THEKERNEL->streams->puts(this->answer_if_high.c_str());
				THEKERNEL->streams->puts("\n\r");
			}
			else
			{
				THEKERNEL->streams->puts(this->answer_if_low.c_str());
				THEKERNEL->streams->puts("\n\r");
			}
		}
	}
}
