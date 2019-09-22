/*
 * resource_motor_control.h
 *
 *  Created on: Sep 2, 2019
 *      Author: Cho
 */

#ifndef RESOURCE_RESOURCE_MOTOR_CONTROL_H_
#define RESOURCE_RESOURCE_MOTOR_CONTROL_H_

typedef enum {
	MOTOR_HANDLE_ERROR_NONE = 0,                /**< Successful */
	MOTOR_L_PWM_HANDLE_ERROR_NOT_OPEN,
	MOTOR_R_PWM_HANDLE_ERROR_NOT_OPEN,
	MOTOR_L_DIR_HANDLE_ERROR_NOT_OPEN,
	MOTOR_R_DIR_HANDLE_ERROR_NOT_OPEN,
	MOTOR_L_PWM_HANDLE_ERROR_INVALID_PIN,
	MOTOR_R_PWM_HANDLE_ERROR_INVALID_PIN,
	MOTOR_L_DIR_HANDLE_ERROR_INVALID_PIN,
	MOTOR_R_DIR_HANDLE_ERROR_INVALID_PIN

}resource_motor_handle_error_e;



typedef enum {
	GO,
	STOP,
	TURN_LEFT,
	TURN_RIGHT,
	MOTOR_A_GO,
	MOTOR_B_GO,
	MOTOR_A_REVERSE,
	MOTOR_B_REVERSE,
	GO_REVERSE,
}resource_motor_command;
extern int _resource_validate_motor(int l_pwm_pin_num,int l_dir_pin_num,int r_pwm_pin_num, int r_dir_pin_num);
extern int resource_motor_open(int l_pwm_pin_num,int l_dir_pin_num,int r_pwm_pin_num, int r_dir_pin_num);
extern void resource_motor_close(void);
extern int resource_motor_both_Start();
extern int resource_motor_stop_All();
extern int resource_motor_turnLeft();
extern int resource_motor_turnRight();
extern int resource_motor_L_Rotation();
extern int resource_motor_R_Rotation();
extern int resource_motor_L_Reverse();
extern int resource_motor_R_Reverse();
extern int resource_motor_both_Reverse();
#endif /* RESOURCE_RESOURCE_MOTOR_CONTROL_H_ */
