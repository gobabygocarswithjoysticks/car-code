#ifndef SETTINGS_NAMES_H
#define SETTINGS_NAMES_H

#define FV(x) (__FlashStringHelper*)(x)

const char control_right[] PROGMEM = "CONTROL_RIGHT";
const char control_center_x[] PROGMEM = "CONTROL_CENTER_X";
const char control_left[] PROGMEM = "CONTROL_LEFT";
const char x_deadzone[] PROGMEM = "X_DEADZONE";
const char control_up[] PROGMEM = "CONTROL_UP";
const char control_center_y[] PROGMEM = "CONTROL_CENTER_Y";
const char control_down[] PROGMEM = "CONTROL_DOWN";
const char y_deadzone[] PROGMEM = "Y_DEADZONE";
const char acceleration_forward[] PROGMEM = "ACCELERATION_FORWARD";
const char deceleration_forward[] PROGMEM = "DECELERATION_FORWARD";
const char acceleration_backward[] PROGMEM = "ACCELERATION_BACKWARD";
const char deceleration_backward[] PROGMEM = "DECELERATION_BACKWARD";
const char acceleration_turning[] PROGMEM = "ACCELERATION_TURNING";
const char deceleration_turning[] PROGMEM = "DECELERATION_TURNING";
const char fastest_forward[] PROGMEM = "FASTEST_FORWARD";
const char fastest_backward[] PROGMEM = "FASTEST_BACKWARD";
const char turn_speed[] PROGMEM = "TURN_SPEED";
const char scale_turning_when_moving[] PROGMEM = "SCALE_TURNING_WHEN_MOVING";
const char reverse_turn_in_reverse[] PROGMEM = "REVERSE_TURN_IN_REVERSE";
const char left_motor_center[] PROGMEM = "LEFT_MOTOR_CENTER";
const char left_motor_slow[] PROGMEM = "LEFT_MOTOR_SLOW";
const char left_motor_fast[] PROGMEM = "LEFT_MOTOR_FAST";
const char right_motor_center[] PROGMEM = "RIGHT_MOTOR_CENTER";
const char right_motor_slow[] PROGMEM = "RIGHT_MOTOR_SLOW";
const char right_motor_fast[] PROGMEM = "RIGHT_MOTOR_FAST";
const char use_speed_knob[] PROGMEM = "USE_SPEED_KNOB";
const char speed_knob_slow_val[] PROGMEM = "SPEED_KNOB_SLOW_VAL";
const char speed_knob_fast_val[] PROGMEM = "SPEED_KNOB_FAST_VAL";
const char scale_accel_with_speed[] PROGMEM = "SCALE_ACCEL_WITH_SPEED";
const char speed_knob_pin[] PROGMEM = "SPEED_KNOB_PIN";
const char joy_x_pin[] PROGMEM = "JOY_X_PIN";
const char joy_y_pin[] PROGMEM = "JOY_Y_PIN";
#ifdef IS_PCB
const char swap_motors[] PROGMEM = "SWAP_MOTORS";
#else
const char left_motor_controller_pin[] PROGMEM = "LEFT_MOTOR_CONTROLLER_PIN";
const char right_motor_controller_pin[] PROGMEM = "RIGHT_MOTOR_CONTROLLER_PIN";
#endif
const char enable_startup_pulse[] PROGMEM = "ENABLE_STARTUP_PULSE";
const char left_motor_pulse[] PROGMEM = "LEFT_MOTOR_PULSE";
const char right_motor_pulse[] PROGMEM = "RIGHT_MOTOR_PULSE";
const char start_motor_pulse_time[] PROGMEM = "START_MOTOR_PULSE_TIME";
const char joy_calib_count[] PROGMEM = "JOY_CALIB_COUNT";
const char enable_button_ctrl[] PROGMEM = "ENABLE_BUTTON_CTRL";
const char use_button_mode_pin[] PROGMEM = "USE_BUTTON_MODE_PIN";
const char button_mode_toggle[] PROGMEM = "BUTTON_MODE_TOGGLE";
const char button_mode_pin[] PROGMEM = "BUTTON_MODE_PIN";
const char num_drive_buttons[] PROGMEM = "NUM_DRIVE_BUTTONS";

const char steering_off_switch[] PROGMEM = "STEERING_OFF_SWITCH";
const char steering_off_switch_pin[] PROGMEM = "STEERING_OFF_SWITCH_PIN";

const char use_stop_switch[] PROGMEM = "USS";//"USE_STOP_SWITCH";
const char on_off_switch_pin[] PROGMEM = "SP";//"STOP_PIN";
const char on_off_active_high[] PROGMEM = "SPH";//"STOP_PIN_HIGH";
const char no_stop_until_start[] PROGMEM = "NSU";//"NO_STOP_UNTIL_START";

const char use_on_off_buttons[] PROGMEM = "UOB";//"USE_ON_OFF_BUTTONS";
const char on_button[] PROGMEM = "NB";//"ON_BUTTON";
const char off_button[] PROGMEM = "FB";//"OFF_BUTTON";
const char on_off_buttons_active_high[] PROGMEM = "BAH";//"ON_OFF_BUTTONS_ACTIVE_HIGH";

const char use_rc_control[] PROGMEM = "UR";//"USE_RC";
const char speed_rc_pin[] PROGMEM = "RSP";//"RC_SPEED_PIN";
const char turn_rc_pin[] PROGMEM = "RTP";//"RC_TURN_PIN";
const char stop_rc_pin[] PROGMEM = "RPP";//"RC_STOP_PIN";
const char ctrl_rc_pin[] PROGMEM = "RCP";//"RC_CTRL_PIN";
const char no_rc_stop_until_start[] PROGMEM = "NRS";//"NO_RC_STOP_UNTIL_START";

