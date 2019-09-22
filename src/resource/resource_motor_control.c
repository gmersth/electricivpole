
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

#include <peripheral_io.h>
#include <log.h>
#include <resource/resource_motor_control.h>
#define LOW 0
#define HIGH 1


static peripheral_gpio_h g_motor_L_pwm;
static peripheral_gpio_h g_motor_L_dir;
static peripheral_gpio_h g_motor_R_pwm;
static peripheral_gpio_h g_motor_R_dir;

static int g_l_pwm_pin_num = -1;
static int g_l_dir_pin_num = -1;
static int g_r_pwm_pin_num = -1;
static int g_r_dir_pin_num = -1;


int _resource_validate_motor(int l_pwm_pin_num,int l_dir_pin_num,int r_pwm_pin_num, int r_dir_pin_num)
{
	int ret = MOTOR_HANDLE_ERROR_NONE;

	if (!g_motor_L_pwm)
	{
		ret = MOTOR_L_PWM_HANDLE_ERROR_NOT_OPEN;

	}else if(!g_motor_L_dir){
		ret = MOTOR_L_DIR_HANDLE_ERROR_NOT_OPEN;


	}else if(!g_motor_R_pwm){
		ret = MOTOR_R_PWM_HANDLE_ERROR_NOT_OPEN;


	}else if(!g_motor_R_dir){
		ret = MOTOR_R_DIR_HANDLE_ERROR_NOT_OPEN;


	}else if (g_l_pwm_pin_num != l_pwm_pin_num) {
		ret = MOTOR_L_PWM_HANDLE_ERROR_INVALID_PIN;
	}else if (g_l_dir_pin_num != l_dir_pin_num) {
		ret = MOTOR_L_DIR_HANDLE_ERROR_INVALID_PIN;
	}else if (g_r_pwm_pin_num != r_pwm_pin_num) {
		ret = MOTOR_R_PWM_HANDLE_ERROR_INVALID_PIN;
	}else if (g_r_dir_pin_num != r_dir_pin_num) {
		ret = MOTOR_R_DIR_HANDLE_ERROR_INVALID_PIN;
	}

	return ret;
}
int resource_motor_open(int l_pwm_pin_num,int l_dir_pin_num,int r_pwm_pin_num, int r_dir_pin_num){
	peripheral_gpio_h temp_l_pwm = NULL;
	peripheral_gpio_h temp_l_dir = NULL;
	peripheral_gpio_h temp_r_pwm = NULL;
	peripheral_gpio_h temp_r_dir = NULL;
	int ret_l_p = peripheral_gpio_open(l_pwm_pin_num, &temp_l_pwm);
	int ret_l_d = peripheral_gpio_open(l_dir_pin_num, &temp_l_dir);
	int ret_r_p = peripheral_gpio_open(r_pwm_pin_num, &temp_r_pwm);
	int ret_r_d = peripheral_gpio_open(r_dir_pin_num, &temp_r_dir);


	if ((ret_l_p+ret_l_d+ret_r_p+ret_r_d)<0)
	{
		peripheral_gpio_close(temp_l_pwm);
		peripheral_gpio_close(temp_l_dir);
		peripheral_gpio_close(temp_r_pwm);
		peripheral_gpio_close(temp_r_dir);

		LOGE("peripheral_gpio_open failed.");

		if(ret_l_p)
			return MOTOR_L_PWM_HANDLE_ERROR_NOT_OPEN;
		if(ret_l_d)
			return MOTOR_L_DIR_HANDLE_ERROR_NOT_OPEN;
		if(ret_r_p)
			return MOTOR_R_PWM_HANDLE_ERROR_NOT_OPEN;
		if(ret_r_d)
			return MOTOR_R_DIR_HANDLE_ERROR_NOT_OPEN;
	}

	ret_l_p = peripheral_gpio_set_direction(temp_l_pwm,PERIPHERAL_GPIO_DIRECTION_OUT_INITIALLY_LOW);
	ret_l_d = peripheral_gpio_set_direction(temp_l_dir,PERIPHERAL_GPIO_DIRECTION_OUT_INITIALLY_LOW);
	ret_r_p = peripheral_gpio_set_direction(temp_r_pwm,PERIPHERAL_GPIO_DIRECTION_OUT_INITIALLY_LOW);
	ret_r_d = peripheral_gpio_set_direction(temp_r_dir,PERIPHERAL_GPIO_DIRECTION_OUT_INITIALLY_LOW);


	if((ret_l_p+ret_l_d+ret_r_p+ret_r_d)!=0)
	{

		peripheral_gpio_close(temp_l_pwm);
		peripheral_gpio_close(temp_l_dir);
		peripheral_gpio_close(temp_r_pwm);
		peripheral_gpio_close(temp_r_dir);
		LOGE("peripheral_gpio_set_direction failed.");

		return -1;
	}

	g_motor_L_pwm = temp_l_pwm;
	g_motor_L_dir = temp_l_dir;
	g_motor_R_pwm = temp_r_pwm;
	g_motor_R_dir = temp_r_dir;

	g_l_pwm_pin_num = l_pwm_pin_num;
	g_l_dir_pin_num = l_dir_pin_num;
	g_r_pwm_pin_num = r_pwm_pin_num;
	g_r_dir_pin_num = r_dir_pin_num;
	return MOTOR_HANDLE_ERROR_NONE;
}

