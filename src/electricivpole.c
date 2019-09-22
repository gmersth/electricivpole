/*
 * Copyright (c) 2019 Youngkyu Cho
 *
 * Contact: Youngkyu Cho <ykc33556@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <tizen.h>
#include <service_app.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <Ecore.h>
#include "log.h"
#include "resource/resource_motor_control.h"

#define MOTOR_L_PWM (27)
#define MOTOR_L_DIR (22)
#define MOTOR_R_PWM (23)
#define MOTOR_R_DIR (24)

#define TIMER_GATHER_INTERVAL (2.0f)

#define MOTOR_L_PWM_GPIO_NUMBER (19)
#define MOTOR_L_DIR_GPIO_NUMBER (13)
#define MOTOR_R_PWM_GPIO_NUMBER (6)
#define MOTOR_R_DIR_GPIO_NUMBER (5)





static int _motor_move(int movement)
{
	int ret;
	switch (movement)
	{
	case GO:
		ret = resource_motor_both_Start();
		break;
	case STOP:
		ret = resource_motor_stop_All();
		break;
	case TURN_LEFT:
		ret = resource_motor_turnLeft();
		break;
	case TURN_RIGHT:
		ret = resource_motor_turnRight();
		break;
	case MOTOR_A_GO:
		ret = resource_motor_L_Rotation();
		break;
	case MOTOR_B_GO:
		ret = resource_motor_R_Rotation();
		break;
	case MOTOR_A_REVERSE:
		ret = resource_motor_L_Reverse();
		break;
	case MOTOR_B_REVERSE:
		ret = resource_motor_R_Reverse();
		break;
	case GO_REVERSE:
		ret = resource_motor_both_Reverse();
		break;
	}
	if(ret<0)
		LOGE("failed working motor");

}

bool service_app_create(void *data)
{
    // Todo: add your code here.
    return true;
}

void service_app_terminate(void *data)
{
    // Todo: add your code here.
	resource_motor_close();
    return;
}

void service_app_control(app_control_h app_control, void *data)
{
	resource_motor_open(MOTOR_L_PWM_GPIO_NUMBER, MOTOR_L_DIR_GPIO_NUMBER, MOTOR_R_PWM_GPIO_NUMBER, MOTOR_R_DIR_GPIO_NUMBER);

	_motor_move(GO);
	sleep(1);
	_motor_move(STOP);
	sleep(1);
	_motor_move(TURN_LEFT);
	sleep(1);
	_motor_move(TURN_RIGHT);
	sleep(1);
    // Todo: add your code here.
    return;
}


static void
service_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/
	return;
}

static void
service_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
service_app_low_battery(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_BATTERY*/
}

static void
service_app_low_memory(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_MEMORY*/
}

int main(int argc, char* argv[])
{
    char ad[50] = {0,};
	service_app_lifecycle_callback_s event_callback;
	app_event_handler_h handlers[5] = {NULL, };
	int loop;

	event_callback.create = service_app_create;
	event_callback.terminate = service_app_terminate;
	event_callback.app_control = service_app_control;

	service_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, service_app_low_battery, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, service_app_low_memory, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, service_app_lang_changed, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, service_app_region_changed, &ad);


	_motor_move(GO);
	sleep(1);
	_motor_move(STOP);
	sleep(1);
	_motor_move(TURN_LEFT);
	sleep(1);
	_motor_move(TURN_RIGHT);
	sleep(1);

	return service_app_main(argc, argv, &event_callback, ad);
}