#ifdef HAS_WIFI
const char car_wifi_name[] PROGMEM = "CAR_WIFI_NAME";
const char car_wifi_password[] PROGMEM = "CAR_WIFI_PASSWORD";
const char use_wifi[] PROGMEM = "USE_WIFI";
#endif

const char* const SETTING[] PROGMEM = {
  control_right,
  control_center_x,
  control_left,
  x_deadzone,
  control_up,
  control_center_y,
  control_down,
  y_deadzone,
  acceleration_forward,
  deceleration_forward,
  acceleration_backward,
  deceleration_backward,
  acceleration_turning,
  deceleration_turning,
  fastest_forward,
  fastest_backward,
  turn_speed,
  scale_turning_when_moving,
  reverse_turn_in_reverse,
  left_motor_center,
  left_motor_slow,
  left_motor_fast,
  right_motor_center,
  right_motor_slow,
  right_motor_fast,
  use_speed_knob,
  speed_knob_slow_val,
  speed_knob_fast_val,
  scale_accel_with_speed,
  speed_knob_pin,
  joy_x_pin,
  joy_y_pin,
#ifdef IS_PCB
  swap_motors,
#else
  left_motor_controller_pin,
  right_motor_controller_pin,
#endif
  enable_startup_pulse,
  left_motor_pulse,
  right_motor_pulse,
  start_motor_pulse_time,
  joy_calib_count,
  enable_button_ctrl,
  use_button_mode_pin,
  button_mode_toggle,
  button_mode_pin,
  num_drive_buttons,
  steering_off_switch,
  steering_off_switch_pin,
  use_stop_switch,
  on_off_switch_pin,
  on_off_active_high,
  no_stop_until_start,
  use_on_off_buttons,
  on_button,
  off_button,
  on_off_buttons_active_high,
  use_rc_control,
  speed_rc_pin,
  turn_rc_pin,
  stop_rc_pin,
  ctrl_rc_pin,
  no_rc_stop_until_start,
#ifdef HAS_WIFI
  car_wifi_name,
  car_wifi_password,
  use_wifi,
#endif
};

enum SettingID{
  S_CONTROL_RIGHT = 0,
  S_CONTROL_CENTER_X,
  S_CONTROL_LEFT,
  S_X_DEADZONE,
  S_CONTROL_UP,
  S_CONTROL_CENTER_Y,
  S_CONTROL_DOWN,
  S_Y_DEADZONE,
  S_ACCELERATION_FORWARD,
  S_DECELERATION_FORWARD,
  S_ACCELERATION_BACKWARD,
  S_DECELERATION_BACKWARD,
  S_ACCELERATION_TURNING,
  S_DECELERATION_TURNING,
  S_FASTEST_FORWARD,
  S_FASTEST_BACKWARD,
  S_TURN_SPEED,
  S_SCALE_TURNING_WHEN_MOVING,
  S_REVERSE_TURN_IN_REVERSE,
  S_LEFT_MOTOR_CENTER,
  S_LEFT_MOTOR_SLOW,
  S_LEFT_MOTOR_FAST,
  S_RIGHT_MOTOR_CENTER,
  S_RIGHT_MOTOR_SLOW,
  S_RIGHT_MOTOR_FAST,
  S_USE_SPEED_KNOB,
  S_SPEED_KNOB_SLOW_VAL,
  S_SPEED_KNOB_FAST_VAL,
  S_SCALE_ACCEL_WITH_SPEED,
  S_SPEED_KNOB_PIN,
  S_JOY_X_PIN,
  S_JOY_Y_PIN,
#ifdef IS_PCB
  S_SWAP_MOTORS,
#else
  S_LEFT_MOTOR_CONTROLLER_PIN,
  S_RIGHT_MOTOR_CONTROLLER_PIN,
#endif
  S_ENABLE_STARTUP_PULSE,
  S_LEFT_MOTOR_PULSE,
  S_RIGHT_MOTOR_PULSE,
  S_START_MOTOR_PULSE_TIME,
  S_JOY_CALIB_COUNT,
  S_ENABLE_BUTTON_CTRL,
  S_USE_BUTTON_MODE_PIN,
  S_BUTTON_MODE_TOGGLE,
  S_BUTTON_MODE_PIN,
  S_NUM_DRIVE_BUTTONS,
  S_STEERING_OFF_SWITCH,
  S_STEERING_OFF_SWITCH_PIN,
  S_USE_STOP_SWITCH,
  S_ON_OFF_SWITCH_PIN,
  S_ON_OFF_ACTIVE_HIGH,
  S_ARM_STOP_AFTER_START,
  S_USE_ON_OFF_BUTTONS,
  S_ON_BUTTON,
  S_OFF_BUTTON,
  S_ON_OFF_BUTTONS_ACTIVE_HIGH,
  S_USE_RC_CONTROL,
  S_SPEED_RC_PIN,
  S_TURN_RC_PIN,
  S_STOP_RC_PIN,
  S_CTRL_RC_PIN,
  S_NO_RC_STOP_UNTIL_START,
#ifdef HAS_WIFI
  S_CAR_WIFI_NAME,
  S_CAR_WIFI_PASSWORD,
  S_USE_WIFI,
#endif
  S_NUM
};

#endif // SETTINGS_NAMES_H