void resource_motor_close(void)
{
	peripheral_gpio_close(g_motor_L_pwm);
	peripheral_gpio_close(g_motor_L_dir);
	peripheral_gpio_close(g_motor_R_pwm);
	peripheral_gpio_close(g_motor_R_dir);

	g_motor_L_pwm = NULL;
	g_motor_L_dir = NULL;
	g_motor_R_pwm = NULL;
	g_motor_R_dir = NULL;

	g_l_pwm_pin_num = -1;
	g_l_dir_pin_num = -1;
	g_r_pwm_pin_num = -1;
	g_r_dir_pin_num = -1;
}




//모터A,B 정회전
int resource_motor_both_Start()
{
	int ret = PERIPHERAL_ERROR_NONE;
	ret += peripheral_gpio_write(g_motor_L_pwm, HIGH);
    ret += peripheral_gpio_write(g_motor_L_dir,LOW);
    ret += peripheral_gpio_write(g_motor_R_pwm,HIGH);
    ret += peripheral_gpio_write(g_motor_R_dir,LOW);
    if(ret<0)
    	{
    		LOGE("error! returned %d",ret);
    		return ret;
    	}

    LOGI("rotate all");
    return 0;
}

//모터A,B Stop
int resource_motor_stop_All()
{
	int ret= PERIPHERAL_ERROR_NONE;
	ret += peripheral_gpio_write(g_motor_L_pwm, LOW);
	ret += peripheral_gpio_write(g_motor_L_dir,LOW);
	ret += peripheral_gpio_write(g_motor_R_pwm,LOW);
	ret += peripheral_gpio_write(g_motor_R_dir,LOW);
	if(ret<0)
		{
			LOGE("error! returned %d",ret);
			return ret;
		}

	LOGI("Stop all");
	return 0;
}

//모터L 역회전, 모터R 정회전
int resource_motor_turnLeft()
{
	int ret= PERIPHERAL_ERROR_NONE;
	ret += peripheral_gpio_write(g_motor_L_pwm, LOW);
	ret += peripheral_gpio_write(g_motor_L_dir,HIGH);
	ret += peripheral_gpio_write(g_motor_R_pwm,HIGH);
	ret += peripheral_gpio_write(g_motor_R_dir,LOW);

	if(ret<0)
	{
		LOGE("error! returned %d",ret);
		return ret;
	}

	LOGI("turn left");
	return 0;
}

//모터L 정회전, 모터R 역회전
int resource_motor_turnRight()
{
	int ret= PERIPHERAL_ERROR_NONE;



	ret += peripheral_gpio_write(g_motor_L_pwm, HIGH);
	ret += peripheral_gpio_write(g_motor_L_dir,LOW);
	ret += peripheral_gpio_write(g_motor_R_pwm,LOW);
	ret += peripheral_gpio_write(g_motor_R_dir,HIGH);
	if(ret<0)
	{
		LOGE("error! returned %d",ret);
		return ret;
	}

	LOGI("turn right");
	return 0;
}

//모터L 정회전, 모터R Stop
int resource_motor_L_Rotation()
{
	int ret= PERIPHERAL_ERROR_NONE;
	ret += peripheral_gpio_write(g_motor_L_pwm, HIGH);
	ret += peripheral_gpio_write(g_motor_L_dir,LOW);
	ret += peripheral_gpio_write(g_motor_R_pwm,LOW);
	ret += peripheral_gpio_write(g_motor_R_dir,LOW);
	if(ret<0)
		{
			LOGE("error! returned %d",ret);
			return ret;
		}
	LOGI("L rotate B stop");
	return 0;
}

//모터A Stop, 모터B 정회전
int resource_motor_R_Rotation()
{
	int ret= PERIPHERAL_ERROR_NONE;
	ret += peripheral_gpio_write(g_motor_L_pwm, LOW);
	ret += peripheral_gpio_write(g_motor_L_dir, LOW);
	ret += peripheral_gpio_write(g_motor_R_pwm, HIGH);
	ret += peripheral_gpio_write(g_motor_R_dir, LOW);
	if(ret<0)
		{
			LOGE("error! returned %d",ret);
			return ret;
		}
	LOGI("L stop B rotate");
	return 0;
}

//모터A 역회전, 모터B Stop
int resource_motor_L_Reverse()
{
	int ret= PERIPHERAL_ERROR_NONE;
	ret += peripheral_gpio_write(g_motor_L_pwm, LOW);
	ret += peripheral_gpio_write(g_motor_L_dir, HIGH);
	ret += peripheral_gpio_write(g_motor_R_pwm, LOW);
	ret += peripheral_gpio_write(g_motor_R_dir, LOW);
	if(ret<0)
		{
			LOGE("error! returned %d",ret);
			return ret;
		}
	LOGI("L reverse B stop");
	return 0;
}

//모터A Stop, 모터B 역회전
int resource_motor_R_Reverse()
{
	int ret= PERIPHERAL_ERROR_NONE;
	ret += peripheral_gpio_write(g_motor_L_pwm, LOW);
	ret += peripheral_gpio_write(g_motor_L_dir, LOW);
	ret += peripheral_gpio_write(g_motor_R_pwm, LOW);
	ret += peripheral_gpio_write(g_motor_R_dir, HIGH);
	if(ret<0)
		{
			LOGE("error! returned %d",ret);
			return ret;
		}
	LOGI("L stop B reverse");
	return 0;
}

int resource_motor_both_Reverse()
{
	int ret= PERIPHERAL_ERROR_NONE;
	ret += peripheral_gpio_write(g_motor_L_pwm, LOW);
	ret += peripheral_gpio_write(g_motor_L_dir, HIGH);
	ret += peripheral_gpio_write(g_motor_R_pwm, LOW);
	ret += peripheral_gpio_write(g_motor_R_dir, HIGH);
	if(ret<0)
	{
		LOGE("error! returned %d",ret);
		return ret;
	}
	LOGI("Reverse all");
	return 0;
}


