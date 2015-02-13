/*
 * SensorPool.cpp
 *
 *  Created on: 22.01.2015
 *      Author: rf
 */
#include "libs/Kernel.h"
#include "SensorPool.h"
#include "Sensor.h"
#include "Config.h"
#include "checksumm.h"
#include "ConfigValue.h"

#define sensor_checksum CHECKSUM("sensor")
#define enable_checksum CHECKSUM("enable")

void SensorPool::load_tools()
{
    std::vector<uint16_t> modules;
    THEKERNEL->config->get_module_list( &modules, sensor_checksum );

    for( unsigned int i = 0; i < modules.size(); i++ ) {
        // only create, if the sensor is enabled in config file
        if( THEKERNEL->config->value(sensor_checksum, modules[i], enable_checksum )->as_bool() == true ) {
            Sensor *s = new Sensor(modules[i]);
            THEKERNEL->add_module(s);
        }
    }
}
